#include "boXGL.h"
#include <algorithm>
#include <cmath>


void boXGL::boXGLClearColor(guiWin::gui_window* window, unsigned char r, unsigned char g, unsigned char b)
{
    if (!window)
        return;

    unsigned int colour =
        static_cast<unsigned int>(b) |
        static_cast<unsigned int>(g) << 8 |
        static_cast<unsigned int>(r) << 16;

    std::fill(
        window->pixels.begin(),
        window->pixels.end(),
        colour);
}

void boXGL::boXGLDrawPixel(guiWin::gui_window* window, int x, int y, Vec3 colour)
{
    if (!window)
        return;

    if (x < 0 || x >= window->width ||
        y < 0 || y >= window->height)
    {
        return;
    }

    unsigned int pixelColour = Vec3ToColour(colour);

    window->pixels[y * window->width + x] = pixelColour;
}

void boXGL::boXGLDrawLine(guiWin::gui_window* window, int sx, int sy, int ex, int ey, float lineSize, Vec3 colour)
{
    if (!window)
        return;

    unsigned int pixelColour = Vec3ToColour(colour);

    int dx = std::abs(ex - sx);
    int sxStep = sx < ex ? 1 : -1;

    int dy = -std::abs(ey - sy);
    int syStep = sy < ey ? 1 : -1;

    int error = dx + dy;

    int x = sx;
    int y = sy;

    int thickness = static_cast<int>(lineSize);

    if (thickness < 1)
        thickness = 1;

    while (true)
    {
        for (int oy = -thickness / 2; oy <= thickness / 2; ++oy)
        {
            for (int ox = -thickness / 2; ox <= thickness / 2; ++ox)
            {
                int px = x + ox;
                int py = y + oy;

                if (px >= 0 && px < window->width &&
                    py >= 0 && py < window->height)
                {
                    window->pixels[py * window->width + px] = pixelColour;
                }
            }
        }

        if (x == ex && y == ey)
            break;

        int doubledError = error * 2;

        if (doubledError >= dy)
        {
            error += dy;
            x += sxStep;
        }

        if (doubledError <= dx)
        {
            error += dx;
            y += syStep;
        }
    }
}

void boXGL::boXGLDrawRect(guiWin::gui_window* window, int x, int y, int width, int height, float lineSize, Vec3 colour)
{
	if (!window)
		return;
	unsigned int pixelColour = Vec3ToColour(colour);
	int thickness = static_cast<int>(lineSize);
	if (thickness < 1)
		thickness = 1;
	// Draw top and bottom edges
	for (int i = 0; i < thickness; ++i)
	{
		boXGLDrawLine(window, x, y + i, x + width - 1, y + i, 1.0f, colour); // Top edge
		boXGLDrawLine(window, x, y + height - 1 - i, x + width - 1, y + height - 1 - i, 1.0f, colour); // Bottom edge
	}
	// Draw left and right edges
	for (int i = 0; i < thickness; ++i)
	{
		boXGLDrawLine(window, x + i, y, x + i, y + height - 1, 1.0f, colour); // Left edge
		boXGLDrawLine(window, x + width - 1 - i, y, x + width - 1 - i, y + height - 1, 1.0f, colour); // Right edge
	}
}
