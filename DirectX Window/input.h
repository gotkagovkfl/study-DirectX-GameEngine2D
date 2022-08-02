#ifndef _INPUT_H
#define _INPUT_H
#define WIN32_LEAN_AND_MEAN

class Input;

#include <windows.h>
#include <WindowsX.h>
#include <string>
#include "constants.h"
#include "gameError.h"

// for high-definition mouse
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC      ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE     ((USHORT) 0x02)
#endif

namespace inputNS
{
	const int KEYS_ARRAY_LEN = 256;

	const UCHAR KEYS_DOWN = 1;
	const UCHAR KEYS_PRESSED = 2;
	const UCHAR MOUSE = 4;
	const UCHAR TEXT_IN = 8;
	const UCHAR KEYS_MOUSE_TEXT = KEYS_DOWN + KEYS_PRESSED + MOUSE + TEXT_IN;
}

//const short GAMEPAD_THUMBSTICK_DEADZONE = (short)(0.20f * 0X7FFF);    // default to 20% of range as deadzone
//const short GAMEPAD_TRIGGER_DEADZONE = 20;                      // trigger range 0-255
//const DWORD MAX_CONTROLLERS = 4;                                // Maximum number of controllers supported by XInput

//// Bit corresponding to gamepad button in state.Gamepad.wButtons
//const DWORD GAMEPAD_DPAD_UP = 0x0001;
//const DWORD GAMEPAD_DPAD_DOWN = 0x0002;
//const DWORD GAMEPAD_DPAD_LEFT = 0x0004;
//const DWORD GAMEPAD_DPAD_RIGHT = 0x0008;
//const DWORD GAMEPAD_START_BUTTON = 0x0010;
//const DWORD GAMEPAD_BACK_BUTTON = 0x0020;
//const DWORD GAMEPAD_LEFT_THUMB = 0x0040;
//const DWORD GAMEPAD_RIGHT_THUMB = 0x0080;
//const DWORD GAMEPAD_LEFT_SHOULDER = 0x0100;
//const DWORD GAMEPAD_RIGHT_SHOULDER = 0x0200;
//const DWORD GAMEPAD_A = 0x1000;
//const DWORD GAMEPAD_B = 0x2000;
//const DWORD GAMEPAD_X = 0x4000;
//const DWORD GAMEPAD_Y = 0x8000;

//struct ControllerState
//{
	//XINPUT_STATE        state;
	//XINPUT_VIBRATION    vibration;
	//float               vibrateTimeLeft;    // mSec
	//float               vibrateTimeRight;   // mSec
	//bool                connected;
//};


class Input
{
private:
	bool keysDown[inputNS::KEYS_ARRAY_LEN];		//Ư�� Ű�� ���� ��� true�� �ȴ�. 
	bool keysPressed[inputNS::KEYS_ARRAY_LEN];	//���ӷ����� �����ϴ� ���� Ư��Ű�� ������ �ִ� ��� true �� �ȴ�.

	std::string textIn;				// ����ڰ� �Է��� �ؽ�Ʈ
	char charIn;					//�������� �Է��� ����
	bool newLine;					// ���ο� ���� ������ �� true
	int mouseX, mouseY;				// ȭ�� �� ���콺 ��ǥ
	int mouseRawX, mouseRawY;		// ������ ���콺 ������
	int mouseWheel;

	RAWINPUTDEVICE Rid[1];			// ������ ���콺��
	bool mouseCaptured;				// ���콺�� ĸó�� ��� true
	bool mouseLButton;
	bool mouseMButton;
	bool mouseRButton;
	bool mouseX1Button;
	bool mouseX2Button;
	//ControllerState controllers[MAX_CONTROLLERS]; //��Ʈ�ѷ� ����  ( ##��Ʈ�ѷ��� ���� ���߿� �Ұ��� ��)


public:
	Input();
	virtual ~Input();

	void initialize(HWND hwnd, bool capture);

	void keyDown(WPARAM);
	void keyUp(WPARAM);
	void keyIn(WPARAM);
	bool isKeyDown(UCHAR vkey) const;
	bool wasKeyPressed(UCHAR vkey) const;
	bool anyKeyPressed() const;
	void clearKeyPress(UCHAR vkey);

	void clear(UCHAR what);
	void clearAll() { clear(inputNS::KEYS_MOUSE_TEXT); } // Ű, ���콺, �ؽ�Ʈ �Է� �����͸� �����.
	void clearTextIn() { textIn.clear(); }		// �ؽ�Ʈ �Է� ���۸� �����.
	void clearCharln() { charIn = 0; }

	std::string getTextIn() { return textIn; }	//�Է��� �ؽ�Ʈ�� string ���� ��ȯ�Ѵ�.
	char getCharIn() { return charIn; }			//�������� �Է��� ���ڸ� ��ȯ�Ѵ�.

	void mouseIn(LPARAM);
	void mouseRawIn(LPARAM);
	void mouseWheelIn(WPARAM);

	void setMouseLButton(bool b) { mouseLButton = b;}
	void setMouseMButton(bool b) { mouseMButton = b;}
	void setMouseRButton(bool b) { mouseRButton = b;}
	void setMouseXButton(WPARAM wParam)
	{
		mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
		mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
	}

	//���콺 x,y ��ġ
	int getMouseX() const { return mouseX; }
	int getMouseY() const { return mouseY; }
	// ó������ ���� ���콺 x, y ������, (<0�̸� ����, >0�̸� ������. ������ ���콺�� ȣȯ��)
	int getMouseRawX() const { return mouseRawX; }
	int getMouseRawY() const { return mouseRawY; }
	//
	bool getMouseLButton() const { return mouseLButton; }
	bool getMouseMButton() const { return mouseMButton; }
	bool getMouseRButton() const { return mouseRButton; }
	bool getMouseX1Button() const { return mouseX1Button; }
	bool getMouseX2Button() const { return mouseX2Button; }
};

#endif