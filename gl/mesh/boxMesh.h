#pragma once

#include <vector>
#include <boXM\boXM.h>

struct boXVertex
{
    vec3 position;
	// is the vertex selected? This can be used for highlighting or other purposes.
    bool selected = false;
};

struct boXEdge
{
    int v0 = -1;
    int v1 = -1;
	// is the edge selected? This can be used for highlighting or other purposes.
    bool selected = false;
};

struct boXFace
{
    // For now our cube faces are quads.
    // Later this can support triangles, ngons, etc.
    int v0 = -1;
    int v1 = -1;
    int v2 = -1;
    int v3 = -1;

    vec3 colour = vec3(0.6f, 0.6f, 0.6f);
	// is the face selected? This can be used for highlighting or other purposes.
    bool selected = false;
};

struct boXMesh
{
    std::vector<boXVertex> vertices;
    std::vector<boXEdge> edges;
    std::vector<boXFace> faces;
};

// This is useful for calculating the normal of a face, which can be used for lighting calculations or backface culling.
inline vec3 boXFaceNormal(const boXMesh& mesh, const boXFace& face)
{
    const vec3& a = mesh.vertices[face.v0].position;
    const vec3& b = mesh.vertices[face.v1].position;
    const vec3& c = mesh.vertices[face.v2].position;

    vec3 edge1 = b - a;
    vec3 edge2 = c - a;

    return normalize(cross(edge1, edge2));
}

inline boXMesh boXCreateCubeMesh(vec3 position, vec3 size)
{
    boXMesh mesh;

    vec3 h = size * 0.5f;

    // 8 vertices
    mesh.vertices =
    {
        // Back face vertices
        { position + vec3(-h.x, -h.y, -h.z) }, // 0
        { position + vec3(h.x, -h.y, -h.z) }, // 1
        { position + vec3(h.x,  h.y, -h.z) }, // 2
        { position + vec3(-h.x,  h.y, -h.z) }, // 3

        // Front face vertices
        { position + vec3(-h.x, -h.y,  h.z) }, // 4
        { position + vec3(h.x, -h.y,  h.z) }, // 5
        { position + vec3(h.x,  h.y,  h.z) }, // 6
        { position + vec3(-h.x,  h.y,  h.z) }  // 7
    };

    // 12 edges
    mesh.edges =
    {
        // Back face
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 0},

        // Front face
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 4},

        // Connecting edges
        {0, 4},
        {1, 5},
        {2, 6},
        {3, 7}
    };

    // 6 quad faces
    // Winding is important.
    // These are arranged so normals point outward.
    mesh.faces =
    {
        // Back face: -Z
        {1, 0, 3, 2, vec3(0.55f, 0.55f, 0.60f)},

        // Front face: +Z
        {4, 5, 6, 7, vec3(0.65f, 0.65f, 0.70f)},

        // Left face: -X
        {0, 4, 7, 3, vec3(0.50f, 0.50f, 0.55f)},

        // Right face: +X
        {5, 1, 2, 6, vec3(0.70f, 0.70f, 0.75f)},

        // Bottom face: -Y
        {0, 1, 5, 4, vec3(0.45f, 0.45f, 0.50f)},

        // Top face: +Y
        {3, 7, 6, 2, vec3(0.80f, 0.80f, 0.85f)}
    };

    return mesh;
}
