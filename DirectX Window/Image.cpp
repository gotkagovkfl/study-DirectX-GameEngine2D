// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Image.cpp v1.0

#include "Image.h"

//=============================================================================
// default constructor
//=============================================================================
Image::Image()
{
	initialized = false;            // set true when successfully initialized
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;       // used to select one frame from multi-frame image
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;      // the sprite texture (picture)
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0;               // default to 1 second per frame of animation
	animTimer = 0.0;
	visible = true;                 // the image is visible
	loop = true;                    // loop frames
	animComplete = false;
	graphics = NULL;                // link to graphics system
	colorFilter = graphicsNS::WHITE; // WHITE for no change
}

//=============================================================================
// destructor
//=============================================================================
Image::~Image()
{}

//===============================================================
//Image 객체를 초기화한다.
// Graphics를 가리키는 포인터
// 이미지의 픽셀 단위 폭 (0 = 텍스처 전체의 폭을 사용)
// 이미지의 픽셀 단위 높이 ( 0 = 텍스처 전체의 높이를 사용)
// 텍스처의 행 수 (1 ~ n) (0 은 1과 같다)
//TextureManager를 가리키는 포인터
//=================================================================
bool Image::initialize(Graphics* g, int width, int height, int ncols, TextureManager* textureM)
{
	try
	{
		graphics = g;
		textureManager = textureM;

		spriteData.texture = textureManager->getTexture();
		
		if (width == 0)
			width = textureManager->getWidth();
		spriteData.width = width;

		if (height == 0)
			height = textureManager->getHeight();
		spriteData.height = height;
		
		cols = ncols;
		if (cols == 0)
			cols = 1;

		// currentFrame을 그리기 위해 spriteData.rect를 구성한다.
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		// 오른쪽 모서리 +1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols) * spriteData.height;
		// 아래쪽 모서리 +1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch (...) { return false; }
	initialized = true;
	return true;
}

//=================================================================================
//color를 필터로 사용해 이미지를 그린다.
//color 매개변수는 선택적인데, image.h에 있는 흰색이 기본 값으로 할당된다.
// 이전:  spriteBegin()이 호출된다.
// 이후 : spriteEnd()가 호출된다.
//==================================================================================
void Image::draw(COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	
	// onReset()이 호출된 경우를 대비해 새로운 텍스처를 가져온다.
	spriteData.texture = textureManager->getTexture();

	if (color == graphicsNS::FILTER)
		graphics->drawSprite(spriteData, colorFilter);
	else
		graphics->drawSprite(spriteData, color);
}

//=================================================================================
// 지정한 SprtieData를 사용해 이미지를 그린다. 
// 현재 SpriteData.rect는 텍스처를 선택하는 데 사용된다. 
// 이전:  spriteBegin()이 호출된다.
// 이후 : spriteEnd()가 호출된다.
//==================================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	// 텍스처를 선택하기 위해 이미지의 rect를 사용한다.
	sd.rect = spriteData.rect;

	// onReset()이 호출된 경우를 대비해 새로운 텍스처를 가져온다.
	sd.texture = textureManager->getTexture();

	if (color == graphicsNS::FILTER)
		graphics->drawSprite(spriteData, colorFilter);
	else
		graphics->drawSprite(spriteData, color);
}

//=============================================================================
// update
// 일반적으로 프레임당 1번 호출
// frameTime 은 이동이나 애니메이션의 속도를 조절하는 데 사용됨.
//=============================================================================
void Image::update(float frameTime)
{
	if (endFrame - startFrame > 0)
	{
		animTimer += frameTime;
		if (animTimer > frameDelay)
		{
			animTimer -= frameDelay;
			currentFrame++;
			if (currentFrame < startFrame || currentFrame > endFrame)
			{
				if (loop == true)
					currentFrame = startFrame;
				else
				{
					currentFrame = endFrame;
					animComplete = true;
				}
			}
			setRect();
		}
	}
}

//=============================================================================
// 이미지의 현재 프레임 설정
//=============================================================================
void Image::setCurrentFrame(int c)
{
	if (c >= 0)
	{
		currentFrame = c;
		animComplete = false;
		setRect();                          // set spriteData.rect
	}
}

//=============================================================================
//  currentFrame에 그리게 spriteData.rect를 설정 
//=============================================================================
inline void Image::setRect()
{

	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	// 오른쪽 모서리 +1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;

	spriteData.rect.top = (currentFrame / cols) * spriteData.height;
	// 아래쪽 모서리 +1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}



