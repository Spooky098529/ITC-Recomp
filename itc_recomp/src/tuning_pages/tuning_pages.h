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
#include <optional>

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

// --- The decal images' update and unload (decals.cpp) ---
// They allocate and free (the allocation layer's pages and physical
// blocks), so the adapters run them only while the native heap is active
// (hooks::NativeHeapActive()). Guest calls through `call` (from the
// original's frame, with its return address): TGA::CreateImage where its
// plain C++ declines, Resource::Release, memcpy where Buffers_Memcpy
// declines, VirtualFree outside the layer's range, Texture::Free where
// car_models' plain C++ isn't taken, Texture::FromTga where it declines.

// sub_820ED528 Texture::FromTga: the TGA file `file` (`size` bytes) into a
// new A8R8G8B8 texture (Texture::Create into `header_out` and `data_out`),
// its rows copied at the level's pitch, the pixels freed. 160-byte frame
// (`file` stored at frame + 180, the caller's sp + 20, and passed as the
// buffer's address; the header at frame + 96, the pixels at + 80, the pitch
// at + 88). r3 frame + 96 (the empty sub_82106540's argument); nullopt,
// with nothing done, where memcpy isn't native (Buffers_NativeMemcpy).
std::optional<uint64_t> TextureFromTga(uint8_t* base, const tuning_screen::GuestCall& call,
                                       uint32_t file, uint32_t size, uint32_t header_out,
                                       uint32_t data_out, uint32_t sp);
// sub_821FE628 DecalImages::Update: each entry with +20 set and +8 2 made a
// texture (Texture::FromTga(+12, +16, e + 28, e + 32)), then +20 = 0.
// 112-byte frame; r3 the last FromTga's, else `images` (all 64 bits).
uint64_t UpdateImages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                      uint32_t sp);
// sub_821FEA68 DecalImages::Unload: entry i (not checked), when loaded
// (+8 2): +0 = -1, +4 = +8 = 0, FreeTexture, +28 = +32 = 0, its file freed
// (VirtualFree(+12, 0, MEM_RELEASE)), +12 to +24 = 0. 112-byte frame; r3
// VirtualFree's, else `images`. `native_free` Texture::Free by car_models'
// plain C++ (--native_car_models) where TextureFreeable.
uint64_t Unload(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images, uint64_t i,
                uint32_t sp, bool native_free);
// sub_82208738 DecalImages::UnloadAll: RestoreVinylMovie, RestoreStickerMovie,
// Unload for the 200 entries, then Texture::Free(&header, &data) for each
// kept texture (images + 7236 + 8k) whose words are both set. 112-byte
// frame; r3 the last call's.
uint64_t UnloadAll(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                   uint32_t sp, bool native_free);

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

// --- The decal images' loads (loads.cpp) ---
// Guest calls through `call` from the originals' frames with their return
// addresses: VirtualAlloc (its override) and Archive::Load (its override,
// r7 = r8 = 0); Archive::AssetSize by its plain C++.

// sub_821FE830 DecalImages::Request: r3 = the decal images, r4 an asset id,
// r5 an entry i (all 64 bits; e = i * 36 + r3, i not checked). Nothing when
// e+8 is 1 or 2; else e+0 the id (a group's "none" id swapped for its
// stand-in, entries above 108 keeping theirs), the file's pages into +12,
// Archive::Load into +4, +8 = +24 = 1, +16 the size, +20 = 0. 96-byte frame;
// r3 the last AssetSize's, else `images`.
uint64_t RequestImage(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                      uint64_t id, uint64_t i, uint32_t sp);
// sub_821F6D40 DecalImages::Load: Request without its state check and id
// swap. 96-byte frame; r3 the last AssetSize's.
uint64_t LoadImage(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                   uint64_t id, uint64_t i, uint32_t sp);
// sub_821FEE10 TuningScreen::LoadDecalImages: r3 = the screen (all 64 bits).
// With +9656 5 and byte +9660 6 the vinyls' 51 entries requested (its row),
// else the 200 sticker images loaded. 128-byte frame; r3 the last call's.
uint64_t LoadDecalImages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp);

// --- The input pages and the list menu's step (pages.cpp) ---
// Guest calls through `call` from the originals' frames with their return
// addresses: Menu::Step, MenuSound::Play, UiMovie::Finish, Play and Loop
// (their overrides); the player's mapper's slots 10 and 12 by
// ActionMapper::Pressed's and Fired's plain C++ with `native_slots`
// (--native_action_mappers) where the vtable holds them, else through `call`;
// ScreenFades::FadeIn and FadeOut, TuningScreen::CategoryItemCount and
// MoveCategory by their plain C++.

// The flags the pages pass on: the mappers' slots (--native_action_mappers),
// the native heap (hooks::NativeHeapActive(): DecalImages::UnloadAll by its
// plain C++, else the guest's) and Texture::Free under it by car_models'
// plain C++ (--native_car_models).
struct PageFlags {
  bool native_slots = false;
  bool native_heap = false;
  bool native_free = false;
};

// sub_821CE1D0 Menu::StepList: Menu::Step for a scrolling list, r3 = the
// menu (all 64 bits; its row): +4 = +0, +44 = +36 = 0, the first of up,
// down (the cursor and the row +48 stepped, +52 the row before), left, right
// (+36) fired on the player's mapper (slot 12), then the cursor wrapped
// (+44 = 1) or clamped to [+12, +16], the row to [+56, +60 - 1] (or to the
// other end after a wrap), +8 the way it moved. 128-byte frame; r3 the last
// slot's, else the menu.
uint64_t StepList(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp,
                  bool native_slots);
// sub_821D8670 TuningScreen::LeaveEditor: state 61's page (its row): with
// layer 0's fade counts 0, Push(61), the queue to state 62, FadeIn, movie
// 2's item animation (27, item + 2) finished, played and looped, the busy
// icon. 128-byte frame; r3 Loop's, else the screen (all 64 bits).
uint64_t LeaveEditor(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp);
// sub_821D6AD0 TuningScreen::InputCategories: state 3's page (its row):
// Menu::Step on [+10040]; moved, A or B (the player's mapper's slot 10 on
// 159, 158); MoveCategory always. 112-byte frame; r3 MoveCategory's.
uint64_t InputCategories(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                         uint32_t sp, const PageFlags& flags);

// sub_821D87C0 TuningScreen::BackToItems: state 62's page (its row): with
// layer 0's fade counts 0, DecalImages::UnloadAll (its plain C++ with the
// native heap), +9708 = +9716 = +9720 = 1, the history popped until state 4
// (no bound), the queue to state 4 with command 2 (OpenItems). 96-byte
// frame; r3 UnloadAll's, else the screen (all 64 bits).
uint64_t BackToItems(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp, const PageFlags& flags);
// sub_821EE390 TuningScreen::InputWheelParts: state 15's page (its row): up
// and down on +25196 (0 to 2, wrapped: +25200), A (brand 0, SetWheelListEnd,
// Push(15), Go(16, 8), PreviewWheels and ShowPricePanel by their plain C++),
// B (Pop, the menu reset by PageItemCount and PageCursor, command 2); then
// AnimateWheelParts. 128-byte frame; r3 AnimateWheelParts'.
uint64_t InputWheelParts(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                         uint32_t sp, const PageFlags& flags);
// sub_821E8CE0 TuningScreen::AnimateWheelParts: state 15's movie side (its
// row): movie 2's named animations 389 to 401, 966 by the same actions asked
// again; the bar node (27, 0)'s +16 after a wrap. 128-byte frame; the last
// call's r3, else the movie's entry.
uint64_t AnimateWheelParts(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                           uint32_t sp, bool native_slots);
// sub_821EEEA8 TuningScreen::InputInches: state 17's page (its row): up and
// down on +25228 (0 to +25232, wrapped: +25236), A (the confirm dialog by
// IsPickOwned and GetPrice, Push(17)), B (Pop, command 8); PreviewWheels
// after any of them; then AnimateInches. 112-byte frame; r3 AnimateInches'.
uint64_t InputInches(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                     uint32_t sp, const PageFlags& flags);
// sub_821E9110 TuningScreen::AnimateInches: state 17's movie side (its row):
// named animations 20 to 31 and 62; the bar Child(node (1, 9), 0)'s +16
// after a wrap. 128-byte frame; the last call's r3, else the movie's entry.
uint64_t AnimateInches(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                       uint32_t sp, bool native_slots);

}  // namespace tuning_pages
