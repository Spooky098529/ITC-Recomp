// Measures how far a blended camera matrix puts points on screen from the
// true in-between camera, for straight motion and turns. Build and run:
//   clang++ -std=c++23 -O1 -I src tests/interpolation_screen_error.cpp \
//     src/interpolation/transform.cpp -o /tmp/interpolation_screen_error

#include "interpolation/transform.h"
#include <cmath>
#include <cstdio>
using namespace interpolation;
static Matrix4 CameraMatrix(float yaw, Vec3 c) {
  Transform t; t.rotation = {0, std::sin(yaw / 2), 0, std::cos(yaw / 2)}; t.position = c;
  Matrix4 view = ViewMatrixFromTransform(t);
  float n = 0.5f, f = 5000, sy = 2.4f, sx = 1.35f;
  Matrix4 p{sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, f / (f - n), 1, 0, 0, -n * f / (f - n), 0};
  return Multiply(view, p);
}
// Screen position in pixels (1280x720) of world point x through m (row vectors).
static void Project(const Matrix4& m, Vec3 x, double& px, double& py) {
  double c[4];
  for (int j = 0; j < 4; j++) c[j] = x[0] * m[j] + x[1] * m[4 + j] + x[2] * m[8 + j] + m[12 + j];
  px = (c[0] / c[3] * 0.5 + 0.5) * 1280; py = (c[1] / c[3] * 0.5 + 0.5) * 720;
}
int main() {
  Vec3 ca{-2178, 16.7f, 919}, cb{-2176.5f, 16.7f, 920.3f};
  for (float degrees : {0.0f, 3.0f, 8.0f}) {
    float ya = 1.0f, yb = 1.0f + degrees * 3.14159265f / 180;
    Matrix4 a = CameraMatrix(ya, ca), b = CameraMatrix(yb, cb);
    auto motion = FindRigidMotion(a, b);
    if (!motion) { printf("%.0f deg: not rigid\n", degrees); continue; }
    RigidMotion no_pivot = *motion; no_pivot.pivot.reset();
    double worst_pivot = 0, worst_plain = 0;
    for (float t : {0.25f, 0.5f, 0.75f}) {
      Vec3 c; for (int k = 0; k < 3; k++) c[k] = ca[k] + (cb[k] - ca[k]) * t;
      float yaw = ya + (yb - ya) * t;
      Matrix4 truth = CameraMatrix(yaw, c), with = BlendRigidMotion(*motion, t), without = BlendRigidMotion(no_pivot, t);
      for (float d : {5.0f, 20.0f, 100.0f}) for (float side : {-0.6f, 0.0f, 0.6f}) for (float up : {-0.3f, 0.3f}) {
        // A point d metres ahead of the true camera, spread across the view.
        // Camera forward is row 2 = (-sin, 0, cos); right is row 0 = (cos, 0, sin).
        float fx = -std::sin(yaw), fz = std::cos(yaw), rx = std::cos(yaw), rz = std::sin(yaw);
        Vec3 x{c[0] + fx * d + rx * side * d, c[1] + up * d, c[2] + fz * d + rz * side * d};
        double tx, ty, wx, wy, nx, ny;
        Project(truth, x, tx, ty); Project(with, x, wx, wy); Project(without, x, nx, ny);
        worst_pivot = std::fmax(worst_pivot, std::hypot(wx - tx, wy - ty));
        worst_plain = std::fmax(worst_plain, std::hypot(nx - tx, ny - ty));
      }
    }
    printf("%.0f deg turn per tick: worst screen error %.2f px with pivot, %.2f px without\n", degrees, worst_pivot, worst_plain);
  }
}
