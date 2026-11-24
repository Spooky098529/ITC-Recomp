// TuningScreen::HandleInput (sub_821E6950, with the recompiler's piece
// sub_821E69B8 for state 0 merged; docs/functions/tuning-pages.md): see
// tuning_pages.h. The original keeps the screen in r31 (all 64 bits of r3 at
// entry) and r4 in r30, with its 112-byte frame at r1 (r30, r31 and lr saved
// below the frame, not written here; the frame itself not made since E5 step
// 6). The pages made native by their plain C++ (the sticker editor's states
// 38 to 46 and 49 too, never seen in play), the other pages never seen in
// play (state 7, the paint, the light kits, the interior's) through the guest
// with the original's return addresses; the empty sub_82106540 (states 35 to
// 37 and 50 to 52) left out, as other native callers of it do.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"
#include "schema/tuning_screen.h"

namespace tuning_pages {

namespace {

// The screen (tuning-screen.toml's TuningScreen).
schema::TuningScreen Screen(uint8_t* base, uint32_t s) { return schema::TuningScreen(base, s); }

// The pages never seen in play, called through the guest: the page's
// address and the original's return address (r3 the screen only).
struct Guest {
  uint32_t function;
  uint32_t lr;
};

Guest PageGuest(uint32_t state) {
  switch (state) {
    case 7: return {0x821D7C90, 0x821E6A0C};   // InputOptionRows
    case 18: return {0x821F5288, 0x821E6A48};  // InputPaint
    case 19: return {0x821EF320, 0x821E6A54};  // InputLightGroups
    case 20: return {0x821EF5E8, 0x821E6A60};  // InputLightKinds
    case 21: return {0x821EFA78, 0x821E6A6C};  // InputLightKind0
    case 22: return {0x821F0318, 0x821E6A78};  // InputLightKind1
    case 53: return {0x821F5B98, 0x821E6BBC};  // InputInteriorModes
    case 54: return {0x821F0998, 0x821E6BC8};  // InputInteriorList
    case 55: return {0x821F0D38, 0x821E6BD4};  // InputInteriorOptions
    case 56: return {0x821F1130, 0x821E6BE0};  // InputInteriorGrid
    case 57: return {0x821F1780, 0x821E6BEC};  // InputPickSlot
    case 58: return {0x821F1AB8, 0x821E6BF8};  // InputRemoveSlot
    default: return {0, 0};
  }
}

}  // namespace

uint64_t HandleInput(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint64_t r4, uint32_t sp, const PageFlags& flags) {
  ITC_NATIVE_RUNNING("sub_821E6950 TuningScreen::HandleInput");
  // No guest frame (E5 step 6): the original's 112 bytes held only the back
  // chain (its saves below it aren't written here); the pages and guest calls
  // are given `sp`.
  const uint32_t s = uint32_t(screen);

  // DecalImages::Update on the screen's +10048, always (its adapter runs its
  // plain C++ only with the native heap).
  const uint64_t images = screen + 10048;
  uint64_t r3 = flags.native_heap ? UpdateImages(base, call, images, sp)
                                  : call(0x821FE628, {images}, sp, 0x821E6974);
  if (Screen(base, s).pages_enabled() == 0) {
    return r3;
  }
  const uint32_t state = guest::LoadU32(base, Screen(base, s).queue().address);
  if (state > 62) {
    return r3;
  }
  const Guest g = PageGuest(state);
  if (g.function != 0) {
    return call(g.function, {screen}, sp, g.lr);
  }
  switch (state) {
    case 0:  // the piece sub_821E69B8: TUNING SHOP MAIN, category 0
      Screen(base, s).set_state(3);
      Screen(base, s).set_category(0);
      return r3;
    case 3: return InputCategories(base, call, s, sp, flags);
    case 4: return InputItems(base, call, screen, sp, flags);
    case 5: return InputLevels(base, call, s, sp, flags);
    case 6: return InputTypes(base, call, s, sp, flags);
    case 8: return InputOptions(base, call, s, sp, flags);
    case 15: return InputWheelParts(base, call, s, sp, flags);
    case 16: return InputWheels(base, call, screen, sp, flags);
    case 17: return InputInches(base, call, s, sp, flags);
    case 23: return InputVinylPages(base, call, screen, sp, flags);
    case 24: return InputVinylLayers(base, call, screen, sp, flags);
    case 25: return InputLayerActions(base, call, screen, sp, flags);
    case 26: return InputLayerEdit(base, call, screen, sp, flags);
    case 27: return InputLayerOrder(base, call, screen, sp, flags);
    case 28: return InputVinylImages(base, call, screen, sp, flags);
    case 29: return InputDecalPosition(base, call, screen, sp, flags);
    case 30: return InputDecalAngle(base, call, screen, sp, flags);
    case 31: return InputDecalScale(base, call, screen, sp, flags);
    case 34: return InputVinylView(base, call, screen, sp, flags.native_slots, flags.fit);
    case 38: return InputStickerAreas(base, call, screen, sp, flags);
    case 39: return InputStickerSlots(base, call, screen, sp, flags);
    case 40: return InputStickerOperate(base, call, screen, sp, flags);
    case 41: return InputStickerEdit(base, call, screen, sp, flags);
    case 42: return InputStickerOrder(base, call, screen, sp, flags);
    case 43: return InputStickerPicker(base, call, screen, sp, flags);
    case 44: return InputStickerMove(base, call, screen, sp, flags);
    case 45: return InputStickerRotate(base, call, screen, sp, flags);
    case 46: return InputStickerScale(base, call, screen, sp, flags);
    case 49: return InputStickerView(base, call, screen, sp, flags);
    case 35: case 36: case 37: case 50: case 51: case 52:
      return screen;  // the empty sub_82106540(screen), left out
    case 59: return Leave(base, screen, r4, sp);
    case 60: return EnterEditor(base, screen, sp);
    case 61: return LeaveEditor(base, call, screen, sp);
    case 62: return BackToItems(base, call, screen, sp, flags);
    default: return r3;  // 1, 2, 9 to 14, 32, 33, 47, 48: no page
  }
}

}  // namespace tuning_pages
