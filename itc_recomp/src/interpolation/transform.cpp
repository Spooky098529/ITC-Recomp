#include "transform.h"

#include <algorithm>
#include <cmath>

namespace interpolation {
namespace {

using Matrix3 = std::array<std::array<float, 3>, 3>;

Matrix3 RotationOf(const Matrix4& m) {
  Matrix3 r;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      r[i][j] = m[i * 4 + j];
    }
  }
  return r;
}

Matrix3 Transposed(const Matrix3& r) {
  Matrix3 t;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      t[i][j] = r[j][i];
    }
  }
  return t;
}

// Row vector times matrix.
Vec3 Multiply(const Vec3& v, const Matrix3& r) {
  Vec3 out{};
  for (int j = 0; j < 3; j++) {
    out[j] = v[0] * r[0][j] + v[1] * r[1][j] + v[2] * r[2][j];
  }
  return out;
}

// True for an orthonormal matrix with determinant +1 (no scale or mirroring),
// the only kind a quaternion can represent.
bool IsRotation(const Matrix3& r) {
  constexpr float kTolerance = 0.01f;
  for (int i = 0; i < 3; i++) {
    for (int j = i; j < 3; j++) {
      float dot = r[i][0] * r[j][0] + r[i][1] * r[j][1] + r[i][2] * r[j][2];
      float expected = i == j ? 1.0f : 0.0f;
      if (!(std::abs(dot - expected) < kTolerance)) {
        return false;
      }
    }
  }
  float determinant = r[0][0] * (r[1][1] * r[2][2] - r[1][2] * r[2][1]) -
                      r[0][1] * (r[1][0] * r[2][2] - r[1][2] * r[2][0]) +
                      r[0][2] * (r[1][0] * r[2][1] - r[1][1] * r[2][0]);
  return determinant > 0;
}

Quaternion QuaternionFromRotation(const Matrix3& m) {
  Quaternion q;
  float trace = m[0][0] + m[1][1] + m[2][2];
  if (trace > 0) {
    float s = 0.5f / std::sqrt(trace + 1.0f);
    q.w = 0.25f / s;
    q.x = (m[2][1] - m[1][2]) * s;
    q.y = (m[0][2] - m[2][0]) * s;
    q.z = (m[1][0] - m[0][1]) * s;
  } else if (m[0][0] > m[1][1] && m[0][0] > m[2][2]) {
    float s = 2.0f * std::sqrt(1.0f + m[0][0] - m[1][1] - m[2][2]);
    q.w = (m[2][1] - m[1][2]) / s;
    q.x = 0.25f * s;
    q.y = (m[0][1] + m[1][0]) / s;
    q.z = (m[0][2] + m[2][0]) / s;
  } else if (m[1][1] > m[2][2]) {
    float s = 2.0f * std::sqrt(1.0f + m[1][1] - m[0][0] - m[2][2]);
    q.w = (m[0][2] - m[2][0]) / s;
    q.x = (m[0][1] + m[1][0]) / s;
    q.y = 0.25f * s;
    q.z = (m[1][2] + m[2][1]) / s;
  } else {
    float s = 2.0f * std::sqrt(1.0f + m[2][2] - m[0][0] - m[1][1]);
    q.w = (m[1][0] - m[0][1]) / s;
    q.x = (m[0][2] + m[2][0]) / s;
    q.y = (m[1][2] + m[2][1]) / s;
    q.z = 0.25f * s;
  }
  return q;
}

Matrix3 RotationFromQuaternion(const Quaternion& q) {
  float xx = q.x * q.x, yy = q.y * q.y, zz = q.z * q.z;
  float xy = q.x * q.y, xz = q.x * q.z, yz = q.y * q.z;
  float wx = q.w * q.x, wy = q.w * q.y, wz = q.w * q.z;
  Matrix3 m;
  m[0] = {1 - 2 * (yy + zz), 2 * (xy - wz), 2 * (xz + wy)};
  m[1] = {2 * (xy + wz), 1 - 2 * (xx + zz), 2 * (yz - wx)};
  m[2] = {2 * (xz - wy), 2 * (yz + wx), 1 - 2 * (xx + yy)};
  return m;
}

float Dot(const Quaternion& a, const Quaternion& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

Quaternion Normalized(const Quaternion& q) {
  float length = std::sqrt(Dot(q, q));
  return {q.x / length, q.y / length, q.z / length, q.w / length};
}

Quaternion Slerp(const Quaternion& a, Quaternion b, float amount) {
  float dot = Dot(a, b);
  if (dot < 0) {
    // q and -q are the same rotation; take the shorter way round.
    b = {-b.x, -b.y, -b.z, -b.w};
    dot = -dot;
  }
  float wa, wb;
  if (dot > 0.9995f) {
    // Nearly identical: a straight blend is accurate and avoids dividing by
    // a tiny sine.
    wa = 1 - amount;
    wb = amount;
  } else {
    float angle = std::acos(dot);
    float sine = std::sin(angle);
    wa = std::sin((1 - amount) * angle) / sine;
    wb = std::sin(amount * angle) / sine;
  }
  return Normalized({wa * a.x + wb * b.x, wa * a.y + wb * b.y, wa * a.z + wb * b.z,
                     wa * a.w + wb * b.w});
}

Matrix4 MatrixFrom(const Matrix3& rotation, const Vec3& translation) {
  Matrix4 m{};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      m[i * 4 + j] = rotation[i][j];
    }
  }
  m[12] = translation[0];
  m[13] = translation[1];
  m[14] = translation[2];
  m[15] = 1;
  return m;
}

using Matrix4d = std::array<double, 16>;

Matrix4d Multiply(const Matrix4d& a, const Matrix4d& b) {
  Matrix4d c{};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      double sum = 0;
      for (int k = 0; k < 4; k++) {
        sum += a[i * 4 + k] * b[k * 4 + j];
      }
      c[i * 4 + j] = sum;
    }
  }
  return c;
}

std::optional<Matrix4d> Inverse(const Matrix4d& m) {
  // Gauss-Jordan elimination with partial pivoting.
  Matrix4d a = m;
  Matrix4d inv{};
  for (int i = 0; i < 4; i++) {
    inv[i * 4 + i] = 1;
  }
  for (int col = 0; col < 4; col++) {
    int pivot = col;
    for (int row = col + 1; row < 4; row++) {
      if (std::abs(a[row * 4 + col]) > std::abs(a[pivot * 4 + col])) {
        pivot = row;
      }
    }
    if (std::abs(a[pivot * 4 + col]) < 1e-12) {
      return std::nullopt;
    }
    for (int k = 0; k < 4; k++) {
      std::swap(a[col * 4 + k], a[pivot * 4 + k]);
      std::swap(inv[col * 4 + k], inv[pivot * 4 + k]);
    }
    double scale = 1 / a[col * 4 + col];
    for (int k = 0; k < 4; k++) {
      a[col * 4 + k] *= scale;
      inv[col * 4 + k] *= scale;
    }
    for (int row = 0; row < 4; row++) {
      if (row == col) {
        continue;
      }
      double factor = a[row * 4 + col];
      for (int k = 0; k < 4; k++) {
        a[row * 4 + k] -= factor * a[col * 4 + k];
        inv[row * 4 + k] -= factor * inv[col * 4 + k];
      }
    }
  }
  return inv;
}

Matrix4d Transposed4(const Matrix4d& m) {
  Matrix4d t;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      t[i * 4 + j] = m[j * 4 + i];
    }
  }
  return t;
}

// Whether `d` is a rigid motion with its translation in row 3 (the game's
// world matrix layout).
bool IsRigidRowLayout(const Matrix4d& d) {
  constexpr double kTolerance = 1e-3;
  if (std::abs(d[3]) > kTolerance || std::abs(d[7]) > kTolerance ||
      std::abs(d[11]) > kTolerance || std::abs(d[15] - 1) > kTolerance) {
    return false;
  }
  Matrix3 r;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      r[i][j] = float(d[i * 4 + j]);
    }
  }
  return IsRotation(r);
}

Matrix4 ToFloat(const Matrix4d& m) {
  Matrix4 f;
  for (int i = 0; i < 16; i++) {
    f[i] = float(m[i]);
  }
  return f;
}

Matrix4d ToDouble(const Matrix4& m) {
  Matrix4d d;
  for (int i = 0; i < 16; i++) {
    d[i] = m[i];
  }
  return d;
}

}  // namespace

std::optional<RigidMotion> FindRigidMotion(const Matrix4& a_in, const Matrix4& b_in) {
  Matrix4d a = ToDouble(a_in);
  Matrix4d b = ToDouble(b_in);
  std::optional<Matrix4d> a_inverse = Inverse(a);
  if (!a_inverse) {
    return std::nullopt;
  }
  // b = D * a or b = a * D, with D in row or column layout.
  for (bool applied_first : {true, false}) {
    Matrix4d d = applied_first ? Multiply(b, *a_inverse) : Multiply(*a_inverse, b);
    for (bool column_layout : {false, true}) {
      Matrix4d row_d = column_layout ? Transposed4(d) : d;
      if (!IsRigidRowLayout(row_d)) {
        continue;
      }
      std::optional<Transform> motion = TransformFromWorldMatrix(ToFloat(row_d));
      if (!motion) {
        continue;
      }
      return RigidMotion{a_in, *motion, applied_first, column_layout};
    }
  }
  return std::nullopt;
}

Matrix4 BlendRigidMotion(const RigidMotion& rigid, float amount) {
  Transform partial = Blend(Transform{}, rigid.motion, amount);
  Matrix4d d = ToDouble(WorldMatrixFromTransform(partial));
  if (rigid.column_layout) {
    d = Transposed4(d);
  }
  Matrix4d a = ToDouble(rigid.from);
  return ToFloat(rigid.applied_first ? Multiply(d, a) : Multiply(a, d));
}

std::optional<Transform> TransformFromWorldMatrix(const Matrix4& m) {
  Matrix3 rotation = RotationOf(m);
  if (!IsRotation(rotation)) {
    return std::nullopt;
  }
  return Transform{QuaternionFromRotation(rotation), {m[12], m[13], m[14]}};
}

Matrix4 WorldMatrixFromTransform(const Transform& t) {
  return MatrixFrom(RotationFromQuaternion(t.rotation), t.position);
}

std::optional<Transform> TransformFromViewMatrix(const Matrix4& m) {
  // view = inverse(world): world rotation is the transpose of the view
  // rotation, and the camera sits where the view translation maps to zero.
  Matrix3 view_rotation = RotationOf(m);
  if (!IsRotation(view_rotation)) {
    return std::nullopt;
  }
  Matrix3 world_rotation = Transposed(view_rotation);
  Vec3 view_translation = {m[12], m[13], m[14]};
  Vec3 position = Multiply(view_translation, world_rotation);
  for (float& v : position) {
    v = -v;
  }
  return Transform{QuaternionFromRotation(world_rotation), position};
}

Matrix4 ViewMatrixFromTransform(const Transform& t) {
  Matrix3 view_rotation = Transposed(RotationFromQuaternion(t.rotation));
  Vec3 translation = Multiply(t.position, view_rotation);
  for (float& v : translation) {
    v = -v;
  }
  return MatrixFrom(view_rotation, translation);
}

Transform Blend(const Transform& a, const Transform& b, float amount) {
  Transform out;
  out.rotation = Slerp(a.rotation, b.rotation, amount);
  for (int k = 0; k < 3; k++) {
    out.position[k] = a.position[k] + (b.position[k] - a.position[k]) * amount;
  }
  return out;
}

float Distance(const Vec3& a, const Vec3& b) {
  float dx = a[0] - b[0], dy = a[1] - b[1], dz = a[2] - b[2];
  return std::sqrt(dx * dx + dy * dy + dz * dz);
}

float AngleBetween(const Quaternion& a, const Quaternion& b) {
  float dot = std::min(1.0f, std::abs(Dot(a, b)));
  return 2 * std::acos(dot);
}

}  // namespace interpolation
