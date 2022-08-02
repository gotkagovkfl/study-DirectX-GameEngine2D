#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H
#define WIN32_LEAN_AND_MEAN

#include "graphics.h"
#include "constants.h"


class TextureManager
{
private:
	UINT		width;
	UINT		height;
	LP_TEXTURE	texture;
	const char	*file;
	Graphics	*graphics;
	bool		initialized;
	HRESULT		hr;

public: 
	TextureManager();
	virtual ~TextureManager();
	
	//텍스처를 가리키는 포인터를 반환한다. 
	LP_TEXTURE getTexture() const { return texture; }
	// 텍스처의 폭을 반환한다.
	UINT getWidth() const { return width; }
	// 텍스처의 높이를 반환한다.
	UINT getHeight() const { return height; }

	virtual bool initialize(Graphics *g, const char *file);

	virtual void onLostDevice();
	virtual void onResetDevice();
};

#endif