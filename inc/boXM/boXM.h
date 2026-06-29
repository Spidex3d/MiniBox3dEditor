#pragma once
#include <cmath>
#include <algorithm>

struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;

    Vec2(float xValue, float yValue)
        : x(xValue), y(yValue)
    {
    }
};

struct Vec3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Vec3() = default;

    Vec3(float xValue, float yValue, float zValue)
        : x(xValue), y(yValue), z(zValue)
    {
    }
};

struct Vec4
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;

    Vec4() = default;

    Vec4(float xValue, float yValue, float zValue, float wValue)
        : x(xValue), y(yValue), z(zValue), w(wValue)
    {
    }
};

// Vec2 operators

inline Vec2 operator+(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x + b.x, a.y + b.y);
}

inline Vec2 operator-(const Vec2& a, const Vec2& b)
{
    return Vec2(a.x - b.x, a.y - b.y);
}

inline Vec2 operator*(const Vec2& a, float scalar)
{
    return Vec2(a.x * scalar, a.y * scalar);
}

inline Vec2 operator/(const Vec2& a, float scalar)
{
    return Vec2(a.x / scalar, a.y / scalar);
}

// Vec3 operators

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vec3 operator-(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vec3 operator*(const Vec3& a, float scalar)
{
    return Vec3(a.x * scalar, a.y * scalar, a.z * scalar);
}

inline Vec3 operator/(const Vec3& a, float scalar)
{
    return Vec3(a.x / scalar, a.y / scalar, a.z / scalar);
}

// Maths helpers

inline float Dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

inline float Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 Cross(const Vec3& a, const Vec3& b)
{
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

inline float Length(const Vec2& v)
{
    return std::sqrt(Dot(v, v));
}

inline float Length(const Vec3& v)
{
    return std::sqrt(Dot(v, v));
}

inline Vec2 Normalize(const Vec2& v)
{
    float length = Length(v);

    if (length <= 0.00001f)
        return Vec2(0.0f, 0.0f);

    return v / length;
}

inline Vec3 Normalize(const Vec3& v)
{
    float length = Length(v);

    if (length <= 0.00001f)
        return Vec3(0.0f, 0.0f, 0.0f);

    return v / length;
}

inline float Clamp(float value, float minValue, float maxValue)
{
    return std::max(minValue, std::min(value, maxValue));
}

inline unsigned int Vec3ToColour(const Vec3& colour)
{
    unsigned char r =
        static_cast<unsigned char>(Clamp(colour.x, 0.0f, 1.0f) * 255.0f);

    unsigned char g =
        static_cast<unsigned char>(Clamp(colour.y, 0.0f, 1.0f) * 255.0f);

    unsigned char b =
        static_cast<unsigned char>(Clamp(colour.z, 0.0f, 1.0f) * 255.0f);

    return
        static_cast<unsigned int>(b) |
        static_cast<unsigned int>(g) << 8 |
        static_cast<unsigned int>(r) << 16;
}