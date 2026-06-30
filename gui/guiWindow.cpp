#include "guiWindow.h"
#include <miniBoxLog.h>
#include <algorithm>
#include <limits>
#include <guiWinDeffs.h>

guiWin::guiWin()
{
}

guiWin::~guiWin()
{
}

guiWin::gui_window* guiWin::guiCreateWindow(
    int width,
    int height,
    const wchar_t* title)
{
    gui_window* window = new gui_window();

    window->width = width;
    window->height = height;
    window->instance = GetModuleHandle(nullptr);
    //########
    // Create our software framebuffer
    window->pixels.resize(width * height);
	// depth buffer for 3D rendering
    window->depthBuffer.resize(width * height);

    // Set up bitmap info for StretchDIBits
    window->bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    window->bitmapInfo.bmiHeader.biWidth = width;
    window->bitmapInfo.bmiHeader.biHeight = -height;
    window->bitmapInfo.bmiHeader.biPlanes = 1;
    window->bitmapInfo.bmiHeader.biBitCount = 32;
    window->bitmapInfo.bmiHeader.biCompression = BI_RGB;

    //#########

    const wchar_t* className = L"MiniBoxEditorWindowClass";

    WNDCLASS wc{};
    wc.lpfnWndProc = guiWin::WindowProc;
    wc.hInstance = window->instance;
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    DWORD windowStyle = WS_OVERLAPPEDWINDOW;

    RECT rect{};
    rect.left = 0;
    rect.top = 0;
    rect.right = width;
    rect.bottom = height;

    AdjustWindowRect(&rect, windowStyle, FALSE);

    int realWidth = rect.right - rect.left;
    int realHeight = rect.bottom - rect.top;

    HWND hwnd = CreateWindowEx(
        0,
        className,
        title,
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        realWidth,
        realHeight,
        nullptr,
        nullptr,
        window->instance,
        window);

    if (!hwnd)
    {
        delete window;
        return nullptr;
    }

    window->hwnd = hwnd;

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    return window;
}

void guiWin::guiMakeContextCurrent(gui_window* window)
{
    // For now, this does nothing.
    // Later, this could be where you attach your software framebuffer
    // or any rendering context you create yourself.

    (void)window;
}

bool guiWin::guiWindowShouldClose(gui_window* window)
{
    if (!window)
        return true;

    return window->shouldClose;
}

void guiWin::guiPollEvents(gui_window* window)
{
    MSG msg{};

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            if (window)
                window->shouldClose = true;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
// Depth buffer management for 3D rendering
void guiWin::boXGLClearDepthBuffer(gui_window* window)
{
	if (!window)
		return;
	std::fill(window->depthBuffer.begin(), window->depthBuffer.end(), std::numeric_limits<float>::infinity());
}
// Pixel drawing with depth testing
void guiWin::boXGLDrawPixelDepth(gui_window* window, int x, int y, float depth, Vec3 colour)
{
	if (!window)
		return;
	if (x < 0 || x >= window->width || y < 0 || y >= window->height)
		return;
	int index = y * window->width + x;
	// Depth test
	if (depth < window->depthBuffer[index])
	{
		window->depthBuffer[index] = depth;
		unsigned int pixelColour = Vec3ToColour(colour);
		window->pixels[index] = pixelColour;
	}

}

void guiWin::guiPresent(gui_window* window)
{
    if (!window || !window->hwnd)
        return;

    HDC hdc = GetDC(window->hwnd);

    StretchDIBits(
        hdc,
        0,
        0,
        window->width,
        window->height,
        0,
        0,
        window->width,
        window->height,
        window->pixels.data(),
        &window->bitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY);

    ReleaseDC(window->hwnd, hdc);
}

void guiWin::guiSetWindowIcon(gui_window* window, const wchar_t* iconPath)
{
    if (!window || !window->hwnd) return;
    HICON hIcon = (HICON)LoadImage(
        nullptr, iconPath, IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE
    );
    if (hIcon) {
        SendMessage(window->hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(window->hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
}

int guiWin::guiGetKeys(gui_window* window, int keycode)
{
	
    if (!window) return GLWIN_RELEASE;
    auto it = window->keyState.find(keycode);
    return (it != window->keyState.end() && it->second) ? GLWIN_PRESS : GLWIN_RELEASE;
}

void guiWin::guiDestroyWindow(gui_window* window)
{
    if (!window)
        return;

    if (window->hwnd)
    {
        DestroyWindow(window->hwnd);
        window->hwnd = nullptr;
    }

    delete window;
}

void guiWin::guiTerminate()
{
    // Nothing needed yet.
    // Later you could clean up global resources here.
}

LRESULT CALLBACK guiWin::WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam)
{
    gui_window* window = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* createStruct =
            reinterpret_cast<CREATESTRUCT*>(lParam);

        window =
            reinterpret_cast<gui_window*>(createStruct->lpCreateParams);

        SetWindowLongPtr(
            hwnd,
            GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(window));
    }
    else
    {
        window =
            reinterpret_cast<gui_window*>(
                GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    switch (msg)
    {
    case WM_CLOSE:
    {
        if (window)
            window->shouldClose = true;

        return 0;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_KEYDOWN:
        if (window) window->keyState[(int)wParam] = true;
        return 0;
    case WM_KEYUP:
        if (window) window->keyState[(int)wParam] = false;
        return 0;

    default:
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


