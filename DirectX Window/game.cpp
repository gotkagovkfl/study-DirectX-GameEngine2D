#include "game.h"

//������
Game::Game()
{
	input = new Input();

	paused = false;
	graphics = NULL;
	initialized = false;
}

//�Ҹ���
Game::~Game()
{
	deleteAll();
	ShowCursor(true);
}

// ������ �޼��� �ڵ鷯
LRESULT Game::messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (initialized)
	{
		switch (msg)
		{
		case WM_DESTROY: //����
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN: // Ű�� ����
		case WM_SYSKEYDOWN:
			input->keyDown(wParam);
			return 0;

		case WM_KEYUP: // Ű�� ��
		case WM_SYSKEYUP:
			input->keyUp(wParam);
			return 0;

		case WM_CHAR: //���� �Է�
			input->keyIn(wParam);
			return 0;

		case WM_MOUSEMOVE: // ���콺�� ������
			input->mouseIn(lParam);
			return 0;

		case WM_INPUT: // ���콺 �����Ͱ� ����
			input->mouseRawIn(lParam);
			return 0;

		case WM_LBUTTONDOWN: // ���콺 ���� ��ư ����
			input->setMouseLButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_LBUTTONUP:
			input->setMouseLButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_MBUTTONDOWN: // ���콺 ��� ��ư ����
			input->setMouseMButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_MBUTTONUP:
			input->setMouseMButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_RBUTTONDOWN: // ���콺 ������ ��ư ����
			input->setMouseRButton(true);
			input->mouseIn(lParam);
			return 0;

		case WM_RBUTTONUP:
			input->setMouseRButton(false);
			input->mouseIn(lParam);
			return 0;

		case WM_XBUTTONDOWN: // ���콺 x��ư ����/��
		case WM_XBUTTONUP:
			input->setMouseXButton(wParam);
			input->mouseIn(lParam);
			return 0;

		case WM_DEVICECHANGE: // ��Ʈ�ѷ� Ȯ��
//			input->checkControllers();
			return 0;
		}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//���� �ʱ�ȭ
void Game::initialize(HWND hw)
{
	hwnd = hw;

	graphics = new Graphics();
	graphics->initialize(hwnd, GAME_WIDTH, GAME_HEIGHT, FULLSCREEN);

	//Input ��ü �ʱ�ȭ, ���콺 ĸó �̻��
	input->initialize(hwnd, false);

	//���� Ÿ�̸� ����� �õ�
	if (QueryPerformanceFrequency(&timerFreq) == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing high resolution timer"));

	QueryPerformanceCounter(&timeStart); // ���۽ð� ��������
	initialized = true;
}

//���� �������� �������Ѵ�.
void Game::renderGame()
{
	// �������� �����Ѵ�.
	if (SUCCEEDED(graphics->beginScene()))
	{
		//render�Լ��� ��ӹ޴� Ŭ�������� �ݵ�� �����ؾ��ϴ� �������� �Լ���.
		render();				// �Ļ�Ŭ�������� render�� ȣ���Ѵ�.
		graphics->endScene();	// �������� �����.

	}
	handleLostGraphicsDevice();

	// ȭ�鿡 �� ���۸� ǥ���Ѵ�.
	graphics->showBackBuffer();
}

//�ν�Ʈ ���°� �� �׷��� ����̽��� ó��
void Game::handleLostGraphicsDevice()
{
	//�ν�Ʈ ���°� �� ����̽��� �׽�Ʈ�ϰ� ó���Ѵ�.
	hr = graphics->getDeviceState();
	
	if (FAILED(hr)) // ����̰� ��ȿ�� ���°� �ƴ϶��
	{
		//����̽��� �ν�Ʈ �����̸� ������ �Ұ����ϴٸ�
		if (hr == D3DERR_DEVICELOST)
		{
			Sleep(100);	//cpu���� �ð��� �纸�Ѵ�.
			return;
		}
		// ����̽��� �ν�Ʈ �������� ������ �����ϴٸ�
		else if (hr == D3DERR_DEVICENOTRESET)
		{
			releaseAll();
			hr = graphics->reset();		// ���� �õ�
			if (FAILED(hr)) //���� �����Ѵٸ� 
				return; 

			resetAll();
		}
		else   //�ٸ� ����̽� ����
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

//WinMain�� ���� �޼��� �������� �ݺ������� ȣ��ȴ�.
void Game::run(HWND hwnd)
{
	// �׷����� �ʱ�ȭ ���� �ʾҴٸ�
	if (graphics == NULL) 
		return;  
	
	// ������ ���������κ��� ����� �ð��� ����� frameTime �� �����Ѵ�.
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

	//������ �����ڵ�� winm.lib�� �ʿ�
	//����� �ð��� ���ϴ� ������ �ӵ��� �������� �ʴ´ٸ�
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);	//������ Ÿ�̸Ӹ� ���� 1ms �ػ� ��û
		Sleep(sleepTime);	// CPU�� sleepTime ��ŭ ���� �Ѵ�.
		timeEndPeriod(1);	// 1ms Ÿ�̸� �ػ� ����
		return;
	}
	if (frameTime > 0.0)
		fps = (fps * 0.99f) + (0.01f / frameTime);
	if (frameTime > MAX_FRAME_TIME)
		frameTime = MAX_FRAME_TIME;
	timeStart = timeEnd;
	//input->readControllers();	//��Ʈ�ѷ��� ���¸� �д´�.
	
	//update, ai, collision �Լ��� ���� �����Լ���.
	if (!paused)
	{
		update();
		ai();
		collisions();
		//input->virateControllers(frameTime); 
	}

	renderGame(); // ��� ���� �������� �׸���.


	// �Է��� �����, ��� Ű Ȯ���� ���� �Ŀ� �� �Լ��� ȣ���Ѵ�.
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