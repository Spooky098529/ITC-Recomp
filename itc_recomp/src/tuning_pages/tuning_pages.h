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
#include <functional>

#include "tuning_screen/tuning_screen.h"

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

// --- The decal images' textures (decals.cpp) ---
// Each with the original's frame below `sp` (its back chain and the movie
// id at frame + 80; its register saves not written) and r3 as the original
// leaves it.

// Texture::Free (sub_820ECFC8) on (header_in, data_in): its r3. The adapter
// gives car_models::TextureFree's plain C++ where it runs natively, else the
// guest's.
using TextureFreeFn = std::function<uint64_t(uint32_t header_in, uint32_t data_in)>;

// sub_821FE6A0 DecalImages::FreeTexture: r3 = the decal images, r4 = an
// entry i (all 64 bits each). Its texture e+28 freed (the tail call
// Texture::Free(e + 28, e + 32), its r3) unless a kept texture's header
// (r3 + 7236 + 8k) or, for i 33 to 50, another of entries 33 to 50 holds it;
// then r3 = e + 28 (64 bits). No frame.
uint64_t FreeTexture(uint8_t* base, uint64_t images, uint64_t i, const TextureFreeFn& free);
// sub_82204698 DecalImages::RestoreVinylTexture: w 0 or 1, movie 13's
// texture n's +0 = [images + 8836] or [+ 8840]. 112-byte frame; r3 the
// movie's entry, else `images` (r3 as it came, all 64 bits).
uint64_t RestoreVinylTexture(uint8_t* base, uint64_t images, uint32_t w, uint32_t n, uint32_t sp);
// sub_821F89B8 DecalImages::RestoreStickerTexture: w 2 or 3, movie 14's
// texture n's +0 = [images + 8844] or [+ 8848]. The same frame and r3.
uint64_t RestoreStickerTexture(uint8_t* base, uint64_t images, uint32_t w, uint32_t n,
                               uint32_t sp);
// sub_82204730 DecalImages::BindVinylTexture: entry i's texture into movie
// 13's texture by i and the group (its row). 128-byte frame; r3 the movie's
// entry.
uint32_t BindVinylTexture(uint8_t* base, uint64_t images, uint32_t i, uint32_t group,
                          uint32_t sp);
// sub_822081A0 DecalImages::RestoreMovieTexture: RestoreVinylTexture then
// RestoreStickerTexture with (images, w, n), from its 112-byte frame; r3 the
// second's.
uint64_t RestoreMovieTexture(uint8_t* base, uint64_t images, uint32_t w, uint32_t n,
                             uint32_t sp);
// sub_822081D8 DecalImages::RestoreVinylMovie: movie 13's 24 textures back
// to their placeholders (its row), each followed by RestoreStickerTexture.
// 128-byte frame; r3 the last RestoreStickerTexture's (`images`).
uint64_t RestoreVinylMovie(uint8_t* base, uint64_t images, uint32_t sp);
// sub_821F6DD8 DecalImages::RestoreStickerMovie: RestoreMovieTexture for
// movie 14's 32 textures (placeholder 3, then 2; its row). 96-byte frame; r3
// the last one's.
uint64_t RestoreStickerMovie(uint8_t* base, uint64_t images, uint32_t sp);
// sub_82203D40 TuningScreen::BindVinylTextures: with +9656 5 and byte +9660
// 6, the 24 entries' textures into movie 13's (its row). 112-byte frame; r3
// the movie's entry, else the screen (all 64 bits).
uint64_t BindVinylTextures(uint8_t* base, uint64_t screen, uint32_t sp);
// sub_821D84F8 TuningScreen::EnterEditor: state 60's page (its row). With
// layer 0's fade counts 0 and the decal images all loaded: FadeIn, then for
// the vinyls or the stickers BindVinylTextures and KeepListTextures, Push(4),
// the queue to state 23 (command 15, SetUpVinylBusyIcon) or 38 (command 32).
// 96-byte frame; r3 the screen, AllLoaded's 0 or FadeIn's (the passes).
uint64_t EnterEditor(uint8_t* base, uint64_t screen, uint32_t sp);

// --- The vinyls' records (vinyls.cpp; flush-to-zero off) ---

// sub_82203F20 TuningScreen::StoreVinyls: the screen's six vinyl records
// (+30880 + 24n, +30952 + 24n, the ids +31168 and +31180) into the selected
// garage car's decal records (its row). No frame: the index list {0, 1, 2}
// at sp - 16 and the conversions' doublewords at sp - 48 to sp - 24 as the
// original leaves them. r3 left.
void StoreVinyls(uint8_t* base, uint64_t screen, uint32_t sp);
// sub_822040A8 TuningScreen::LoadVinyls: the inverse, then the ids and the
// records copied to their saved copies (+31192, +31024: memcpy's plain C++
// where Buffers_Memcpy takes it, else the guest's through `call`). 144-byte
// frame (the index list at frame + 80); r3 the last memcpy's (its
// destination).
uint64_t LoadVinyls(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                    uint32_t sp);

// --- Movie 2's nodes and the color conversion (nodes.cpp) ---

// sub_821D9E68 TuningScreen::ShowPricePanel: node (-1, 19) shown (UiMovie::
// ShowNode's plain steps), then hidden in state 16 with the brand +25204 0
// and in states 9, 10, 11, 14. 112-byte frame; r3 the node.
uint32_t ShowPricePanel(uint8_t* base, uint32_t screen, uint32_t sp);
// sub_821EED98 TuningScreen::SetBrandLogo (its brand 1 the split piece
// sub_821EEE14, merged): node (-1, 17)'s child 0's +108 by the brand +25204
// (1 to 16; else nothing). 112-byte frame; r3 the child.
uint32_t SetBrandLogo(uint8_t* base, uint32_t screen, uint32_t sp);
// sub_8224AB50 Color::HsvToRgb: `in` (h, s, v floats) into `out` (r, g, b,
// then 1.0 at +12), with the original's single-precision steps; s 0.0 gives
// v three times (no +12); no frame: the input copied below `sp` and the
// sector's conversions there as the original leaves them. Needs
// flush-to-zero off. r3 left.
void HsvToRgb(uint8_t* base, uint32_t out, uint32_t in, uint32_t sp);

// --- The current choices (choices.cpp) ---
// Guest calls through `call` (tuning_screen::GuestCall) where the plain C++
// isn't taken: Params::GetCarParts and AeroParts::Get82 without
// `native_specs` (--native_car_specs) or where Get82's plain C++ declines,
// memcpy where Buffers_Memcpy declines.

// sub_821DE160 TuningScreen::SetAeroAvailability: the aero items'
// availability +25128 to +25164 by the selected garage car's body kit
// [+36588] (its row). 192-byte frame (the parts at frame + 128, their copy
// at frame + 80); r3 Get82's (frame + 128), else memcpy's (frame + 80).
uint64_t SetAeroAvailability(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t sp, bool native_specs);
// sub_821E1DB0 TuningScreen::SetCurrentChoice (its category 4 case 0 the
// split piece sub_821E203C, merged): the choice just picked into the
// current choice words (its row). 96-byte frame; r3 as the original leaves
// it: 36080 (category 3 item 0), SetAeroAvailability's (category 4 item 9),
// the last WheelNumber's (category 5 item 0), else r3 as it came.
uint64_t SetCurrentChoice(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3,
                          uint32_t sp, bool native_specs);

}  // namespace tuning_pages
