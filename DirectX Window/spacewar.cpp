// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Draw animated spaceship
// Chapter 5 spacewar.cpp v1.0
// This class is the core of the game

#include "spacewar.h"

//������
Spacewar::Spacewar() {}

// �Ҹ���
Spacewar::~Spacewar() 
{
	releaseAll(); // ��� �׷��� �����ۿ� ���� onLostDevice() �� ȣ���Ѵ�.
}

// ������ �ʱ�ȭ�Ѵ�. ������ �߻��� ��� GameError�� ������. 
void Spacewar::initialize(HWND hwnd)
{
	Game::initialize(hwnd); //throws GameError

	//���� �ؽ�ó
	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	//�༺ �ؽ�ó
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));

	//���ּ� �ؽ�ó
	if (!shipTexture.initialize(graphics, SHIP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship texture"));

	//----------------------------------------------------------------------------------------------

	//����
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialzing nebula"));

	//�༺
	if (!planet.initialize(graphics, 0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialzing planet"));
		//�༺�� ȭ�� �߾ӿ� ��ġ�Ѵ�.
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	// ���ּ�
	if (!ship.initialize(graphics, SHIP_WIDTH, SHIP_HEIGHT, SHIP_COLS, &shipTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship"));
	ship.setX(GAME_WIDTH / 4);                    // �༺�� ���� ��ܿ��� ������
	ship.setY(GAME_HEIGHT / 4);
	ship.setFrames(SHIP_START_FRAME, SHIP_END_FRAME);   // �ִϸ��̼� ������ ����
	ship.setCurrentFrame(SHIP_START_FRAME);     // ����������
	ship.setFrameDelay(SHIP_ANIMATION_DELAY);
	ship.setDegrees(45.0f);                     // ���ּ� ���� ����

	return;
}

// ��� ���� �������� �����Ѵ�.
void Spacewar::update()
{
	ship.update(frameTime);
}

// �ΰ�����
void Spacewar::ai()
{

}

// �浹ó��
void Spacewar::collisions()
{

}

// ���� �������� �������Ѵ�.
void Spacewar::render()
{
	graphics->spriteBegin();

	nebula.draw();
	planet.draw();
	ship.draw();

	graphics->spriteEnd();
}

//�׷��� ����̽��� �ν�Ʈ ���°� ���� ��, ����� ��� ���� �޸𸮸� �����ϰ� �׷��� ����̽��� ������ �� �ְ� �Ѵ�.
void Spacewar::releaseAll()
{
	shipTexture.onLostDevice();
	planetTexture.onLostDevice();
	nebulaTexture.onLostDevice();

	Game::releaseAll();
	return;
}

// �׷��� ����̽��� �����Ѵ�. ��� ǥ���� ������Ѵ�.
void Spacewar::resetAll()
{
	nebulaTexture.onResetDevice();
	planetTexture.onResetDevice();
	shipTexture.onResetDevice();

	Game::resetAll();
	return;
}