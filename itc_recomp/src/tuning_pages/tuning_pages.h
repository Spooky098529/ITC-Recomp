// The tuning screen's pages natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/tuning-pages.md, the fields in
// docs/known-data.md's "tuning screen"): the input handler's pages, their
// movie-side updates and helpers, and the decal images. Plain C++ over guest
// memory (the screen is shared with the recompiled code), with the
// originals' loads and stores in their order. Adapters:
// src/hooks/tuning_pages.cpp (behind --native_tuning_pages), register steps
// shared with the harness in src/hooks/tuning_pages.h. The screen's own
// functions are src/tuning_screen/.

#pragma once

#include <cstdint>

namespace tuning_pages {

// --- The pages' counts and the owned flags (tables.cpp): leaves, no frame ---

// sub_821CC728 TuningScreen::LevelCount: the level list's length for the
// category +9656 (unsigned) and the item byte +9660: 0: item 0 10, 1 to 3 7;
// 1: 0 to 3 7, 4 10; 3: 0 to 2 5; else 0.
uint32_t LevelCount(uint8_t* base, uint32_t screen);
// sub_821CC7B8 TuningScreen::TypeCount: the type list's length (its row).
uint32_t TypeCount(uint8_t* base, uint32_t screen);
// sub_821CCD38 TuningScreen::PageCursor (its case 0 the split piece
// sub_821CCD70, merged): the cursor of the page for the state +9652.
uint32_t PageCursor(uint8_t* base, uint32_t screen);
// sub_821CCDB0 TuningScreen::PageItemCount (its case 0 the split piece
// sub_821CCDE8, merged): the item count of the page for the state +9652.
uint32_t PageItemCount(uint8_t* base, uint32_t screen);
// sub_821CCE30 TuningScreen::IsPickOwned: the owned flag of the current pick
// (the screen's own level, type and option bytes), 0 where there's none.
uint32_t IsPickOwned(uint8_t* base, uint32_t screen);
// sub_821CD498 TuningScreen::MarkOwned (its category 4 case 0 the split
// piece sub_821CD6C8, merged): the bought choice's owned flags set to 1 (its
// row). r3 left as it is.
void MarkOwned(uint8_t* base, uint32_t screen);
// sub_821CDF98 TuningScreen::TestAndSetWheelFlag: wheel number `n` (r4, its
// low word signed): the wheels' flag word by its brand returned as it was
// and set to 1; 0 and nothing written for n below 0 or above 178.
uint32_t TestAndSetWheelFlag(uint8_t* base, uint32_t screen, uint64_t n);
// sub_821E7DB0 TuningScreen::SetWheelListEnd (its cases 0, 7 and 13 the
// split piece sub_821E7DE4, merged): +25224 by the brand +25204 (above 16
// unsigned, nothing). r3 left as it is.
void SetWheelListEnd(uint8_t* base, uint32_t screen);
// sub_821EF288 TuningScreen::InchListEnd (its cases 0, 4, 13, 14, 17, 21
// the split piece sub_821EF2FC, merged): 3, 2, 4 or 0 by the selected
// garage car's id (its row).
uint32_t InchListEnd(uint8_t* base, uint32_t screen);

// --- The vinyl editor's set-up (editor.cpp): leaves, no frame ---

// sub_821FE560 TuningScreen::SetUpVinylBusyIcon: +10016 = 0, +10020 = 13,
// +10024 = 155, +10028 = 154, +10032 = 156. r3 left as it is.
void SetUpVinylBusyIcon(uint8_t* base, uint32_t screen);
// sub_821FE5D8 DecalImages::AllLoaded: r3 = the decal images; 0 when an
// entry's +8 is 1 (loading), else 1 (all 200 looked at).
uint32_t AllLoaded(uint8_t* base, uint32_t images);
// sub_821FE7D8 TuningScreen::KeepListTextures: with +9656 5 and byte +9660
// 6, the kept textures 6 to 14 = entries 12 to 20's +28 and +32. r3 left.
void KeepListTextures(uint8_t* base, uint32_t screen);
// sub_821FE420 TuningScreen::SetEditorLimits: +30784 to +30796 by the item
// and the set +30724 (its row). r3 left.
void SetEditorLimits(uint8_t* base, uint32_t screen);
// sub_822045F8 TuningScreen::LoadPageLayouts: +30800 to +30831 from the
// table at 0x8200C560 by the selected garage car's id (unchecked). r3 left.
void LoadPageLayouts(uint8_t* base, uint32_t screen);

}  // namespace tuning_pages
