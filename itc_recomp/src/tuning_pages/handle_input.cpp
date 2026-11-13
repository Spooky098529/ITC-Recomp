// TuningScreen::HandleInput (sub_821E6950, with the recompiler's piece
// sub_821E69B8 for state 0 merged; docs/functions/tuning-pages.md): see
// tuning_pages.h. The original keeps the screen in r31 (all 64 bits of r3 at
// entry) and r4 in r30, with its 112-byte frame at r1 (r30, r31 and lr saved
// below the frame, not written here). The pages made native by their plain
// C++ (as their adapters run them), the pages never seen in play (state 7,
// the paint, the light kits, the sticker editor's, the interior's) through
// the guest with the original's return addresses; the empty sub_82106540
// (states 35 to 37 and 50 to 52) left out, as other native callers of it do.

#include "tuning_pages/tuning_pages.h"

#include "base/log.h"
#include "guest.h"

namespace tuning_pages {

namespace {

constexpr uint32_t kFrameBytes = 112;

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
    case 38: return {0x821FBD40, 0x821E6B20};  // the sticker editor's pages
    case 39: return {0x821F9050, 0x821E6B2C};
    case 40: return {0x821F9328, 0x821E6B38};
    case 41: return {0x821F9608, 0x821E6B44};
    case 42: return {0x821FBF18, 0x821E6B50};
    case 43: return {0x821FC270, 0x821E6B5C};
    case 44: return {0x821F99A8, 0x821E6B68};
    case 45: return {0x821F9D30, 0x821E6B74};
    case 46: return {0x821F9FB8, 0x821E6B80};
    case 49: return {0x821F7000, 0x821E6B8C};
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
  const uint32_t frame = sp - kFrameBytes;
  guest::StoreU32(base, frame, sp);  // the back chain, as stwu
  const uint32_t s = uint32_t(screen);

  // DecalImages::Update on the screen's +10048, always (its adapter runs its
  // plain C++ only with the native heap).
  const uint64_t images = screen + 10048;
  uint64_t r3 = flags.native_heap ? UpdateImages(base, call, images, frame)
                                  : call(0x821FE628, {images}, frame, 0x821E6974);
  if (guest::LoadU32(base, s + 9728) == 0) {
    return r3;
  }
  const uint32_t state = guest::LoadU32(base, guest::LoadU32(base, s + 10036));
  if (state > 62) {
    return r3;
  }
  const Guest g = PageGuest(state);
  if (g.function != 0) {
    return call(g.function, {screen}, frame, g.lr);
  }
  switch (state) {
    case 0:  // the piece sub_821E69B8: TUNING SHOP MAIN, category 0
      guest::StoreU32(base, s + 9652, 3);
      guest::StoreU32(base, s + 9656, 0);
      return r3;
    case 3: return InputCategories(base, call, s, frame, flags);
    case 4: return InputItems(base, call, screen, frame, flags);
    case 5: return InputLevels(base, call, s, frame, flags);
    case 6: return InputTypes(base, call, s, frame, flags);
    case 8: return InputOptions(base, call, s, frame, flags);
    case 15: return InputWheelParts(base, call, s, frame, flags);
    case 16: return InputWheels(base, call, screen, frame, flags);
    case 17: return InputInches(base, call, s, frame, flags);
    case 23: return InputVinylPages(base, call, screen, frame, flags);
    case 24: return InputVinylLayers(base, call, screen, frame, flags);
    case 25: return InputLayerActions(base, call, screen, frame, flags);
    case 26: return InputLayerEdit(base, call, screen, frame, flags);
    case 27: return InputLayerOrder(base, call, screen, frame, flags);
    case 28: return InputVinylImages(base, call, screen, frame, flags);
    case 29: return InputDecalPosition(base, call, screen, frame, flags);
    case 30: return InputDecalAngle(base, call, screen, frame, flags);
    case 31: return InputDecalScale(base, call, screen, frame, flags);
    case 34: return InputVinylView(base, call, screen, frame, flags.native_slots);
    case 35: case 36: case 37: case 50: case 51: case 52:
      return screen;  // the empty sub_82106540(screen), left out
    case 59: return Leave(base, screen, r4, frame);
    case 60: return EnterEditor(base, screen, frame);
    case 61: return LeaveEditor(base, call, screen, frame);
    case 62: return BackToItems(base, call, screen, frame, flags);
    default: return r3;  // 1, 2, 9 to 14, 32, 33, 47, 48: no page
  }
}

}  // namespace tuning_pages
