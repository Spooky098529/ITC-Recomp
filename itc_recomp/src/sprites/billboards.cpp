#include "sprites/billboards.h"

#include "guest.h"

#include <rex/platform/fpscr.h>
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>

namespace sprites {

void InitBillboardMaterial(uint8_t* base, uint32_t material) {
  guest::StoreU32(base, material + 8, 0);
  guest::StoreU32(base, material + 0, 1);
  guest::StoreU32(base, material + 4, 1);
  guest::StoreU32(base, material + 12, 0);
  guest::StoreU32(base, material + 16, 0);
  guest::StoreU32(base, material + 32, 0);
  guest::StoreU32(base, material + 36, 0);
}

void SetBillboardLights(uint8_t* base, uint32_t material, uint32_t count, uint32_t lights) {
  if (count == 0) {
    return;
  }
  guest::StoreU32(base, material + 32, count);
  guest::StoreU32(base, material + 36, lights);
}

namespace {

// Globals and constants the translated code reads.
constexpr uint32_t kRingCount = 0x826A509C;   // vertices written into the ring so far
constexpr uint32_t kRingHandle = 0x826A50A8;  // recorded with command 10
constexpr uint32_t kRingIndex = 0x826A5098;   // the ring's part, 4,096 vertices each
constexpr uint32_t kRingVertices = 0x8266D098;
constexpr uint32_t kRingRoom = 4096;
constexpr uint32_t kOne = 0x82000A30;         // 1.0
constexpr uint32_t kZero = 0x82000A34;        // 0.0
constexpr uint32_t kForward = 0x82001BE8;     // the camera's forward axis' scale
constexpr uint32_t kSizeScale = 0x82001BE4;   // the light's width and height scale
constexpr uint32_t kOffsetW = 0x82001BE0;     // w of the light's offset (not read)
constexpr uint32_t kFarthest = 0x82001BDC;    // the draw distance's cap
constexpr uint32_t kVertexBytes = 28;
constexpr uint32_t kLightBytes = 48;

// Camera pass fields.
constexpr uint32_t kView = 16;  // its columns' x, y and z are the quad's axes
constexpr uint32_t kEye = 400;
constexpr uint32_t kLookAt = 416;
constexpr uint32_t kDrawDistance = 548;

// The command list slots' return addresses in the original.
constexpr uint32_t kBeginCall = 0x8209C754, kHandleCall = 0x8209C77C;
constexpr uint32_t kDrawCall = 0x8209CBDC, kDrawArgsCall = 0x8209CBFC;

float Load(uint8_t* base, uint32_t address) {
  return guest::LoadFloat(base, address);
}

// fctidz then stfiwx: the low word of the 64-bit integer, truncated.
uint32_t ToInteger(float value) {
  double d = value;
  int64_t i;
  if (std::isnan(d)) {
    i = INT64_MIN;
  } else if (d > double(LLONG_MAX)) {
    i = LLONG_MAX;
  } else {
    i = simde_mm_cvttsd_si64(simde_mm_load_sd(&d));
  }
  return uint32_t(i);
}

// A guest vector (words 0-3) as the translated code holds it (word 0 in
// the top lane).
simde__m128 Vector(float w0, float w1, float w2, float w3) {
  return simde_mm_set_ps(w0, w1, w2, w3);
}

float Word0(simde__m128 v) {
  return simde_mm_cvtss_f32(simde_mm_shuffle_ps(v, v, 0xFF));
}

// vrsqrtefp, then one Newton step as the game does (vnmsubfp, vmaddfp):
// v / |v| with the translated code's operations, or v's length squared
// where that's 0.
simde__m128 Normalize(simde__m128 v, simde__m128 length_squared, simde__m128 half) {
  const simde__m128 sign = simde_mm_castsi128_ps(simde_mm_set1_epi32(int(0x80000000)));
  simde__m128 zero = simde_mm_cmpeq_ps(length_squared, simde_mm_setzero_ps());
  simde__m128 estimate = simde_mm_div_ps(simde_mm_set1_ps(1), simde_mm_sqrt_ps(length_squared));
  simde__m128 half_length = simde_mm_mul_ps(length_squared, half);
  simde__m128 squared = simde_mm_mul_ps(estimate, estimate);
  simde__m128 step = simde_mm_xor_ps(simde_mm_sub_ps(simde_mm_mul_ps(half_length, squared), half), sign);
  simde__m128 inverse = simde_mm_add_ps(simde_mm_mul_ps(estimate, step), estimate);
  simde__m128 normalized = simde_mm_mul_ps(v, inverse);
  return simde_mm_or_ps(simde_mm_andnot_ps(zero, normalized), simde_mm_and_ps(zero, length_squared));
}

void StoreU32(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void StoreF(uint8_t* base, uint32_t address, float value) {
  guest::StoreFloat(base, address, value);
}

}  // namespace

uint32_t BuildBillboardVertices(uint8_t* base, uint32_t set, uint32_t material,
                                const SlotAllocator& slots) {
  using Fpscr = rex::platform::FPSCRPlatform;
  const int32_t alpha_mode = int32_t(guest::LoadU32(base, material + 0));
  uint32_t total = guest::LoadU32(base, material + 32);
  uint32_t type = 2;
  switch (alpha_mode) {
    case 2: case 4: case 8: case 16: case 32: case 64:
    case 256: case 512: case 1024: case 2048:
      total <<= 2;
      type = 13;
      break;
    default:
      break;
  }

  uint32_t slot = slots(1, kBeginCall);
  StoreU32(base, slot + 0, 10);
  StoreU32(base, slot + 4, 1);
  StoreU32(base, slot + 8, 0);
  slot = slots(1, kHandleCall);
  const uint32_t handle = guest::LoadU32(base, kRingHandle);
  const uint32_t ring_index = guest::LoadU32(base, kRingIndex);
  StoreU32(base, slot + 0, handle);
  const uint32_t ring_count = guest::LoadU32(base, kRingCount);
  const uint32_t first_vertex = ((ring_index << 12) + ring_count) * kVertexBytes + kRingVertices;
  uint32_t vertex = first_vertex;

  if (guest::LoadU32(base, material + 32) != 0) {
    const float one = Load(base, kOne);
    const float zero = Load(base, kZero);
    const float forward = Load(base, kForward);
    const float size_scale = Load(base, kSizeScale);
    const float offset_w = Load(base, kOffsetW);
    const float farthest = Load(base, kFarthest);
    const simde__m128 half = simde_mm_set1_ps(0.5f);
    const uint32_t csr = Fpscr::getcsr();
    const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
    for (uint32_t i = 0; i < guest::LoadU32(base, material + 32); i++) {
      const uint32_t light = guest::LoadU32(base, material + 36) + i * kLightBytes;
      if (!(Load(base, light + 32) > zero) && !(Load(base, light + 36) > zero)) {
        total -= 4;
        continue;
      }
      const uint32_t pass = guest::LoadU32(base, set);
      const float dx = Load(base, pass + kEye) - Load(base, light + 0);
      const float dy = Load(base, pass + kEye + 4) - Load(base, light + 4);
      const float dz = Load(base, pass + kEye + 8) - Load(base, light + 8);
      float distance = Load(base, pass + kDrawDistance);
      if (!(distance < farthest)) {
        distance = farthest;
      }
      distance = distance * distance;
      const simde__m128 offset = Vector(dx, dy, dz, one);
      const simde__m128 to_light = Vector(-dx, zero, -dz, offset_w);
      const simde__m128 look = Vector(Load(base, pass + kLookAt) - Load(base, pass + kEye), zero,
                                      Load(base, pass + kLookAt + 8) - Load(base, pass + kEye + 8),
                                      one);
      Fpscr::setcsr(flushing);
      const float distance_squared = Word0(simde_mm_dp_ps(offset, offset, 0xEF));
      const simde__m128 to_light_squared = simde_mm_dp_ps(to_light, to_light, 0xEF);
      const simde__m128 look_squared = simde_mm_dp_ps(look, look, 0xEF);
      const simde__m128 look_unit = Normalize(look, look_squared, half);
      const simde__m128 to_light_unit = Normalize(to_light, to_light_squared, half);
      const float facing = Word0(simde_mm_dp_ps(look_unit, to_light_unit, 0xEF));
      Fpscr::setcsr(csr);
      if (distance < distance_squared || facing < zero || ring_count + 4 > kRingRoom) {
        total -= 4;
        continue;
      }

      // The quad's axes: the view's columns times the light's width, its
      // height and the forward scale.
      const float width = Load(base, light + 32) * size_scale;
      const float height = Load(base, light + 36) * size_scale;
      const float rx = Load(base, pass + kView + 0) * width;
      const float ry = Load(base, pass + kView + 16) * width;
      const float rz = Load(base, pass + kView + 32) * width;
      const float ux = Load(base, pass + kView + 4) * height;
      const float uy = Load(base, pass + kView + 20) * height;
      const float uz = Load(base, pass + kView + 36) * height;
      const float fx = Load(base, pass + kView + 8) * forward;
      const float fy = Load(base, pass + kView + 24) * forward;
      const float fz = Load(base, pass + kView + 40) * forward;
      const uint32_t r = ToInteger(Load(base, light + 16));
      const uint32_t g = ToInteger(Load(base, light + 20));
      const uint32_t b = ToInteger(Load(base, light + 24));
      const uint32_t a = ToInteger(Load(base, light + 28));
      const uint32_t color = (((((a << 8) | r) << 8) | g) << 8) | b;

      // Corners: +right +up, -right +up, -right -up, +right -up, each
      // position read again as the original does.
      struct Corner {
        float sx, sy;  // right's and up's signs
        bool u, v;     // texture coordinates 1 (else 0)
      };
      const Corner corners[4] = {
          {1, 1, true, true}, {-1, 1, false, true}, {-1, -1, false, false}, {1, -1, true, false}};
      for (const Corner& c : corners) {
        const uint32_t at = guest::LoadU32(base, material + 36) + i * kLightBytes;
        const float px = Load(base, at + 0), py = Load(base, at + 4), pz = Load(base, at + 8);
        const float x = (c.sx > 0 ? px + rx : px - rx);
        const float y = (c.sx > 0 ? py + ry : py - ry);
        const float z = (c.sx > 0 ? pz + rz : pz - rz);
        StoreF(base, vertex + 0, (c.sy > 0 ? x + ux : x - ux) + fx);
        StoreF(base, vertex + 4, (c.sy > 0 ? y + uy : y - uy) + fy);
        StoreF(base, vertex + 8, (c.sy > 0 ? z + uz : z - uz) + fz);
        StoreF(base, vertex + 12, one);
        StoreU32(base, vertex + 16, color);
        StoreF(base, vertex + 20, c.u ? one : zero);
        StoreF(base, vertex + 24, c.v ? one : zero);
        vertex += kVertexBytes;
      }
    }
  }

  if (total != 0) {
    slot = slots(1, kDrawCall);
    StoreU32(base, slot + 4, 1);
    StoreU32(base, slot + 0, 19);
    StoreU32(base, slot + 8, 0);
    slot = slots(1, kDrawArgsCall);
    StoreU32(base, slot + 0, type);
    guest::StoreU16(base, slot + 4, uint16_t(total));
    guest::StoreU16(base, slot + 6, uint16_t(kVertexBytes));
    StoreU32(base, slot + 8, first_vertex);
    StoreU32(base, kRingCount, guest::LoadU32(base, kRingCount) + total);
  }
  return total;
}

}  // namespace sprites
