#include "guiUI.h"

void guiUI::UInewFrame(
    guiWin& app,
    guiWin::gui_window* window,
    boXGL& gl)
{
    m_app = &app;
    m_window = window;
    m_gl = &gl;

    if (!m_window)
        return;

    double mx = 0.0;
    double my = 0.0;

    app.guiGetCursorPos(m_window, &mx, &my);

    m_mouseX = static_cast<float>(mx);
    m_mouseY = static_cast<float>(my);

    m_mouseDown =
        app.guiGetMouseButton(
            m_window,
            GLWIN_MOUSE_BUTTON_LEFT) == GLWIN_PRESS;

    m_mousePressed =
        app.guiGetMouseButtonPressed(
            m_window,
            GLWIN_MOUSE_BUTTON_LEFT) == GLWIN_PRESS;

    m_mouseReleased =
        app.guiGetMouseButtonReleased(
            m_window,
            GLWIN_MOUSE_BUTTON_LEFT) == GLWIN_PRESS;

    m_currentWindow = nullptr;
}

bool guiUI::IsMouseInside(
    float x,
    float y,
    float width,
    float height) const
{
    return
        m_mouseX >= x &&
        m_mouseX <= x + width &&
        m_mouseY >= y &&
        m_mouseY <= y + height;
}

UIwindow* guiUI::FindOrCreateWindow(
    const char* title,
    bool* p_open,
    int UIid)
{
    auto it = std::find_if(
        m_windows.begin(),
        m_windows.end(),
        [UIid](const UIwindow& w)
    {
        return w.UIid == UIid;
    });

    if (it == m_windows.end())
    {
        UIwindow newWindow;
        newWindow.UIid = UIid;
        newWindow.title = title;
        newWindow.isOpen = p_open;
        newWindow.x = 100.0f;
        newWindow.y = 100.0f;
        newWindow.width = 300.0f;
        newWindow.height = 200.0f;

        m_windows.push_back(newWindow);
        return &m_windows.back();
    }

    it->title = title;
    it->isOpen = p_open;

    return &(*it);
}

void guiUI::UIbegin(
    const char* title,
    bool* p_open,
    int UIid)
{
    if (!m_window || !m_gl)
        return;

    UIwindow* win = FindOrCreateWindow(title, p_open, UIid);

    if (!win)
        return;

    if (p_open && !(*p_open))
    {
        m_currentWindow = nullptr;
        return;
    }

    m_currentWindow = win;

    win->isHovered =
        IsMouseInside(
            win->x,
            win->y,
            win->width,
            win->height);

    m_gl->boXGLDrawFilledRect(
        m_window,
        static_cast<int>(win->x),
        static_cast<int>(win->y),
        static_cast<int>(win->width),
        static_cast<int>(win->height),
        vec3(0.16f, 0.16f, 0.18f));

    m_gl->boXGLDrawRect(
        m_window,
        static_cast<int>(win->x),
        static_cast<int>(win->y),
        static_cast<int>(win->width),
        static_cast<int>(win->height),
        2.0f,
        vec3(0.65f, 0.65f, 0.68f));

    m_gl->boXGLDrawFilledRect(
        m_window,
        static_cast<int>(win->x),
        static_cast<int>(win->y),
        static_cast<int>(win->width),
        28,
        vec3(0.24f, 0.24f, 0.30f));
}

void guiUI::End()
{
    m_currentWindow = nullptr;
}

bool guiUI::UIbutton(
    const char* label,
    float x,
    float y,
    float width,
    float height)
{
    if (!m_window || !m_gl || !m_currentWindow)
        return false;

    float screenX = m_currentWindow->x + x;
    float screenY = m_currentWindow->y + y;

    bool hovered =
        IsMouseInside(screenX, screenY, width, height);

    bool clicked = hovered && m_mousePressed;

    vec3 colour = hovered
        ? vec3(0.36f, 0.36f, 0.40f)
        : vec3(0.26f, 0.26f, 0.30f);

    if (hovered && m_mouseDown)
    {
        colour = vec3(0.18f, 0.18f, 0.22f);
    }

    m_gl->boXGLDrawFilledRect(
        m_window,
        static_cast<int>(screenX),
        static_cast<int>(screenY),
        static_cast<int>(width),
        static_cast<int>(height),
        colour);

    m_gl->boXGLDrawRect(
        m_window,
        static_cast<int>(screenX),
        static_cast<int>(screenY),
        static_cast<int>(width),
        static_cast<int>(height),
        1.0f,
        vec3(0.75f, 0.75f, 0.78f));

    return clicked;

}

//void guiUI::UInewFrame(guiWin::gui_window* window, float mouseX, float mouseY, bool down, bool pressed, bool released)
//{
//	UIctx.window = window;
//	UIctx.mouseX = mouseX;
//	UIctx.mouseY = mouseY;
//	UIctx.mouseDown = down;
//	UIctx.mousePressed = pressed;
//	UIctx.mouseReleased = released;
//
//	for (auto& w : UIwin)
//	{
//		w.mouseX = mouseX;
//		w.mouseY = mouseY;
//		w.mouseDown = down;
//		w.mousePressed = pressed;
//		w.mouseReleased = released;
//	}
//}
//
//void guiUI::UIbegin(const char* title, bool* p_open, int UIid)
//{
//	auto it = std::find_if(UIwin.begin(), UIwin.end(), [UIid](const UIwindow& w) { return w.UIid == UIid; });
//
//	if (!UIctx.window)
//		return;
//
//	if (it == UIwin.end()) {
//		UIwindow newWindow;
//		newWindow.UIid = UIid;
//		newWindow.title = title;
//		newWindow.isOpen = p_open;
//		newWindow.x = 100; // default position
//		newWindow.y = 100; // default position
//		newWindow.width = 300; // default size
//		newWindow.height = 200; // default size
//		UIwin.push_back(newWindow);
//	}
//	else {
//		it->title = title;
//		it->isOpen = p_open;
//	}
//
//	if (p_open && !(*p_open))
//	{
//		UIctx.currentWindow = nullptr;
//		return;
//	}
//
//	UIwindow& win = *it;
//	UIctx.currentWindow = &win;
//
//	win.isHovered =
//		UIctx.mouseX >= win.x &&
//		UIctx.mouseX <= win.x + win.width &&
//		UIctx.mouseY >= win.y &&
//		UIctx.mouseY <= win.y + win.height;
//
//	// Draw window background
//	boXGLUI.boXGLDrawFilledRect(
//		UIctx.window,
//		static_cast<int>(win.x),
//		static_cast<int>(win.y),
//		static_cast<int>(win.width),
//		static_cast<int>(win.height),
//		vec3(0.16f, 0.16f, 0.18f));
//
//	// Draw border
//	boXGLUI.boXGLDrawRect(
//		UIctx.window,
//		static_cast<int>(win.x),
//		static_cast<int>(win.y),
//		static_cast<int>(win.width),
//		static_cast<int>(win.height),
//		2.0f,
//		vec3(0.65f, 0.65f, 0.68f));
//
//	// Draw title bar
//	boXGLUI.boXGLDrawFilledRect(
//		UIctx.window,
//		static_cast<int>(win.x),
//		static_cast<int>(win.y),
//		static_cast<int>(win.width),
//		28,
//		vec3(0.24f, 0.24f, 0.30f));
//
//
//
//}
//
//void guiUI::UIend()
//{
//	UIctx.currentWindow = nullptr;
//}
//
//bool guiUI::UIbutton()
//{
//	// Placeholder for button logic
//	return true; // Return true if the button is pressed, false otherwise
//}
//
