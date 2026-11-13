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

// The plain C++ for a slot's target, from the frame at `sp` (the callee's
// entry r1) returning to `lr`; nullopt where the target isn't one of ours
// (or needs an r4 the call leaves unset).
std::optional<uint64_t> SlotNative(uint8_t* base, uint32_t target, uint64_t r3,
                                   std::optional<uint64_t> r4, uint32_t sp, uint32_t lr,
                                   const Calls& calls) {
  // The list part's and its inner part's slots, and the inner's pages and
  // steps (list_part.cpp); r5, which only the guest fallbacks see, isn't
  // passed on (the plain C++ here doesn't read it).
  switch (target) {
    case 0x82252420:
      return ListPartBaseDestruct(base, r3);
    case 0x82252600:
      return ListPartInnerBaseDestruct(base, r3);
    case 0x82252430:
      return ListPartOpen(base, r3, sp, calls);
    case 0x82252530:
      return ListPartClose(base, r3, sp, calls);
    case 0x82252590:
      return ListPartIsIdle(base, r3, sp, lr, calls);
    case 0x822525B8:
      return ListPartCanLeave(base, r3, sp, lr, calls);
    case 0x822526F8:
      return ListPartInnerDestruct(base, r3, sp, calls);
    case 0x82252968:
      return ListPartDestruct(base, r3, sp, calls);
    case 0x82252748:
      return ListPartInnerIsIdle(base, r3, sp);
    case 0x82252740:
      return ListPartInnerClose(base, r3, sp, lr, calls);
    case 0x822528C8:
      return ListPartInnerCanLeave(base, r3);
    case 0x82252778:
      return ListPartInnerPage0(base, r3, sp, calls);
    case 0x82252878:
      return ListPartInnerPage1(base, r3);
    case 0x822528A8:
      return ListPartInnerPage2(base, r3);
    default:
      break;
  }
  if (r4) {
    switch (target) {
      case 0x82252610:
        return ListPartBaseDeletingDestruct(base, r3, *r4, sp, calls);
      case 0x82252658:
        return ListPartInnerBaseDeletingDestruct(base, r3, *r4, sp, calls);
      case 0x822529B0:
        return ListPartInnerDeletingDestruct(base, r3, *r4, sp, calls);
      case 0x82252A38:
        return ListPartDeletingDestruct(base, r3, *r4, sp, calls);
      case 0x82252490:
        return ListPartPick(base, r3, *r4, sp, calls);
      case 0x82252500:
        return ListPartInput(base, r3, *r4, sp, lr, calls);
      case 0x82252A88:
        return ListPartInnerOpen(base, r3, *r4, sp, calls);
      case 0x82252A00:
        // r5 as the inner's pages leave it unread.
        return ListPartInnerPick(base, r3, *r4, *r4, sp, lr, calls);
      case 0x82252DF0:
        return ListPartInnerInput(base, r3, *r4, sp, calls);
      case 0x82252BB8:
        return ListPartInnerStep0(base, r3, *r4, sp, calls);
      case 0x82252C98:
        return ListPartInnerStep1(base, r3, *r4, sp, calls);
      case 0x82252D38:
        return ListPartInnerStep2(base, r3, *r4, sp, calls);
      default:
        break;
    }
  }
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
    case 0x82230790:
      return PanelScreenIsLoaded(base, sp);
    case 0x82230758:
      return PanelScreenOpenMovies(base, sp, false, calls);
    case 0x822401D0:
      return PanelScreenOpenMovies(base, sp, true, calls);
    case 0x82240208:
      return PanelScreenDeleteMovies(base, sp, calls);
    case 0x82240860:
    case 0x82247058:
    case 0x822408B8:
      if (!r4) return std::nullopt;
      return PanelScreenPost(base, r3, *r4, target);
    case 0x822405F8:
      if (!r4) return std::nullopt;
      return Type7PanelScreenPickCar(base, r3, *r4, sp, calls);
    case 0x82247E88:
      if (!r4) return std::nullopt;
      return NodeTableDeletingDestruct(base, r3, *r4, sp, calls);
    case 0x82248650:
      return ColorNodeTableReset(base, r3, sp, calls);
    case 0x82240238:
      return Type7CarWord(base, 1972);
    case 0x82240248:
      return Type7CarWord(base, 1976);
    case 0x82240258:
      return Type7CarWord(base, 1980);
    case 0x82249C30:
      return GroupPairSelectA(base, r3);
    case 0x82249C40:
      return GroupPairReset(base, r3, sp, calls);
    case 0x82249BE0:
      return GroupPairRun(base, r3, sp, calls);
    case 0x82249BA8:
      return GroupPairWaitForFade(base, r3);
    case 0x82249C70:
      return GroupPairFinishAll(base, r3, sp, calls);
    default:
      return std::nullopt;
  }
}

// Whether SlotNative runs `target` (given whether the call sets r4).
bool SlotIsNative(uint32_t target, bool has_r4) {
  switch (target) {
    case 0x82252610:
    case 0x82252658:
    case 0x822529B0:
    case 0x82252A38:
    case 0x82252490:
    case 0x82252500:
    case 0x82252A88:
    case 0x82252A00:
    case 0x82252DF0:
    case 0x82252BB8:
    case 0x82252C98:
    case 0x82252D38:
    case 0x82240860:
    case 0x82247058:
    case 0x822408B8:
    case 0x822405F8:
    case 0x82247E88:
      return has_r4;
    case kEmpty:
    case 0x82242F70:
    case 0x82233320:
    case 0x82242F10:
    case 0x82233330:
    case 0x82242F20:
    case 0x82236FD8:
    case 0x82236EB0:
    case 0x82236EC0:
    case 0x82230790:
    case 0x82230758:
    case 0x822401D0:
    case 0x82240208:
    case 0x82240238:
    case 0x82240248:
    case 0x82240258:
    case 0x82249C30:
    case 0x82249C40:
    case 0x82249BE0:
    case 0x82249BA8:
    case 0x82249C70:
    case 0x82248650:
    case 0x82252420:
    case 0x82252600:
    case 0x82252430:
    case 0x82252530:
    case 0x82252590:
    case 0x822525B8:
    case 0x822526F8:
    case 0x82252968:
    case 0x82252748:
    case 0x82252740:
    case 0x822528C8:
    case 0x82252778:
    case 0x82252878:
    case 0x822528A8:
      return true;
    default:
      return false;
  }
}

uint64_t Slot(uint8_t* base, uint64_t object_r3, uint32_t slot, std::optional<uint64_t> r4,
              uint32_t frame, uint32_t lr, const Calls& calls) {
  const uint32_t target = LoadU32(base, LoadU32(base, uint32_t(object_r3)) + slot);
  if (SlotIsNative(target, r4.has_value())) {
    // The original sets r4 before the call; the callee may leave another.
    if (r4) {
      calls.set_r4(*r4);
    }
    return *SlotNative(base, target, object_r3, r4, frame, lr, calls);
  }
  if (r4) {
    return calls.call(target, frame, lr, object_r3, *r4);
  }
  const uint64_t args[6] = {object_r3, 0, 0, 0, 0, 0};
  return calls.call_set(target, args, 1, frame, lr);
}

}  // namespace type67_screens
