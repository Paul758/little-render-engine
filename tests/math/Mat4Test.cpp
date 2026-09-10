#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "math/Mat4.h"

#include "components/TransformComponent.h"
#include "math/MathUtils.h"

using Catch::Approx;

inline void requireMat4ApproxEqual(const Mat4& actual, const Mat4& expected, float epsilon = 0.00001f)
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

    Vec3 result = mat.transformPoint(vec);

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

    Vec3 result = mat.transformPoint(vec);

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

    Vec3 rotated = rotation.transformDirection(forward);

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

TEST_CASE("Mat4 rotate with only Y rotation equals rotateY")
{
    Mat4 combined = Mat4::rotate(Vec3{0.0f, 45.0f, 0.0f});
    Mat4 yOnly = Mat4::rotateY(45.0f);

    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t column = 0; column < 4; ++column)
        {
            REQUIRE(combined.at(row, column) == Approx(yOnly.at(row, column)));
        }
    }
}

TEST_CASE("Mat4 rotate with only Z rotation equals rotateZ")
{
    Mat4 combined = Mat4::rotate(Vec3{0.0f, 0.0f, 45.0f});
    Mat4 zOnly = Mat4::rotateZ(45.0f);

    for (std::size_t row = 0; row < 4; ++row)
    {
        for (std::size_t column = 0; column < 4; ++column)
        {
            REQUIRE(combined.at(row, column) == Approx(zOnly.at(row, column)));
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

TEST_CASE("Identity quaternion does not affect model rotation")
{
    TransformComponent transform;
    transform.position = Vec3{2.0f, 3.0f, 4.0f};
    transform.rotation = Quaternion::identity();
    transform.scale = Vec3{1.0f, 1.0f, 1.0f};

    Mat4 model = transform.getModelMatrix();

    Mat4 expected = Mat4::translate(transform.position);

    requireMat4ApproxEqual(model, expected);
}

TEST_CASE("Quaternion rotation contributes correctly to model matrix")
{
    TransformComponent transform;
    transform.position = Vec3{0.0f, 0.0f, 0.0f};

    transform.rotation = Quaternion::fromAxisAngle(Vec3{0.0f, 1.0f, 0.0f}, 90.0f);
    transform.scale = Vec3{1.0f, 1.0f, 1.0f};

    Mat4 model = transform.getModelMatrix();

    Mat4 expected = Mat4::rotateY(90.0f);

    requireMat4ApproxEqual(model, expected);
}

/*---------------------------------------------------*/
/*----------View and Projection Matrix tests---------*/
/*---------------------------------------------------*/

TEST_CASE("View matrix is identity for canonical camera")
{
    Vec3 position{0.0f, 0.0f, 0.0f};
    Vec3 forward{0.0f, 0.0f, -1.0f};
    Vec3 right{1.0f, 0.0f, 0.0f};
    Vec3 up{0.0f, 1.0f, 0.0f};

    Mat4 actual = Mat4::view(
        position,
        forward,
        right,
        up
    );

    Mat4 expected = Mat4::identity();

    requireMat4ApproxEqual(actual, expected);
}

TEST_CASE("View matrix translates world opposite camera position")
{
    Vec3 position{0.0f, 0.0f, 5.0f};
    Vec3 forward{0.0f, 0.0f, -1.0f};
    Vec3 right{1.0f, 0.0f, 0.0f};
    Vec3 up{0.0f, 1.0f, 0.0f};

    Mat4 actual = Mat4::view(
        position,
        forward,
        right,
        up
    );

    REQUIRE(actual.at(0, 3) == Approx(0.0f));
    REQUIRE(actual.at(1, 3) == Approx(0.0f));
    REQUIRE(actual.at(2, 3) == Approx(-5.0f));
}

TEST_CASE("View matrix transforms camera position to origin")
{
    Vec3 cameraPosition{3.0f, 2.0f, 5.0f};

    Mat4 matrix = Mat4::view(
        cameraPosition,
        Vec3{0.0f, 0.0f, -1.0f},
        Vec3{1.0f, 0.0f, 0.0f},
        Vec3{0.0f, 1.0f, 0.0f});

    Vec3 result = matrix.transformPoint(cameraPosition);

    REQUIRE(result.x == Approx(0.0f).margin(0.00001f));
    REQUIRE(result.y == Approx(0.0f).margin(0.00001f));
    REQUIRE(result.z == Approx(0.0f).margin(0.00001f));
}

TEST_CASE("Perspective matrix has expected values")
{
    Mat4 matrix = Mat4::perspective(Math::radians(90.0f), 1.0f, 1.0f, 10.0f);
    
    REQUIRE(matrix.at(0, 0) == Approx(1.0f));
    REQUIRE(matrix.at(1, 1) == Approx(1.0f));

    REQUIRE(matrix.at(2, 2) == Approx(-11.0f / 9.0f));
    REQUIRE(matrix.at(2, 3) == Approx(-20.0f / 9.0f));

    REQUIRE(matrix.at(3, 2) == Approx(-1.0f));
}

TEST_CASE("Perspective rejects invalid aspect ratio")
{
    REQUIRE_THROWS_AS(Mat4::perspective(Math::radians(60.0f), 0.0f, 0.1f, 100.0f), std::invalid_argument);
}

TEST_CASE("Perspective rejects invalid near and far planes")
{
    REQUIRE_THROWS_AS(Mat4::perspective(Math::radians(60.0f), 1.0f, 0.0f, 100.0f), std::invalid_argument);
    REQUIRE_THROWS_AS(Mat4::perspective(Math::radians(60.0f), 1.0f, 10.0f, 5.0f), std::invalid_argument);
}

TEST_CASE("Orthographic matrix has expected values")
{
    Mat4 matrix = Mat4::orthographic(-10.0f, 10.0f, -5.0f, 5.0f, 1.0f, 11.0f);

    REQUIRE(matrix.at(0, 0) == Approx(0.1f));
    REQUIRE(matrix.at(1, 1) == Approx(0.2f));
    REQUIRE(matrix.at(2, 2) == Approx(-0.2f));
    REQUIRE(matrix.at(0, 3) == Approx(0.0f));
    REQUIRE(matrix.at(1, 3) == Approx(0.0f));
    REQUIRE(matrix.at(2, 3) == Approx(-1.2f));
    REQUIRE(matrix.at(3, 3) == Approx(1.0f));
}

TEST_CASE("Orthographic rejects invalid bounds")
{
    REQUIRE_THROWS_AS(Mat4::orthographic(1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f), std::invalid_argument);
}

/*---------------------------------------------------*/
/*----------Translating points and directions---------*/
/*---------------------------------------------------*/
TEST_CASE("Translation affects points")
{
    Mat4 matrix = Mat4::translate(Vec3{10.0f, 20.0f, 30.0f});
    Vec3 result = matrix.transformPoint(Vec3{1.0f, 2.0f, 3.0f});

    REQUIRE(result.x == Approx(11.0f));
    REQUIRE(result.y == Approx(22.0f));
    REQUIRE(result.z == Approx(33.0f));
}

TEST_CASE("Translation does not affect directions")
{
    Mat4 matrix = Mat4::translate(Vec3{10.0f, 20.0f, 30.0f});

    Vec3 result = matrix.transformDirection(Vec3{1.0f, 2.0f, 3.0f});

    REQUIRE(result.x == Approx(1.0f));
    REQUIRE(result.y == Approx(2.0f));
    REQUIRE(result.z == Approx(3.0f));
}