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
// ########################################## Text rendering function (currently empty, to be implemented later)



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

void guiUI::UIbegin(const char* title, bool* p_open, int UIid)
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
    // close button
    float closeSize = win->closeButtonSize;

    float closeX = win->x + win->width - closeSize - 4.0f;
    float closeY = win->y + 3.0f;

    bool closeHovered =
        IsMouseInside(
            closeX,
            closeY,
            closeSize,
            closeSize);
	// if we click the close button, set the window to closed and return
    if (closeHovered && m_mousePressed)
    {
        if (p_open)
        {
            *p_open = false;
            m_currentWindow = nullptr;
            return;
        }
    }

    win->isHovered =  IsMouseInside(win->x, win->y, win->width, win->height);

	// ########################### new dragging code
    bool titleBarHovered =
        IsMouseInside(
            win->x,
            win->y,
            win->width,
            win->titleBarHeight);

    // Start dragging when left button is first pressed on title bar
    if (titleBarHovered && !closeHovered && m_mousePressed)
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

    m_gl->boXGLDrawText(
        m_window,
        static_cast<int>(win->x + 8),
        static_cast<int>(win->y + 8),
        win->title.c_str(),
        vec3(1.0f, 1.0f, 1.0f),
        2);


    //####################################### close button ############################################
    closeX = win->x + win->width - closeSize - 4.0f;
    closeY = win->y + 3.0f;

    closeHovered =
        IsMouseInside(
            closeX,
            closeY,
            closeSize,
            closeSize);

    vec3 closeColour = closeHovered
        ? vec3(0.85f, 0.20f, 0.20f)
        : vec3(0.45f, 0.12f, 0.12f);

    m_gl->boXGLDrawFilledRect(
        m_window,
        static_cast<int>(closeX),
        static_cast<int>(closeY),
        static_cast<int>(closeSize),
        static_cast<int>(closeSize),
        closeColour);

    m_gl->boXGLDrawRect(
        m_window,
        static_cast<int>(closeX),
        static_cast<int>(closeY),
        static_cast<int>(closeSize),
        static_cast<int>(closeSize),
        1.0f,
        vec3(0.95f, 0.95f, 0.95f));

    m_gl->boXGLDrawText(
        m_window,
        static_cast<int>(closeX + 6),
        static_cast<int>(closeY + 4),
        "X",
        vec3(1.0f, 1.0f, 1.0f),
        2);
   
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

    m_gl->boXGLDrawText(
        m_window,
        static_cast<int>(screenX + 8),
        static_cast<int>(screenY + 8),
        label,
        vec3(1.0f, 1.0f, 1.0f),
        2);

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