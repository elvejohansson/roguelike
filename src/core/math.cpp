#include <format>
#include <string>

#include "math.h"

[[nodiscard]] std::string vector_str(Vector2 vec) {
    return std::format("Vector2({}, {})", vec.x, vec.y);
}

[[nodiscard]] std::string vector_str(Vector3 vec) {
    return std::format("Vector3({}, {}, {})", vec.x, vec.y, vec.z);
}

[[nodiscard]] std::string vector_str(Vector4 vec) {
    return std::format("Vector4({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.x);
}

[[nodiscard]] std::string mat_str(Mat2 mat) {
    return std::format("Mat2([{}, {}], [{}, {}])", mat[0][0], mat[0][1],
                       mat[1][0], mat[1][1]);
}

[[nodiscard]] std::string mat_str(Mat3 mat) {
    return std::format("Mat3([{}, {}, {}], [{}, {}, {}], [{}, {}, {}])",
                       mat[0][0], mat[0][1], mat[0][2], mat[1][0], mat[1][1],
                       mat[1][2], mat[2][0], mat[2][1], mat[2][2]);
}

[[nodiscard]] std::string mat_str(Mat4 mat) {
    return std::format("Mat4([{}, {}, {}, {}], [{}, {}, {}, {}], [{}, {}, {}, "
                       "{}], [{}, {}, {}, {}])",
                       mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0],
                       mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1],
                       mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2],
                       mat[3][3]);
}
