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
	// ########################### new dragging code
    bool titleBarHovered =
        IsMouseInside(
            win->x,
            win->y,
            win->width,
            win->titleBarHeight);

    // Start dragging when left button is first pressed on title bar
    if (titleBarHovered && m_mousePressed)
    {
        win->dragging = true;

        win->dragOffsetX = m_mouseX - win->x;
        win->dragOffsetY = m_mouseY - win->y;
    }

    // Stop dragging when left button is released
    if (m_mouseReleased)
    {
        win->dragging = false;
    }

    // Move while dragging
    if (win->dragging && m_mouseDown)
    {
        win->x = m_mouseX - win->dragOffsetX;
        win->y = m_mouseY - win->dragOffsetY;
    }

	// ##################################### end new dragging code

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
        1.0f,
        vec3(0.65f, 0.65f, 0.68f));

    m_gl->boXGLDrawFilledRect(
        m_window,
        static_cast<int>(win->x),
        static_cast<int>(win->y),
        static_cast<int>(win->width),
        static_cast<int>(win->titleBarHeight),
        vec3(0.24f, 0.24f, 0.30f));

   
}

void guiUI::End()
{
    m_currentWindow = nullptr;
}


bool guiUI::WidgetButton(const char* label, float x, float y, float width, float height)
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




/*if (!m_window || !m_gl || !m_currentWindow)
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

    return clicked;*/