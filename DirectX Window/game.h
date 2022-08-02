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
	// 게임 공통 속성
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

	// 윈도우 메세지 핸들러
	LRESULT messageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// 게임을 초기화한다.
	virtual void initialize(HWND hwnd);
	
	// WinMain 메인 메세지 루프에서 반복적으로 run 함수가 호출된다.
	virtual void run(HWND);

	//그래픽 디바이스가 로스트 상태가 됐을 때 호출한다. 
	//예약된 모든 비디오 메모리를 해제하고 그래픽 디바이스를 리셋한다.
	virtual void releaseAll();

	// 모든 표면을 재생성하고 모든 개체를 리셋한다.
	virtual void resetAll();

	// 예약된 모든 메모리를 삭제한다.
	virtual void deleteAll();

	// 게임 아이템을 렌더링한다.
	virtual void renderGame();

	// 로스트 상태가 된 그래픽 디바이스를 처리한다.
	virtual void handleLostGraphicsDevice();

	// Set display mode (fullscreen, window or toggle)
	void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	// Graphics 객체를 가리키는 포인터를 반환한다.
	Graphics* getGraphics() { return graphics; }

	// Input 객체를 가리키는 포인터를 반환한다.
	Input* getInput() { return input; }

	//게임을 종료한다.
	void exitGame() { PostMessage(hwnd, WM_DESTROY, 0, 0); }

	// --순수 가상함수 선언
	// 게임 아이템을 갱신한다.
	virtual void update() = 0;

	// AI 계산을 수행한다.
	virtual void ai() = 0;

	//충돌을 확인한다.
	virtual void collisions() = 0;

	//그래픽을 렌더링한다.
	virtual void render() = 0;
};

#endif
