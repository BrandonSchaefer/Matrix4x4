// -*- Mode: C++; indent-tabs-mode: nil; tab-width: 2 -*-
/*
 * Copyright (C) 2014
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Brandon Schaefer <brandon.schaefer@canonical.com>
 */

#include "Matrix4x4.h"

#include <cmath>
#include <limits>
#include <string.h>

namespace matrix
{

Matrix4x4::Matrix4x4()
{
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      matrix_.m[i][j] = 0.0f;

  matrix_.m[0][0] = 1.0f;
  matrix_.m[1][1] = 1.0f;
  matrix_.m[2][2] = 1.0f;
  matrix_.m[3][3] = 1.0f;
}

Matrix4x4::Matrix4x4(Matrix4x4 const& matrix)
{
  GLMatrix4x4 mb = matrix.GetGLMatrix();

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      matrix_.m[i][j] = mb.m[i][j];
}

Matrix4x4::Matrix4x4(GLMatrix4x4 const& matrix)
{
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      matrix_.m[i][j] = matrix.m[i][j];
}

void Matrix4x4::Scale(GLfloat scale_x, GLfloat scale_y, GLfloat scale_z)
{
  GLfloat scales[3] = {scale_x, scale_y, scale_z};

  for (int i = 0; i < 3; ++i)
  {
    matrix_.m[i][0] *= scales[i];
    matrix_.m[i][1] *= scales[i];
    matrix_.m[i][2] *= scales[i];
    matrix_.m[i][3] *= scales[i];
  }
}

void Matrix4x4::Translate(GLfloat tran_x, GLfloat tran_y, GLfloat tran_z)
{
  for (int i = 0; i < 4; ++i)
    matrix_.m[3][i] += (matrix_.m[0][i] * tran_x +
                        matrix_.m[1][i] * tran_y +
                        matrix_.m[2][i] * tran_z);
}

void CopyGLMatrix(GLMatrix4x4* ma, GLMatrix4x4 mb)
{
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      ma->m[i][j] = mb.m[i][j];
}

void Matrix4x4::Frustum(GLfloat left, GLfloat right, GLfloat bottom,
                        GLfloat top, GLfloat near_z, GLfloat far_z)
{
  GLfloat delta_x = right - left;
  GLfloat delta_y = top - bottom;
  GLfloat delta_z = far_z - near_z;
  GLMatrix4x4 frust;

  if (delta_x <= 0.0f || delta_y <= 0.0f || delta_z <= 0.0f ||
      near_z <= 0.0f || far_z <= 0.0f)
  {
    return;
  }

  frust.m[0][0] = 2.0f * near_z / delta_x;
  frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

  frust.m[1][1] = 2.0f * near_z / delta_y;
  frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

  frust.m[2][0] = (right + left) / delta_x;
  frust.m[2][1] = (top + bottom) / delta_y;
  frust.m[2][2] = -(near_z + far_z) / delta_z;
  frust.m[2][3] = -1.0f;

  frust.m[3][2] = -2.0f * near_z * far_z / delta_z;
  frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

  Matrix4x4 new_m = *this * Matrix4x4(frust);
  CopyGLMatrix(&matrix_, new_m.GetGLMatrix());
}

void Matrix4x4::Perspective(GLfloat fovy, GLfloat aspect, GLfloat near_z, GLfloat far_z)
{
  GLfloat frustum_w, frustum_h;

  frustum_h = tanf(fovy / 360.0f * M_PI) * near_z;
  frustum_w = frustum_h * aspect;

  Frustum(-frustum_w, frustum_w, -frustum_h, frustum_h, near_z, far_z);
}

void Matrix4x4::Rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
  GLfloat sin_angle, cos_angle;
  GLfloat mag = sqrtf(x * x + y * y + z * z);

  sin_angle = sinf(angle * M_PI / 180.0f);
  cos_angle = cosf(angle * M_PI / 180.0f);

  if (mag > 0.0f)
  {
    GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
    GLfloat one_minus_cos;
    GLMatrix4x4 rotate;

    x /= mag;
    y /= mag;
    z /= mag;

    xx = x * x;
    yy = y * y;
    zz = z * z;
    xy = x * y;
    yz = y * z;
    zx = z * x;
    xs = x * sin_angle;
    ys = y * sin_angle;
    zs = z * sin_angle;
    one_minus_cos = 1.0f - cos_angle;

    rotate.m[0][0] = (one_minus_cos * xx) + cos_angle;
    rotate.m[0][1] = (one_minus_cos * xy) - zs;
    rotate.m[0][2] = (one_minus_cos * zx) + ys;
    rotate.m[0][3] = 0.0f;

    rotate.m[1][0] = (one_minus_cos * xy) + zs;
    rotate.m[1][1] = (one_minus_cos * yy) + cos_angle;
    rotate.m[1][2] = (one_minus_cos * yz) - xs;
    rotate.m[1][3] = 0.0f;

    rotate.m[2][0] = (one_minus_cos * zx) - ys;
    rotate.m[2][1] = (one_minus_cos * yz) + xs;
    rotate.m[2][2] = (one_minus_cos * zz) + cos_angle;
    rotate.m[2][3] = 0.0f;

    rotate.m[3][0] = 0.0f;
    rotate.m[3][1] = 0.0f;
    rotate.m[3][2] = 0.0f;
    rotate.m[3][3] = 1.0f;

    Matrix4x4 new_m = *this * Matrix4x4(rotate);
    CopyGLMatrix(&matrix_, new_m.GetGLMatrix());
  }
}

void Matrix4x4::Orthographic(GLfloat left, GLfloat right, GLfloat bottom,
                             GLfloat top, GLfloat near_z, GLfloat far_z)
{
  float delta_x = right - left;
  float delta_y = top - bottom;
  float delta_z = far_z - near_z;
  GLMatrix4x4 ortho;

  if (delta_x == 0.0f || delta_y == 0.0f || delta_z == 0.0f)
    return;

  ortho.m[0][0] = 2.0f / delta_x;
  ortho.m[3][0] = -(right + left) / delta_x;
  ortho.m[1][1] = 2.0f / delta_y;
  ortho.m[3][1] = -(top + bottom) / delta_y;
  ortho.m[2][2] = -2.0f / delta_z;
  ortho.m[3][2] = -(near_z + far_z) / delta_z;

  Matrix4x4 new_m = *this * Matrix4x4(ortho);
  CopyGLMatrix(&matrix_, new_m.GetGLMatrix());
}

GLMatrix4x4 Matrix4x4::GetGLMatrix() const
{
  return matrix_;
}

Matrix4x4 Matrix4x4::operator*(Matrix4x4 const& m) const
{
  GLMatrix4x4 new_m;
  GLMatrix4x4 mb = m.GetGLMatrix();

  for (int i = 0; i < 4; ++i)
  {
    new_m.m[i][0] = (matrix_.m[i][0] * mb.m[0][0]) +
                    (matrix_.m[i][1] * mb.m[1][0]) +
                    (matrix_.m[i][2] * mb.m[2][0]) +
                    (matrix_.m[i][3] * mb.m[3][0]);

    new_m.m[i][1] = (matrix_.m[i][0] * mb.m[0][1]) +
                    (matrix_.m[i][1] * mb.m[1][1]) +
                    (matrix_.m[i][2] * mb.m[2][1]) +
                    (matrix_.m[i][3] * mb.m[3][1]);

    new_m.m[i][2] = (matrix_.m[i][0] * mb.m[0][2]) +
                    (matrix_.m[i][1] * mb.m[1][2]) +
                    (matrix_.m[i][2] * mb.m[2][2]) +
                    (matrix_.m[i][3] * mb.m[3][2]);

    new_m.m[i][3] = (matrix_.m[i][0] * mb.m[0][3]) +
                    (matrix_.m[i][1] * mb.m[1][3]) +
                    (matrix_.m[i][2] * mb.m[2][3]) +
                    (matrix_.m[i][3] * mb.m[3][3]);
  }

  return Matrix4x4(new_m);
}

bool CheckFloatEquailty(float a, float b)
{
  return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

bool Matrix4x4::operator==(Matrix4x4 const& m) const
{
  GLMatrix4x4 ma = this->GetGLMatrix();
  GLMatrix4x4 mb = m.GetGLMatrix();

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      if (!CheckFloatEquailty(ma.m[i][j], mb.m[i][j]))
        return false;

  return true;
}

bool Matrix4x4::operator!=(Matrix4x4 const& m) const
{
  return !(*this == m);
}

}
