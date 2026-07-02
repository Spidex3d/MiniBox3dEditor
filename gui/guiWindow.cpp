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
    if (!window || keycode < 0 || keycode >= 256)
        return GLWIN_RELEASE;

    return window->keys[keycode] ? GLWIN_PRESS : GLWIN_RELEASE;
}

int guiWin::guiGetKeyPressed(gui_window* window, int keycode)
{
    if (!window || keycode < 0 || keycode >= 256)
        return GLWIN_RELEASE;

    return window->keysPressed[keycode] ? GLWIN_PRESS : GLWIN_RELEASE;
}

int guiWin::guiGetKeyReleased(gui_window* window, int keycode)
{
    if (!window || keycode < 0 || keycode >= 256)
        return GLWIN_RELEASE;

    return window->keysReleased[keycode] ? GLWIN_PRESS : GLWIN_RELEASE;
}

void guiWin::guiGetCursorPos(gui_window* window, double* xpos, double* ypos)
{
    if (!window || !xpos || !ypos)
        return;

    *xpos = window->mouseX;
    *ypos = window->mouseY;
}

void guiWin::guiGetMouseDelta(gui_window* window, double* dx, double* dy)
{
    if (!window || !dx || !dy)
        return;

    *dx = window->mouseDeltaX;
    *dy = window->mouseDeltaY;
}

int guiWin::guiGetMouseButton(gui_window* window, int button)
{
    if (!window || button < 0 || button > 2)
        return GLWIN_RELEASE;

    return window->mouseButtons[button] ? GLWIN_PRESS : GLWIN_RELEASE;
}

int guiWin::guiGetMouseButtonPressed(gui_window* window, int button)
{
    if (!window || button < 0 || button > 2)
        return GLWIN_RELEASE;

    return window->mouseButtonsPressed[button] ? GLWIN_PRESS : GLWIN_RELEASE;
}

int guiWin::guiGetMouseButtonReleased(gui_window* window, int button)
{
    if (!window || button < 0 || button > 2)
        return GLWIN_RELEASE;

    return window->mouseButtonsReleased[button] ? GLWIN_PRESS : GLWIN_RELEASE;
}

int guiWin::guiGetMouseWheel(gui_window* window)
{
    if (!window)
        return 0;

    return window->mouseWheelDelta;
}

void guiWin::guiEndFrame(gui_window* window)
{
    if (!window)
        return;

    for (int i = 0; i < 256; ++i)
    {
        window->keysPressed[i] = false;
        window->keysReleased[i] = false;
    }

    for (int i = 0; i < 3; ++i)
    {
        window->mouseButtonsPressed[i] = false;
        window->mouseButtonsReleased[i] = false;
    }

    window->mouseDeltaX = 0.0;
    window->mouseDeltaY = 0.0;
    window->mouseWheelDelta = 0;
   
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
    {
        if (window)
        {
            int key = static_cast<int>(wParam);

            if (key >= 0 && key < 256)
            {
                if (!window->keys[key])
                {
                    window->keysPressed[key] = true;
                }

                window->keys[key] = true;
            }
        }

        return 0;
    }

    case WM_KEYUP:
    {
        if (window)
        {
            int key = static_cast<int>(wParam);

            if (key >= 0 && key < 256)
            {
                window->keys[key] = false;
                window->keysReleased[key] = true;
            }
        }

        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (window)
        {
            double newX = static_cast<double>(GET_X_LPARAM(lParam));
            double newY = static_cast<double>(GET_Y_LPARAM(lParam));

            window->mouseDeltaX += newX - window->mouseX;
            window->mouseDeltaY += newY - window->mouseY;

            window->lastMouseX = window->mouseX;
            window->lastMouseY = window->mouseY;

            window->mouseX = newX;
            window->mouseY = newY;
        }

        return 0;
    }
    if (window)
    {
        if (!window->mouseButtons[GLWIN_MOUSE_BUTTON_LEFT])
            window->mouseButtonsPressed[GLWIN_MOUSE_BUTTON_LEFT] = true;

        window->mouseButtons[GLWIN_MOUSE_BUTTON_LEFT] = true;
        SetCapture(hwnd);
    }

        return 0;
    

    case WM_LBUTTONUP:

    if (window)
    {
        window->mouseButtons[GLWIN_MOUSE_BUTTON_LEFT] = false;
        window->mouseButtonsReleased[GLWIN_MOUSE_BUTTON_LEFT] = true;
        ReleaseCapture();
    }

        return 0;

    case WM_RBUTTONDOWN:
        if (window) window->mouseButtons[GLWIN_MOUSE_BUTTON_RIGHT] = true;
        break;
    case WM_RBUTTONUP:
        if (window) window->mouseButtons[GLWIN_MOUSE_BUTTON_RIGHT] = false;
        break;
    case WM_MBUTTONDOWN:
        if (window) window->mouseButtons[GLWIN_MOUSE_BUTTON_MIDDLE] = true;
        break;
    case WM_MBUTTONUP:
        if (window) window->mouseButtons[GLWIN_MOUSE_BUTTON_MIDDLE] = false;
        break;
    case WM_MOUSEWHEEL:
    {
        if (window)
        {
            int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            window->mouseWheelDelta += wheelDelta / WHEEL_DELTA;

        }

        return 0;
    }
	default:
		break;
    }   

    return DefWindowProc(hwnd, msg, wParam, lParam);
}


