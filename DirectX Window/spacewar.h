// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 5 spacewar.h v1.0

#ifndef _SPACEWAR_H
#define _SPACEWAR_H
#define _WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"

class Spacewar : public Game
{
private:
	//게임 아이템
	TextureManager nebulaTexture;
	TextureManager planetTexture;
	TextureManager shipTexture;

	Image planet;
	Image nebula;
	Image ship;              

public :
	Spacewar();
	virtual ~Spacewar();

	//게임 초기화
	void initialize(HWND hwnd);
	
	// 순수 가상함수
	void update();
	void ai();
	void collisions();
	void render();

	//
	void releaseAll();
	void resetAll();
};

#endif