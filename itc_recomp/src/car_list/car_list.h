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

// sub_822501C8 TabPanel::SetTab (r4 the tab): +8 = it, the cursor at 40 x
// the tab, the tabs' texts, nodes +88 and +92's +108 by the tab, caption
// 929 with +140 (two players), else 930 + the tab.
uint64_t TabSetTab(uint8_t* base, uint32_t panel, uint64_t tab, uint32_t sp, const Calls& calls);
// sub_8224FF88 TabPanel::Move (r4 the tab, r5 the tab before, r6 the
// moves).
uint64_t TabMove(uint8_t* base, uint32_t panel, uint64_t tab, uint64_t before, uint64_t moves,
                 uint32_t sp, const Calls& calls);

// --- The car rows E (+0 the group) ---

// sub_8224F758 RowPanel::Fill (r4 the list, r5 the scroll, r6 the item).
uint64_t RowFill(uint8_t* base, uint64_t panel, uint32_t list, uint64_t scroll, uint64_t item,
                 uint32_t sp, const Calls& calls);
// sub_8224F888 RowPanel::Refresh (r4 the list, r5 the scroll).
uint64_t RowRefresh(uint8_t* base, uint64_t panel, uint32_t list, uint64_t scroll, uint32_t sp,
                    const Calls& calls);
// sub_8224F8F8 RowPanel::Move (r4 the row, r5 the row before, r6 the
// moves, r7 the scroll, r9 the list; r8 unread).
uint64_t RowMove(uint8_t* base, uint64_t panel, uint64_t row, uint64_t before, uint64_t moves,
                 uint64_t scroll, uint32_t list, uint32_t sp, const Calls& calls);

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
// sub_8224D580 ColorPanel::Move (r4 the item, r5 the one before, r6 the
// moves, r7 the scroll, r8 the menu's +84, r9 its +92, r10 the colors).
uint64_t ColorMove(uint8_t* base, uint64_t panel, uint64_t item, uint64_t before, uint64_t moves,
                   uint64_t scroll, uint64_t step, uint64_t placed, uint32_t list, uint32_t sp,
                   const Calls& calls);
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

// --- PanelSet's leaves (set.cpp; r3 the set, b = the set + 4 its base) ---

// The two-player word (0 in PanelSet, 1 in PanelSetPair).
constexpr uint32_t kTwoPlayers = 0x82815EAC;
// The players' save data pointer sub_82105678 answers (two players).
constexpr uint32_t kPairSaveData = 0x826C5B5C;

// sub_822496C8 PanelSet::Reset (slot 2): the base's words reset. r3 left.
uint64_t SetReset(uint8_t* base, uint64_t set);
// sub_82249848 PanelSet::Picked, sub_82249870 WentBack, sub_82249898
// WantsPowerGraph (slots 8 to 10).
uint64_t SetPicked(uint8_t* base, uint32_t set);
uint64_t SetWentBack(uint8_t* base, uint32_t set);
uint64_t SetWantsPowerGraph(uint8_t* base, uint32_t set);
// sub_8224B6D8 SetOpenKind (+1932), sub_8224B6E8 SetPreviousTab (+1884),
// sub_8224B6F0 SetPreviousEntry (+1888), sub_8224B6F8 SetPreviousColor
// (+1892): the set's word at `offset` = r4. r3 left.
uint64_t SetWord(uint8_t* base, uint64_t set, uint32_t offset, uint64_t value);
// sub_82248EF0 PanelSetBase::PickedPage (page 5; r3 = b): +12 = 1. r3 left.
uint64_t PickedPage(uint8_t* base, uint64_t b);
// sub_82249668 PanelSetBase::SaveData (r3 = b): with two players, the
// players' save data ([0x826C5B5C], sub_82105678's, never seen in play:
// ported) + 120,108 x +1952, else SaveData::Of(0).
uint64_t SaveData(uint8_t* base, uint32_t b, uint32_t sp);

// --- PanelSet's pages and steps (pages.cpp; r3 = b, a step's r4 its
// first-pass word b + 56) ---

// The guest calls: MenuList::Init (r3 to r10 and two words at the
// caller's frame + 84 and + 92), SetSelection, Selection, the previous
// selection (sub_8220BC28), Release; VirtualFree (where the block isn't the
// allocation layer's pages); ScreenFades::FadeIn and FadeOut; sub_825C8390
// (TabPanel's "finished", 1).
constexpr uint32_t kMenuListInit = 0x8220BAC0;
constexpr uint32_t kMenuListSetSelection = 0x8220BC30;
constexpr uint32_t kMenuListSelection = 0x8220BC20;
constexpr uint32_t kMenuListPrevious = 0x8220BC28;
constexpr uint32_t kMenuListRelease = 0x8220BBB0;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kFadeOut = 0x8209D0C8;
constexpr uint32_t kTabFinished = 0x825C8390;
// The fades' pass counts (FadeIn's, FadeOut's) and the steps' wait word.
constexpr uint32_t kFadeInPasses = 0x82001BEC;
constexpr uint32_t kFadeOutPasses = 0x82001BF0;
constexpr uint32_t kFadeWait = 0x82815EB0;

// sub_82249558 PanelSetBase::ToTabs.
uint64_t ToTabs(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls);
// sub_82249730 PanelSet::Open (slot 3; r3 the set).
uint64_t Open(uint8_t* base, uint64_t set, uint32_t sp, const Calls& calls);
// sub_8224B538 PanelSetBase::Free (the lists' entries and the colors freed,
// VirtualFree; the menus released) and sub_8224B880 PanelSet::Free (slot
// 7, on the set + 4).
uint64_t Free(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls);
// sub_8224BF30 PanelSetBase::HandleInput and sub_8224CA28
// PanelSet::HandleInput (slot 5, on the set + 4): the page's handler from
// the table of 9 on the frame (the plain C++ where it's ours, else a guest
// call).
uint64_t HandleInput(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls);
// sub_8224A3F8 PanelSetBase::BackPage8 (page 8).
uint64_t BackPage8(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls);
// The steps (each returns its answer): sub_82248F00 Step2 (F's Move),
// sub_8224A718 Step3 (the fade out, F finished), sub_82249098 Step7 (E's
// Move), sub_8224A7E0 Step11 (C's Select), sub_82249258 Step18 (D's
// Select), sub_8224A8E0 Step26 (the fade out, D and F finished).
uint64_t Step2(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls);
uint64_t Step3(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls);
uint64_t Step7(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls);
uint64_t Step11(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls);
uint64_t Step18(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls);
uint64_t Step26(uint8_t* base, uint32_t b, uint32_t word, uint32_t sp, const Calls& calls);

// MenuList::Fit (sub_8220BC40) and CarPanel::ShowCarStats (sub_8224EA10,
// never seen in play: the original) through `Calls`.
constexpr uint32_t kMenuListFit = 0x8220BC40;
constexpr uint32_t kShowCarStats = 0x8224EA10;
// sub_8224A978 PanelSetBase::SetUpRows: the two menus set up for the list's
// (or, tab 2, the garage's) count, the selection and scroll put back, E
// filled and refreshed.
uint64_t SetUpRows(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls);
// sub_8224C560 PanelSetBase::SetUpCarPage: the actions' menu set up, the
// rows' selection put back, C's cursor, texts, stats (the garage tab's
// through ShowCarStats, the original) and name.
uint64_t SetUpCarPage(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls);

// memcpy (sub_825A7410) where c_runtime::Memcpy refuses.
constexpr uint32_t kMemcpy = 0x825A7410;
// sub_8224C698 PanelSetBase::Build and sub_8224CAD8 PanelSet::Build (slot
// 1, on the set + 4): nothing when +4 (built) and the two-player word share
// a bit; else the lists' entries freed when built, then the unlocked cars
// (the save data's bits at +104740) to lists 0 and 1 by their spec's byte
// +771 (else freed) and the garage's cars to list 2, each an 8,448-byte
// entry from the allocation layer's pages (VirtualAlloc, the 2026-10-22
// call) with the spec copied in (Params::GetCarSpec's and memcpy's plain
// C++, the original where c_runtime::Memcpy refuses).
uint64_t Build(uint8_t* base, uint64_t b, uint32_t sp, const Calls& calls);

// --- The power graph's record (panels.cpp) ---

// The power graph screen's record's holder ([0x82815EBC]; the record at
// its +8).
constexpr uint32_t kPowerGraph = 0x82815EBC;
// sub_82250368 PowerGraph::SetEntry (r3 an entry, r4 the tab; a leaf, its
// switch's case 0 and 4 the split piece sub_822503F0, merged). r3 left.
uint64_t PowerGraphSetEntry(uint8_t* base, uint64_t entry, uint64_t tab);

}  // namespace car_list
