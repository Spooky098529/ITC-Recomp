#include "sprites/billboards.h"

#include "guest.h"
#include "schema/cameras.h"
#include "schema/sprites.h"

#include "base/float_mode.h"
#include <simde/x86/sse4.1.h>

#include <bit>
#include <climits>
#include <cmath>

namespace sprites {

using G = schema::SpritesGlobals;
using Material = schema::BillboardMaterial;
using Light = schema::BillboardLight;

void InitBillboardMaterial(uint8_t* base, uint32_t material) {
  const Material m(base, material);
  m.set_unknown_8(0);
  m.set_alpha_mode(1);
  m.set_blend_mode(1);
  m.set_texture(0);
  m.set_unknown_16(0);
  m.set_light_count(0);
  m.set_lights(0u);
}

void SetBillboardLights(uint8_t* base, uint32_t material, uint32_t count, uint32_t lights) {
  if (count == 0) {
    return;
  }
  const Material m(base, material);
  m.set_light_count(count);
  m.set_lights(lights);
}

namespace {

// The ring's halves hold 4,096 vertices each (SpritesGlobals' ring_*).
constexpr uint32_t kRingRoom = 4096;
constexpr uint32_t kVertexBytes = 28;
constexpr uint32_t kLightBytes = 48;  // a BillboardLight

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
  using Fpscr = ::base::FloatMode;
  const Material m(base, material);
  const int32_t alpha_mode = int32_t(m.alpha_mode());
  uint32_t total = m.light_count();
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
  const uint32_t handle = G::ring_declaration(base);
  const uint32_t ring_index = G::ring_half(base);
  StoreU32(base, slot + 0, handle);
  const uint32_t ring_count = G::ring_count(base);
  const uint32_t first_vertex =
      ((ring_index << 12) + ring_count) * kVertexBytes + G::ring_vertices_address();
  uint32_t vertex = first_vertex;

  if (m.light_count() != 0) {
    const float one = G::one(base);
    const float zero = G::zero(base);
    const float forward = G::forward_scale(base);
    const float size_scale = G::size_scale(base);
    const float offset_w = G::offset_w(base);
    const float farthest = G::farthest(base);
    const simde__m128 half = simde_mm_set1_ps(0.5f);
    const uint32_t csr = Fpscr::getcsr();
    const uint32_t flushing = csr | uint32_t(Fpscr::FlushMask);
    for (uint32_t i = 0; i < m.light_count(); i++) {
      const Light light(base, m.lights().address + i * kLightBytes);
      if (!(light.width() > zero) && !(light.height() > zero)) {
        total -= 4;
        continue;
      }
      const schema::CameraPass pass = schema::BillboardPass(base, set).camera_pass();
      const float dx = pass.eye_x() - light.x();
      const float dy = pass.eye_y() - light.y();
      const float dz = pass.eye_z() - light.z();
      float distance = pass.init_far();  // the draw distance
      if (!(distance < farthest)) {
        distance = farthest;
      }
      distance = distance * distance;
      const simde__m128 offset = Vector(dx, dy, dz, one);
      const simde__m128 to_light = Vector(-dx, zero, -dz, offset_w);
      const simde__m128 look = Vector(pass.target_x() - pass.eye_x(), zero,
                                      pass.target_z() - pass.eye_z(), one);
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
      // height and the forward scale (its columns' x, y and z are the
      // quad's axes).
      const uint32_t view = pass.view_address();
      const float width = light.width() * size_scale;
      const float height = light.height() * size_scale;
      const float rx = Load(base, view + 0) * width;
      const float ry = Load(base, view + 16) * width;
      const float rz = Load(base, view + 32) * width;
      const float ux = Load(base, view + 4) * height;
      const float uy = Load(base, view + 20) * height;
      const float uz = Load(base, view + 36) * height;
      const float fx = Load(base, view + 8) * forward;
      const float fy = Load(base, view + 24) * forward;
      const float fz = Load(base, view + 40) * forward;
      const uint32_t r = ToInteger(light.red());
      const uint32_t g = ToInteger(light.green());
      const uint32_t b = ToInteger(light.blue());
      const uint32_t a = ToInteger(light.alpha());
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
        const Light at(base, m.lights().address + i * kLightBytes);
        const float px = at.x(), py = at.y(), pz = at.z();
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
    G::set_ring_count(base, G::ring_count(base) + total);
  }
  return total;
}

}  // namespace sprites
