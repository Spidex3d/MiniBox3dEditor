#pragma once
#include <string>
#include <algorithm>
#include <vector>

#include <guiWindow.h>
#include <boXGL.h>
#include <guiWinDeffs.h>
#include <boXM\boXM.h>

struct UIwindow
{
    int UIid = 0;
    std::string title;
    bool* isOpen = nullptr;

    float x = 100.0f;
    float y = 100.0f;
    float width = 300.0f;
    float height = 200.0f;

    bool isHovered = false;
    bool isActive = false;
};

class guiUI
{
public:
    guiUI() = default;
    ~guiUI() = default;

    void UInewFrame(guiWin& app, guiWin::gui_window* window, boXGL& gl);

    void UIbegin(const char* title, bool* p_open = nullptr, int UIid = 0);

    void End();

    bool UIbutton(const char* label, float x, float y, float width, float height);

private:
    guiWin* m_app = nullptr;
    guiWin::gui_window* m_window = nullptr;
    boXGL* m_gl = nullptr;

    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;

    bool m_mouseDown = false;
    bool m_mousePressed = false;
    bool m_mouseReleased = false;

    UIwindow* m_currentWindow = nullptr;
    std::vector<UIwindow> m_windows;

private:
    bool IsMouseInside(float x, float y, float width, float height) const;

    UIwindow* FindOrCreateWindow(const char* title, bool* p_open, int UIid);
};
