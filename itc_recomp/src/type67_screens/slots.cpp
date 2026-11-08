// The screens' vtable calls and the ring post (docs/functions/type67-screens.md):
// see type67_screens.h.

#include "type67_screens/type67_screens.h"

#include "guest.h"

namespace type67_screens {

using guest::LoadU32;
using guest::StoreU32;

void Post(uint8_t* base, uint32_t dispatcher, uint32_t message) {
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  StoreU32(base, slot + 0, message);
  StoreU32(base, slot + 4, 0);
  StoreU32(base, slot + 8, 0);
}

namespace {

// The plain C++ for a slot's target, from the frame at `sp` (the callee's
// entry r1) returning to `lr`; nullopt where the target isn't one of ours
// (or needs an r4 the call leaves unset).
std::optional<uint64_t> Native(uint8_t* base, uint32_t target, uint64_t r3,
                               std::optional<uint64_t> r4, uint32_t sp, uint32_t lr,
                               const Calls& calls) {
  (void)r4;
  switch (target) {
    case kEmpty:
      return r3;
    case 0x82242F70:
      return ListScreenIsLoaded(base, r3, sp);
    case 0x82233320:
      return ListScreenOpenAsset(base, r3, false, sp, lr, calls);
    case 0x82242F10:
      return ListScreenOpenAsset(base, r3, true, sp, lr, calls);
    case 0x82233330:
      return ListScreenRelease(base, r3, false, sp, calls);
    case 0x82242F20:
      return ListScreenRelease(base, r3, true, sp, calls);
    case 0x82236FD8:
      return NodeScreenIsLoaded(base, r3, sp);
    case 0x82236EB0:
      return NodeScreenOpenAsset(base, r3, sp, lr, calls);
    case 0x82236EC0:
      return NodeScreenRelease(base, r3, sp, calls);
    default:
      return std::nullopt;
  }
}

}  // namespace

uint64_t Slot(uint8_t* base, uint64_t object_r3, uint32_t slot, std::optional<uint64_t> r4,
              uint32_t frame, uint32_t lr, const Calls& calls) {
  const uint32_t target = LoadU32(base, LoadU32(base, uint32_t(object_r3)) + slot);
  if (const std::optional<uint64_t> r3 = Native(base, target, object_r3, r4, frame, lr, calls)) {
    return *r3;
  }
  if (r4) {
    return calls.call(target, frame, lr, object_r3, *r4);
  }
  const uint64_t args[6] = {object_r3, 0, 0, 0, 0, 0};
  return calls.call_set(target, args, 1, frame, lr);
}

}  // namespace type67_screens
