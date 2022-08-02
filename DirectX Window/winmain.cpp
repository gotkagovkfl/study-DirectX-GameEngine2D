#define _CRTDBG_MAP_ALLOC
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "spacewar.h"

// 함수 프로토타입
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int); 
bool CreateMainWindow(HWND &, HINSTANCE, int);
LRESULT WINAPI WinProc(HWND, UINT, WPARAM, LPARAM); 

// Game pointer
Spacewar *game = NULL;
HWND hwnd = NULL;

// 전역 변수
HINSTANCE hinst;


//=============================================================================
// Starting point for a Windows application
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 디버그 모드로 빌드 시 메모리 누수 확인
    #if defined (DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    MSG	 msg;

    game = new Spacewar;

    // Create the window
    if (!CreateMainWindow(hwnd, hInstance, nCmdShow))
        return 1;

    try
    {
        game->initialize(hwnd);

        // 메인 메세지 루프
        int done = 0;
        while (!done)
        {
            // PeekMessage는 윈도우 메세지를 확인하는 논블로킹 메소드다.
            if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                // 종료메세지를 찾는다. 
                if (msg.message == WM_QUIT)
                    done = 1;

                //해석한 후 메세지를 WinProc에 전달함.
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } 
            else
                game->run(hwnd);
        }
        SAFE_DELETE(game);
        return msg.wParam;
    }
    catch (const GameError& err)
    {
        game->deleteAll();
        DestroyWindow(hwnd);
        MessageBox(NULL, err.getMessage(), "Error", MB_OK);
    }
    catch (...)
    {
        game->deleteAll();
        DestroyWindow(hwnd);
        MessageBox(NULL, "Unknown error occurred in game.", "Error", MB_OK);
    }
    SAFE_DELETE(game);
    return 0;
}

//=============================================================================
// window event callback function
//=============================================================================
LRESULT WINAPI WinProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    return (game->messageHandler(hwnd, msg, wParam, lParam));
}

//=============================================================================
// Create the window
// Returns: false on error
//=============================================================================
bool CreateMainWindow(HWND &hwnd, HINSTANCE hInstance, int nCmdShow) 
{ 
    WNDCLASSEX wcx; 
 
    // Fill in the window class structure with parameters 
    wcx.cbSize = sizeof(wcx);           // size of structure 
    wcx.style = CS_HREDRAW | CS_VREDRAW;    // redraw if size changes 
    wcx.lpfnWndProc = WinProc;          // points to window procedure 
    wcx.cbClsExtra = 0;                 // no extra class memory 
    wcx.cbWndExtra = 0;                 // no extra window memory 
    wcx.hInstance = hInstance;          // handle to instance 
    wcx.hIcon = NULL; 
    wcx.hCursor = LoadCursor(NULL,IDC_ARROW);   // predefined arrow 
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);    // black background brush 
    wcx.lpszMenuName =  NULL;           // name of menu resource 
    wcx.lpszClassName = CLASS_NAME;     // name of window class 
    wcx.hIconSm = NULL;                 // small class icon 
 
    // Register the window class. 
    // RegisterClassEx returns 0 on error.
    if (RegisterClassEx(&wcx) == 0) // if error
        return false; 

    //전체화면이나 창 화면 설정
    DWORD style;
    if (FULLSCREEN)
        style = WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP;
    else 
        style = WS_OVERLAPPEDWINDOW; 

    // Create window
    hwnd = CreateWindow(
        CLASS_NAME,             // name of the window class
        GAME_TITLE,              // title bar text
        style,    // window style
        CW_USEDEFAULT,          // default horizontal position of window
        CW_USEDEFAULT,          // default vertical position of window
        GAME_WIDTH,           // width of window
        GAME_HEIGHT,          // height of the window
        (HWND) NULL,            // no parent window
        (HMENU) NULL,           // no menu
        hInstance,              // handle to application instance
        (LPVOID) NULL);         // no window parameters

    // if there was an error creating the window
    if (!hwnd)
        return false;

    if (!FULLSCREEN)
    {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        MoveWindow(hwnd, 0, 0, GAME_WIDTH + (GAME_WIDTH - clientRect.right), GAME_HEIGHT + (GAME_HEIGHT - clientRect.bottom), TRUE);
    }

    // Show the window
    ShowWindow(hwnd, nCmdShow);

    //// Send a WM_PAINT message to the window procedure
    //UpdateWindow(hwnd);
    return true;
}