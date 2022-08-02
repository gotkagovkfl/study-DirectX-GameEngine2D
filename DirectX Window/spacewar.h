#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define _WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

class Spacewar : public Game
{
private:
	//���� ������
	TextureManager nebulaTexture;
	TextureManager planetTexture;
	Image planet;
	Image nebula;

public :
	Spacewar();
	virtual ~Spacewar();

	//���� �ʱ�ȭ
	void initialize(HWND hwnd);
	
	// ���� �����Լ�
	void update();
	void ai();
	void collisions();
	void render();

	//
	void releaseAll();
	void resetAll();
};

#endif