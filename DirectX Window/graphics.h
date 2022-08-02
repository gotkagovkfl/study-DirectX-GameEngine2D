#ifndef _GRAPHICS_H
#define _GRAPHICS_H
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include "constants.h"
#include "gameError.h"

// DirectX 포인터 형식
#define LP_TEXTURE	LPDIRECT3DTEXTURE9
#define LP_SPRITE	LPD3DXSPRITE
#define LP_3DDEVICE LPDIRECT3DDEVICE9
#define LP_3D		LPDIRECT3D9

// 색상정의
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a, r, g, b) ((COLOR_ARGB)((((a)&0xff)<< 24) | (((r)&0xff)<<16 ) (((g)&0xff)<<8 )| ((b)&0xff)))

namespace graphicsNS
{
	// Some common colors
	// ARGB numbers range from 0 through 255
	// A = Alpha channel (transparency where 255 is opaque)
	// R = Red, G = Green, B = Blue
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB FILTER = D3DCOLOR_ARGB(0, 0, 0, 0);  // use to specify drawing with colorFilter
	const COLOR_ARGB ALPHA25 = D3DCOLOR_ARGB(64, 255, 255, 255);  // AND with color to get 25% alpha
	const COLOR_ARGB ALPHA50 = D3DCOLOR_ARGB(128, 255, 255, 255);  // AND with color to get 50% alpha
	const COLOR_ARGB BACK_COLOR = NAVY;                         // background color of game

	const COLOR_ARGB TRANSCOLOR = D3DCOLOR_ARGB(0, 255, 0, 255);

	enum DISPLAY_MODE { TOGGLE, FULLSCREEN, WINDOW };
}

// SpriteData: The properties required by Graphics::drawSprite to draw a sprite
struct SpriteData
{
	int         width;      // width of sprite in pixels
	int         height;     // height of sprite in pixels
	float       x;          // screen location (top left corner of sprite)
	float       y;
	float       scale;      // <1 smaller, >1 bigger
	float       angle;      // rotation angle in radians
	RECT        rect;       // used to select an image from a larger texture
	LP_TEXTURE  texture;    // pointer to texture
	bool        flipHorizontal; // true to flip sprite horizontally (mirror)
	bool        flipVertical;   // true to flip sprite vertically
};


class Graphics
{
private:
	// DirectX pointers and stuff
	LP_3D		direct3d;
	LP_3DDEVICE device3d;
	LP_SPRITE	sprite;
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE pMode;

	// other variables
	HRESULT		result;
	HWND		hwnd;
	bool		fullscreen;
	int			width;
	int			height;
	COLOR_ARGB	backColor;

	// Initialize D3D presentation parameters
	void initD3Dpp();

public:
	Graphics();
	virtual ~Graphics();

	// Releases direct3d and device3d.
	void releaseAll();

	// Initialize DirectX graphics
	void initialize(HWND hw, int width, int height, bool fullscreen);

	// Load the texture into default D3D memory (normal texture use)
	HRESULT loadTexture(const char* filename, COLOR_ARGB transcolor, UINT &width, UINT &height, LP_TEXTURE& texture);

	// Display the offscreen backbuffer to the screen.
	HRESULT showBackBuffer();

	// Checks the adapter to see if it is compatible with the BackBuffer height,
	bool isAdapterCompatible();

	// Draw the sprite described in SpriteData structure.
	void drawSprite(const SpriteData& spriteData, COLOR_ARGB color = graphicsNS::WHITE);

	//그래픽 디바이스를 리셋한다.
	HRESULT reset();

	// Toggle, fullscreen or window display mode
	void changeDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

	//getter ===========================================================
	LP_3D get3D() { return direct3d; }
	LP_3DDEVICE get3Ddevice() { return device3d; }
	LP_SPRITE	getSprite() { return sprite;  }
	HDC getDC() { return GetDC(hwnd); }

	// 로스트 상태가 된 디바이스를 확인
	HRESULT getDeviceState();  

	// is fullscreen
	bool getFullscreen() { return fullscreen; }
	
	//Set color used to clear screen
	void setBackColor(COLOR_ARGB c) { backColor = c; }

	//===========================================================
	//백버퍼를 지우고 DirectX의 BeginScene()을 호출한다.
	HRESULT beginScene()
	{
		result = E_FAIL;
		if (device3d == NULL) { return result; }

		// 백버퍼를 backColor로 지운다.
		device3d->Clear(0, NULL, D3DCLEAR_TARGET, backColor, 1.0f, 0);
		result = device3d->BeginScene();
		return result;
	}

	//DirectX의 EndScene()을 호출한다.
	HRESULT endScene()
	{
		result = E_FAIL;
		if (device3d) { result = device3d->EndScene(); }
		return result;
	}

	// Sprite Begin
	void spriteBegin()
	{
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}

	// Sprite End
	void spriteEnd()
	{
		sprite->End();
	}

};

#endif