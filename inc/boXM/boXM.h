#pragma once
#include <cmath>
#include <algorithm>


struct vec2
{
    float x = 0.0f;
    float y = 0.0f;

    vec2() = default;

    vec2(float xValue, float yValue)
        : x(xValue), y(yValue)
    {
    }
};

struct vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    vec3() = default;

    vec3(float xValue, float yValue, float zValue)
        : x(xValue), y(yValue), z(zValue)
    {
    }
};

struct vec4
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    vec4() = default;

    vec4(float xValue, float yValue, float zValue, float wValue)
        : x(xValue), y(yValue), z(zValue), w(wValue)
    {
    }
};

struct mat4
{
	float m[4][4] = { 0 };
	mat4() = default;
	mat4(float diagonal)
	{
		m[0][0] = diagonal;
		m[1][1] = diagonal;
		m[2][2] = diagonal;
		m[3][3] = diagonal;
	}
};


// ########################################### vec2 operators ###########################

inline vec2 operator+(const vec2& a, const vec2& b)
{
    return vec2(a.x + b.x, a.y + b.y);
}

inline vec2 operator-(const vec2& a, const vec2& b)
{
    return vec2(a.x - b.x, a.y - b.y);
}

inline vec2 operator*(const vec2& a, float scalar)
{
    return vec2(a.x * scalar, a.y * scalar);
}

inline vec2 operator/(const vec2& a, float scalar)
{
    return vec2(a.x / scalar, a.y / scalar);
}


// ################################################## Operators #########################################
inline vec3 operator+(const vec3& a, const vec3& b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline vec3 operator-(const vec3& a, const vec3& b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline vec3 operator*(const vec3& a, float scalar)
{
    return vec3(a.x * scalar, a.y * scalar, a.z * scalar);
}

inline vec3 operator/(const vec3& a, float scalar)
{
    return vec3(a.x / scalar, a.y / scalar, a.z / scalar);
}

inline vec3& operator+=(vec3& a, const vec3& b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
    return a;
}
inline vec3& operator-=(vec3& a, const vec3& b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
    return a;
}
// ################################################## End Operators ######################################

// ##################################################  math's helpers; ###################################
inline float radians(float degrees)
{
    return degrees * (3.14159265358979323846f / 180.0f);
}

inline float dot(const vec2& a, const vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

inline float dot(const vec3& a, const vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3 cross(const vec3& a, const vec3& b)
{
    return vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

inline float length(const vec2& v)
{
    return std::sqrt(dot(v, v));
}

inline float length(const vec3& v)
{
    return std::sqrt(dot(v, v));
}

inline vec2 normalize(const vec2& v)
{
    float len = length(v);

    if (len <= 0.00001f)
        return vec2(0.0f, 0.0f);

    return v / len;
}

inline vec3 normalize(const vec3& v)
{
    float len = length(v);

    if (len <= 0.00001f)
        return vec3(0.0f, 0.0f, 0.0f);

    return v / len;
}

// ###############################################  End math's helpers; ###################################
// 
// ###############################################  matrix helpers; #######################################
inline mat4 perspective(float fovDegrees, float aspect, float nearPlane, float farPlane)
{
    mat4 result(0.0f);

    float fovRadians = radians(fovDegrees);
    float tanHalfFov = std::tan(fovRadians * 0.5f);

    result.m[0][0] = 1.0f / (aspect * tanHalfFov);
    result.m[1][1] = 1.0f / tanHalfFov;

    result.m[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    result.m[2][3] = -1.0f;

    result.m[3][2] = -(2.0f * farPlane * nearPlane) / (farPlane - nearPlane);

    return result;
}

inline mat4 lookAt(const vec3& position, const vec3& target, const vec3& up)
{
    vec3 f = normalize(target - position);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);

    mat4 result(1.0f);

    result.m[0][0] = s.x;
    result.m[1][0] = s.y;
    result.m[2][0] = s.z;

    result.m[0][1] = u.x;
    result.m[1][1] = u.y;
    result.m[2][1] = u.z;

    result.m[0][2] = -f.x;
    result.m[1][2] = -f.y;
    result.m[2][2] = -f.z;

    result.m[3][0] = -dot(s, position);
    result.m[3][1] = -dot(u, position);
    result.m[3][2] = dot(f, position);

    return result;
}

inline float clamp(float value, float minValue, float maxValue)
{
    return std::max(minValue, std::min(value, maxValue));
}

inline unsigned int vec3ToColour(const vec3& colour)
{
    unsigned char r =
        static_cast<unsigned char>(clamp(colour.x, 0.0f, 1.0f) * 255.0f);

    unsigned char g =
        static_cast<unsigned char>(clamp(colour.y, 0.0f, 1.0f) * 255.0f);

    unsigned char b =
        static_cast<unsigned char>(clamp(colour.z, 0.0f, 1.0f) * 255.0f);

    return
        static_cast<unsigned int>(b) |
        static_cast<unsigned int>(g) << 8 |
        static_cast<unsigned int>(r) << 16;
}



