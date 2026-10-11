#include "blend_state.h"

#include "guest.h"
#include "transform.h"

#include <rex/cvar.h>

#include <algorithm>
#include <vector>

REXCVAR_DEFINE_DOUBLE(debug_draw_camera_lift, 0.0, "ITC",
                      "Debug: raise the camera by this many metres in render-only passes");
REXCVAR_DEFINE_DOUBLE(debug_draw_vehicle_shift, 0.0, "ITC",
                      "Debug: shift vehicles sideways by this many metres in render-only "
                      "passes");

namespace interpolation {
namespace {

// See "Known data" in docs/functions.md.
constexpr uint32_t kCameraViewMatrixAddress = 0x826BAC10;
constexpr uint32_t kVehicleWorldMatrixOffsets[] = {0xD0, 0xCF0};
constexpr size_t kMaxVehicles = 16;

Matrix4 LoadMatrix(uint8_t* base, uint32_t address) {
  Matrix4 m;
  for (int i = 0; i < 16; i++) {
    m[i] = guest::LoadFloat(base, address + 4 * i);
  }
  return m;
}

void StoreMatrix(uint8_t* base, uint32_t address, const Matrix4& m) {
  for (int i = 0; i < 16; i++) {
    guest::StoreFloat(base, address + 4 * i, m[i]);
  }
}

// A matrix replaced for one Draw, and its real value.
struct Saved {
  uint32_t address;
  Matrix4 value;
};

std::vector<uint32_t> g_vehicles;
std::vector<Saved> g_saved;

void Replace(uint8_t* base, uint32_t address, const Matrix4& value) {
  g_saved.push_back({address, LoadMatrix(base, address)});
  StoreMatrix(base, address, value);
}

}  // namespace

void BlendState_OnVehicleSeen(uint32_t vehicle) {
  if (std::find(g_vehicles.begin(), g_vehicles.end(), vehicle) == g_vehicles.end() &&
      g_vehicles.size() < kMaxVehicles) {
    g_vehicles.push_back(vehicle);
  }
}

void BlendState_BeforeDraw(uint8_t* base, bool render_only_pass) {
  g_saved.clear();
  if (!render_only_pass) {
    return;
  }
  float lift = float(REXCVAR_GET(debug_draw_camera_lift));
  if (lift != 0) {
    Matrix4 view = LoadMatrix(base, kCameraViewMatrixAddress);
    if (auto camera = TransformFromViewMatrix(view)) {
      camera->position[1] += lift;
      Replace(base, kCameraViewMatrixAddress, ViewMatrixFromTransform(*camera));
    }
  }
  float shift = float(REXCVAR_GET(debug_draw_vehicle_shift));
  if (shift != 0) {
    for (uint32_t vehicle : g_vehicles) {
      for (uint32_t offset : kVehicleWorldMatrixOffsets) {
        Matrix4 world = LoadMatrix(base, vehicle + offset);
        // Move along the vehicle's own sideways axis (row 2).
        for (int k = 0; k < 3; k++) {
          world[12 + k] += shift * world[8 + k];
        }
        Replace(base, vehicle + offset, world);
      }
    }
  }
}

void BlendState_AfterDraw(uint8_t* base) {
  for (auto it = g_saved.rbegin(); it != g_saved.rend(); ++it) {
    StoreMatrix(base, it->address, it->value);
  }
  g_saved.clear();
}

}  // namespace interpolation
