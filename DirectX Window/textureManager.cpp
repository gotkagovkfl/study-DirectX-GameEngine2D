#include "textureManager.h"

//=============================================================================
// default constructor
//=============================================================================
TextureManager::TextureManager()
{
	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;	// set true when successfully initialized
}

//=============================================================================
// destructor
//=============================================================================
TextureManager::~TextureManager()
{
	SAFE_RELEASE(texture);
}


// 디스크로부터 텍스처 파일을 불러온다. 
bool TextureManager::initialize(Graphics* g, const char* f)
{
	try
	{
		graphics = g; // Graphics 객체
		file = f;

		hr = graphics->loadTexture(file, graphicsNS::TRANSCOLOR, width, height, texture);
		if (FAILED(hr))
		{
			SAFE_RELEASE(texture);
			return false;
		}
	}
	catch (...) { return false; }

	initialized = true;
	return true;
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void TextureManager::onLostDevice()
{
	if (!initialized)
		return;
	SAFE_RELEASE(texture);
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void TextureManager::onResetDevice()
{
	if (!initialized)
		return;
	graphics->loadTexture(file, graphicsNS::TRANSCOLOR, width, height, texture);
}

