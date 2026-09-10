#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "math/Mat4.h"

using Catch::Approx;

void requireMat4ApproxEqual(const Mat4& actual, const Mat4& expected, float epsilon = 0.00001f)
{
    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t column = 0; column < 4; ++column)
        {
            REQUIRE(actual.at(row, column) == Approx(expected.at(row, column)).margin(epsilon));
        }
    }
}

TEST_CASE("Identity Matrix is constructed correct")
{
    Mat4 mat = Mat4::identity();

    REQUIRE(mat.at(0, 0) == Approx(1.0f));
    REQUIRE(mat.at(1, 0) == Approx(0.0f));
    REQUIRE(mat.at(2, 0) == Approx(0.0f));
    REQUIRE(mat.at(3, 0) == Approx(0.0f));

    REQUIRE(mat.at(0, 1) == Approx(0.0f));
    REQUIRE(mat.at(1, 1) == Approx(1.0f));
    REQUIRE(mat.at(2, 1) == Approx(0.0f));
    REQUIRE(mat.at(3, 1) == Approx(0.0f));

    REQUIRE(mat.at(0, 2) == Approx(0.0f));
    REQUIRE(mat.at(1, 2) == Approx(0.0f));
    REQUIRE(mat.at(2, 2) == Approx(1.0f));
    REQUIRE(mat.at(3, 2) == Approx(0.0f));

    REQUIRE(mat.at(0, 3) == Approx(0.0f));
    REQUIRE(mat.at(1, 3) == Approx(0.0f));
    REQUIRE(mat.at(2, 3) == Approx(0.0f));
    REQUIRE(mat.at(3, 3) == Approx(1.0f));
}

TEST_CASE("At() function returns correct results")
{
    Mat4 mat;
    mat.setRow(0, Vec3{1.0f, 2.0f, 3.0f});
    mat.setRow(1, Vec3{4.0f, 5.0f, 6.0f});
    mat.setRow(2, Vec3{7.0f, 8.0f, 9.0f});
    mat.setRow(3, Vec3{10.0f, 11.0f, 12.0f});
    
    REQUIRE(mat.at(0, 0) == Approx(1.0f));
    REQUIRE(mat.at(1, 0) == Approx(4.0f));
    REQUIRE(mat.at(2, 0) == Approx(7.0f));
    REQUIRE(mat.at(3, 0) == Approx(10.0f));

    REQUIRE(mat.at(0, 1) == Approx(2.0f));
    REQUIRE(mat.at(1, 1) == Approx(5.0f));
    REQUIRE(mat.at(2, 1) == Approx(8.0f));
    REQUIRE(mat.at(3, 1) == Approx(11.0f));

    REQUIRE(mat.at(0, 2) == Approx(3.0f));
    REQUIRE(mat.at(1, 2) == Approx(6.0f));
    REQUIRE(mat.at(2, 2) == Approx(9.0f));
    REQUIRE(mat.at(3, 2) == Approx(12.0f));

    REQUIRE(mat.at(0, 3) == Approx(0.0f));
    REQUIRE(mat.at(1, 3) == Approx(0.0f));
    REQUIRE(mat.at(2, 3) == Approx(0.0f));
    REQUIRE(mat.at(3, 3) == Approx(0.0f));
}

TEST_CASE("Matrix multiplication with other Matrix is correct")
{
    Mat4 matFirst;
    matFirst.setRow(0, Vec3{1.0f, 2.0f, 3.0f});
    matFirst.setRow(1, Vec3{4.0f, 5.0f, 6.0f});
    matFirst.setRow(2, Vec3{7.0f, 8.0f, 9.0f});
    matFirst.setRow(3, Vec3{10.0f, 11.0f, 12.0f});


}

TEST_CASE("Matrix multiplication with vector is correct")
{
    Mat4 mat;
    mat.setRow(0, Vec3{1.0f, 2.0f, 3.0f});
    mat.setRow(1, Vec3{4.0f, 5.0f, 6.0f});
    mat.setRow(2, Vec3{7.0f, 8.0f, 9.0f});
    mat.setRow(3, Vec3{10.0f, 11.0f, 12.0f});

    Vec3 vec{1.0f, 2.0f, 3.0f};

    Vec3 result = mat * vec;

    REQUIRE(result.x == Approx(14.0f));
    REQUIRE(result.y == Approx(32.0f));
    REQUIRE(result.z == Approx(50.0f));
}

TEST_CASE("Matrix multiplication with vector is correct with only zeros")
{
    Mat4 mat;
    mat.setRow(0, Vec3{0.0f, 0.0f, 0.0f});
    mat.setRow(1, Vec3{0.0f, 0.0f, 0.0f});
    mat.setRow(2, Vec3{0.0f, 0.0f, 0.0f});
    mat.setRow(3, Vec3{0.0f, 0.0f, 0.0f});

    Vec3 vec{1.0f, 2.0f, 3.0f};

    Vec3 result = mat * vec;

    REQUIRE(result.x == Approx(0.0f));
    REQUIRE(result.y == Approx(0.0f));
    REQUIRE(result.z == Approx(0.0f));
}

TEST_CASE("Mat4 translate creates translates matrix")
{
    Mat4 matrix = Mat4::translate(Vec3{2.0f, 3.0f, 4.0f});

    REQUIRE(matrix.at(0, 0) == Approx(1.0f));
    REQUIRE(matrix.at(1, 1) == Approx(1.0f));
    REQUIRE(matrix.at(2, 2) == Approx(1.0f));
    REQUIRE(matrix.at(3, 3) == Approx(1.0f));

    REQUIRE(matrix.at(0, 3) == Approx(2.0f));
    REQUIRE(matrix.at(1, 3) == Approx(3.0f));
    REQUIRE(matrix.at(2, 3) == Approx(4.0f));
}

TEST_CASE("Mat4 translate by zero produces identity")
{
    Mat4 matrix = Mat4::translate(Vec3{0.0f, 0.0f, 0.0f});

    REQUIRE(matrix.at(0, 0) == Approx(1.0f));
    REQUIRE(matrix.at(1, 1) == Approx(1.0f));
    REQUIRE(matrix.at(2, 2) == Approx(1.0f));
    REQUIRE(matrix.at(3, 3) == Approx(1.0f));

    REQUIRE(matrix.at(0, 3) == Approx(0.0f));
    REQUIRE(matrix.at(1, 3) == Approx(0.0f));
    REQUIRE(matrix.at(2, 3) == Approx(0.0f));
}

TEST_CASE("Mat4 scale creates scale matrix")
{
    Mat4 matrix = Mat4::scale(Vec3{2.0f, 3.0f, 4.0f});

    REQUIRE(matrix.at(0, 0) == Approx(2.0f));
    REQUIRE(matrix.at(1, 1) == Approx(3.0f));
    REQUIRE(matrix.at(2, 2) == Approx(4.0f));
    REQUIRE(matrix.at(3, 3) == Approx(1.0f));
}

TEST_CASE("Mat4 scale by one produces identity")
{
    Mat4 matrix = Mat4::scale(Vec3{1.0f, 1.0f, 1.0f});

    REQUIRE(matrix.at(0, 0) == Approx(1.0f));
    REQUIRE(matrix.at(1, 1) == Approx(1.0f));
    REQUIRE(matrix.at(2, 2) == Approx(1.0f));
    REQUIRE(matrix.at(3, 3) == Approx(1.0f));
}

TEST_CASE("Mat4 rotateX by zero produces identity rotation")
{
    Mat4 matrix = Mat4::rotateX(0.0f);

    REQUIRE(matrix.at(1, 1) == Approx(1.0f));
    REQUIRE(matrix.at(1, 2) == Approx(0.0f));
    REQUIRE(matrix.at(2, 1) == Approx(0.0f));
    REQUIRE(matrix.at(2, 2) == Approx(1.0f));
}

TEST_CASE("Mat4 rotateX by by 90 degrees")
{
    Mat4 matrix = Mat4::rotateX(90.0f);

    REQUIRE(matrix.at(1, 1) == Approx(0.0f).margin(0.00001f));
    REQUIRE(matrix.at(1, 2) == Approx(-1.0f));
    REQUIRE(matrix.at(2, 1) == Approx(1.0f));
    REQUIRE(matrix.at(2, 2) == Approx(0.0f).margin(0.00001f));
}

TEST_CASE("Mat4 rotateY by 90 degrees")
{
    Mat4 matrix = Mat4::rotateY(90.0f);
    REQUIRE(matrix.at(0, 0) == Approx(0.0f).margin(0.00001f));
    REQUIRE(matrix.at(0, 2) == Approx(1.0f));
    REQUIRE(matrix.at(2, 0) == Approx(-1.0f));
    REQUIRE(matrix.at(2, 2) == Approx(0.0f).margin(0.00001f));
}

TEST_CASE("Mat4 rotateZ by 90 degrees")
{
    Mat4 matrix = Mat4::rotateZ(90.0f);

    REQUIRE(matrix.at(0, 0) == Approx(0.0f).margin(0.00001f));
    REQUIRE(matrix.at(0, 1) == Approx(-1.0f));
    REQUIRE(matrix.at(1, 0) == Approx(1.0f));
    REQUIRE(matrix.at(1, 1) == Approx(0.0f).margin(0.00001f));
}

TEST_CASE("Positive 90 degree Y rotation rotates forward toward negative X")
{
    Mat4 rotation = Mat4::rotateY(90.0f);

    Vec3 forward{0.0f, 0.0f, -1.0f};

    Vec3 rotated = rotation * forward;

    REQUIRE(rotated.x == Approx(-1.0f));
    REQUIRE(rotated.y == Approx(0.0f));
    REQUIRE(rotated.z == Approx(0.0f).margin(0.00001f));
}

TEST_CASE("Mat4 rotate with only X rotation equals rotateX")
{
    Mat4 combined = Mat4::rotate(Vec3{45.0f, 0.0f, 0.0f});
    Mat4 xOnly = Mat4::rotateX(45.0f);

    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t column = 0; column < 4; ++column)
        {
            REQUIRE(combined.at(row, column) == Approx(xOnly.at(row, column)));
        }
    }
}

TEST_CASE("Mat4 rotate applies X Y Z using expected order")
{
    Vec3 rotation{20.0f, 30.0f, 40.0f};

    Mat4 expected = Mat4::rotateZ(40.0f) * Mat4::rotateY(30.0f) * Mat4::rotateX(20.0f);

    Mat4 actual = Mat4::rotate(rotation);

    requireMat4ApproxEqual(actual, expected);
}

TEST_CASE("Quaternion Y rotation conerts to same matrix as rotate Y")
{
    Quaternion q = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 90.0f);

    Mat4 quaternionMatrix = Mat4::fromQuaternion(q);

    Mat4 expected = Mat4::rotateY(90.0f);

    requireMat4ApproxEqual(quaternionMatrix, expected);
}

TEST_CASE("Quaternion X rotation conerts to same matrix as rotate X")
{
    Quaternion q = Quaternion::fromAxisAngle(Vec3{1.0f, 0.0f, 0.0f}, 90.0f);

    Mat4 quaternionMatrix = Mat4::fromQuaternion(q);

    Mat4 expected = Mat4::rotateX(90.0f);

    requireMat4ApproxEqual(quaternionMatrix, expected);
}

TEST_CASE("Quaternion Z rotation conerts to same matrix as rotate Z")
{
    Quaternion q = Quaternion::fromAxisAngle(Vec3{0.0f, 0.0f, 1.0f}, 90.0f);

    Mat4 quaternionMatrix = Mat4::fromQuaternion(q);

    Mat4 expected = Mat4::rotateZ(90.0f);

    requireMat4ApproxEqual(quaternionMatrix, expected);
}

TEST_CASE("Identity quaternion converts to identity matrix")
{
    Mat4 actual = Mat4::fromQuaternion(Quaternion::identity());
    Mat4 expected = Mat4::identity();

    requireMat4ApproxEqual(actual, expected);
}

TEST_CASE("Quaternion from Euler matches Mat4 rotate")
{
    Vec3 rotation{20.0f, 30.0f, 40.0f};

    Quaternion quaternion = Quaternion::fromEuler(rotation);

    Mat4 quaternionMatrix = Mat4::fromQuaternion(quaternion);

    Mat4 expected = Mat4::rotate(rotation);

    requireMat4ApproxEqual(quaternionMatrix, expected);
}

TEST_CASE("Quaternion fromEuler X rotation matches rotate X")
{
    Quaternion q = Quaternion::fromEuler(Vec3{45.0f, 0.0f, 0.0f});

    Mat4 actual = Mat4::fromQuaternion(q);

    Mat4 expected = Mat4::rotateX(45.0f);

    requireMat4ApproxEqual(actual, expected);
}

TEST_CASE("Quaternion fromEuler Y rotation matches rotate Y")
{
    Quaternion q = Quaternion::fromEuler(Vec3{0.0f, 45.0f, 0.0f});

    Mat4 actual = Mat4::fromQuaternion(q);

    Mat4 expected = Mat4::rotateY(45.0f);

    requireMat4ApproxEqual(actual, expected);
}

TEST_CASE("Quaternion fromEuler Z rotation matches rotate Z")
{
    Quaternion q = Quaternion::fromEuler(Vec3{0.0f, 0.0f, 45.0f});

    Mat4 actual = Mat4::fromQuaternion(q);

    Mat4 expected = Mat4::rotateZ(45.0f);

    requireMat4ApproxEqual(actual, expected);
}