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

#ifndef MATRIX4x4_H
#define MATRIX4x4_H

#include <GLES2/gl2.h>

namespace matrix
{

typedef struct
{
  GLfloat m[4][4];
} GLMatrix4x4;

class Matrix4x4
{
public:
  Matrix4x4();
  Matrix4x4(Matrix4x4 const& matrix);
  Matrix4x4(GLMatrix4x4 const& matrix);

  void Scale(GLfloat scale_x, GLfloat scale_y, GLfloat scale_z);
  void Translate(GLfloat tran_x, GLfloat tran_y, GLfloat tran_z);
  void Frustum(GLfloat left, GLfloat right, GLfloat bottom,
               GLfloat top, GLfloat near_z, GLfloat far_z);
  void Perspective(GLfloat fovy, GLfloat aspect, GLfloat near_z, GLfloat far_z);
  void Rotate(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

  Matrix4x4 operator*(Matrix4x4 const& m) const;
  bool operator==(Matrix4x4 const& m) const;
  bool operator!=(Matrix4x4 const& m) const;

  GLMatrix4x4 GetGLMatrix() const;

private:
  GLMatrix4x4 matrix_;
};

}

#endif // MATRIX4x4_H
