#ifndef _GAME_H
#define _GAME_H
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "gameError.h"

class Game
{
protected:
	// ���� ���� �Ӽ�
	Graphics* graphics;
	Input* input;
	HWND hwnd;
	HRESULT hr;
	LARGE_INTEGER timeStart;
	LARGE_INTEGER timeEnd;
	LARGE_INTEGER timerFreq;
	float frameTime;
	float fps;
	DWORD sleepTime;
	bool paused;
	bool initialized;

public :
	Game();
	virtual ~Game();

	// ������ �޼��� �ڵ鷯
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ������ �ʱ�ȭ�Ѵ�.
	virtual void initialize(HWND hwnd);
	
	// WinMain ���� �޼��� �������� �ݺ������� run �Լ��� ȣ��ȴ�.
	virtual void run(HWND);

	//�׷��� ����̽��� �ν�Ʈ ���°� ���� �� ȣ���Ѵ�. 
	//����� ��� ���� �޸𸮸� �����ϰ� �׷��� ����̽��� �����Ѵ�.
	virtual void releaseAll();

	// ��� ǥ���� ������ϰ� ��� ��ü�� �����Ѵ�.
	virtual void resetAll();

	// ����� ��� �޸𸮸� �����Ѵ�.
	virtual void deleteAll();

	// ���� �������� �������Ѵ�.
	virtual void renderGame();

	// �ν�Ʈ ���°� �� �׷��� ����̽��� ó���Ѵ�.
	virtual void handleLostGraphicsDevice();

	// Set display mode (fullscreen, window or toggle)
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	// Graphics ��ü�� ����Ű�� �����͸� ��ȯ�Ѵ�.
	Graphics* getGraphics() { return graphics; }

	// Input ��ü�� ����Ű�� �����͸� ��ȯ�Ѵ�.
	Input* getInput() { return input; }

	//������ �����Ѵ�.
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// --���� �����Լ� ����
	// ���� �������� �����Ѵ�.
	virtual void update() = 0;

	// AI ����� �����Ѵ�.
	virtual void ai() = 0;

	//�浹�� Ȯ���Ѵ�.
	virtual void collisions() = 0;

	//�׷����� �������Ѵ�.
	virtual void render() = 0;
};

#endif
