#include "input.h"

// ����Ʈ ������
Input::Input()
{
	//keysDown �迭�� �����.
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++) { keysDown[i] = false; }
	//keysPressed �迭�� �����.
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++) { keysPressed[i] = false; }

	newLine = true;
	textIn = "";
	charIn = 0;

	//���콺 ������
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

//�Ҹ���
Input:: ~Input()
{
	if (mouseCaptured) { ReleaseCapture(); }
}

//���콺�� ��Ʈ�ѷ� �Է��� �ʱ�ȭ�Ѵ�. ���콺�� ĸó�ϸ� capture=true�� �����ϰ� GameError�� ������.
void Input::initialize(HWND hwnd, bool captured)
{
	try
	{
		mouseCaptured = captured;
		//������ ���콺�� ����Ѵ�.
		Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
		Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
		Rid[0].dwFlags = RIDEV_INPUTSINK;
		Rid[0].hwndTarget = hwnd;
		
		RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
		if (mouseCaptured) { SetCapture(hwnd); } // ���콺�� ĸó�Ѵ�.

		// ��Ʈ�ѷ� ���¸� �����. 
		//ZeroMemory(controllers, sizeof(ControllerState) * MAX_CONTROLLERS);
		//checkControllers();
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing input system"));
	}
}

// �ش� Ű�� ����  keysDown ��  KeyPressed �迭�� true�� �����Ѵ�.
void Input::keyDown(WPARAM wParam)
{
	//Ű �ڵ尡 ���� ���� �ȿ� �ִ��� Ȯ���Ѵ�.
	if (wParam < inputNS::KEYS_ARRAY_LEN)
	{
		keysDown[wParam] = true;
		keysPressed[wParam] = true;
	}
}

// �ش� Ű�� ���� keysDown �迭�� false�� �����Ѵ�.
void Input::keyUp(WPARAM wParam)
{
	//Ű �ڵ尡 ���� ���� �ȿ� �ִ��� Ȯ���Ѵ�.
	if (wParam < inputNS::KEYS_ARRAY_LEN )
		keysDown[wParam] = false;
}

// textIn ���ڿ��� �Է��� ���ڸ� �����Ѵ�.
void Input::keyIn(WPARAM wParam)
{
	if (newLine)
	{
		textIn.clear();
		newLine = false;
	}
	
	if (wParam == '\b') //�齺���̽����
	{
		if (textIn.length() > 0)
			textIn.erase(textIn.size() - 1);
	}
	else
	{
		textIn += wParam;
		charIn = wParam;
	}
	
	if ((char)wParam == '\r') // ĳ���� ���� �̶��
		newLine = true;
}

//Ư�� ���� Ű�� �����ٸ� true, �ƴ϶�� false�� ��ȯ
bool Input::isKeyDown(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysDown[vkey];
	else
		return false;
}

//���� �ֱ� �����ӿ� Ư�� ���� Ű�� �����ٸ� true�� ��ȯ�Ѵ�. (�� ������ ���� Ű ������ ���� ������ ��������. )
bool Input::wasKeyPressed(UCHAR vkey) const
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		return keysPressed[vkey];
	else
		return false;
}

//���� �ֱ� �����ӿ� �ƹ� Ű�� �����ٸ� true�� ��ȯ�Ѵ�. (�� ������ ���� Ű ������ ���� ������ ��������.)
bool Input::anyKeyPressed() const
{
	for (size_t i = 0; i < inputNS::KEYS_ARRAY_LEN; i++)
		if (keysPressed[i] == true)
			return true;
	return false;
}

//Ư�� Ű ������ ���� ������ �����.
void Input::clearKeyPress(UCHAR vkey)
{
	if (vkey < inputNS::KEYS_ARRAY_LEN)
		keysPressed[vkey] = false;
}

//Ư�� �Է� ���۸� �����. input.h�� �� ������ �����Ǿ� ����
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

//ȭ����� ���콺 ��ġ�� �о� mouseX, mouseY�� �����Ѵ�.
void Input::mouseIn(LPARAM lParam)
{
	mouseX = GET_X_LPARAM(lParam);
	mouseY = GET_Y_LPARAM(lParam);
}

// ó������ ���� ���콺 �����͸� �о� mouseRawX, mouseRawY�� �����Ѵ�. ( �̷� ��ƾ�� ������ ���콺�� ȣȯ�ȴ�.)
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