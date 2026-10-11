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

// Blends between two 4x4 matrices that differ by a rigid motion, such as
// view-projection or world-view-projection matrices from consecutive ticks:
// a * D or D * a for a rotation plus translation D, in either layout. Blends
// D from identity, so the result stays a proper transform instead of
// shrinking mid-turn as an element-wise blend would. Returns nothing if the
// matrices don't differ by a rigid motion (not a transform, or the
// projection changed).
struct RigidMotion {
  Matrix4 from;      // the earlier matrix
  Transform motion;  // the rigid motion D, in row layout
  bool applied_first;  // b = D * a (true) or b = a * D
  bool column_layout;  // D's translation is in column 3 rather than row 3
  // The point D's rotation is blended about: the camera's eye for a
  // perspective matrix. Blending a turn about the world origin (kilometres
  // away) would swing the in-between camera sideways.
  std::optional<Vec3> pivot;
};
std::optional<RigidMotion> FindRigidMotion(const Matrix4& a, const Matrix4& b);
Matrix4 BlendRigidMotion(const RigidMotion& motion, float amount);

// General 4x4 matrix helpers (computed in double precision).
Matrix4 Multiply(const Matrix4& a, const Matrix4& b);
std::optional<Matrix4> Inverse(const Matrix4& m);

// Whether `m` is an affine transform in the game's row layout: column 3 is
// (0, 0, 0, 1). Rotation, scale and translation are all allowed.
bool IsAffineRowLayout(const Matrix4& m);

float Distance(const Vec3& a, const Vec3& b);

// Angle in radians between two rotations.
float AngleBetween(const Quaternion& a, const Quaternion& b);

}  // namespace interpolation
