#include "graphics.h"

Graphics::Graphics()
{
	direct3d = NULL;
	device3d = NULL;
	sprite = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
	backColor = graphicsNS::BACK_COLOR;
}

Graphics::~Graphics()
{
	releaseAll();
}


//releaseAll
void Graphics::releaseAll()
{
	SAFE_RELEASE(sprite); 
	SAFE_RELEASE(device3d);
	SAFE_RELEASE(direct3d);
}


void Graphics::initialize(HWND hw, int w, int h, bool full)
{
	hwnd = hw;
	width = w;
	height = h;
	fullscreen = full;

	// Direct3d 초기화 
	direct3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct3d == NULL)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Direct3D"));

	initD3Dpp();
	if (fullscreen)
	{
		if (isAdapterCompatible())
			d3dpp.FullScreen_RefreshRateInHz = pMode.RefreshRate;
		else 
			throw(GameError(gameErrorNS::FATAL_ERROR, "The graphics device does not support the specified resolution and/or format."));
	}

	// 그래픽 카드가 텍스처, 라이팅, 정점 셰이더를 지원하는지 확인한다.
	D3DCAPS9 caps;
	DWORD behavior;
	result = direct3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
	
	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 || caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else 
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	// Direct3d 디바이스 생성
	result = direct3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		behavior,
		&d3dpp,
		&device3d
	);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3d device"));
	
	result = D3DXCreateSprite(device3d, &sprite);
	if (FAILED(result))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error creating Direct3D sprite"));
}

void Graphics::initD3Dpp()
{
	try
	{
		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen)
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; 
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.Windowed = (!fullscreen);
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing D3D presentation parameters"));
	}
}

HRESULT Graphics::showBackBuffer()
{
	//result = E_FAIL;

	//device3d->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 255, 0), 0.0f, 0);

	result = device3d->Present(NULL, NULL, NULL, NULL);
	return result;
}

// Load the texture into default D3D memory (normal texture use)
HRESULT Graphics::loadTexture(const char* filename, COLOR_ARGB transcolor, UINT& width, UINT& height, LP_TEXTURE& texture)
{
	// The struct for reading file info
	D3DXIMAGE_INFO info;
	result = E_FAIL;

	try
	{
		if (filename == NULL)
		{
			texture = NULL;
			return D3DERR_INVALIDCALL;
		}

		// Get width and height from file
		result = D3DXGetImageInfoFromFile(filename, &info);
		if (result != D3D_OK)
			return result;
		
		width = info.Width;
		height = info.Height;

		result = D3DXCreateTextureFromFileEx(
			device3d,			//3D device
			filename,			//image filename
			info.Width,			//texture width
			info.Height,		//texture height
			1,					//mip-map levels (1 for no chain)
			0,					//usage
			D3DFMT_UNKNOWN,		//surface format (default)
			D3DPOOL_DEFAULT,	//memory class for the texture
			D3DX_DEFAULT,		//image filter
			D3DX_DEFAULT,		//mip filter
			transcolor,			//color key for transparency
			&info,				//bitmap file info (from loaded file)
			NULL,				//color palette
			&texture);			//destination texture
	}
	catch (...)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error in Graphics::loadTexture"));
	}
	return result;
}



bool Graphics::isAdapterCompatible()
{
	UINT modes = direct3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat);

	for (UINT i = 0; i < modes; i++)
	{
		result = direct3d->EnumAdapterModes(D3DADAPTER_DEFAULT, d3dpp.BackBufferFormat, i, &pMode);
		
		if (pMode.Height == d3dpp.BackBufferHeight &&
			pMode.Width == d3dpp.BackBufferWidth &&
			pMode.RefreshRate >=d3dpp.FullScreen_RefreshRateInHz)
			return true;
	}
	return false;
}

// Draw the sprite described in SpriteData structure
void Graphics::drawSprite(const SpriteData& spriteData, COLOR_ARGB color)
{
	if (spriteData.texture == NULL)
		return;
	// 스프라이트의 중심을 찾는다. 
	D3DXVECTOR2 spriteCenter = D3DXVECTOR2((float)(spriteData.width / 2 * spriteData.scale), (float)(spriteData.height / 2 * spriteData.scale));

	// 스프라이트의 화면 위치
	D3DXVECTOR2 translate = D3DXVECTOR2((float)spriteData.x, (float)spriteData.y);

	// x, y 크기 조정
	D3DXVECTOR2 scaling(spriteData.scale, spriteData.scale);

	// 수평으로 뒤집은 스프라이트의 위치 조정
	if (spriteData.flipHorizontal)
	{
		scaling.x *= -1;
		// 뒤집은 이미지의 중심을 가져온다. 
		spriteCenter.x -= (float)(spriteData.width * spriteData.scale);
		translate.x += (float)(spriteData.width * spriteData.scale);
	}

	// 직으로 뒤집은 스프라이트의 위치 조정
	if (spriteData.flipVertical)
	{
		scaling.y *= -1;
		// 뒤집은 이미지의 중심을 가져온다. 
		spriteCenter.y -= (float)(spriteData.height * spriteData.scale);
		translate.y += (float)(spriteData.height * spriteData.scale);
	}

	// 스프라이트를 회전, 크기 조정, 배치하기 위한 행렬을 생성한다.
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(
		&matrix,	// 행렬
		NULL,		// 크기를 조정할 때 기준을 왼쪽 상다능로 유지
		0.0f,		// 크기 조정 회전 없음
		&scaling,	// 크기 조정 값
		&spriteCenter,	// 회전 중심
		(float)(spriteData.angle),	//회전 각도
		&translate	// x,y 위치
	);

	//스프라이트에게 행렬에 대해 알려준다.
	sprite->SetTransform(&matrix);

	// 스프라이트를 그린다.
	sprite->Draw(spriteData.texture, &spriteData.rect, NULL, NULL, color);
}

//로스트 상태가 된 디바이스를 확인
HRESULT Graphics::getDeviceState()
{
	result = E_FAIL;
	if (device3d == NULL)
		return result;
	
	result = device3d->TestCooperativeLevel();
	return result;
}


//그래픽 디바이스를 리셋한다.
HRESULT Graphics::reset()
{
	result = E_FAIL;

	initD3Dpp();
	sprite->OnLostDevice();
	result = device3d->Reset(&d3dpp);
	
	sprite->OnResetDevice();
	return result;
}

// Toggle window or fullscreen mode
void Graphics::changeDisplayMode(graphicsNS::DISPLAY_MODE mode)
{
	try
	{
		switch (mode)
		{
		case graphicsNS::FULLSCREEN:
			if (fullscreen)
				return;
			fullscreen = true; break;
		case graphicsNS::WINDOW:
			if (fullscreen == false)
				return;
			fullscreen = false; break;
		default:
			fullscreen = !fullscreen;
		}

		reset();

		if (fullscreen) // fullscreen
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP);
		}
		else // windowed
		{
			SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
			SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

			// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
			RECT clientRect;
			GetClientRect(hwnd, &clientRect); // get size of client area of window
			MoveWindow(hwnd, 
						0,													//Left
						0,													//Top
						GAME_WIDTH + (GAME_WIDTH - clientRect.right),		//Right
						GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom),	//Bottom
						TRUE);												//Repaint the window
		}
	}
	catch (...)
	{
		// An error occured, try windowed mode 
		SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowPos(hwnd, HWND_TOP, 0, 0, GAME_WIDTH, GAME_HEIGHT, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

		// Adjust window size so client area is GAME_WIDTH x GAME_HEIGHT
		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		MoveWindow(hwnd,
					0,													// Left
					0,													// Top
					GAME_WIDTH + (GAME_WIDTH - clientRect.right),		// Right
					GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom),	// Bottom
					TRUE);												// Repaint the window
	}
}
