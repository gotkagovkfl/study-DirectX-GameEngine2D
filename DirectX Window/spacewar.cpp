#include "spacewar.h"

//생성자
Spacewar::Spacewar() {}

// 소멸자
Spacewar::~Spacewar() 
{
	releaseAll(); // 모든 그래픽 아이템에 대해 onLostDevice() 를 호출한다.
}

// 게임을 초기화한다. 에러가 발생할 경우 GameError를 던진다. 
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd);

	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	//행성 텍스처
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));

	//성운
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialzing nebula"));

	//행성
	if (!planet.initialize(graphics, 0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialzing planet"));

	//행성을 화면 중앙에 배치한다.
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	return;
}

// 모든 게임 아이템을 갱신한다.
void Spacewar::update()
{

}

// 인공지능
void Spacewar::ai()
{

}

// 충돌처리
void Spacewar::collisions()
{

}

// 게임 아이템을 렌더링한다.
void Spacewar::render()
{
	graphics->spriteBegin();

	nebula.draw();
	planet.draw();

	graphics->spriteEnd();
}

//그래픽 디바이스가 로스트 상태가 됐을 때, 예약된 모든 비디오 메모리를 해제하고 그래픽 디바이스를 리셋할 수 있게 한다.
void Spacewar::releaseAll()
{
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();

	Game::releaseAll();
	return;
}

// 그래픽 디바이스를 리셋한다. 모든 표면을 재생성한다.
void Spacewar::resetAll()
{
	nebulaTexture.onResetDevice();
	planetTexture.onResetDevice();

	Game::resetAll();
	return;
}