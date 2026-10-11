// Rigid transforms (rotation + position) and blending between them, plus
// conversion to and from the game's 4x4 matrices.
//
// The game uses row-major matrices with row vectors (p' = p * M): rows 0-2
// are the rotation, row 3 is the translation and column 3 is (0, 0, 0, 1).
// Pure host code.

#pragma once

#include <array>
#include <optional>

namespace interpolation {

using Vec3 = std::array<float, 3>;
using Matrix4 = std::array<float, 16>;

struct Quaternion {
  float x = 0, y = 0, z = 0, w = 1;
};

struct Transform {
  Quaternion rotation;
  Vec3 position{};
};

// A world matrix places an object: its translation is the object's position.
// Returns nothing if the matrix isn't a pure rotation plus translation.
std::optional<Transform> TransformFromWorldMatrix(const Matrix4& m);
Matrix4 WorldMatrixFromTransform(const Transform& t);

// A view matrix is the inverse of the camera's world matrix.
std::optional<Transform> TransformFromViewMatrix(const Matrix4& m);
Matrix4 ViewMatrixFromTransform(const Transform& t);

// Blends from `a` (amount 0) to `b` (amount 1): positions linearly,
// rotations along the shortest arc.
Transform Blend(const Transform& a, const Transform& b, float amount);

float Distance(const Vec3& a, const Vec3& b);

// Angle in radians between two rotations.
float AngleBetween(const Quaternion& a, const Quaternion& b);

}  // namespace interpolation
