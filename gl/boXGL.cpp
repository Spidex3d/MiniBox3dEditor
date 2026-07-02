#include "boXGL.h"

#include <algorithm>
#include <cmath>
#include <limits>


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

void boXGL::boXGLDrawPixel(guiWin::gui_window* window, int x, int y, vec3 colour)
{
    if (!window)
        return;

    if (x < 0 || x >= window->width ||
        y < 0 || y >= window->height)
    {
        return;
    }

    unsigned int pixelColour = vec3ToColour(colour);

    window->pixels[y * window->width + x] = pixelColour;
}

void boXGL::boXGLDrawLine(guiWin::gui_window* window, int sx, int sy, int ex, int ey, float lineSize, vec3 colour)
{
    if (!window)
        return;

    unsigned int pixelColour = vec3ToColour(colour);

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

void boXGL::boXGLDrawCircle(guiWin::gui_window* window, int centerX, int centerY, int radius, float lineSize, vec3 colour)
{
	if (!window)
		return;
	unsigned int pixelColour = vec3ToColour(colour);
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

void boXGL::boXGLDrawRect(guiWin::gui_window* window, int x, int y, int width, int height, float lineSize, vec3 colour)
{
	if (!window)
		return;
	unsigned int pixelColour = vec3ToColour(colour);
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

void boXGL::boXGLDrawFilledRect(guiWin::gui_window* window, int x, int y, int width, int height, vec3 colour)
{
	if (!window)
		return;
	unsigned int pixelColour = vec3ToColour(colour);
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

static bool boXGLProjectPoint(
    guiWin::gui_window* window,
    const vec3& point, boXGL::boXScreenVertex& out)
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

    out.z = point.z; // Add depth information to boXScreenVertex.  

    return true;
}

// ####################################################### Camera Helper ###############################
static bool boXGLProjectPointCamera(
    guiWin::gui_window* window,
    const Camera& camera,
    const vec3& worldPoint,
    boXGL::boXScreenVertex& out)
{
    if (!window)
        return false;

    vec3 relative = worldPoint - camera.Position;

    float cameraX = dot(relative, camera.Right);
    float cameraY = dot(relative, camera.Up);
    float cameraZ = dot(relative, camera.Front);

    const float nearPlane = 0.1f;
    const float focalLength = 500.0f;

    if (cameraZ <= nearPlane)
        return false;

    out.x =
        static_cast<float>(window->width) * 0.5f +
        (cameraX * focalLength) / cameraZ;

    out.y =
        static_cast<float>(window->height) * 0.5f -
        (cameraY * focalLength) / cameraZ;

    out.z = cameraZ;

    return true;
}


// Update the boXGLDrawCube function to use boXScreenVertex instead of vec2 for the 'projected' array.  
void boXGL::boXGLDrawCube(guiWin::gui_window* window, vec3 position, vec3 size, vec3 colour)
{
    if (!window)
        return;

    vec3 halfSize = size * 0.5f;

    vec3 vertices[8] =
    {
        // Back face  
        vec3(-halfSize.x, -halfSize.y, -halfSize.z),
        vec3(halfSize.x, -halfSize.y, -halfSize.z),
        vec3(halfSize.x,  halfSize.y, -halfSize.z),
        vec3(-halfSize.x,  halfSize.y, -halfSize.z),

        // Front face  
        vec3(-halfSize.x, -halfSize.y,  halfSize.z),
        vec3(halfSize.x, -halfSize.y,  halfSize.z),
        vec3(halfSize.x,  halfSize.y,  halfSize.z),
        vec3(-halfSize.x,  halfSize.y,  halfSize.z)
    };

    // Move cube into world position  
    for (int i = 0; i < 8; ++i)
    {
        vertices[i] = vertices[i] + position;
    }

    boXScreenVertex projected[8];
    bool visible[8];

    for (int i = 0; i < 8; ++i)
    {
        visible[i] = boXGLProjectPoint(window, vertices[i], projected[i]);
        //visible[i] = boXGLProjectPointCamera(window, camera, vertex.position, p)
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


static float boXGLEdgeFunction(const vec2& a, const vec2& b, const vec2& p)
{
    return
        (p.x - a.x) * (b.y - a.y) -
        (p.y - a.y) * (b.x - a.x);
}


void boXGL::boXGLDrawFilledTriangle(guiWin::gui_window* window, vec2 a, vec2 b, vec2 c, vec3 colour)
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

    unsigned int pixelColour = vec3ToColour(colour);

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            vec2 p(
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

void boXGL::boXGLDrawPixelDepth(guiWin::gui_window* window, int x, int y, float depth, vec3 colour)
{
    if (!window)
        return;
    if (x < 0 || x >= window->width ||
        y < 0 || y >= window->height)
    {
        return;
    }
    int index = y * window->width + x;
    if (depth < window->depthBuffer[index])
    {
        window->depthBuffer[index] = depth;
        unsigned int pixelColour = vec3ToColour(colour);
        window->pixels[index] = pixelColour;
    }
}

void boXGL::boXGLClearDepth(guiWin::gui_window* window)
{
	if (!window)
		return;
	std::fill(window->depthBuffer.begin(), window->depthBuffer.end(), std::numeric_limits<float>::infinity());
}


void boXGL::boXGLDrawFilledTriangleDepth(guiWin::gui_window* window, boXScreenVertex a, boXScreenVertex b, boXScreenVertex c, vec3 colour)
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

    float area =
        boXGLEdgeFunction(
            vec2(a.x, a.y),
            vec2(b.x, b.y),
            vec2(c.x, c.y));

    if (std::abs(area) <= 0.00001f)
        return;

    for (int y = minY; y <= maxY; ++y)
    {
        for (int x = minX; x <= maxX; ++x)
        {
            vec2 p(
                static_cast<float>(x) + 0.5f,
                static_cast<float>(y) + 0.5f);

            float w0 =
                boXGLEdgeFunction(
                    vec2(b.x, b.y),
                    vec2(c.x, c.y),
                    p);

            float w1 =
                boXGLEdgeFunction(
                    vec2(c.x, c.y),
                    vec2(a.x, a.y),
                    p);

            float w2 =
                boXGLEdgeFunction(
                    vec2(a.x, a.y),
                    vec2(b.x, b.y),
                    p);

            bool inside =
                (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f) ||
                (w0 <= 0.0f && w1 <= 0.0f && w2 <= 0.0f);

            if (!inside)
                continue;

            float alpha = w0 / area;
            float beta = w1 / area;
            float gamma = w2 / area;

            float depth =
                alpha * a.z +
                beta * b.z +
                gamma * c.z;

            boXGLDrawPixelDepth(
                window,
                x,
                y,
                depth,
                colour);
        }
    }
}
// Depth buffer management for 3D rendering
void boXGL::boXGLClearDepthBuffer(guiWin::gui_window* window)
{
    if (!window)
        return;
    std::fill(window->depthBuffer.begin(), window->depthBuffer.end(), std::numeric_limits<float>::infinity());
}


void boXGL::boXGLDrawMeshFaces(guiWin::gui_window* window, const Camera& camera, const boXMesh& mesh)
{
    if (!window)
        return;

    for (const boXFace& face : mesh.faces)
    {
        boXScreenVertex p0, p1, p2, p3;

        const vec3& v0 = mesh.vertices[face.v0].position;
        const vec3& v1 = mesh.vertices[face.v1].position;
        const vec3& v2 = mesh.vertices[face.v2].position;
        const vec3& v3 = mesh.vertices[face.v3].position;

        if (!boXGLProjectPointCamera(window, camera, v0, p0)) continue;
        if (!boXGLProjectPointCamera(window, camera, v1, p1)) continue;
        if (!boXGLProjectPointCamera(window, camera, v2, p2)) continue;
        if (!boXGLProjectPointCamera(window, camera, v3, p3)) continue;

        boXGLDrawFilledTriangleDepth(window, p0, p1, p2, face.colour);
        boXGLDrawFilledTriangleDepth(window, p0, p2, p3, face.colour);
    }
}

void boXGL::boXGLDrawMeshEdges(guiWin::gui_window* window, const Camera& camera, const boXMesh& mesh, vec3 colour)
{
    if (!window)
        return;

    for (const boXEdge& edge : mesh.edges)
    {
        const vec3& v0 = mesh.vertices[edge.v0].position;
        const vec3& v1 = mesh.vertices[edge.v1].position;

        boXScreenVertex p0;
        boXScreenVertex p1;

        if (!boXGLProjectPointCamera(window, camera, v0, p0))
            continue;

        if (!boXGLProjectPointCamera(window, camera, v1, p1))
            continue;

        vec3 edgeColour = edge.selected
            ? vec3(1.0f, 0.8f, 0.1f)
            : colour;

        float lineSize = edge.selected ? 3.0f : 1.0f;

        boXGLDrawLine(
            window,
            static_cast<int>(p0.x),
            static_cast<int>(p0.y),
            static_cast<int>(p1.x),
            static_cast<int>(p1.y),
            lineSize,
            edgeColour);
    }
}

void boXGL::boXGLDrawMeshVertices(guiWin::gui_window* window, const Camera& camera, const boXMesh& mesh, vec3 colour)
{
    if (!window)
        return;

    for (const boXVertex& vertex : mesh.vertices)
    {
        boXScreenVertex p;

        if (!boXGLProjectPointCamera(window, camera, vertex.position, p))
            continue;

        vec3 drawColour = vertex.selected
            ? vec3(1.0f, 0.8f, 0.1f)
            : colour;

        float radius = vertex.selected ? 5.0f : 3.0f;

        boXGLDrawCircle(
            window,
            static_cast<int>(p.x),
            static_cast<int>(p.y),
            static_cast<int>(radius),
            1.0f,
            drawColour);
    }
}

void boXGL::boXGLDrawMesh(guiWin::gui_window* window, const Camera& camera, const boXMesh& mesh)
{
    
    if (!window)
        return;

    boXGLDrawMeshFaces(window, camera, mesh);

    boXGLDrawMeshEdges(
        window,
        camera,
        mesh,
        vec3(0.05f, 0.05f, 0.05f));

    boXGLDrawMeshVertices(
        window,
        camera,
        mesh,
        vec3(0.95f, 0.95f, 0.95f));
}


