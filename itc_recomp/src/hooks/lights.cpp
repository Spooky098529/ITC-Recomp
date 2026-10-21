// Lights::FindNearest (sub_820F4128) done natively: the lights nearest a
// point, nearest first. See docs/functions/functions.md for what it does, and "The main
// thread" in docs/native-renderer.md for why (13% of the main thread).
//
// The translated original spends most of its time per light tested on
// switching the flush-to-zero mode around each vector instruction (an MXCSR
// write each time) and on reading back through its guest stack frame what it
// just stored there. Here the distance and cone tests run on four lights at
// once, with the original's operations in its order (`vmsum3fp128` is `dpps`
// with mask 0xEF, which adds z² to y² + x²; the reciprocal square root is
// 1/sqrt and one Newton step), so the same lights pass. The lights that do are
// then merged and sorted one by one as the original does.

#include "guest.h"

#include <rex/cvar.h>

#include <smmintrin.h>

#include <cstring>
#include <iterator>
#include <utility>

REXCVAR_DEFINE_BOOL(native_nearest_lights, true, "ITC",
                    "Find the lights nearest a point (sub_820F4128) natively, four lights "
                    "tested at once, instead of through the translated code (same results)");

namespace {

constexpr uint32_t kSetListOffset = 216;  // the owner's list of light sets
constexpr uint32_t kSetEntryStride = 272;
constexpr uint32_t kGroupCountOffset = 60;
constexpr uint32_t kGroupLightsOffset = 76;
constexpr uint32_t kLightStride = 48;
constexpr uint32_t kOutputStride = 32;
constexpr uint32_t kOutputW = 0x82000A30;  // the float written as each position's w
constexpr int32_t kMostLights = 32;

// A light kept: its first 32 bytes as in guest memory (copied out), its
// position and its squared distance.
struct Kept {
  uint8_t bytes[32];
  float x, y, z;
  float distance;
};

struct Query {
  float px, py, pz;
  __m128 dx, dy, dz;    // the direction, each component in every lane
  float merge_squared;  // f1 * f1: a light this near one kept is merged with it
  __m128 farthest;      // f2, squared
  __m128 least_dot;     // f3
  int32_t most;         // r6
};

struct Search {
  Kept kept[kMostLights + 1];  // the last one is where a light goes when it's full
  int32_t count = 0;
};

// The lights of a group that passed both tests, in order: a group scanned
// again passes the same ones (the tests don't depend on what's kept), so
// only these are taken again.
struct Passed {
  struct Light {
    uint32_t address;
    float x, y, z, distance;
  };
  Light lights[64];
  uint32_t count = 0;
  bool overflowed = false;
};

uint32_t LoadLittleEndianU32(uint8_t* base, uint32_t address) {
  return __builtin_bswap32(guest::LoadU32(base, address));
}

__m128 LoadSwapped(uint8_t* base, uint32_t address) {
  return _mm_castsi128_ps(
      guest::SwapVector(_mm_loadu_si128(reinterpret_cast<const __m128i*>(REX_RAW_ADDR(address)))));
}

// One light that passed both tests, as the original's loop at 0x820F42A8
// takes it: dropped or merged into a kept light within the merge distance
// (the nearest such, searching from the last kept), otherwise sorted in by
// distance; once full, the farthest drops off the end.
void Keep(uint8_t* base, const Query& query, Search& search, uint32_t light, float x, float y,
          float z, float distance) {
  Kept& added = search.kept[search.count];
  std::memcpy(added.bytes, REX_RAW_ADDR(light), sizeof(added.bytes));
  added.x = x;
  added.y = y;
  added.z = z;
  added.distance = distance;
  for (int32_t i = search.count - 1; i >= 0; i--) {
    Kept& other = search.kept[i];
    float ex = x - other.x, ey = y - other.y, ez = z - other.z;
    float apart = ez * ez + (ey * ey + ex * ex);
    if (!(apart > query.merge_squared)) {
      if (distance < other.distance) {
        other = added;
      }
      return;
    }
  }
  for (int32_t i = search.count; i > 0; i--) {
    if (!(search.kept[i - 1].distance > search.kept[i].distance)) {
      break;
    }
    std::swap(search.kept[i - 1], search.kept[i]);
  }
  if (search.count < query.most) {
    search.count++;
  }
}

// Tests count lights from lights, four at a time.
void ScanGroup(uint8_t* base, const Query& query, Search& search, uint32_t lights,
               uint32_t count, Passed* passed_lights) {
  const __m128 px = _mm_set1_ps(query.px), py = _mm_set1_ps(query.py),
               pz = _mm_set1_ps(query.pz);
  const __m128 zero = _mm_setzero_ps(), one = _mm_set1_ps(1.0f), half = _mm_set1_ps(0.5f);
  const __m128 sign = _mm_castsi128_ps(_mm_set1_epi32(int(0x80000000)));
  for (uint32_t first = 0; first < count; first += 4) {
    uint32_t in_group = count - first < 4 ? count - first : 4;
    // Lanes past the group's end repeat its last light, and are ignored.
    __m128 l0 = LoadSwapped(base, lights + kLightStride * first);
    __m128 l1 = LoadSwapped(base, lights + kLightStride * (first + (in_group > 1 ? 1 : 0)));
    __m128 l2 = LoadSwapped(base, lights + kLightStride * (first + (in_group > 2 ? 2 : 0)));
    __m128 l3 = LoadSwapped(base, lights + kLightStride * (first + (in_group > 3 ? 3 : 0)));
    _MM_TRANSPOSE4_PS(l0, l1, l2, l3);  // now x, y, z, w of each light
    __m128 dx = _mm_sub_ps(l0, px), dy = _mm_sub_ps(l1, py), dz = _mm_sub_ps(l2, pz);
    __m128 distance =
        _mm_add_ps(_mm_mul_ps(dz, dz), _mm_add_ps(_mm_mul_ps(dy, dy), _mm_mul_ps(dx, dx)));
    int valid = (1 << in_group) - 1;
    int near = _mm_movemask_ps(_mm_cmpngt_ps(distance, query.farthest)) & valid;
    if (near == 0) {
      continue;
    }
    // The direction to each light (0 for one at the point), as the original:
    // 1/sqrt, then r + r * -((d / 2) * r² - 1/2).
    __m128 r = _mm_div_ps(one, _mm_sqrt_ps(distance));
    __m128 step = _mm_xor_ps(
        _mm_sub_ps(_mm_mul_ps(_mm_mul_ps(distance, half), _mm_mul_ps(r, r)), half), sign);
    r = _mm_add_ps(_mm_mul_ps(r, step), r);
    __m128 at_point = _mm_cmpeq_ps(distance, zero);
    __m128 nx = _mm_blendv_ps(_mm_mul_ps(dx, r), distance, at_point);
    __m128 ny = _mm_blendv_ps(_mm_mul_ps(dy, r), distance, at_point);
    __m128 nz = _mm_blendv_ps(_mm_mul_ps(dz, r), distance, at_point);
    __m128 dot = _mm_add_ps(_mm_add_ps(zero, _mm_mul_ps(nz, query.dz)),
                            _mm_add_ps(_mm_mul_ps(ny, query.dy), _mm_mul_ps(nx, query.dx)));
    int passed = near & _mm_movemask_ps(_mm_cmpnlt_ps(dot, query.least_dot));
    if (passed == 0) {
      continue;
    }
    alignas(16) float xs[4], ys[4], zs[4], distances[4];
    _mm_store_ps(xs, l0);
    _mm_store_ps(ys, l1);
    _mm_store_ps(zs, l2);
    _mm_store_ps(distances, distance);
    for (; passed; passed &= passed - 1) {
      int lane = __builtin_ctz(passed);
      uint32_t light = lights + kLightStride * (first + lane);
      Keep(base, query, search, light, xs[lane], ys[lane], zs[lane], distances[lane]);
      if (passed_lights) {
        if (passed_lights->count < std::size(passed_lights->lights)) {
          passed_lights->lights[passed_lights->count++] = {light, xs[lane], ys[lane], zs[lane],
                                                           distances[lane]};
        } else {
          passed_lights->overflowed = true;
        }
      }
    }
  }
}

// The search as the original's loops at 0x820F41A8 walk it: each light set
// in the owner's list, each entry of a set (through sub_82294730's offset
// table), and the entry's group of lights as many times as its count says
// (the scans after the first take the lights the first passed, unless there
// were too many to keep).
void FindNearest(uint8_t* base, uint32_t owner, const Query& query, Search& search) {
  uint32_t list = owner + kSetListOffset;
  for (uint32_t i = 0; i < guest::LoadU32(base, list); i++) {
    uint32_t entry = guest::LoadU32(base, list + 8) + kSetEntryStride * i;
    uint32_t set = guest::LoadU32(base, guest::LoadU32(base, entry + 12) + 80);
    if (set == 0) {
      continue;
    }
    for (uint32_t j = 0; j < LoadLittleEndianU32(base, set); j++) {
      uint32_t block = set + LoadLittleEndianU32(base, set + 4 + 4 * j);
      uint32_t scans = guest::LoadU32(base, block);
      uint32_t lights = guest::LoadU32(base, block + kGroupCountOffset);
      if (lights == 0) {
        continue;
      }
      if (scans == 1) {
        ScanGroup(base, query, search, block + kGroupLightsOffset, lights, nullptr);
        continue;
      }
      Passed passed;
      for (uint32_t scan = 0; scan < scans; scan++) {
        if (scan == 0 || passed.overflowed) {
          ScanGroup(base, query, search, block + kGroupLightsOffset, lights,
                    scan == 0 ? &passed : nullptr);
          continue;
        }
        for (uint32_t k = 0; k < passed.count; k++) {
          const Passed::Light& light = passed.lights[k];
          Keep(base, query, search, light.address, light.x, light.y, light.z, light.distance);
        }
      }
    }
  }
}

// Writes the kept lights as the original does: position, the w at
// kOutputW, then the light's bytes 16-31.
void Write(uint8_t* base, const Search& search, uint32_t output) {
  uint8_t w[4];
  std::memcpy(w, REX_RAW_ADDR(kOutputW), 4);
  for (int32_t i = 0; i < search.count; i++) {
    uint8_t* out = REX_RAW_ADDR(output + kOutputStride * i);
    std::memcpy(out, search.kept[i].bytes, 12);
    std::memcpy(out + 12, w, 4);
    std::memcpy(out + 16, search.kept[i].bytes + 16, 16);
  }
}

bool IsFloat(double value) {
  return double(float(value)) == value;
}

}  // namespace

// Lights_FindNearest: r3 = the light sets' owner, r4 = output (32 bytes a
// light), r5 = the point, r6 = most lights (at most 32), r9 = the direction,
// f1 = merge distance, f2 = farthest squared distance, f3 = least dot product
// with the direction; returns r3 = lights written. The caller reads only r3
// after the call (checked at 0x820C92F8), so no other register is set.
extern "C" REX_FUNC(sub_820F4128) {
  REX_FUNC_PROLOGUE();
  ctx.fpscr.disableFlushMode();
  // Comparisons the original makes in double are made here in float: the
  // caller passes floats (lfs, frsp), but anything else goes to the original.
  if (!REXCVAR_GET(native_nearest_lights) || !IsFloat(ctx.f2.f64) || !IsFloat(ctx.f3.f64))
      [[unlikely]] {
    __imp__sub_820F4128(ctx, base);
    return;
  }
  uint32_t owner = ctx.r3.u32;
  int32_t most = ctx.r6.s32;
  if (guest::LoadU32(base, owner + kSetListOffset) == 0 || most > kMostLights) {
    ctx.r3.s64 = 0;
    return;
  }
  Query query;
  query.px = guest::LoadFloat(base, ctx.r5.u32);
  query.py = guest::LoadFloat(base, ctx.r5.u32 + 4);
  query.pz = guest::LoadFloat(base, ctx.r5.u32 + 8);
  uint32_t direction = ctx.r9.u32 & ~0xFu;
  query.dx = _mm_set1_ps(guest::LoadFloat(base, direction));
  query.dy = _mm_set1_ps(guest::LoadFloat(base, direction + 4));
  query.dz = _mm_set1_ps(guest::LoadFloat(base, direction + 8));
  query.merge_squared = float(ctx.f1.f64 * ctx.f1.f64);
  query.farthest = _mm_set1_ps(float(ctx.f2.f64));
  query.least_dot = _mm_set1_ps(float(ctx.f3.f64));
  query.most = most;
  Search search;
  FindNearest(base, owner, query, search);
  Write(base, search, ctx.r4.u32);
  ctx.r3.s64 = search.count;
}
