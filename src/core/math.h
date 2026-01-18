#ifndef MATH_H
#define MATH_H

#include <cmath>
#include <string>

struct Vector2 {
    float x;
    float y;

    Vector2 operator+(const Vector2 &other) const noexcept {
        return Vector2{x + other.x, y + other.y};
    }

    Vector2 operator-(const Vector2 &other) const noexcept {
        return Vector2{x - other.x, y - other.y};
    }

    Vector2 operator*(const float other) const noexcept {
        return Vector2{x * other, y * other};
    }

    Vector2 operator/(const float other) const noexcept {
        return Vector2{x / other, y / other};
    }

    Vector2 operator-() const noexcept {
        return Vector2{-x, -y};
    }

    float magnitude() const noexcept {
        return std::sqrt((x * x) + (y * y));
    }

    float dot(const Vector2 &other) const noexcept {
        return (x * other.x) + (y * other.y);
    }

    [[nodiscard]] Vector2 normalized(float eps = 1e-8f) const noexcept {
        float ls = (x * x) + (y * y);
        if (ls <= eps * eps) {
            return Vector2{0.0f, 0.0f};
        }
        float inverseLength = 1.0f / std::sqrt(ls);
        return Vector2{x * inverseLength, y * inverseLength};
    }
};

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3 operator+(const Vector3 &other) const noexcept {
        return Vector3{x + other.x, y + other.y, z + other.z};
    }

    Vector3 operator-(const Vector3 &other) const noexcept {
        return Vector3{x - other.x, y - other.y, z - other.z};
    }

    Vector3 operator*(const float other) const noexcept {
        return Vector3{x * other, y * other, z * other};
    }

    Vector3 operator/(const float other) const noexcept {
        return Vector3{x / other, y / other, z / other};
    }

    Vector3 operator-() const noexcept {
        return Vector3{-x, -y, -z};
    }

    float magnitude() const noexcept {
        return std::sqrt((x * x) + (y * y) + (z * z));
    }

    float dot(const Vector3 &other) const noexcept {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }

    [[nodiscard]] Vector3 normalized(float eps = 1e-8f) const noexcept {
        float ls = (x * x) + (y * y) + (z * z);
        if (ls <= eps * eps) {
            return Vector3{0.0f, 0.0f, 0.0f};
        }
        float inverseLength = 1.0f / std::sqrt(ls);
        return Vector3{x * inverseLength, y * inverseLength, z * inverseLength};
    }

    Vector3 cross(const Vector3 &other) const noexcept {
        return Vector3{(y * other.z) - (other.y * z),
                       (z * other.x) - (other.z * x),
                       (x * other.y) - (other.x * y)};
    }
};

struct Vector4 {
    float x;
    float y;
    float z;
    float w;

    Vector4 operator+(const Vector4 &other) const noexcept {
        return Vector4{x + other.x, y + other.y, z + other.z, w + other.w};
    }

    Vector4 operator-(const Vector4 &other) const noexcept {
        return Vector4{x - other.x, y - other.y, z - other.z, w - other.w};
    }

    Vector4 operator*(const float other) const noexcept {
        return Vector4{x * other, y * other, z * other, w * other};
    }

    Vector4 operator/(const float other) const noexcept {
        return Vector4{x / other, y / other, z / other, w / other};
    }

    Vector4 operator-() const noexcept {
        return Vector4{-x, -y, -z, -w};
    }

    float magnitude() const noexcept {
        return std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
    }

    float dot(const Vector4 &other) const noexcept {
        return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
    }

    [[nodiscard]] Vector4 normalized(float eps = 1e-8f) const noexcept {
        float ls = (x * x) + (y * y) + (z * z) + (w * w);
        if (ls <= eps * eps) {
            return Vector4{0.0f, 0.0f, 0.0f, 0.0f};
        }
        float inverseLength = 1.0f / std::sqrt(ls);
        return Vector4{x * inverseLength, y * inverseLength, z * inverseLength,
                       w * inverseLength};
    }
};

[[nodiscard]] constexpr Vector2 vector2() {
    return Vector2{0, 0};
}

// NOLINTNEXTLINE(readability-identifier-length)
[[nodiscard]] constexpr Vector2 vector2(float x, float y) {
    return Vector2{x, y};
}

[[nodiscard]] constexpr Vector2 vector2_up() {
    return Vector2{0, 1};
}

[[nodiscard]] constexpr Vector2 vector2_down() {
    return Vector2{0, -1};
}

// NOLINTNEXTLINE(readability-redundant-inline-specifier)
[[nodiscard]] constexpr Vector3 vector3() {
    return Vector3{0, 0, 0};
}

// NOLINTNEXTLINE(readability-identifier-length)
[[nodiscard]] constexpr Vector3 vector3(float x, float y, float z) {
    return Vector3{x, y, z};
}

[[nodiscard]] constexpr Vector3 vector3_up() {
    return Vector3{0, 1, 0};
}

[[nodiscard]] constexpr Vector3 vector3_down() {
    return Vector3{0, -1, 0};
}

[[nodiscard]] constexpr Vector4 vector4() {
    return Vector4{0, 0, 0, 0};
}

// NOLINTNEXTLINE(readability-identifier-length)
[[nodiscard]] constexpr Vector4 vector4(float x, float y, float z, float w) {
    return Vector4{x, y, z, w};
}

[[nodiscard]] constexpr Vector4 vector4_up() {
    return Vector4{0, 1, 0, 0};
}

[[nodiscard]] constexpr Vector4 vector4_down() {
    return Vector4{0, -1, 0, 0};
}

[[nodiscard]] std::string vector_str(Vector2 vec);
[[nodiscard]] std::string vector_str(Vector3 vec);
[[nodiscard]] std::string vector_str(Vector4 vec);

struct Mat2 {
    float entries[2][2];

    float (&operator[](int row))[2] {
        return entries[row];
    }

    const float (&operator[](const int row) const)[2] {
        return entries[row];
    }

    Mat2 operator+(const Mat2 &other) const noexcept {
        return Mat2{
            {{entries[0][0] + other[0][0], entries[0][1] + other[0][1]},
             {entries[1][0] + other[1][0], entries[1][1] + other[1][1]}}};
    }

    Mat2 operator-(const Mat2 &other) const noexcept {
        return Mat2{
            {{entries[0][0] - other[0][0], entries[0][1] - other[0][1]},
             {entries[1][0] - other[1][0], entries[1][1] - other[1][1]}}};
    }

    Mat2 operator*(const float other) const noexcept {
        return Mat2{{{entries[0][0] * other, entries[0][1] * other},
                     {entries[1][0] * other, entries[1][1] * other}}};
    }

    Mat2 operator*(const Mat2 &other) const noexcept {
        Mat2 out{};
        for (int r = 0; r < 2; ++r) {
            for (int c = 0; c < 2; ++c) {
                out[r][c] =
                    entries[r][0] * other[0][c] + entries[r][1] * other[1][c];
            }
        }
        return out;
    }

    Vector2 operator*(const Vector2 &v) const noexcept {
        return Vector2{entries[0][0] * v.x + entries[0][1] * v.y,
                       entries[1][0] * v.x + entries[1][1] * v.y};
    }

    Mat2 transpose() const noexcept {
        return Mat2{
            {{entries[0][0], entries[1][0]}, {entries[0][1], entries[1][1]}}};
    }
};

[[nodiscard]] constexpr Mat2 mat2_identity() {
    return Mat2{{
        {1, 0},
        {0, 1},
    }};
}

struct Mat3 {
    float entries[3][3];

    float (&operator[](int row))[3] {
        return entries[row];
    }

    const float (&operator[](const int row) const)[3] {
        return entries[row];
    }

    Mat3 operator+(const Mat3 &other) const noexcept {
        return Mat3{{
            {entries[0][0] + other[0][0], entries[0][1] + other[0][1],
             entries[0][2] + other[0][2]},
            {entries[1][0] + other[1][0], entries[1][1] + other[1][1],
             entries[1][2] + other[1][2]},
            {entries[2][0] + other[2][0], entries[2][1] + other[2][1],
             entries[2][2] + other[2][2]},
        }};
    }

    Mat3 operator-(const Mat3 &other) const noexcept {
        return Mat3{{
            {entries[0][0] - other[0][0], entries[0][1] - other[0][1],
             entries[0][2] - other[0][2]},
            {entries[1][0] - other[1][0], entries[1][1] - other[1][1],
             entries[1][2] - other[1][2]},
            {entries[2][0] - other[2][0], entries[2][1] - other[2][1],
             entries[2][2] - other[2][2]},
        }};
    }

    Mat3 operator*(const float other) const noexcept {
        return Mat3{{
            {entries[0][0] * other, entries[0][1] * other,
             entries[0][2] * other},
            {entries[1][0] * other, entries[1][1] * other,
             entries[1][2] * other},
            {entries[2][0] * other, entries[2][1] * other,
             entries[2][2] * other},
        }};
    }

    Mat3 operator*(const Mat3 &other) const noexcept {
        Mat3 out{};
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                out[r][c] = entries[r][0] * other[0][c] +
                            entries[r][1] * other[1][c] +
                            entries[r][2] * other[2][c];
            }
        }
        return out;
    }

    Vector3 operator*(const Vector3 &v) const noexcept {
        return Vector3{
            entries[0][0] * v.x + entries[0][1] * v.y + entries[0][2] * v.z,
            entries[1][0] * v.x + entries[1][1] * v.y + entries[1][2] * v.z,
            entries[2][0] * v.x + entries[2][1] * v.y + entries[2][2] * v.z};
    }

    Mat3 transpose() const noexcept {
        return Mat3{{{entries[0][0], entries[1][0], entries[2][0]},
                     {entries[0][1], entries[1][1], entries[2][1]},
                     {entries[0][2], entries[1][2], entries[2][2]}}};
    }
};

[[nodiscard]] constexpr Mat3 mat3_identity() {
    return Mat3{{
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
    }};
}

struct Mat4 {
    float entries[4][4];

    float (&operator[](int row))[4] {
        return entries[row];
    }

    const float (&operator[](const int row) const)[4] {
        return entries[row];
    }

    Mat4 operator+(const Mat4 &other) const noexcept {
        return Mat4{{
            {entries[0][0] + other[0][0], entries[0][1] + other[0][1],
             entries[0][2] + other[0][2], entries[0][3] + other[0][3]},
            {entries[1][0] + other[1][0], entries[1][1] + other[1][1],
             entries[1][2] + other[1][2], entries[1][3] + other[1][3]},
            {entries[2][0] + other[2][0], entries[2][1] + other[2][1],
             entries[2][2] + other[2][2], entries[2][3] + other[2][3]},
            {entries[3][0] + other[3][0], entries[3][1] + other[3][1],
             entries[3][2] + other[3][2], entries[3][3] + other[3][3]},
        }};
    }

    Mat4 operator-(const Mat4 &other) const noexcept {
        return Mat4{{
            {entries[0][0] - other[0][0], entries[0][1] - other[0][1],
             entries[0][2] - other[0][2], entries[0][3] - other[0][3]},
            {entries[1][0] - other[1][0], entries[1][1] - other[1][1],
             entries[1][2] - other[1][2], entries[1][3] - other[1][3]},
            {entries[2][0] - other[2][0], entries[2][1] - other[2][1],
             entries[2][2] - other[2][2], entries[2][3] - other[2][3]},
            {entries[3][0] - other[3][0], entries[3][1] - other[3][1],
             entries[3][2] - other[3][2], entries[3][3] - other[3][3]},
        }};
    }

    Mat4 operator*(const float other) const noexcept {
        return Mat4{{
            {entries[0][0] * other, entries[0][1] * other,
             entries[0][2] * other, entries[0][3] * other},
            {entries[1][0] * other, entries[1][1] * other,
             entries[1][2] * other, entries[1][3] * other},
            {entries[2][0] * other, entries[2][1] * other,
             entries[2][2] * other, entries[2][3] * other},
            {entries[3][0] * other, entries[3][1] * other,
             entries[3][2] * other, entries[3][3] * other},
        }};
    }

    Mat4 operator*(const Mat4 &other) const noexcept {
        Mat4 out{};
        for (int r = 0; r < 4; ++r) {
            for (int c = 0; c < 4; ++c) {
                out[r][c] =
                    entries[r][0] * other[0][c] + entries[r][1] * other[1][c] +
                    entries[r][2] * other[2][c] + entries[r][3] * other[3][c];
            }
        }
        return out;
    }

    Vector4 operator*(const Vector4 &v) const noexcept {
        return Vector4{entries[0][0] * v.x + entries[0][1] * v.y +
                           entries[0][2] * v.z + entries[0][3] * v.w,
                       entries[1][0] * v.x + entries[1][1] * v.y +
                           entries[1][2] * v.z + entries[1][3] * v.w,
                       entries[2][0] * v.x + entries[2][1] * v.y +
                           entries[2][2] * v.z + entries[2][3] * v.w,
                       entries[3][0] * v.x + entries[3][1] * v.y +
                           entries[3][2] * v.z + entries[3][3] * v.w};
    }

    Mat4 transpose() const noexcept {
        return Mat4{
            {{entries[0][0], entries[1][0], entries[2][0], entries[3][0]},
             {entries[0][1], entries[1][1], entries[2][1], entries[3][1]},
             {entries[0][2], entries[1][2], entries[2][2], entries[3][2]},
             {entries[0][3], entries[1][3], entries[2][3], entries[3][3]}}};
    }
};

[[nodiscard]] constexpr Mat4 mat4_identity() {
    return Mat4{{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1},
    }};
}

[[nodiscard]] std::string mat_str(Mat2 mat);
[[nodiscard]] std::string mat_str(Mat3 mat);
[[nodiscard]] std::string mat_str(Mat4 mat);

#endif
