// The career's location screen natively (docs/functions/location-screen.md):
// plain C++ over the guest layout of the screen (912 bytes at [0x82814E50]),
// its page object and menus, store for store as the recompiled functions.
// Adapters in src/hooks/location_screen.cpp (behind --native_location_screen),
// register steps shared with the harness in src/hooks/location_screen.h.

#pragma once

#include <cstdint>
#include <optional>

#include "race_event/race_event.h"
#include "strings_and_font/strings_and_font.h"

namespace location_screen {

// The screen's globals (pointers).
constexpr uint32_t kPages = 0x82814E48;    // the page object (28 bytes)
constexpr uint32_t kMenu = 0x82814E4C;     // the location's menu (68 bytes)
constexpr uint32_t kScreen = 0x82814E50;   // the screen (912 bytes)
constexpr uint32_t kListMenu = 0x82814E54;      // the location's list
constexpr uint32_t kOpponentMenu = 0x82814E58;  // the opponents
constexpr uint32_t kConfirmMenu = 0x82814E5C;   // race, the card, back
// The race data (a pointer) and its racer records' offset.
constexpr uint32_t kRaceData = 0x826C5B2C;
constexpr uint32_t kRacerRecords = 0x59AE8;
// The save data's unlock list's count.
constexpr uint32_t kUnlockCount = 0x1CA78;
// The save data's item records (12 bytes each; +2 a halfword).
constexpr uint32_t kItemRecords = 0x199F8;
// The frames of the functions that copy an item (3,008 bytes) onto their
// stack.
constexpr uint32_t kItemFrame = 3120;

// --- Small helpers (no calls) ---

// sub_8228F0A0 LocationScreen::SceneryConfig: (a + 2) x 3 + b (64-bit, as
// the original's addi, mulli and add).
uint64_t SceneryConfig(uint64_t a, uint64_t b);
// sub_82227690 LocationScreen::KindText: a string id by kind (1 to 7) from
// `index` (64-bit, as the original's addi), else 0.
uint64_t KindText(uint32_t kind, uint64_t index);
// sub_822291E8 LocationScreen::IsPictureQueued: 1 when ring A (+140, 32
// words from +272, +268 of them) holds `id`, else 0.
uint32_t IsPictureQueued(uint8_t* base, uint32_t id);
// sub_82229108 LocationScreen::PictureA and sub_82229158
// LocationScreen::PictureB: the texture record (+8) of the list's picture
// with id `id`, else the default (+544 or +548).
uint32_t PictureA(uint8_t* base, uint32_t id);
uint32_t PictureB(uint8_t* base, uint32_t id);
// sub_82227158 LocationScreen::OnMessage14: +880 and +884 by the location's
// menu's cursor.
void OnMessage14(uint8_t* base);

// --- The lists' steps ---

// sub_822281D8 LocationScreen::HasMoreUnlocks: 1 when +868 is below player
// 0's unlock count (unsigned), else 0.
uint32_t HasMoreUnlocks(uint8_t* base);
// sub_82227618 LocationScreen::NextUnlockEntry: Entry56(+876, +872)'s next
// (+8) added to +872 (1), or +872 and +876 cleared (0).
uint32_t NextUnlockEntry(uint8_t* base);
// sub_82227708 LocationScreen::OpponentHasCard: the picked opponent's item
// (RaceEvent::GroupItem of +804 and +800, +800 stored at frame + 80) copied
// to frame + 96 (memcpy); 1 when its halfword +88 is 4 or 5. `sp` the
// caller's stack pointer (the frame is kItemFrame below it). nullopt, with
// nothing done, where the copy can't be native (the original runs).
std::optional<uint32_t> OpponentHasCard(uint8_t* base, uint32_t sp);
// sub_822271F8 LocationScreen::NextResultText: racer 0's item copied to
// frame + 80; its 20-byte entry k (12, or 10 or 11 by the human racer's
// outcome and an item record) stepped at +872 as NextUnlockEntry. nullopt,
// with nothing done, where the copy can't be native.
std::optional<uint32_t> NextResultText(uint8_t* base, uint32_t sp);

// --- The movies (src/location_screen/movies.cpp) ---

// The screen's UI movies: 21 (the menus) and 30 (an opponent's card).
constexpr uint32_t kMenusMovie = 21;
constexpr uint32_t kCardMovie = 30;

// UiMovie::Run (sub_8211E1B8) through its override, with r3 = `movie`, r4 =
// `draw_only`, from a frame `frame_bytes` below the caller's r1 (the
// original's) and the return address `lr`; returns its r3.
struct RunCall {
  void* user;
  uint64_t (*run)(void* user, uint32_t movie, uint32_t draw_only, uint32_t frame_bytes,
                  uint32_t lr);
};

// Each takes `sp`, the caller's stack pointer: the movie's id is stored at
// its frame's +80 for UiMovies::Find, as the original does.
//
// sub_8222A060 LocationScreen::ShowPictures: movie 21's nodes (1, 4), (1, 5)
// textures 0, (1, 6), (1, 7) textures 1, (1, 6), (1, 7) shown, (1, 8),
// (1, 9), (1, 13), (1, 14) hidden. Returns the last node (r3).
uint32_t ShowPictures(uint8_t* base, uint32_t sp);
// sub_8222A150 LocationScreen::HidePictures: (1, 4), (1, 5) textures 0,
// (1, 6) to (1, 9), (1, 13), (1, 14) hidden. Returns the last node.
uint32_t HidePictures(uint8_t* base, uint32_t sp);
// sub_8222AC18 LocationScreen::SetCarColor: node (9, 7)'s color 2 when +892
// is 1, else 0. Returns the node.
uint32_t SetCarColor(uint8_t* base, uint32_t sp);
// sub_8222BFE8 LocationScreen::SetCursor and sub_8222C058
// LocationScreen::ListNode: node (9, 0) or (10, 0)'s +16 = `value` (its 32
// bits, unsigned) as a float; the frame's +80 left holding it as a
// doubleword (the original's conversion). Return the node.
uint32_t SetCursor(uint8_t* base, uint32_t sp, uint32_t value);
uint32_t ListNode(uint8_t* base, uint32_t sp, uint32_t value);
// sub_8222C0C8 LocationScreen::ShowArrows: nodes (-1, 17) and (-1, 18) by
// the picked opponent (+800) against the count (+808). Returns the last
// node.
uint32_t ShowArrows(uint8_t* base, uint32_t sp);
// sub_822291A8 LocationScreen::RunCardMovie: movie 30 run (r4 = 0); its r3
// (all 64 bits).
uint64_t RunCardMovie(uint8_t* base, uint32_t sp, const RunCall& run);
// sub_8222AD50 LocationScreen::Transition1: on the first step (+0 of
// `step` 0) ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0) and the step 1;
// movie 21 run; 1 when either fade's counts word (0x826A50DC, 0x826A50E4)
// was set before the run, else 0.
uint32_t Transition1(uint8_t* base, uint32_t sp, uint32_t step, const RunCall& run);
// sub_8222BE60 LocationScreen::Transition18: on the first step movie 30's
// named animation 53 played (PlayNamed) and the step 1; both movies run;
// when movie 30's run returns 0, the confirm menu's +24 = 0 and 1, else 0.
uint32_t Transition18(uint8_t* base, uint32_t sp, uint32_t step, const RunCall& run);

// --- The screen's state (src/location_screen/pages.cpp) ---

// sub_822895E0 ScreenMachine::Reenter: the machine's +9604 (its state) 4
// when +9608 (the movies loaded) is set, else 1.
void Reenter(uint8_t* base, uint32_t machine);
// sub_822279A8 LocationScreen::Reset, with sub_82227818
// LocationScreen::ResetView on the screen's +560: the screen's fields
// cleared or set as the original stores them; the view's vectors from the
// floats at 0x82000A34 and 0x82000A30 (through a float register, as lfs and
// stfs leave them) and its two identity matrices (+112, +176; memcpy's in
// the original, from an identity built in its frame, not written here).
// False, with nothing done, where a matrix's copy would land in the
// physical views (memcpy's override records those): the original runs.
bool ResetNative(uint32_t screen);
void Reset(uint8_t* base, uint32_t screen);
void ResetView(uint8_t* base, uint32_t view);
// sub_8222C9B0 LocationScreen::ApplyQueuedPage: HidePictures (from its
// 96-byte frame) when +880 is 0; the page object set from +880 and +884
// (a transition when +884 isn't -1), both then -1. Returns HidePictures'
// r3 when it ran, else nullopt (r3 left as it came).
std::optional<uint32_t> ApplyQueuedPage(uint8_t* base, uint32_t sp);

// A page's input handler (`handler`, one of the table's 16) called with r3
// = `dispatcher`, r4 = `machine`, from a frame `frame_bytes` below the
// caller's r1 with the return address `lr`.
struct PageCall {
  void* user;
  void (*call)(void* user, uint32_t handler, uint32_t dispatcher, uint32_t machine,
               uint32_t frame_bytes, uint32_t lr);
};
// sub_8222EC08 LocationScreen::UpdatePage (r3 = the machine, r4 = the
// dispatcher): the 16 pages' handlers stored in its 240-byte frame (+80
// on), and unless the page is -1 the page's (read from there) called;
// then [0x8265ECE0] = the screen's +592 (16 bytes), and when +900 is set
// it's cleared and the page object restarted. r3 is the handler's (or as
// it came).
void UpdatePage(uint8_t* base, uint32_t sp, uint32_t machine, uint32_t dispatcher,
                const PageCall& call);

// sub_8222F258 LocationScreen::InitStates (src/location_screen/init_states.cpp):
// the 19 transitions added to `machine` (StateMachine::Add) from functors
// built in its 1,360-byte frame (`frame`, the original's stores in order),
// then the machine's +9604 = 0. Returns the last Add's r3 (an index when
// `replaced`, else the new transition).
constexpr uint32_t kInitStatesFrame = 1360;
uint32_t InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

// --- The guest calls of the second migration's functions ---

// A guest function called as the original calls it: r3 to r6 = `args`, the
// stack pointer `r1` (the caller's frame) and the return address `lr`;
// returns its r3 (64 bits). The adapter's job (hooks::LocationScreenCalls):
// the heap's, memcpy's and the archive's adapters, the UI movies' stops and
// runs (UiMovie::Finish, FinishNamed, FinishAll, Run), and the career's and
// the race set-up's register steps (Career::EventQuery, Race::SetUp), which
// run their plain C++. `sp` is the adapter's stack pointer: the race event's
// tests (`event`) count their depth from it. `format` is Strings::FormatV's
// calls, for UiMovie::SetText.
struct Calls {
  void* user;
  uint64_t (*call)(void* user, uint32_t function, const uint64_t args[4], uint32_t r1,
                   uint32_t lr);
  uint32_t sp;
  const race_event::Guest* event;
  const strings_and_font::FormatCalls* format;

  uint64_t Call(uint32_t function, uint32_t r1, uint32_t lr, uint64_t r3, uint64_t r4 = 0,
                uint64_t r5 = 0, uint64_t r6 = 0) const {
    const uint64_t args[4] = {r3, r4, r5, r6};
    return call(user, function, args, r1, lr);
  }
  // The race event's Guest for a call from the frame at `frame`.
  race_event::Guest EventFrom(uint32_t frame) const { return event->Down(sp - frame); }
};

// The guest functions called.
constexpr uint32_t kEventQuery = 0x82111638;   // Career::EventQuery
constexpr uint32_t kRaceSetUp = 0x82115D98;    // Race::SetUp
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kOperatorDelete = 0x825A70E8;
constexpr uint32_t kXprRelease = 0x82108860;   // XPR::Release of a slot
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kArchiveLoad = 0x8209DE48;

// memcpy natively (c_runtime::Memcpy), or through `calls` from the frame at
// `r1` with the return address `lr` where that refuses. Returns the
// destination (memcpy's r3).
uint32_t Copy(uint8_t* base, const Calls& calls, uint32_t r1, uint32_t lr, uint32_t destination,
              uint32_t source, uint32_t size);

// sub_822280D8 LocationScreen::Page9 (r4 = the machine): Career::EventQuery
// (save, 0), Race::SetUp(save, -1), the machine's +9620 = 11 and +9604 = 10.
// Returns Race::SetUp's r3. sub_82228138 LocationScreen::Page11: the picked
// opponent's item (RaceEvent::GroupItem of +804 and +800, +800 at frame +
// 80) copied to frame + 96, Career::EventQuery(save, 1), Race::SetUp(save,
// the item's id), the race data's +0x59AE4 |= 0x10, then as Page9. `sp` the
// caller's stack pointer.
uint64_t Page9(uint8_t* base, uint32_t sp, uint32_t machine, const Calls& calls);
uint64_t Page11(uint8_t* base, uint32_t sp, uint32_t machine, const Calls& calls);

// sub_82227430 LocationScreen::NextOpponentText: the picked opponent's item
// copied to its frame's +96 (6,176 bytes); Items::Available of its id, and
// by the save data's item records the entry kind (6 to 9), or with an item
// of kind 3 the first owned item its +92 names (copied to +3104, followed
// while their records' +2 is set); the 20-byte entry stepped at +872 as
// NextUnlockEntry. Returns 1 while there's more, else 0.
constexpr uint32_t kNextOpponentTextFrame = 6176;
uint32_t NextOpponentText(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The pictures (src/location_screen/pictures.cpp) ---

// sub_8222C270 LocationScreen::LoadPicture (r3 = the screen, r4 = an asset
// id, r5 = the list, 0 or 1): the asset's size (Archive::AssetSize) of pages
// (VirtualAlloc), cleared (XMemSet), and for list 0 or 1 Archive::Load(id,
// them, OnPictureLoadedA or B, the screen). Returns Archive::Load's r3, or
// the pages for another list.
uint64_t LoadPicture(uint8_t* base, uint32_t sp, uint32_t screen, uint32_t id, uint32_t list,
                     const Calls& calls);
// sub_82228438 Picture::Free (r3 = a picture): +0 = 0; its texture record
// (+8) cleared and deleted (operator delete), its XPR slot (+12) released
// (XPR::Release) when set; +8, +12 = 0. Returns the r3 the last call
// leaves (0 with neither).
uint64_t PictureFree(uint8_t* base, uint32_t sp, uint32_t picture, const Calls& calls);
// sub_82228498 LocationScreen::FreePictures (r3 = the screen): list A's,
// then B's pictures, last first (the count down, then the picture), freed
// (Picture::Free, VirtualFree(MEM_RELEASE)). Returns the last VirtualFree's
// r3, or the screen.
uint64_t FreePictures(uint8_t* base, uint32_t sp, uint32_t screen, const Calls& calls);

}  // namespace location_screen
