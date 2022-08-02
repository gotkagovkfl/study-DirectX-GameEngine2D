#include "input.h"

// 디폴트 생성자
Input::Input()
{
	//keysDown 배열을 지운다.
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++) { keysDown[i] = false; }
	//keysPressed 배열을 지운다.
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++) { keysPressed[i] = false; }

	newLine = true;
	textIn = "";
	charIn = 0;

	//마우스 데이터
	mouseX = 0;
	mouseY = 0;
	mouseRawX = 0;
	mouseRawY = 0;
	mouseLButton = false;
	mouseMButton = false;
	mouseRButton = false;
	mouseX1Button = false;
	mouseX2Button = false;
	
	//for (int i = 0; i < MAX_CONTROLLERS; i++)
	//{
		//controllers[i].vibrateTimeLeft = 0;
		//controllers[i].vibrateTimeRight = 0;
	//}
}

//소멸자
Input:: ~Input()
{
	if (mouseCaptured) { ReleaseCapture(); }
}

//마우스와 컨트롤러 입력을 초기화한다. 마우스를 캡처하면 capture=true로 설정하고 GameError를 던진다.
void Input::initialize(HWND hwnd, bool captured)
{
	try
	{
		mouseCaptured = captured;
		//고정밀 마우스를 등록한다.
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = hwnd;
		
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
		if (mouseCaptured) { SetCapture(hwnd); } // 마우스를 캡처한다.

		// 컨트롤러 상태를 지운다. 
		//ZeroMemory(controllers, sizeof(ControllerState) * MAX_CONTROLLERS);
		//checkControllers();
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing input system"));
	}
}

// 해당 키에 대해  keysDown 과  KeyPressed 배열을 true로 설정한다.
void Input::keyDown(WPARAM wParam)
{
	//키 코드가 버퍼 범위 안에 있는지 확인한다.
	if (wParam < inputNS::KEYS_ARRAY_LEN)
	{
		keysDown[wParam] = true;
		keysPressed[wParam] = true;
	}
}

// 해당 키에 대해 keysDown 배열을 false로 설정한다.
void Input::keyUp(WPARAM wParam)
{
	//키 코드가 버퍼 범위 안에 있는지 확인한다.
	if (wParam < inputNS::KEYS_ARRAY_LEN )
		keysDown[wParam] = false;
}

// textIn 문자열에 입력한 문자를 저장한다.
void Input::keyIn(WPARAM wParam)
{
	if (newLine)
	{
		textIn.clear();
		newLine = false;
	}
	
	if (wParam == '\b') //백스페이스라면
	{
		if (textIn.length() > 0)
			textIn.erase(textIn.size() - 1);
	}
	else
	{
		textIn += wParam;
		charIn = wParam;
	}
	
	if ((char)wParam == '\r') // 캐리지 리턴 이라면
		newLine = true;
}

//특정 가상 키를 눌렀다면 true, 아니라면 false를 반환
bool Input::isKeyDown(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysDown[vkey];
	else
		return false;
}

//가장 최근 프레임에 특정 가상 키를 눌렀다면 true를 반환한다. (각 프레임 끝에 키 누름에 관한 정보는 지워진다. )
bool Input::wasKeyPressed(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysPressed[vkey];
	else
		return false;
}

//가장 최근 프레임에 아무 키를 눌렀다면 true를 반환한다. (각 프레임 끝에 키 누름에 관한 정보는 지워진다.)
bool Input::anyKeyPressed() const
{
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		if (keysPressed[i] == true)
			return true;
	return false;
}

//특정 키 누름에 관한 정보를 지운다.
void Input::clearKeyPress(UCHAR vkey)
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		keysPressed[vkey] = false;
}

//특정 입력 버퍼를 지운다. input.h에 값 종류가 나열되어 있음
void Input::clear(UCHAR what)
{
	if (what & inputNS::KEYS_DOWN)
	{
		for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
			keysDown[i] = false;
	}
	if (what & inputNS::KEYS_PRESSED)
	{
		for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
			keysPressed[i] = false;
	}
	if (what & inputNS::MOUSE)
	{
		mouseX = 0;
		mouseY = 0;
		mouseRawX = 0;
		mouseRawY = 0;
	}
	if (what & inputNS::TEXT_IN)
		clearTextIn();
}

//화면상의 마우스 위치를 읽어 mouseX, mouseY에 저장한다.
void Input::mouseIn(LPARAM lParam)
{
	mouseX = GET_X_LPARAM(lParam);
	mouseY = GET_Y_LPARAM(lParam);
}

// 처리되지 않은 마우스 데이터를 읽어 mouseRawX, mouseRawY에 저장한다. ( 이런 루틴은 고정밀 마우스와 호환된다.)
void Input::mouseRawIn(LPARAM lParam)
{
	UINT dwSize = 40;
	static BYTE lpb[40];
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		mouseRawX = raw->data.mouse.lLastX;
		mouseRawY = raw->data.mouse.lLastY;
	}
}

// Reads mouse wheel movement expressed in multiples of WHEEL_DELTA, which
// is 120. A positive value indicates that the wheel was rotated away from the
// user, a negative value indicates that the wheel was rotated toward the user.
void Input::mouseWheelIn(WPARAM wParam)
{
	mouseWheel = GET_WHEEL_DELTA_WPARAM(wParam);
}