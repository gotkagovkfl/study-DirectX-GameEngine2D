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
//Image ��ü�� �ʱ�ȭ�Ѵ�.
// Graphics�� ����Ű�� ������
// �̹����� �ȼ� ���� �� (0 = �ؽ�ó ��ü�� ���� ���)
// �̹����� �ȼ� ���� ���� ( 0 = �ؽ�ó ��ü�� ���̸� ���)
// �ؽ�ó�� �� �� (1 ~ n) (0 �� 1�� ����)
//TextureManager�� ����Ű�� ������
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

		// currentFrame�� �׸��� ���� spriteData.rect�� �����Ѵ�.
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		// ������ �𼭸� +1
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols) * spriteData.height;
		// �Ʒ��� �𼭸� +1
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch (...) { return false; }
	initialized = true;
	return true;
}

//=================================================================================
//color�� ���ͷ� ����� �̹����� �׸���.
//color �Ű������� �������ε�, image.h�� �ִ� ����� �⺻ ������ �Ҵ�ȴ�.
// ����:  spriteBegin()�� ȣ��ȴ�.
// ���� : spriteEnd()�� ȣ��ȴ�.
//==================================================================================
void Image::draw(COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	
	// onReset()�� ȣ��� ��츦 ����� ���ο� �ؽ�ó�� �����´�.
	spriteData.texture = textureManager->getTexture();

	if (color == graphicsNS::FILTER)
		graphics->drawSprite(spriteData, colorFilter);
	else
		graphics->drawSprite(spriteData, color);
}

//=================================================================================
// ������ SprtieData�� ����� �̹����� �׸���. 
// ���� SpriteData.rect�� �ؽ�ó�� �����ϴ� �� ���ȴ�. 
// ����:  spriteBegin()�� ȣ��ȴ�.
// ���� : spriteEnd()�� ȣ��ȴ�.
//==================================================================================
void Image::draw(SpriteData sd, COLOR_ARGB color)
{
	if (!visible || graphics == NULL)
		return;
	// �ؽ�ó�� �����ϱ� ���� �̹����� rect�� ����Ѵ�.
	sd.rect = spriteData.rect;

	// onReset()�� ȣ��� ��츦 ����� ���ο� �ؽ�ó�� �����´�.
	sd.texture = textureManager->getTexture();

	if (color == graphicsNS::FILTER)
		graphics->drawSprite(spriteData, colorFilter);
	else
		graphics->drawSprite(spriteData, color);
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
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
// Set the current frame of the image
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
//  Set spriteData.rect to draw currentFrame
//=============================================================================
inline void Image::setRect()
{
	// configure spriteData.rect to draw currentFrame
	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	// right edge + 1
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height;
	// bottom edge + 1
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
}


