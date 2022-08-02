#include "game.h"

//생성자
Game::Game()
{
	input = new Input();

	paused = false;
	graphics = NULL;
	initialized = false;
}

//소멸자
Game::~Game()
{
	deleteAll();
	ShowCursor(true);
}

// 윈도우 메세지 핸들러
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (initialized)
	{
		switch (msg)
		{
		case WM_DESTROY: //종료
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN: // 키를 누름
		case WM_SYSKEYDOWN:
			input->keyDown(wParam);
			return 0;

		case WM_KEYUP: // 키를 뗌
		case WM_SYSKEYUP:
			input->keyUp(wParam);
			return 0;

		case WM_CHAR: //문자 입력
			input->keyIn(wParam);
			return 0;

		case WM_MOUSEMOVE: // 마우스를 움직임
			input->mouseIn(lParam);
			return 0;

		case WM_INPUT: // 마우스 데이터가 들어옴
			input->mouseRawIn(lParam);
			return 0;

		case WM_LBUTTONDOWN: // 마우스 왼쪽 버튼 누름
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_LBUTTONUP:
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_MBUTTONDOWN: // 마우스 가운데 버튼 누름
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_MBUTTONUP:
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_RBUTTONDOWN: // 마우스 오른쪽 버튼 누름
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_RBUTTONUP:
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_XBUTTONDOWN: // 마우스 x버튼 누름/뗌
		case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return 0;

		case WM_DEVICECHANGE: // 컨트롤러 확인
//			input->checkControllers();
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//게임 초기화
void Game::initialize(HWND hw)
{
	hwnd = hw;

	graphics = new Graphics();
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	//Input 객체 초기화, 마우스 캡처 미사용
	input->initialize(hwnd, false);

	//고성능 타이머 사용을 시도
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

	QueryPerformanceCounter(&timeStart); // 시작시간 가져오기
	initialized = true;
}

//게임 아이템을 렌더링한다.
void Game::renderGame()
{
	// 렌더링을 시작한다.
	if (SUCCEEDED(graphics->beginScene()))
	{
		//render함수는 상속받는 클래스에서 반드시 구현해야하는 순수가상 함수다.
		render();				// 파생클래스에서 render를 호출한다.
		graphics->endScene();	// 렌더링을 멈춘다.

	}
	handleLostGraphicsDevice();

	// 화면에 백 버퍼를 표시한다.
	graphics->showBackBuffer();
}

//로스트 상태가 된 그래픽 디바이스를 처리
void Game::handleLostGraphicsDevice()
{
	//로스트 상태가 된 디바이스를 테스트하고 처리한다.
	hr = graphics->getDeviceState();
	
	if (FAILED(hr)) // 디바이가 유효한 상태가 아니라면
	{
		//디바이스가 로스트 상태이며 리셋이 불가능하다면
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);	//cpu에게 시간을 양보한다.
			return;
		}
		// 디바이스가 로스트 상태지만 리셋이 가능하다면
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();		// 리셋 시도
			if (FAILED(hr)) //리셋 실패한다면 
				return; 

			resetAll();
		}
		else   //다른 디바이스 에러
			return; 
	}
}


//=============================================================================
// Toggle window or fullscreen mode
//=============================================================================
void Game::setDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	releaseAll();                   // free all user created surfaces
	graphics->changeDisplayMode(mode);
	resetAll();                     // recreate surfaces
}

//WinMain의 메인 메세지 루프에서 반복적으로 호출된다.
void Game::run(HWND hwnd)
{
	// 그래픽이 초기화 되지 않았다면
	if (graphics == NULL) 
		return;  
	
	// 마지막 프레임으로부터 경과된 시간을 계산해 frameTime 에 저장한다.
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

	//절전을 위한코드는 winm.lib이 필요
	//경과된 시간이 원하는 프레임 속도에 충족하지 않는다면
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);	//윈도우 타이머를 위한 1ms 해상도 요청
		Sleep(sleepTime);	// CPU를 sleepTime 만큼 쉬게 한다.
		timeEndPeriod(1);	// 1ms 타이머 해상도 종료
		return;
	}
	if (frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime);
	if (frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;
	timeStart = timeEnd;
	//input->readControllers();	//컨트롤러의 상태를 읽는다.
	
	//update, ai, collision 함수는 순수 가상함수다.
	if (!paused)
	{
		update();
		ai();
		collisions();
		//input->virateControllers(frameTime); 
	}

	renderGame(); // 모든 게임 아이템을 그린다.


	// 입력을 지우고, 모든 키 확인이 끝난 후에 이 함수를 호출한다.
	input->clear(inputNS::KEYS_PRESSED);

}

void Game::releaseAll()
{}

void Game::resetAll()
{}

void Game::deleteAll()
{
	releaseAll();
	SAFE_DELETE(graphics);
	SAFE_DELETE(input);
	initialized = false;
}