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

// Texture::Free for FreeTexture's and FreeKept's tail calls from `frame`
// returning to `lr`: car_models' plain C++ with `native_free` (the native
// heap and --native_car_models) where TextureFreeable, else the guest's.
TextureFreeFn GuestTextureFree(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t frame,
                               uint32_t lr, bool native_free);
// sub_821FE608 DecalImages::OnLoaded: the archive's callback, r3 = a
// finished request's handle, r6 = the entry e: when the handle is e+4's,
// e+8 = 2 (loaded) and e+20 = 1 (a texture to make). Leaf.
void OnImageLoaded(uint8_t* base, uint32_t handle, uint32_t e);
// sub_821FE960 DecalImages::Reload: Request's id swap, then with no state
// check and no new pages the file into the entry's buffer +12
// (Archive::Load into +4), +8 = +24 = 1, +16 the size, +20 = 0. 96-byte
// frame; r3 AssetSize's.
uint64_t ReloadImage(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                     uint64_t id, uint64_t i, uint32_t sp);
// sub_821FEAF0 DecalImages::LoadWindow: Reload for n entries from i with the
// ids from `first` moved into the list `list` (0 163 to 167, 1 189 to 382, 2
// 383 to 581; 3 or more nothing; its row). 144-byte frame; r3 the last
// Reload's, else `images`.
uint64_t LoadWindow(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                    uint64_t first, uint64_t i, uint64_t n, uint64_t list, uint32_t sp);
// sub_821FED30 DecalImages::Scroll: direction 1 (FreeTexture(i), entries i
// to i + n - 2 take the next one's texture, Reload into i + n - 1) or 0 (the
// other way, Reload into i); other directions nothing. 128-byte frame; r3
// Reload's, else `images`. `native_free` as GuestTextureFree's.
uint64_t ScrollImages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t images,
                      uint64_t direction, uint64_t i, uint64_t n, uint64_t id, uint32_t sp,
                      bool native_free);
// sub_821FE740 DecalImages::FreeKept: the kept texture k (images + 7236 +
// 8k) freed (the tail call Texture::Free(kept, kept + 4)) unless an entry's
// +28 holds its header; then r3 = the kept word's address (64 bits). No
// frame; the kept words aren't cleared.
uint64_t FreeKept(uint8_t* base, uint64_t images, uint64_t k, const TextureFreeFn& free);

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
// sub_821F40B8 TuningScreen::FillPartRowsRight and sub_821F4768
// FillPartRowsLeft: the wheel list's rows after a step right or left (their
// rows), each row as FillPartRows lays it (tuning_screen::FillPartRow), after
// HidePartMarks; n = +25224 1, 2 or 4 and above (0, 3 or negative nothing).
// r3 = the screen (all 64 bits). 176-byte frame; the last ShowPartMarks' r3,
// else the movie's entry.
uint64_t FillPartRowsRight(uint8_t* base, uint64_t r3, uint32_t sp);
uint64_t FillPartRowsLeft(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_821F60E0 TuningScreen::AnimateWheels: state 16's movie side (its row):
// by the same actions asked again, the list's slides (58, 59, 63, 64) with
// SetPartName and FillPartRowsRight or Left, the tabs' bars (30, 31; the bar
// Child(node (1, 9), 0)'s +16 after a wrap) with SetPartName,
// PlayGroupAnimation, FillPartRows and the tabs' highlights (20 to 23), A's
// and B's finishes (B: the part pick 415 + row, 1008, 1012 played). r3 = the
// screen (all 64 bits). 128-byte frame; the last call's r3, else the movie's
// entry or the slot's r3.
uint64_t AnimateWheels(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                       uint32_t sp, bool native_slots);
// sub_821F6688 TuningScreen::InputWheels: state 16's page (its row): left
// and right on the model cursor +25220, up and down on the brand +25204 and
// the tabs' row +25212 (SetWheelListEnd), A (brand 0 the confirm dialog,
// else the inch list with InchListEnd; Push(16)), B (Pop, command 7);
// PreviewWheels and ShowPricePanel after the steps and A; then the price
// panel's three SetText calls (the varargs they leave unset passed as 0),
// the brand logo node (SetBrandLogo) and AnimateWheels. r3 = the screen (all
// 64 bits). 128-byte frame; AnimateWheels' r3.
uint64_t InputWheels(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp, const PageFlags& flags);

// sub_821D0CC0 TuningScreen::MoveItem: state 4's movie side (its row): by
// the front-end menu's step (+8) and wrap (+40), sub-scene 27's bar slid or
// snapped (node (27, 0)'s +16 = float(item) x 40.0) and the item's
// animation restarted and looped, the aero list's rows (+25100 now, +25104
// before) by their availability; with no step A (the rows finished, 1013,
// category 3's 401 + item) or B (966, or for aero 1013, 15 and 967) asked
// again. 128-byte frame; the last call's r3, else the movie's entry or the
// slot's r3.
uint64_t MoveItem(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                  uint32_t sp, bool native_slots);
// sub_821DFD48 TuningScreen::InputItems: state 4's page (its row): Menu::Step
// (StepList for aero); moved, A (the stand car's record copied into the
// screen, SceneModelRecord::Copy through its override; then by the item's
// bit in +9688 the levels, types, door, wheels, paint, light kits, vinyl or
// sticker editor or accessory page) or B (Pop, PlayIntro); then MoveItem.
// The sticker editor's ResetStickerEditor sub_821F8440 and LoadStickers
// sub_821F86C0 (not seen in play) through the guest. r3 = the screen (all
// 64 bits: the editors' callees get it). 128-byte frame; MoveItem's r3.
uint64_t InputItems(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                    uint32_t sp, const PageFlags& flags);

// sub_821D5610 TuningScreen::MoveOption: state 8's movie side (its row): by
// the front-end menu's step (+8) and wrap (+40), the option list's bars
// slid or snapped (the list's node +16 = float(byte +9664) x 40.0) and the
// option's rows restarted and looped, for the tire (category 3), the
// bonnet (category 4, item 4) and the door (category 5, item 1); with no
// step A (the rows and bars finished) or B (the same, then the page before
// played) asked again. 128-byte frame; the last call's r3, else the movie's
// entry or the slot's r3.
uint64_t MoveOption(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                    uint32_t sp, bool native_slots);
// sub_821D8160 TuningScreen::InputOptions: state 8's page (its row):
// Menu::Step; moved, A (the confirm dialog: equip, money short or buy, then
// Push(8)) or B (Pop, the menu reset, ShowLevels for the tire); then
// MoveOption. 112-byte frame; MoveOption's r3.
uint64_t InputOptions(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                      uint32_t sp, const PageFlags& flags);

// sub_821D1A18 TuningScreen::MoveLevel: state 5's movie side (its row): by
// the front-end menu's step (+8) and wrap (+40), the level list's bars slid
// or snapped (the list's node +16 = float(level) x 40.0) and the level's
// rows restarted and looped, or finished and dimmed (+68) when
// IsItemAvailable says the level isn't, for the short lists (sub-scene
// 28), the long ones (+25100 now, +25104 before, with more than 8 levels)
// and the tire's (sub-scene 30); with no step A (the rows finished, the
// type or option list's level pictures played) or B (the rows finished,
// the page before played) asked again. IsItemAvailable's r4 leftover is the
// level (the leaf leaves it). 128-byte frame; the last call's r3, else the
// movie's entry, the slot's r3 or IsItemAvailable's 0.
uint64_t MoveLevel(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                   uint32_t sp, bool native_slots);
// sub_821D32C8 TuningScreen::MoveType: state 6's movie side (its row): by
// the front-end menu's step (+8) and wrap (+40), the type list's bars slid
// or snapped (the list's node +16 = float(type) x 40.0) and the type's rows
// restarted and looped, or (G2 only) finished and dimmed (+68) when
// IsItemAvailable says the type isn't, for G1 (category 0 item 2:
// sub-scene 29), G2 (category 4, category 5 item 3, category 6 items 0 and
// 1: sub-scene 28, rows 489 and 428) and G3 (category 5 item 2, category 6
// item 2: rows 489 only); with no step A (with the level available: the
// rows finished, the type's pictures played) or B (the rows finished, the
// page before played) asked again. IsItemAvailable's r4 leftover is the
// type (the leaf leaves it). 128-byte frame; the last call's r3, else the
// movie's entry, the slot's r3 or IsItemAvailable's.
uint64_t MoveType(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                  uint32_t sp, bool native_slots);
// sub_821D6E20 TuningScreen::InputLevels: state 5's page (its row):
// Menu::StepList for the 10-level lists, else Menu::Step; moved (the level
// and the long list's rows), A (refused when IsItemAvailable says the level
// isn't; else the type list Go(6, 4), the LSD's types Go(7, 5), the tire's
// options Go(8, 6) or the confirm dialog: equip, money short or buy; then
// Push(5)) or B (Pop, +9732, the menu reset, PlayIntro for category 2, else
// OpenItems); then MoveLevel. 112-byte frame; MoveLevel's r3.
uint64_t InputLevels(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                     uint32_t sp, const PageFlags& flags);
// sub_821D74A0 TuningScreen::InputTypes: state 6's page (its row):
// Menu::Step on the lists that step; moved (the type and the long list's
// rows), action 157 (the horn's sounds), A (refused when IsItemAvailable
// says the type isn't; else the type's list Go(7, 5), state 14's dialog or
// the confirm dialog: equip, money short or buy; then Push(6)) or B (Pop,
// +9732, the menu reset and ShowLevels or OpenItems by the list); then
// MoveType. 112-byte frame; MoveType's r3.
uint64_t InputTypes(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                    uint32_t sp, const PageFlags& flags);

// sub_821DD408 TuningScreen::StoreOwnedParts: the screen's owned flags
// packed into the selected garage car's bitset (c+1904) and the save data's
// (+0x1996C), every wheel's flag set on the way (TestAndSetWheelFlag; its
// row). 128-byte frame, its {3, 2, 1, 0} table at frame + 80 as the
// original leaves it (register saves not written). Returns r3 as the
// original leaves it: the old word of the last bit set after the wheels,
// else wheel 178's old flag.
uint64_t StoreOwnedParts(uint8_t* base, uint32_t screen, uint32_t sp);
// sub_821E25B0 TuningScreen::Leave: state 59's page (its row). With both
// layer 0 fade counts 0: the money into the save data, StoreToGarageCar,
// StoreOwnedParts, then message 11 posted to r4's ring (+204). 112-byte
// frame; r3 as given, else StoreOwnedParts' r3.
uint64_t Leave(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp);

// --- The confirm dialogs' yes answers (dialogs.cpp; flush-to-zero off) ---
// Guest calls through `call` from the originals' frames with their return
// addresses: memcpy where Buffers_Memcpy declines, Career::PickNext and
// GarageStage::FitSelectedCar (their adapters); SaveData::Of, MarkOwned,
// SetCurrentChoice, PageItemCount, PageCursor, StoreVinyls and
// BindVinylTexture by their plain C++.

// sub_821FE798 DecalImages::SwapKept: entry i's texture (+28, +32) swapped
// with the kept texture k (images + 7236 + 8k, + 7240 + 8k). No frame.
void SwapKept(uint8_t* base, uint64_t images, uint64_t k, uint64_t i);
// sub_821F84C8 TuningScreen::GarageDecal: the selected garage car's 32-byte
// decal record for an area 0 to 7 (unsigned: else 0) and a slot (its row).
// No frame.
uint32_t GarageDecal(uint8_t* base, uint32_t screen, uint32_t area, uint64_t slot);
// sub_821F6CE8 TuningScreen::SaveAreaDecals: area a's five placed decals
// (ids +33320 + 20a, records +32360 + 120a) copied to the saved ones (+32200,
// +31240). 112-byte frame; r3 the last memcpy's (its destination).
uint64_t SaveAreaDecals(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint64_t area, uint32_t sp);
// sub_821F6C90 TuningScreen::RestoreAreaDecals: area a's five saved decals
// (ids +32200 + 20a, records +31240 + 120a) copied to the placed ones
// (+33320, +32360), SaveAreaDecals' other way. 112-byte frame; r3 the last
// memcpy's (its destination).
uint64_t RestoreAreaDecals(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint64_t area, uint32_t sp);
// sub_821F86C0 TuningScreen::LoadStickers: the garage car's sticker records
// (GarageDecal) into the screen's saved ones (+32200 ids, +31240 records;
// its row: mirrored areas 2 and 7), then RestoreAreaDecals for each area.
// 176-byte frame (its saves not written); r3 the last RestoreAreaDecals'.
uint64_t LoadStickers(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                      uint32_t sp);
// sub_821F8440 TuningScreen::ResetStickerEditor: +33508 = whether the
// selected garage car's id (its +224) is 0, the busy icon +10016 and the
// editor's page, layer and cursors 0 (its row). Leaf; r3 left the screen.
uint64_t ResetStickerEditor(uint8_t* base, uint64_t screen);
// sub_821F8598 TuningScreen::StoreStickers: the saved sticker records into
// the garage car's (GarageDecal; its row), the int conversions' doublewords
// at frame + 80 and + 88 as the last ones leave them. 112-byte frame; r3 the
// last GarageDecal's.
uint64_t StoreStickers(uint8_t* base, uint64_t screen, uint32_t sp);
// sub_821FA230 TuningScreen::OnStickerDialogYes: states 50 to 52's answers
// (its row). 96-byte frame; r3 SaveData::Of(0)'s in state 50, else `screen`.
uint64_t OnStickerDialogYes(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp);
// sub_821E2660 TuningScreen::OnDialogYes: the confirm dialogs' yes answers by
// the state +9652 (its row), then OnStickerDialogYes. 112-byte frame; r3
// OnStickerDialogYes's. `native_specs` as SetCurrentChoice's.
uint64_t OnDialogYes(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint32_t sp, bool native_specs);

// --- The vinyl editor's pages (vinyl_pages.cpp) ---
// Guest calls through `call` from the originals' frames with their return
// addresses: MenuSound::Play, the UI movie calls, GarageStage::Command,
// CameraCommand and FitSelectedCar (their overrides); the player's mapper's
// slots 8, 10 and 12 by ActionMapper::Held's, Pressed's and Fired's plain
// C++ with `native_slots` (--native_action_mappers) where the vtable holds
// them, else through `call` (slot 11 always through `call`).

// sub_82201B40 TuningScreen::LeaveVinylView: state 34's leaving (its row):
// movie 13 looked up; with A or B pressed +10012 = 8, FitSelectedCar, 241
// finished and played, the camera preset by the area +30724 (Command 40 or
// 41). 128-byte frame; the last call's r3 (the movie's entry or the last
// slot's when neither is pressed).
uint64_t LeaveVinylView(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots);
// sub_82202928 TuningScreen::InputVinylView: state 34's page (its row): A or
// B (Pop, Go(the popped state, 18), the stand's turns cancelled and its
// camera reset), the held buttons' stand and camera commands, then
// LeaveVinylView. 112-byte frame; LeaveVinylView's r3.
uint64_t InputVinylView(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots);

// sub_821FE498 TuningScreen::SetLastImage: +30796 the image list's last
// index: 4 or 193 on the vinyls' pages 0 and 1 (other pages nothing), else
// 198. Leaf.
void SetLastImage(uint8_t* base, uint32_t screen);
// sub_821FF3A0 TuningScreen::MoveVinylMenu: state 23's movie side (its
// row): movie 13's bars (16, 17) and pages (+30724 + 5 to + 7) by up, down
// (only with +9708 set; the bar node (3, 0)'s +16 after a wrap), A (the
// stand's decals applied: +10012 = 5, FitSelectedCar) or B. 128-byte frame;
// the last call's r3, else the movie's entry or the slot's r3.
uint64_t MoveVinylMenu(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                       uint32_t sp, bool native_slots);
// sub_82204B70 TuningScreen::InputVinylPages: state 23's page (its row): a
// kept page's textures freed (FreeKept) and kept again on its third frame,
// else up and down on the page +30724, A (layers 0 and 1 to their saved
// copies, Push(23), Go(24, 16) with the kept textures set between its
// halves) or B (state 61, StoreVinyls, FadeOut); then MoveVinylMenu.
// 128-byte frame; MoveVinylMenu's r3. Texture::Free as GuestTextureFree's
// with the native heap and car models' flag.
uint64_t InputVinylPages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags);

// sub_82206A50 TuningScreen::InputDecalPosition, sub_82206F48
// InputDecalAngle and sub_82207320 InputDecalScale: states 29 to 31's pages
// (their rows): 164 held or released (TuningScreen::HighlightSelected
// through `call`), the moves (x and y +30832, +30836 clamped to the page's
// limits at 0x8200C840; the angle +30840 by 1.0 wrapped to 0.0 to 359.0;
// the scale +30844 by 0.01 clamped to 0.1 to 2.0), A or B with 164 neither
// held nor released (Pop, Go(the popped state, 18); B puts the decal back
// from +30856 between Go's halves), then the movie side. 112-byte frame;
// the movie side's r3.
uint64_t InputDecalPosition(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp, const PageFlags& flags);
uint64_t InputDecalAngle(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags);
uint64_t InputDecalScale(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags);
// sub_82200DC0 TuningScreen::MoveDecalPosition, sub_822012E8 MoveDecalAngle
// and sub_82201620 MoveDecalScale: their movie sides (their rows): on a
// move the decal's node (movie 13's (-1, 19 + layer) or (-1, 22 + layer))
// placed, turned or sized; on A or B (164 neither held nor released) B's
// the same, then 228 finished and played. 128-byte frame; the last call's
// r3, the node, or the slot's.
uint64_t MoveDecalPosition(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, bool native_slots);
uint64_t MoveDecalAngle(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots);
uint64_t MoveDecalScale(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots);

// sub_82205110 TuningScreen::InputLayerEdit: state 26's page (its row): 164
// held or released (HighlightSelected through `call`), up and down on the
// cursor +30748 (0 to 4, the buttons 5 and 6 back to 4 or 0), left and right
// between the buttons, A (the edits Go(29 to 31, 25 to 27) with the decal
// kept at +30856, the flip, the free look Go(34, 29), OK: Pop until 24,
// Go(24, 16), the edit into the layer's saved record; cancel: Pop, from 28
// back to 25 with 24 left pushed, Go(+9652, 17 or 36)) or B (the cursor to
// 6); then MoveLayerEdit. 112-byte frame; MoveLayerEdit's r3.
uint64_t InputLayerEdit(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, const PageFlags& flags);
// sub_82200760 TuningScreen::MoveLayerEdit: state 26's movie side (its row):
// movie 13's rows (130 to 134), buttons (136, 137), arrows (102, 103) and
// the list's scroll node (Child(FindNode(11, 1), 0)'s +16) by the moves, A
// and B asked again; last the edited decal's node +132 the flip. 128-byte
// frame; the node, or the last call's r3.
uint64_t MoveLayerEdit(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                       uint32_t sp, bool native_slots);

// sub_821FFB20 TuningScreen::MoveVinylLayers: state 24's movie side (its
// row): on a move with +9708 set, a layer's row (195 + c, or 169 + c past
// page 0) restarted and looped with 44 to 46 finished and 46 played, or a
// button's (44, 45) with the layers' rows and 213 (189) finished; on A over
// a layer the rows finished and the picker's row (198 + c or 172 + c)
// played; else B asked, its answer's low byte stored into the frame's +80.
// 128-byte frame; the last call's r3, else the movie's entry or the slot's.
uint64_t MoveVinylLayers(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, bool native_slots);
// sub_82208CE0 TuningScreen::InputVinylLayers: state 24's page (its row):
// with the load wait [0x826D84B8] 0, up, down, left and right on the cursor
// +30732 (the layers 0 to 2, the buttons 3 and 4), A (an empty layer: the
// wait started, the picker's row played, the page's six picker textures put
// back to their placeholder; a layer with an image: Push(24), Go(25, 17);
// the buttons Go(35, 30) or Go(36, 30)) or B (the cursor to 4), then
// MoveVinylLayers unless the wait started; with the wait set, its count: on
// 3 the picker's window freed (FreeTexture) and loaded (LoadWindow) from
// +30764, past 3 once nothing loads the window's middle texture bound to
// the layer's movie texture and Push(24), Go(28, 22), then MoveVinylLayers.
// 176-byte frame; MoveVinylLayers', LoadWindow's or the last call's r3.
// Texture::Free as GuestTextureFree's with the native heap and car models'
// flag.
uint64_t InputVinylLayers(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags);
// sub_82202530 TuningScreen::MoveVinylImages: state 28's movie side (its
// row): nothing while 164 is held or released or on a move left or right;
// else on B the window's arrows (167 and 166, or 162 and 161 on page 1)
// finished and the layer's row (234 an empty layer, else 224) restarted.
// 128-byte frame; the last call's r3, else the movie's entry or the slot's.
uint64_t MoveVinylImages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, bool native_slots);
// sub_822058F8 TuningScreen::InputVinylImages: state 28's page (its row):
// with its wait words [0x826D84B4], [0x826D84B0] and [0x826D84A8] 0, left
// and right on the picked image +30764 (wrapped by +30796; the window
// scrolled by DecalImages::Scroll, the scroll wait started), the group (page
// 0's window swapped with the kept textures; page 1's group +30768 stepped
// and its window loaded by LoadWindow), A (the edited decal from the layer's
// saved record or reset, the layer's entries freed and reloaded with the
// pick, the A wait started) or B (Pop, the layer's texture bound again,
// command 16 or 17); then each wait set: A's once nothing loads (to state
// +9652's page, the saved id the pick, the window's middle entries swapped
// into the layer's through the scratch entry at images + 7200), the
// scroll's (command 23 the way it went), the group's (the kept textures
// freed and kept again, or page 1's windows swapped and freed); the layer's
// node shown or hidden by [0x82636358]; MoveVinylImages with no wait set.
// 240-byte frame; the last call's r3, else the movie's entry or the slot's.
// Texture::Free as GuestTextureFree's with the native heap and car models'
// flag; the sticker editor's SetGroupImage and SetImageGroup by their plain
// C++.
uint64_t InputVinylImages(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags);

// sub_821FE370 TuningScreen::BeginLayerOrder: r3 = the screen (its row).
// Only on the vinyls (+9656 5, byte +9660 6): the work list's ids +31216 +
// 4k = the page's saved ids (+31192 + 12 x +30724 + 4k, the page read again
// each pass) and its indices +31228 + 4k = k, k 0 to 2. Leaf, no frame; r3
// left the screen.
void BeginLayerOrder(uint8_t* base, uint64_t screen);
// sub_82200230 TuningScreen::MoveLayerActions: state 25's movie side (its
// row): on up or down with +9708 set, a wrap (+30744) placing the cursor's
// bar (FindNode(10, 0)'s +16 = +30740 x 40.0) with both arrows finished,
// else the arrow restarted; the row left finished and the cursor's row
// (49 + +30740) restarted and looped; on A the rows and arrows finished, on
// B the same and 222 restarted. 128-byte frame; the last call's r3, else
// the movie's entry or the slot's.
uint64_t MoveLayerActions(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, bool native_slots);
// sub_82209738 TuningScreen::InputLayerActions: state 25's page (its row):
// 164 held or released HighlightSelected; with the load wait [0x826D84BC]
// 0, up and down on the cursor +30740 (0 to 3, wrapping with +30744 = 1), A
// (0 the edit: the layer's saved record into the edited decal, Push(25),
// Go(26, 18); 1 the order: Push(25), Go(27, 19), BeginLayerOrder; 2
// another image: the wait started from the layer's saved id, the sticker
// editor's SetImageGroup by its plain C++, the page's six picker textures
// put back; 3 remove: Push(25), Go(37, 30)) or B (Pop, Go(the popped
// state, 16)), then MoveLayerActions unless the wait started; with the
// wait set, its count, used only with +30740 2: on 3 the picker's window
// freed (FreeTexture) and loaded (LoadWindow) from +30764, past 3 once
// nothing loads the window's middle texture bound to the layer's movie
// texture and Push(25), Go(28, 22), then MoveLayerActions (also when the
// count comes to 0). 176-byte frame; MoveLayerActions', LoadWindow's or
// the last call's r3. Texture::Free as GuestTextureFree's with the native
// heap and car models' flag.
uint64_t InputLayerActions(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags);

// sub_821FD5C0 TuningScreen::SetGroupImage: +30764 = group `group`'s first
// image (0, 43, 78, 106, 114, 142, 156, 165, 175, 180, 187, 192 for 0 to 11;
// above 11 unsigned nothing; case 0 the piece sub_821FD5F0, merged). Leaf; r3
// left the screen.
void SetGroupImage(uint8_t* base, uint64_t screen, uint64_t group);
// sub_821FD658 TuningScreen::SetImageGroup: +30768 = image `image`'s group
// by the same bounds (signed; 11 from 192 up). Leaf; r3 left the screen.
void SetImageGroup(uint8_t* base, uint64_t screen, uint64_t image);
// sub_821FD518 TuningScreen::SwapLayerRecords: the page's (+30724 0: +31024,
// 1: +31096) 24-byte layer records `first` and `second` exchanged through
// frame + 80 (memcpy three times; its plain C++ where Buffers_Memcpy takes
// it); other pages nothing. 144-byte frame; the last memcpy's r3, else the
// screen.
uint64_t SwapLayerRecords(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                          uint64_t first, uint64_t second, uint32_t sp);
// sub_821FE3C8 TuningScreen::EndLayerOrder: only on the vinyls (+9656 5,
// byte +9660 6): the page's saved ids (+31192 + 12 x +30724 + 4k, the page
// read again each pass) = the work list's ids +31216 + 4k, its indices
// +31228 + 4k = k, k 0 to 2. Leaf; r3 left the screen.
void EndLayerOrder(uint8_t* base, uint64_t screen);
// sub_82207F38 TuningScreen::SwapOrderTextures: on page 0 or 1 the two
// entries a move swapped exchange their textures (+30760 1: 7 + p and 6 + p,
// 2: one lower; page 1 21 higher), then movie 13's slots +764 to +772 (page
// 1 +776 to +784) = entries 6 to 8's (27 to 29's) headers, the movie looked
// up again each time. 128-byte frame; the last lookup's entry, else the
// screen.
uint64_t SwapOrderTextures(uint8_t* base, uint64_t screen, uint32_t sp);
// sub_82203A30 TuningScreen::ShowOrderNodes: on page 0 or 1 movie 13's nodes
// 19 to 21 (22 to 24) hidden where the work list's id is -1, else shown
// (UiMovie::HideNode and ShowNode inline: FindNode, +80). 128-byte frame;
// the last node, else the movie's entry.
uint64_t ShowOrderNodes(uint8_t* base, uint64_t screen, uint32_t sp);
// sub_82201DB8 TuningScreen::MoveLayerOrder: state 27's movie side: nothing
// on 164 held or released, a move or A; on B 238 restarted. 112-byte frame;
// PlayNamed's r3, else the last slot read's or the movie's entry.
uint64_t MoveLayerOrder(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                        uint32_t sp, bool native_slots);
// sub_8220A1C8 TuningScreen::InputLayerOrder: state 27's page (its row): with
// the count [0x826D84C0] 0, up or down moving the layer through the work
// list (SwapLayerRecords, SwapOrderTextures, command 20), A starting the
// count with the layer's animations, B Pop and command 21 with 17 next; the
// count's frame 10 placing the layers' nodes, textures and slots in the new
// order, 13 popping down to state 24 with command 16 and EndLayerOrder; then
// ShowOrderNodes, and MoveLayerOrder with the count 0. 176-byte frame;
// MoveLayerOrder's r3, else ShowOrderNodes'.
uint64_t InputLayerOrder(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                         uint32_t sp, const PageFlags& flags);

// sub_821E6950 TuningScreen::HandleInput (handle_input.cpp; its state 0 the
// split piece sub_821E69B8, merged: +9652 = 3, +9656 = 0): r3 = the screen
// (all 64 bits), r4 passed to Leave (state 59). DecalImages::Update on the
// screen's +10048 (its plain C++ with the native heap, else the guest's);
// then, with +9728 set, the page of the state [[+10036]+0] (above 62
// unsigned, -1 among them, none): the native pages by their plain C++ with
// `flags`, the pages never seen in play (7, 18 to 22, 38 to 46, 49, 53 to
// 58) through the guest, the empty sub_82106540 (35 to 37, 50 to 52) left
// out. 112-byte frame (its saves not written); r3 the page's, the screen for
// the empty ones, else DecalImages::Update's.
uint64_t HandleInput(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                     uint64_t r4, uint32_t sp, const PageFlags& flags);

// --- The sticker editor (sticker_pages.cpp; docs/functions/sticker-editor.md) ---
// Its pages (states 38 to 46 and 49), their movie sides and helpers, none
// seen in play. Movie 14's id at frame + 80 (UiMovies::Find), the nodes
// through the UI movies' plain C++ (FindNode, ShowNode's and HideNode's
// stores); guest calls through `call` from the originals' frames with their
// return addresses. Sticker ids index the table T at 0x8200B630 (61 words)
// below 61, negative ones too, as the originals do.

// sub_821F7298 TuningScreen::UpdateSoloHold: with 164 held on the player's
// mapper (its entry's +4 not 1), +33500 0 -> 1, else 1 -> 0; 1 when it
// changed, else 0. 96-byte frame. sub_821F7348 UpdatePreviewHold: the same
// for 163 and +33504.
uint32_t UpdateSoloHold(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                        uint32_t sp, bool native_slots);
uint32_t UpdatePreviewHold(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                           uint32_t sp, bool native_slots);
// sub_821F73F8 TuningScreen::StepUnlockedSticker: the sticker `n` owned
// stickers away from `id` (all 64 bits each; the save data's bits), mod 199
// (signed). Leaf. Loops forever when none is owned (as the original).
uint64_t StepUnlockedSticker(uint8_t* base, uint32_t screen, uint64_t id, uint64_t n);
// sub_821F8070 TuningScreen::ShowSlotNode: slot j's nodes (-1, 2j + 19 and
// 2j + 20) shown or hidden by the sticker `id`'s T word, both hidden while
// solo for a slot other than `current` (below 5). 128-byte frame; r3 the
// last node.
uint32_t ShowSlotNode(uint8_t* base, uint32_t screen, uint32_t id, uint32_t j, uint32_t current,
                      uint32_t sp);
// sub_821F8160 TuningScreen::PlaceStickerNode: slot j's node for the sticker
// `id` placed from the 24-byte record `record` and the garage car's layout
// (its row). 176-byte frame (the conversions' doublewords at frame + 80 to
// + 103 and the layout at + 112 as the original leaves them); r3 the node.
// Needs flush-to-zero off.
uint32_t PlaceStickerNode(uint8_t* base, uint32_t screen, uint32_t id, uint32_t j,
                          uint32_t record, uint32_t sp);
// sub_821F8868 DecalImages::SetSlot: movie 14's nodes (sub, first) and (sub,
// second) shown and hidden by the entry i and `flag`, and the texture slot's
// word the entry's texture header (or entry 199's for i negative; its row).
// 176-byte frame; r3 the last node.
uint32_t SetSlot(uint8_t* base, uint32_t images, uint32_t sub, uint32_t first, uint32_t second,
                 uint32_t first_slot, uint32_t second_slot, uint32_t i, uint32_t flag, uint32_t sp);
// sub_821F7F20 TuningScreen::SetStickerEditTexts: movie 14's edit texts
// (its row; the varargs SetText leaves unset passed as 0, the scale's double
// in r7). 112-byte frame (the angle's fctiwz at frame + 80); r3 the last
// SetText's. Needs flush-to-zero off.
uint64_t SetStickerEditTexts(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t sp);
// sub_821FA408 TuningScreen::ShowSlotImage: SetSlot(screen + 10048, -1,
// 2j + 19, 2j + 20, 2j + 190, 2j + 191, id, T[id] 1 ? 0 : 1), a tail call
// from the caller's `sp`; its r3. sub_821FA460 ShowStickerListImage: the
// same in sub-scene 30 for the list's index n (its row).
uint32_t ShowSlotImage(uint8_t* base, uint32_t screen, uint32_t id, uint32_t j, uint32_t sp);
uint32_t ShowStickerListImage(uint8_t* base, uint32_t screen, uint32_t id, uint32_t n,
                              uint32_t sp);
// The movie sides (their rows), each `show` 0 or not; the named animations
// through `call` (their overrides) with the originals' return addresses,
// the varargs SetText calls leave unset passed as 0. Their r3 the last
// call's. Need flush-to-zero off (PlaceStickerNode under some).
// sub_821FA508 AnimateStickerAreas (state 38; GarageStage::Command first,
// 144-byte frame), sub_821F7518 AnimateStickerOperate (state 40, 144),
// sub_821F8A50 AnimateStickerSlots (state 39, 144), sub_821F8D68
// AnimateStickerEditMenu (state 41, 160; SetStickerEditTexts by its plain
// C++), sub_821FBB60 ShowStickerPicker (state 43, 160; its slot images and
// StepUnlockedSticker by their plain C++).
uint64_t AnimateStickerAreas(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t show, uint32_t sp);
uint64_t AnimateStickerOperate(uint8_t* base, const tuning_screen::GuestCall& call,
                               uint32_t screen, uint32_t show, uint32_t sp);
uint64_t AnimateStickerSlots(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                             uint32_t show, uint32_t sp);
uint64_t AnimateStickerEditMenu(uint8_t* base, const tuning_screen::GuestCall& call,
                                uint32_t screen, uint32_t show, uint32_t sp);
uint64_t ShowStickerPicker(uint8_t* base, const tuning_screen::GuestCall& call, uint32_t screen,
                           uint32_t show, uint32_t sp);
// sub_821FBA40 ShowStickerOrder: state 42's movie side (its row), no guest
// calls. 144-byte frame; r3 the movie's entry with `show` 0, else the last
// ShowSlotNode's node.
uint32_t ShowStickerOrder(uint8_t* base, uint32_t screen, uint32_t show, uint32_t sp);

// --- The update's sticker commands (sticker_commands.cpp) ---
// TuningScreen::Update's command runners 32 to 41 and 43 to 51 (their rows
// in docs/functions/tuning-screen.md), none seen in play: r3 the screen (all
// 64 bits), each from the update's frame `sp`, the UI movie calls through
// `call` with the originals' return addresses, the helpers above by their
// plain C++. Each returns r3 as the original leaves it.
// sub_821FC7B8 Command32 (144-byte frame), sub_821FCA80 Command33 (160),
// sub_821FA658 Command34 (160), sub_821FA9C0 Command35 (96), sub_821FAA28
// Command36 (128), sub_821F8C30 Command37 (144), sub_821FAD00 Command38
// (144), sub_821FB0B8 Command39 (144), sub_821FCC48 Command40 (144),
// sub_821FCED0 Command41 (160), sub_821FD128 Command43 (128), sub_821FD3F8
// Command44 (144), sub_821FB1F0 Command45 (160), sub_821F8FC0 Command46
// (commands 46, 48 and 50; 96), sub_821FB450 Command47 (160), sub_821FB6B0
// Command49 (128), sub_821FB8D8 Command51 (128). Need flush-to-zero off
// (PlaceStickerNode under most).
bool IsStickerCommand(uint32_t command);
std::optional<uint64_t> StickerCommand(uint8_t* base, const tuning_screen::GuestCall& call,
                                       uint32_t command, uint64_t r3, uint32_t sp);
uint64_t Command32(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command33(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command34(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command35(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command36(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command37(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command38(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command39(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command40(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command41(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command43(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command44(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command45(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command46(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command47(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command49(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);
uint64_t Command51(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t r3, uint32_t sp);

// The input pages (their rows), r3 = the screen (all 64 bits), each from
// HandleInput's frame: Menu::Step, Menu::Pressed, MenuSound::Play and the
// stand's commands through `call` with the originals' return addresses, the
// helpers above by their plain C++, memcpy's plain C++ where Buffers_Memcpy
// takes it; r3 as the originals leave it. Need flush-to-zero off
// (PlaceStickerNode under InputStickerEdit).
// sub_821FBD40 InputStickerAreas (state 38, 96-byte frame; RestoreAreaDecals
// and FadeOut by their plain C++), sub_821F9050 InputStickerSlots (39, 112),
// sub_821F9328 InputStickerOperate (40, 112), sub_821F9608 InputStickerEdit
// (41, 112), sub_821F7000 InputStickerView (49, 112; the mapper's slots by
// their plain C++ with `flags.native_slots`, as UpdateSoloHold's).
uint64_t InputStickerAreas(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags);
uint64_t InputStickerSlots(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags);
uint64_t InputStickerOperate(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                             uint32_t sp, const PageFlags& flags);
uint64_t InputStickerEdit(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags);
uint64_t InputStickerView(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags);
// sub_821FBF18 InputStickerOrder (state 42, 272-byte frame: the area's
// records and ids copied into it at +112 and +80; ShowStickerOrder by its
// plain C++), sub_821FC270 InputStickerPicker (43, 128; its group scans
// loop forever when no sticker is owned, as the original), sub_821F99A8
// InputStickerMove, sub_821F9D30 InputStickerRotate and sub_821F9FB8
// InputStickerScale (44 to 46, 112; the mapper's slots by their plain C++
// with `flags.native_slots`).
uint64_t InputStickerOrder(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags);
uint64_t InputStickerPicker(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp, const PageFlags& flags);
uint64_t InputStickerMove(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                          uint32_t sp, const PageFlags& flags);
uint64_t InputStickerRotate(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                            uint32_t sp, const PageFlags& flags);
uint64_t InputStickerScale(uint8_t* base, const tuning_screen::GuestCall& call, uint64_t screen,
                           uint32_t sp, const PageFlags& flags);

}  // namespace tuning_pages
