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


    float titleBarHeight = 28.0f;
    float closeButtonSize = 22.0f;
  
    bool dragging = false;
    float dragOffsetX = 0.0f;
    float dragOffsetY = 0.0f;
};

//class guiUIWidgets;

class guiUI
{
public:

UIwindow UIctx;

    guiUI() = default;
    ~guiUI() = default;

    void UInewFrame(guiWin& app, guiWin::gui_window* window, boXGL& gl);

    
    void UISetNextWindowPos(float x, float y);
    void UISetNextWindowSize(float width, float height);

    void UIbegin(const char* title, bool* p_open = nullptr, int UIid = 0);

    void End();
    bool WidgetButton(const char* label, float x, float y, float width, float height);
    // used for scene collection
    bool WidgetTreeNode(const char* label, bool& expanded, bool selected, float x, float y, float width, float height);

    void WidgetLabel(const char* text, float x, float y);
    //bool WidgetColorBoxLabel(const char* label, float* r, float* g, float* b, float w = 40.0f, float h = 20.0f);

    //bool WidgetInputText(const char* label, char* buf, size_t buf_size, float w, float h);

    //bool WidgetMultiLineText(const char* label, std::string& text, float w, float h);

    //void WidgetDrag3FloatText(float* v, float wBox = 60, float hBox = 20);

    //void WidgetDrag3Float(float* v, float wBox = 60, float hBox = 20);
    //
    //void WidgetDrag4FloatText(float* v, float wBox = 60, float hBox = 20);

    //void WidgetDrag4Float(float* v, float wBox = 60, float hBox = 20); // no text

    //void WidgetSlider();

    //bool WidgetColoredLabel(float r, float gcol, float b, const char* txt);

    //bool WidgetSeparatorLine(float w);

    //void WidgetSeparatorText(const char* label, float w);

    //void WidgetameLine(float spacing = -1.0f);

    //void WidgetSpacing();

    bool WidgetImageButton(const std::string& texPath, float w, float h);

    //bool WidgetImageBox(const std::string& texPath, float w, float h, const char* label = nullptr);

    //bool WidgetColorBox(const char* label, float* r, float* g, float* b, float w = 40.0f, float h = 40.0f);

    //bool WidgetColorSVSquare(float hue, float& sat, float& val, float x, float y, float size);
    //bool WidgetHueSlider(float& hue, float x, float y, float w, float h);

    //bool WidgetColorEdit3(const char* label, float col[3]);

    //bool WidgetColorEdit4(const char* label, float col[4]);



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

    bool m_hasNextWindowPos = false;
    bool m_hasNextWindowSize = false;

    float m_nextWindowX = 100.0f;
    float m_nextWindowY = 100.0f;

    float m_nextWindowWidth = 300.0f;
    float m_nextWindowHeight = 200.0f;
};
