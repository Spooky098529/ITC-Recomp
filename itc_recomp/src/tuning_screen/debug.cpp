// The tuning screen's debug state handlers (docs/functions/tuning-screen.md):
// see tuning_screen.h. Each only prints a debug menu through the empty
// sub_82106540 (audio.md; the print compiled out), so its calls and the
// loads that feed them are left out (the native callers' precedent for the
// empty function); what's left is what the original leaves: its frame's
// back chain, the string tables it builds in the frame (copied as it leaves
// them), and r3, the last call's first argument (the empty function leaves
// every register).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "guest.h"

#include <initializer_list>

namespace tuning_screen {

namespace {

void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

// Words at frame + `offset` on, one after another.
void Words(uint8_t* base, uint32_t frame, uint32_t offset, std::initializer_list<uint32_t> words) {
  for (const uint32_t word : words) {
    Store(base, frame + offset, word);
    offset += 4;
  }
}

// The confirmation pages: four prints from a 96-byte frame.
uint32_t DebugConfirm(uint8_t* base, uint32_t sp) {
  Frame(base, sp, 96);
  return 520;
}

}  // namespace

uint32_t DebugMainMenu(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CBE08 TuningScreen::DebugMainMenu");
  const uint32_t frame = Frame(base, sp, 144);
  Words(base, frame, 80,
        {0x8200AA5C, 0x8200AA50, 0x8200AA48, 0x8200AA40, 0x8200AA38, 0x8200AA70, 0x8200AA64});
  return 520;
}

uint32_t DebugLevelSelect(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CBEE8 TuningScreen::DebugLevelSelect");
  Frame(base, sp, 128);
  return 520;
}

uint32_t DebugTypeSelect(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CBFC0 TuningScreen::DebugTypeSelect");
  Frame(base, sp, 128);
  return 520;
}

uint32_t DebugOptionSelect(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CC248 TuningScreen::DebugOptionSelect");
  Frame(base, sp, 96);
  // The lines after the cursor's: the aero item 4's, the tires', the
  // exterior item 1's; none for the others (the cursor's line, 500, last).
  const int32_t m = int32_t(guest::LoadU32(base, screen + kCategory));
  if (m == 4 && guest::LoadU8(base, screen + kItem) == 4) {
    return 520;
  }
  if (m == 3) {
    return 520;
  }
  if (m == 5 && guest::LoadU8(base, screen + kItem) == 1) {
    return 520;
  }
  return 500;
}

uint32_t DebugConfirmBuy(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CC3A8 TuningScreen::DebugConfirmBuy");
  return DebugConfirm(base, sp);
}

uint32_t DebugConfirmEquip(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CC488 TuningScreen::DebugConfirmEquip");
  return DebugConfirm(base, sp);
}

uint32_t DebugCategoryMenu(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CFCE8 TuningScreen::DebugCategoryMenu");
  const uint32_t frame = Frame(base, sp, 464);
  // Eight item names, then the seven categories' string tables and the
  // table of the seven, as the original builds them.
  Words(base, frame, 80,
        {0x8200ABE0, 0x8200AC80, 0x8200AD0C, 0x8200AC54, 0x8200ACB4, 0x8200AC14, 0x8200ACE0,
         0x8200AD38, 0x8200AC74, 0x8200AC74, 0x8200AC74});
  Words(base, frame, 128, {0x8200AD14, 0x8200ACEC, 0x8200ACC0});
  Words(base, frame, 144,
        {0x8200AD30, 0x8200AD20, 0x8200AD04, 0x8200ACF4, 0x8200AC14, 0x8200ABE0, 0x8200ABD0,
         0x8200ABBC, 0x8200ACD0, 0x8200ACC8, 0x8200ACA8, 0x8200ACA0, 0x8200AC7C});
  Words(base, frame, 208,
        {frame + 144, frame + 176, frame + 112, frame + 128, frame + 272, frame + 240,
         frame + 160});
  Words(base, frame, 240,
        {0x8200ABF4, 0x8200ABEC, 0x8200AD38, 0x8200AD0C, 0x8200ACE0, 0x8200ACB4, 0x8200AC80,
         0x8200AC54, 0x8200AC90, 0x8200AC68, 0x8200AC5C, 0x8200AC44, 0x8200AC3C, 0x8200AC34,
         0x8200AC28, 0x8200AC1C, 0x8200AC08, 0x8200ABFC});
  // The loop over the category's items asks CategoryItemCount once more
  // than it prints: its r3 is the last.
  uint32_t count = CategoryItemCount(base, screen);
  if (int32_t(count) > 0) {
    int32_t i = 0;
    do {
      i++;
      count = CategoryItemCount(base, screen);
    } while (i < int32_t(count));
  }
  return count;
}

uint32_t DebugWheelPartSelect(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8068 TuningScreen::DebugWheelPartSelect");
  const uint32_t frame = Frame(base, sp, 144);
  Words(base, frame, 80, {0x8200B35C, 0x8200ACEC, 0x8200ACC0});
  return 520;
}

uint32_t DebugWheelKindSelect(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8128 TuningScreen::DebugWheelKindSelect");
  const uint32_t frame = Frame(base, sp, 256);
  Words(base, frame, 80,
        {0x8200AAA0, 0x8200B3F4, 0x8200B3D4, 0x8200B3C4, 0x8200B3B4, 0x8200B42C, 0x8200B410,
         0x8200B408, 0x8200B400, 0x8200B3DC, 0x8200B3CC, 0x8200B3C0, 0x8200B3AC, 0x8200B398,
         0x8200B390, 0x8200B388, 0x8200B380});
  return 520;
}

uint32_t DebugWheelInchSelect(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8298 TuningScreen::DebugWheelInchSelect");
  const uint32_t frame = Frame(base, sp, 144);
  Words(base, frame, 80, {0x8200AAA0, 0x8200B454, 0x8200B44C, 0x8200B444, 0x8200B43C});
  return 520;
}

namespace {
// Category 5's item 6 (the stickers, probably): the debug pages' shorter
// lists.
bool Stickers(uint8_t* base, uint32_t screen) {
  return int32_t(guest::LoadU32(base, screen + kCategory)) == 5 &&
         guest::LoadU8(base, screen + kItem) == 6;
}
}  // namespace

uint32_t DebugDecalTopMenu(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FD710 TuningScreen::DebugDecalTopMenu");
  const uint32_t frame = Frame(base, sp, 160);
  if (Stickers(base, screen)) {
    Words(base, frame, 80, {0x8200AC3C, 0x8200B480});
  } else {
    Words(base, frame, 96,
          {0x8200AC3C, 0x8200C904, 0x8200C8F8, 0x8200C8E8, 0x8200C8DC, 0x8200C940, 0x8200C91C,
           0x8200C910});
  }
  return 520;
}

uint32_t DebugLayerSelect(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FD880 TuningScreen::DebugLayerSelect");
  const uint32_t frame = Frame(base, sp, 176);
  if (Stickers(base, screen)) {
    Words(base, frame, 80, {0x8200C9A0, 0x8200C9A8, 0x8200C998, 0x8200C994, 0x8200C98C});
  } else {
    Words(base, frame, 80,
          {0x8200C9A0, 0x8200C9A8, 0x8200C998, 0x8200C96C, 0x8200C964, 0x8200C994, 0x8200C98C});
  }
  return 520;
}

uint32_t DebugLayerEdit(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FDCB8 TuningScreen::DebugLayerEdit");
  const uint32_t frame = Frame(base, sp, 160);
  // Both pages' tables end overwritten by the edit list's.
  Words(base, frame, 80,
        {0x8200CA34, 0x8200CA2C, 0x8200CA24, 0x8200CA1C, 0x8200CA14, 0x8200C994, 0x8200C98C});
  return 720;
}

uint32_t DebugLayerSet(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FE080 TuningScreen::DebugLayerSet");
  Frame(base, sp, 112);
  return 700;
}

uint32_t DebugEditPosition() {
  ITC_NATIVE_RUNNING("sub_821FE100 TuningScreen::DebugEditPosition");
  return 500;
}

uint32_t DebugEditView() {
  ITC_NATIVE_RUNNING("sub_821FE178 TuningScreen::DebugEditView");
  return 500;
}

uint32_t DebugPaintConfirm(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FE190 TuningScreen::DebugPaintConfirm");
  const uint32_t frame = Frame(base, sp, 128);
  Words(base, frame, 80, {0x8200AB74, 0x8200AB70});
  return 520;
}

// The handlers never seen in play (ported 2026-11-11, as the ones above).

uint32_t DebugColorSelect(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CC078 TuningScreen::DebugColorSelect");
  Frame(base, sp, 96);
  // The colors' lines for the aero item 4, the power train's item 3 and the
  // interior's item 0; the cursor's line (500) alone for the others.
  const int32_t m = int32_t(guest::LoadU32(base, screen + kCategory));
  const uint8_t p = guest::LoadU8(base, screen + kItem);
  if ((m == 4 && p == 4) || (m == 1 && p == 3) || (m == 6 && p == 0)) {
    return 520;
  }
  return 500;
}

uint32_t DebugWarnMoney(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CC430 TuningScreen::DebugWarnMoney");
  Frame(base, sp, 96);
  return 500;
}

uint32_t DebugReflectSet(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CC510 TuningScreen::DebugReflectSet");
  return DebugConfirm(base, sp);
}

uint32_t DebugAnnulSet(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CC598 TuningScreen::DebugAnnulSet");
  return DebugConfirm(base, sp);
}

namespace {
// The yes/no pages: a 128-byte frame with the two answers' strings.
uint32_t DebugYesNo(uint8_t* base, uint32_t sp) {
  const uint32_t frame = Frame(base, sp, 128);
  Words(base, frame, 80, {0x8200AB74, 0x8200AB70});
  return 520;
}
}  // namespace

uint32_t DebugRemoveConfirm(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821CC620 TuningScreen::DebugRemoveConfirm");
  return DebugYesNo(base, sp);
}

uint32_t DebugBodyColor(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8358 TuningScreen::DebugBodyColor");
  Frame(base, sp, 112);
  return 520;
}

uint32_t DebugNeonPart(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8440 TuningScreen::DebugNeonPart");
  const uint32_t frame = Frame(base, sp, 128);
  Words(base, frame, 80, {0x8200ACEC, 0x8200B480, 0x8200ACC0, 0x8200B47C});
  return 520;
}

uint32_t DebugNeonKind(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E84F8 TuningScreen::DebugNeonKind");
  const uint32_t frame = Frame(base, sp, 128);
  Words(base, frame, 80, {0x8200B4A4, 0x8200B4AC, 0x8200B49C});
  return 520;
}

uint32_t DebugNeonEdit(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E85A0 TuningScreen::DebugNeonEdit");
  Frame(base, sp, 112);
  return 520;
}

uint32_t DebugLedEdit(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8688 TuningScreen::DebugLedEdit");
  Frame(base, sp, 112);
  return 520;
}

uint32_t DebugCockpitTop(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8758 TuningScreen::DebugCockpitTop");
  const uint32_t frame = Frame(base, sp, 128);
  Words(base, frame, 80, {0x8200B4DC, 0x8200B4E8});
  return 520;
}

uint32_t DebugCockpitType(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8800 TuningScreen::DebugCockpitType");
  const uint32_t frame = Frame(base, sp, 128);
  Words(base, frame, 80, {0x8200B50C, 0x8200B510, 0x8200B504, 0x8200B4FC});
  return 520;
}

uint32_t DebugCockpitColor(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E88C0 TuningScreen::DebugCockpitColor");
  const uint32_t frame = Frame(base, sp, 144);
  Words(base, frame, 80,
        {0x8200AAE4, 0x8200AADC, 0x8200AACC, 0x8200B520, 0x8200B518, 0x8200B528});
  return 520;
}

uint32_t DebugMeterKind(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8998 TuningScreen::DebugMeterKind");
  const uint32_t frame = Frame(base, sp, 144);
  Words(base, frame, 80, {0x8200B544, 0x8200B550, 0x8200B538, 0x8200B530, 0x8200B49C});
  return 520;
}

uint32_t DebugMeterPoint(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8A88 TuningScreen::DebugMeterPoint");
  const uint32_t frame = Frame(base, sp, 144);
  Words(base, frame, 80, {0x8200B5A4, 0x8200B5B0, 0x8200B598, 0x8200B58C, 0x8200B580});
  return 520;
}

uint32_t DebugLayerOperate(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FDA80 TuningScreen::DebugLayerOperate");
  const uint32_t frame = Frame(base, sp, 160);
  // The layer list's table (three names for the stickers, five else), then
  // the operations' four over its first.
  if (Stickers(base, screen)) {
    Words(base, frame, 80, {0x8200C9A0, 0x8200C9A8, 0x8200C998});
  } else {
    Words(base, frame, 80, {0x8200C9A0, 0x8200C9A8, 0x8200C998, 0x8200C96C, 0x8200C964});
  }
  Words(base, frame, 80, {0x8200C9E4, 0x8200C9F0, 0x8200C9D4, 0x8200C9C4});
  return 720;
}

uint32_t DebugLayerOrder(uint8_t* base, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FDF10 TuningScreen::DebugLayerOrder");
  const uint32_t frame = Frame(base, sp, 144);
  if (Stickers(base, screen)) {
    Words(base, frame, 80, {0x8200C9A0, 0x8200C9A8, 0x8200C998});
  } else {
    Words(base, frame, 80, {0x8200C9A0, 0x8200C9A8, 0x8200C998, 0x8200C96C, 0x8200C964});
  }
  return 520;
}

uint32_t DebugEditRotate() {
  ITC_NATIVE_RUNNING("sub_821FE118 TuningScreen::DebugEditRotate");
  return 500;
}

uint32_t DebugEditScale() {
  ITC_NATIVE_RUNNING("sub_821FE130 TuningScreen::DebugEditScale");
  return 500;
}

uint32_t DebugEditColor() {
  ITC_NATIVE_RUNNING("sub_821FE148 TuningScreen::DebugEditColor");
  return 500;
}

uint32_t DebugEditFlip() {
  ITC_NATIVE_RUNNING("sub_821FE160 TuningScreen::DebugEditFlip");
  return 500;
}

uint32_t DebugPaintCancelConfirm(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FE230 TuningScreen::DebugPaintCancelConfirm");
  return DebugYesNo(base, sp);
}

uint32_t DebugPaintRemoveConfirm(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FE2D0 TuningScreen::DebugPaintRemoveConfirm");
  return DebugYesNo(base, sp);
}

}  // namespace tuning_screen
