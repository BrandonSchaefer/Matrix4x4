#include <gtest/gtest.h>
#include <Matrix4x4.h>

using namespace matrix;

GLMatrix4x4 MAT_A = {1.0f, 2.0f, 3.0f, 4.0f,
                     5.0f, 6.0f, 7.0f, 8.0f,
                     8.0f, 7.0f, 6.0f, 5.0f,
                     4.0f, 3.0f, 2.0f, 1.0f};

TEST(TestMatrix4x4, TestMatrixCtor)
{
  Matrix4x4 m;
  GLMatrix4x4 gl_m = m.GetGLMatrix();

  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 4; ++j)
    {
      if (i ==j)
      {
        EXPECT_FLOAT_EQ(gl_m.m[i][j], 1.0f);
      }
      else
      {
        EXPECT_FLOAT_EQ(gl_m.m[i][j], 0.0f);
      }
    }
  }
}

TEST(TestMatrix4x4, TestMatrixScale)
{
  Matrix4x4 m;

  // Scale the id matrix by 2,3,4
  m.Scale(2.0f, 3.0f, 4.0f);

  GLMatrix4x4 gl_m = m.GetGLMatrix();
  EXPECT_FLOAT_EQ(gl_m.m[0][0], 2.0f);
  EXPECT_FLOAT_EQ(gl_m.m[1][1], 3.0f);
  EXPECT_FLOAT_EQ(gl_m.m[2][2], 4.0f);
}

TEST(TestMatrix4x4, TestMatrixTranslate)
{
  Matrix4x4 m;
  m.Translate(2.0f, 3.0f, 4.0f);

  GLMatrix4x4 gl_m = m.GetGLMatrix();
  EXPECT_FLOAT_EQ(gl_m.m[3][0], 2.0f);
  EXPECT_FLOAT_EQ(gl_m.m[3][1], 3.0f);
  EXPECT_FLOAT_EQ(gl_m.m[3][2], 4.0f);
  EXPECT_FLOAT_EQ(gl_m.m[3][3], 1.0f);
}

TEST(TestMatrix4x4, TestMatrixFrustum)
{
  Matrix4x4 m;
  m.Frustum(-5.0f, 5.0f, -5.0f, 5.0f, 1.0f, 1000.0f);

  GLMatrix4x4 gl_m = m.GetGLMatrix();
  EXPECT_FLOAT_EQ(gl_m.m[0][0], 0.2f);
  EXPECT_FLOAT_EQ(gl_m.m[1][1], 0.2f);
  EXPECT_FLOAT_EQ(gl_m.m[2][3], -1.0f);

  EXPECT_NEAR(gl_m.m[2][2], -1.0f, 0.01f);
  EXPECT_NEAR(gl_m.m[3][2], -2.0f, 0.01f);
}

TEST(TestMatrix4x4, TestMatrixPerspective)
{
  Matrix4x4 m;
  m.Perspective(60.0f, 1.0f, 1.0f, 20.f);

  GLMatrix4x4 gl_m = m.GetGLMatrix();
  EXPECT_NEAR(gl_m.m[0][0], 1.73f, 0.01f);
  EXPECT_NEAR(gl_m.m[1][1], 1.73f, 0.01f);
  EXPECT_NEAR(gl_m.m[2][2], -1.10f, 0.01f);
  EXPECT_NEAR(gl_m.m[2][3], -1.0f, 0.01f);
  EXPECT_NEAR(gl_m.m[3][2], -2.1f, 0.01f);
}

TEST(TestMatrix4x4, TestMatrixRotate)
{
  Matrix4x4 m;
  m.Rotate(35.0f, 1.0f, 0.0f, 1.0f);

  GLMatrix4x4 gl_m = m.GetGLMatrix();
  EXPECT_NEAR(gl_m.m[0][0], 0.90f, 0.01f);
  EXPECT_NEAR(gl_m.m[0][1], -0.40f, 0.01f);
  EXPECT_NEAR(gl_m.m[0][2], 0.09f, 0.01f);

  EXPECT_NEAR(gl_m.m[1][0], 0.40f, 0.01f);
  EXPECT_NEAR(gl_m.m[1][1], 0.81f, 0.01f);
  EXPECT_NEAR(gl_m.m[1][2], -0.40f, 0.01f);

  EXPECT_NEAR(gl_m.m[2][0], 0.09f, 0.01f);
  EXPECT_NEAR(gl_m.m[2][1], 0.40f, 0.01f);
  EXPECT_NEAR(gl_m.m[2][2], 0.90f, 0.01f);

  EXPECT_NEAR(gl_m.m[3][3], 1.0f, 0.01f);
}

TEST(TestMatrix4x4, TestMatrixMultiply)
{
  Matrix4x4 m1;
  Matrix4x4 m2(MAT_A);

  m1 = m1 * m2;

  EXPECT_TRUE(m1 == m2);
}

TEST(TestMatrix4x4, TestMatrixEquailty)
{
  Matrix4x4 m1(MAT_A);
  Matrix4x4 m2(MAT_A);

  EXPECT_TRUE(m1 == m2);
}

TEST(TestMatrix4x4, TestMatrixInequailty)
{
  Matrix4x4 m1(MAT_A);
  Matrix4x4 m2(MAT_A);

  EXPECT_FALSE(m1 != m2);
}
