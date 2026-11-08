// The car list the type 6 and 7 panel screens show natively (B7 in
// docs/rexglue-removal.md; the functions are in docs/functions/car-list.md,
// the fields in docs/known-data.md, "car list"): PanelSet's panels (the
// tabs F, the car rows E, the colors D, the car's info C) and their panel
// groups. Plain C++ over guest memory (the panels are shared with the
// recompiled code), with the originals' loads and stores in their order.
// Each function takes the stack pointer the original is entered with
// (`sp`): the back chain of an original's frame and the words it builds
// there are written as it leaves them, its register saves aren't. The UI
// movie calls go through their overrides as guest calls (`Calls`) from the
// original's frame with its return address; UiMovie::FindNode and Child
// are their plain C++. Varargs SetText and SetNodeText calls pass 0 for the
// r7 to r10 (or r6 to r10) the originals leave unset (the user's call,
// 2026-10-30). Each returns r3 as the original leaves it. Adapters:
// src/hooks/car_list.cpp (behind --native_car_list), register steps shared
// with the harness in src/hooks/car_list.h.

#pragma once

#include "race_hud_object/race_hud_object.h"

#include <cstdint>

namespace car_list {

using Calls = race_hud_object::Calls;

// The guest functions called through `Calls`: the UI movie calls (their
// overrides).
constexpr uint32_t kSetText = 0x8211C1F8;      // UiMovie::SetText (varargs)
constexpr uint32_t kSetNodeText = 0x8211C7E8;  // UiMovie::SetNodeText (varargs)
constexpr uint32_t kScrollText = 0x8211C570;   // UiMovie::ScrollText
constexpr uint32_t kResetTextScroll = 0x8211C5F8;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kShowNode = 0x8211AC00;
constexpr uint32_t kHideNode = 0x8211AC48;
constexpr uint32_t kSetNodeTexture = 0x8211A708;
// The C panels' vtable slots called: 1 SetStats, 2 SetCarName (ours where
// the slot holds CarPanel::SetStats or CarPanel::SetCarName, else a guest
// call), and CarPanel::SetPrice (never seen in play: the original).
constexpr uint32_t kSetStats = 0x8224E288;
constexpr uint32_t kSetCarName = 0x822417E8;
constexpr uint32_t kSetPrice = 0x8224E7E8;
// The doubles the stats scale by (lfd).
constexpr uint32_t kStatsScaleA = 0x82009F00;
constexpr uint32_t kStatsScaleB = 0x82009F08;

// --- The panel groups (panels.cpp) ---

// sub_8224EF18 PanelGroup::SetCaption: when the group's +32 isn't
// negative, UiMovie::SetText(m, +12, +32, id), ScrollText(m, +12, +32, 1)
// and the scroll's reset (sub_8211C5F8) on the group's movie.
uint64_t SetCaption(uint8_t* base, uint32_t group, uint64_t id, uint32_t sp, const Calls& calls);
// sub_82249CF0 PanelPart::FinishAll: UiMovie::FinishAll on the movie of
// the panel's group (+4).
uint64_t PartFinishAll(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls);

// --- The tabs F (+4 the group) ---

// sub_8224FD20 TabPanel::SetCursorY: node (+24, +28)'s child +40, its +16
// = (float) y, when it isn't 0.
uint64_t TabSetCursorY(uint8_t* base, uint32_t panel, uint64_t y, uint32_t sp);
// sub_8224FD90 TabPanel::SetButtons: the button prompts (node +36, +84 to
// +87 textures 0, 0, 1, 1; +88, +89 hidden).
uint64_t TabSetButtons(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls);

// --- The car rows E (+0 the group) ---

// sub_8224F018 RowPanel::SetRow: row `row` (-1: +60) from `entry`: the
// logo's frame and the three texts.
uint64_t RowSetRow(uint8_t* base, uint32_t panel, uint64_t row, uint32_t entry, uint32_t sp,
                   const Calls& calls);
// sub_8224F680 RowPanel::SetArrows: the up and down arrows by the scroll
// and the count.
uint64_t RowSetArrows(uint8_t* base, uint64_t panel, uint64_t scroll, uint64_t count,
                      uint32_t sp);
// sub_8224F558 RowPanel::Unhighlight (two players only, +156).
uint64_t RowUnhighlight(uint8_t* base, uint64_t panel, uint32_t sp, const Calls& calls);
// sub_8224F4C8 RowPanel::Hide.
uint64_t RowHide(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls);

// --- The colors D (+4 the group) ---

// sub_8224D010 ColorPanel::Show.
uint64_t ColorShow(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls);
// sub_8224D118 ColorPanel::Select (r4 the item).
uint64_t ColorSelect(uint8_t* base, uint32_t panel, uint64_t item, uint32_t sp,
                     const Calls& calls);
// sub_8224D2D0 ColorPanel::SetArrows (r4 up, r5 down).
uint64_t ColorSetArrows(uint8_t* base, uint32_t panel, uint64_t up, uint64_t down, uint32_t sp);
// sub_8224D358 ColorPanel::ShowSwatches (r4 the count).
uint64_t ColorShowSwatches(uint8_t* base, uint32_t panel, uint64_t count, uint32_t sp);
// sub_8224D3E0 ColorPanel::PlaceCursor (r4 the item).
uint64_t ColorPlaceCursor(uint8_t* base, uint32_t panel, uint64_t item, uint32_t sp);
// sub_8224D458 ColorPanel::SetCount: ShowSwatches, then caption 552 unless
// +88 (two players).
uint64_t ColorSetCount(uint8_t* base, uint32_t panel, uint64_t count, uint32_t sp,
                       const Calls& calls);
// sub_8224D4A0 ColorPanel::SetSwatch (r4 the swatch, r5 a color record).
uint64_t ColorSetSwatch(uint8_t* base, uint32_t panel, uint64_t swatch, uint32_t color,
                        uint32_t sp);
// sub_8224D518 ColorPanel::SetSwatches (r4 the colors' list, r5 the
// scroll).
uint64_t ColorSetSwatches(uint8_t* base, uint64_t panel, uint32_t list, uint64_t scroll,
                          uint32_t sp);

// --- The car's info C (+12 the group) ---

// sub_8224D7F0 CarPanel::SetTexts.
uint64_t CarSetTexts(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls);
// sub_8224D860 CarPanel::SetButtons (r4 to r7 four icons, -1 hidden).
uint64_t CarSetButtons(uint8_t* base, uint32_t panel, uint64_t a, uint64_t b, uint64_t c,
                       uint64_t d, uint32_t sp, const Calls& calls);
// sub_8224D9A0 CarPanel::PlaceCursor (r4 the action).
uint64_t CarPlaceCursor(uint8_t* base, uint64_t panel, uint64_t action, uint32_t sp);
// sub_8224DCF0 CarPanel::Select (r4 the arrows on).
uint64_t CarSelect(uint8_t* base, uint32_t panel, uint64_t arrows, uint32_t sp,
                   const Calls& calls);
// sub_8224DF30 CarPanel::StopArrows.
uint64_t CarStopArrows(uint8_t* base, uint32_t panel, uint32_t sp, const Calls& calls);
// sub_8224DED8 CarPanel::SetName (r4 an entry): caption 1862 + the car's
// id unless +316 (two players).
uint64_t CarSetName(uint8_t* base, uint32_t panel, uint32_t entry, uint32_t sp,
                    const Calls& calls);
// sub_8224E288 CarPanel::SetStats (slot 1; r4 the stats record).
uint64_t CarSetStats(uint8_t* base, uint32_t panel, uint32_t record, uint32_t sp,
                     const Calls& calls);
// sub_822417E8 CarPanel::SetCarName (slot 2; r4 the car's id, r5 the
// spec's +769).
uint64_t CarSetCarName(uint8_t* base, uint32_t panel, uint64_t id, uint64_t kind, uint32_t sp,
                       const Calls& calls);
// sub_82241868 Panel7C::ShowArrows (slot 3; r4 shown).
uint64_t Panel7CShowArrows(uint8_t* base, uint32_t panel, uint64_t shown, uint32_t sp,
                           const Calls& calls);
// sub_8224E920 CarPanel::ShowStats (r4 an entry): the stats record on the
// frame (+80) to slot 1, then slot 2 (the plain C++ where they hold
// SetStats and SetCarName, else guest calls), SetPrice (the original) with
// +4 set.
uint64_t CarShowStats(uint8_t* base, uint64_t panel_r3, uint32_t entry, uint32_t sp,
                      const Calls& calls);

}  // namespace car_list
