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
	Game::initialize(hwnd);

	if (!nebulaTexture.initialize(graphics, NEBULA_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

	//�༺ �ؽ�ó
	if (!planetTexture.initialize(graphics, PLANET_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet texture"));

	//����
	if (!nebula.initialize(graphics, 0, 0, 0, &nebulaTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialzing nebula"));

	//�༺
	if (!planet.initialize(graphics, 0, 0, 0, &planetTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initialzing planet"));

	//�༺�� ȭ�� �߾ӿ� ��ġ�Ѵ�.
	planet.setX(GAME_WIDTH * 0.5f - planet.getWidth() * 0.5f);
	planet.setY(GAME_HEIGHT * 0.5f - planet.getHeight() * 0.5f);

	return;
}

// ��� ���� �������� �����Ѵ�.
void Spacewar::update()
{

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

	graphics->spriteEnd();
}

//�׷��� ����̽��� �ν�Ʈ ���°� ���� ��, ����� ��� ���� �޸𸮸� �����ϰ� �׷��� ����̽��� ������ �� �ְ� �Ѵ�.
void Spacewar::releaseAll()
{
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

	Game::resetAll();
	return;
}