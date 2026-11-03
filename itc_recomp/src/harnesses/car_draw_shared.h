// What the car draw's harness files share (src/harnesses/car_draw.cpp, which
// defines these, car_draw_parts.cpp, car_draw_lamps.cpp,
// car_draw_shadows.cpp and car_draw_top.cpp): one case through both versions and the model draws'
// world, all under the harness name car_draw.

#pragma once

#include "harness.h"

#include "car_draw/car_draw.h"
#include "hooks/car_draw.h"

#include <cstdint>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace harness::car_draw_shared {

using FloatRanges = std::vector<std::pair<uint32_t, uint32_t>>;

// The block's size (0x10000 bytes, filled at random by each case).
constexpr uint32_t kBlockBytes = 0x10000;

// One case through both versions (car_draw.cpp's Case): the block filled at
// random, `setup(t)` writing the inputs and registers; the block compared
// (`floats`, offsets in it, as floats), the globals exactly, then `extra`'s
// outputs and r3. `before(v)` runs before each version (v 0 the recompiled,
// run with --native_car_draw off when `recompiled_flag_off`).
void Case(Context& h, Results& results, uint32_t block, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const FloatRanges& floats, const std::string& what,
          const std::function<void(const PPCContext&, Outputs&)>& extra = {},
          const std::function<void(int)>& before = {}, bool recompiled_flag_off = false);
// Whether the next case starts with flush-to-zero on.
void SetFlushOnEntry(bool on);
// A model draw's world (the model at +0x100, the shape, its materials, the
// list at +0x3000, the render queue, the identity view-projection at +0x400
// and the world at +0x500).
bool ModelWorld(Context& h, Random& r, uint32_t block, uint32_t shape);
// The materials' alphas (+12) as floats.
FloatRanges AlphaFloats();
uint32_t RandomFloat(Random& r);
void RandomFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r);
void FiniteFloats(uint8_t* base, uint32_t address, uint32_t count, Random& r);
void StoreFloats(uint8_t* base, uint32_t address, std::initializer_list<float> values);
float Spread(Random& r, float range);

// CarLighting::Gather's light sets: their owner at `owner` with one set of
// one group of `count` lights around (px, py, pz), mostly above it.
void LightSets(uint8_t* base, uint32_t owner, uint32_t count, float px, float py, float pz,
               Random& r);

// A part draw's register steps for the cases whose adapter is gone (its
// callers are plain C++): `draw` runs the plain C++ with the calls, r3 as
// it leaves it.
template <typename Draw>
void NativeStep(PPCContext& ctx, uint8_t* base, Draw draw) {
  ctx.fpscr.disableFlushMode();
  const uint64_t lr = ctx.lr;
  hooks::SceneryDrawGuest guest{ctx, base};
  const ::car_draw::Calls calls = hooks::CarDrawCalls(&guest);
  const ::car_draw::Left left = draw(calls);
  ctx.lr = lr;
  if (!left.in_ctx) {
    ctx.r3.u64 = left.r3;
  }
  ctx.fpscr.disableFlushMode();
}

// The parts' small draws' cases (car_draw_parts.cpp).
void PartsDraws(Context& h, Random& r, uint32_t block, uint32_t race);
// The head and tail lights' cases (car_draw_lamps.cpp).
void LampsDraws(Context& h, Random& r, uint32_t block, uint32_t race);
// The shadows' cases (car_draw_shadows.cpp).
void ShadowsDraws(Context& h, Random& r, uint32_t block, uint32_t race);
// The parts' top's cases (car_draw_top.cpp).
void TopDraws(Context& h, Random& r, uint32_t block, uint32_t race);

}  // namespace harness::car_draw_shared
