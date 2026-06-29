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

void boXGL::boXGLDrawCircle(guiWin::gui_window* window, int centerX, int centerY, int radius, float lineSize, Vec3 colour)
{
	if (!window)
		return;
	unsigned int pixelColour = Vec3ToColour(colour);
	int thickness = static_cast<int>(lineSize);
	if (thickness < 1)
		thickness = 1;
	for (int y = -radius; y <= radius; ++y)
	{
		for (int x = -radius; x <= radius; ++x)
		{
			int distanceSquared = x * x + y * y;
			int outerRadiusSquared = radius * radius;
			int innerRadiusSquared = (radius - thickness) * (radius - thickness);
			if (distanceSquared <= outerRadiusSquared && distanceSquared >= innerRadiusSquared)
			{
				int px = centerX + x;
				int py = centerY + y;
				if (px >= 0 && px < window->width &&
					py >= 0 && py < window->height)
				{
					window->pixels[py * window->width + px] = pixelColour;
				}
			}
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

void boXGL::boXGLDrawFilledRect(guiWin::gui_window* window, int x, int y, int width, int height, Vec3 colour)
{
	if (!window)
		return;
	unsigned int pixelColour = Vec3ToColour(colour);
	for (int py = y; py < y + height; ++py)
	{
		for (int px = x; px < x + width; ++px)
		{
			if (px >= 0 && px < window->width &&
				py >= 0 && py < window->height)
			{
				window->pixels[py * window->width + px] = pixelColour;
			}
		}
	}
}
// ############################################################################################################
// ########################################### 3d Section #####################################################
// ############################################################################################################

static float boXGLEdgeFunction(const Vec2& a, const Vec2& b, const Vec2& p)
{
    return
        (p.x - a.x) * (b.y - a.y) -
        (p.y - a.y) * (b.x - a.x);
}

static bool boXGLProjectPoint(
    guiWin::gui_window* window,
    const Vec3& point,
    Vec2& out)
{
    if (!window)
        return false;

    const float nearPlane = 0.1f;
    const float focalLength = 500.0f;

    if (point.z <= nearPlane)
        return false;

    out.x =
        static_cast<float>(window->width) * 0.5f +
        (point.x * focalLength) / point.z;

    out.y =
        static_cast<float>(window->height) * 0.5f -
        (point.y * focalLength) / point.z;

    return true;
}

static bool boXGLProjectPoint(
    guiWin::gui_window* window,
    const Vec3& point,
    Vec2& out)
{
    if (!window)
        return false;

    // Very simple perspective camera.
    // Camera is looking forward along +Z.
    const float nearPlane = 0.1f;
    const float focalLength = 500.0f;

    if (point.z <= nearPlane)
        return false;

    out.x = static_cast<float>(window->width) * 0.5f +
        (point.x * focalLength) / point.z;

    out.y = static_cast<float>(window->height) * 0.5f -
        (point.y * focalLength) / point.z;

    return true;
}

void boXGL::boXGLDrawCube(guiWin::gui_window* window, Vec3 position, Vec3 size, Vec3 colour)
{
    if (!window)
        return;

    Vec3 halfSize = size * 0.5f;

    Vec3 vertices[8] =
    {
        // Back face
        Vec3(-halfSize.x, -halfSize.y, -halfSize.z),
        Vec3(halfSize.x, -halfSize.y, -halfSize.z),
        Vec3(halfSize.x,  halfSize.y, -halfSize.z),
        Vec3(-halfSize.x,  halfSize.y, -halfSize.z),

        // Front face
        Vec3(-halfSize.x, -halfSize.y,  halfSize.z),
        Vec3(halfSize.x, -halfSize.y,  halfSize.z),
        Vec3(halfSize.x,  halfSize.y,  halfSize.z),
        Vec3(-halfSize.x,  halfSize.y,  halfSize.z)
    };

    // Move cube into world position
    for (int i = 0; i < 8; ++i)
    {
        vertices[i] = vertices[i] + position;
    }

    Vec2 projected[8];
    bool visible[8];

    for (int i = 0; i < 8; ++i)
    {
        visible[i] = boXGLProjectPoint(window, vertices[i], projected[i]);
    }

    auto DrawEdge = [&](int a, int b)
    {
        if (!visible[a] || !visible[b])
            return;

        boXGLDrawLine(
            window,
            static_cast<int>(projected[a].x),
            static_cast<int>(projected[a].y),
            static_cast<int>(projected[b].x),
            static_cast<int>(projected[b].y),
            1.0f,
            colour);
    };

    // Back face
    DrawEdge(0, 1);
    DrawEdge(1, 2);
    DrawEdge(2, 3);
    DrawEdge(3, 0);

    // Front face
    DrawEdge(4, 5);
    DrawEdge(5, 6);
    DrawEdge(6, 7);
    DrawEdge(7, 4);

    // Connecting edges
    DrawEdge(0, 4);
    DrawEdge(1, 5);
    DrawEdge(2, 6);
    DrawEdge(3, 7);
}

void boXGL::boXGLDrawFilledTriangle(guiWin::gui_window* window, Vec2 a, Vec2 b, Vec2 c, Vec3 colour)
{
    if (!window)
        return;

    int minX = static_cast<int>(std::floor(std::min({ a.x, b.x, c.x })));
    int maxX = static_cast<int>(std::ceil(std::max({ a.x, b.x, c.x })));

    int minY = static_cast<int>(std::floor(std::min({ a.y, b.y, c.y })));
    int maxY = static_cast<int>(std::ceil(std::max({ a.y, b.y, c.y })));

    minX = std::max(minX, 0);
    minY = std::max(minY, 0);

    maxX = std::min(maxX, window->width - 1);
    maxY = std::min(maxY, window->height - 1);

    float area = boXGLEdgeFunction(a, b, c);

    if (std::abs(area) <= 0.00001f)
        return;

    unsigned int pixelColour = Vec3ToColour(colour);

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            Vec2 p(
                static_cast<float>(x) + 0.5f,
                static_cast<float>(y) + 0.5f
            );

            float w0 = boXGLEdgeFunction(b, c, p);
            float w1 = boXGLEdgeFunction(c, a, p);
            float w2 = boXGLEdgeFunction(a, b, p);

            bool inside =
                (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) ||
                (w0 <= 0.0f && w1 <= 0.0f && w2 <= 0.0f);

            if (!inside)
                continue;

            window->pixels[y * window->width + x] = pixelColour;
        }
    }
}

void boXGL::boXGLDrawMeshFaces(guiWin::gui_window* window, const boXMesh& mesh)
{
    if (!window)
        return;

    for (const boXFace& face : mesh.faces)
    {
        Vec2 p0, p1, p2, p3;

        const Vec3& v0 = mesh.vertices[face.v0].position;
        const Vec3& v1 = mesh.vertices[face.v1].position;
        const Vec3& v2 = mesh.vertices[face.v2].position;
        const Vec3& v3 = mesh.vertices[face.v3].position;

        if (!boXGLProjectPoint(window, v0, p0)) continue;
        if (!boXGLProjectPoint(window, v1, p1)) continue;
        if (!boXGLProjectPoint(window, v2, p2)) continue;
        if (!boXGLProjectPoint(window, v3, p3)) continue;

        boXGLDrawFilledTriangle(window, p0, p1, p2, face.colour);
        boXGLDrawFilledTriangle(window, p0, p2, p3, face.colour);
    }
}

void boXGL::boXGLDrawMeshEdges(guiWin::gui_window* window, const boXMesh& mesh, Vec3 colour)
{
    if (!window)
        return;

    for (const boXEdge& edge : mesh.edges)
    {
        Vec2 a;
        Vec2 b;

        const Vec3& v0 = mesh.vertices[edge.v0].position;
        const Vec3& v1 = mesh.vertices[edge.v1].position;

        if (!boXGLProjectPoint(window, v0, a)) continue;
        if (!boXGLProjectPoint(window, v1, b)) continue;

        boXGLDrawLine(
            window,
            static_cast<int>(a.x),
            static_cast<int>(a.y),
            static_cast<int>(b.x),
            static_cast<int>(b.y),
            edge.selected ? 3.0f : 1.0f,
            edge.selected ? Vec3(1.0f, 0.8f, 0.1f) : colour);
    }
}

void boXGL::boXGLDrawMeshVertices(guiWin::gui_window* window, const boXMesh& mesh, Vec3 colour)
{
    if (!window)
        return;

    for (const boXVertex& vertex : mesh.vertices)
    {
        Vec2 p;

        if (!boXGLProjectPoint(window, vertex.position, p))
            continue;

        Vec3 drawColour = vertex.selected
            ? Vec3(1.0f, 0.8f, 0.1f)
            : colour;

        float radius = vertex.selected ? 5.0f : 3.0f;

        boXGLDrawCircle(window,  static_cast<int>(p.x), static_cast<int>(p.y),  radius, drawColour);
    }
}

void boXGL::boXGLDrawMesh(guiWin::gui_window* window, const boXMesh& mesh)
{
    if (!window)
        return;

    // 1. Draw faces first
    boXGLDrawMeshFaces(window, mesh);

    // 2. Draw wire edges on top
    boXGLDrawMeshEdges(
        window,
        mesh,
        Vec3(0.05f, 0.05f, 0.05f));

    // 3. Draw vertices last
    boXGLDrawMeshVertices(
        window,
        mesh,
        Vec3(0.95f, 0.95f, 0.95f));
}


