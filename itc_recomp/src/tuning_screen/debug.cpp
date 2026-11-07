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

}  // namespace tuning_screen
