// The type 6 and 7 modes' list, panel (car pick) and node screens natively
// (B7 in docs/rexglue-removal.md; the functions are in
// docs/functions/type67-screens.md): the handlers their state machine Inits
// register, the screens' vtable slots, the panel screens' group pair and
// its panel groups. Plain C++ over guest memory (the screens are shared
// with the recompiled code), with the originals' loads and stores in their
// order. Each function takes the stack pointer the original is entered
// with (`sp`); the back chain of an original's frame is written as it
// leaves it, its register saves aren't.
//
// The screens' vtable calls (the screens' own slots, the car list's at +9644,
// the group pair's at +9648) go through Slot: the plain C++ here where the
// slot holds one of these functions, nothing for the empty handler
// sub_82106540 (its override does nothing), else a guest call from the
// original's frame with its return address (the car list, PanelSet, stays
// recompiled: a lead). A call the original makes without setting r4 passes
// the r4 the last guest call left (Calls::call_set with only r3 set).
// Adapters: src/hooks/type67_screens.cpp (behind --native_type67_screens),
// register steps shared with the harness in src/hooks/type67_screens.h.

#pragma once

#include "type67_modes/type67_modes.h"

#include <cstdint>
#include <functional>
#include <optional>

namespace type67_screens {

// The guest calls (type67_modes::Calls: call, open, remove, call_set),
// whether MenuList::Release's plain C++ may free (--native_heap), and r4 set
// for the guest calls after a slot the plain C++ ran with an r4 of its own
// (the original's r4 left as the call set it).
struct Calls : type67_modes::Calls {
  bool native_heap = false;
  std::function<void(uint64_t r4)> set_r4;
};

// The guest functions called: UiMovie::ResetPose, FinishAll and Run, and
// MenuList::Release where its plain C++ declines (their overrides); the
// empty handler; Type7PanelScreen::AskQuit (never seen in play: the
// original).
constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kMenuListRelease = 0x8220BBB0;
constexpr uint32_t kEmpty = 0x82106540;
constexpr uint32_t kAskQuit = 0x82240518;

// The screen fades' counts (GroupPair::WaitForFade and Type7PanelScreen::Draw
// wait on both being 0) and the car list's step wait word.
constexpr uint32_t kFadeCountA = 0x826A50E4;
constexpr uint32_t kFadeCountB = 0x826A50DC;
constexpr uint32_t kFadeWait = 0x82815EB0;
// The type 7 mode's car (Type7PanelScreen's slots 6 to 8 read its +1972 to
// +1980; PickCar fills it).
constexpr uint32_t kType7Car = 0x826371D0;

// A message (`message`, 0, 0) queued in the dispatcher's ring (+204), as
// the handlers do it inline.
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message);

// A vtable slot (byte offset `slot`) of the object in `object_r3` (r3, both
// halves) called from the frame at `frame` returning to `lr`; `r4` unset
// for a call the original makes without setting r4. Its r3.
uint64_t Slot(uint8_t* base, uint64_t object_r3, uint32_t slot, std::optional<uint64_t> r4,
              uint32_t frame, uint32_t lr, const Calls& calls);

// --- The list screens (list_screens.cpp; state +9608, loaded +9612, open
// +9616) ---

// sub_82251618 ListScreen::OnEnter: +9612 = 0, +9688 = 0, slot 8 (+32),
// then +9608 = 1 when +9616 is set, else 9. Slot 8's r3.
uint64_t ListScreenOnEnter(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls);
// sub_82251508 ListScreen::OpenMovie: +9612 = 0, a tail call of slot 12
// (+48) with r4 (from `sp`, returning to `lr`).
uint64_t ListScreenOpenMovie(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                             const Calls& calls);
// sub_82251678 ListScreen::WaitForMovie: while +9612 is 0, +9612 = slot 1
// (+4); once set, +9692 = 1 and +9608 = 9 when +9616 is set, else 4; then
// slot 13 (+52) with r4. Slot 13's r3.
uint64_t ListScreenWaitForMovie(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls);
// sub_82251760 ListScreen::Leave: slot 18 (+72), then +9608 = 7.
uint64_t ListScreenLeave(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls);
// sub_82251710 ListScreen::OnLeft: slot 9 (+36), then +9608 = 11 when
// +9688 is 1, else 6.
uint64_t ListScreenOnLeft(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls);
// sub_82242F70 ListScreen::IsLoaded: 1 when UiMovies::IsLoaded([+9680]),
// else 0.
uint64_t ListScreenIsLoaded(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82233320 Type6ListScreen::OpenMovie (asset 22565) and sub_82242F10
// Type7ListScreen::OpenMovie (22570): UiMovies::Open([+9680], asset), a
// tail call from `sp` returning to `lr`.
uint64_t ListScreenOpenAsset(uint8_t* base, uint64_t r3, bool type7, uint32_t sp, uint32_t lr,
                             const Calls& calls);
// sub_82233330 Type6ListScreen::Release (MenuList::Release on +10216,
// +10288, +10144) and sub_82242F20 Type7ListScreen::Release (+10144,
// +10288, +10360, +10216): UiMovies::Delete([+9680]) first. The last
// Release's r3.
uint64_t ListScreenRelease(uint8_t* base, uint64_t r3, bool type7, uint32_t sp,
                           const Calls& calls);

// --- The node screen (node_screens.cpp; state +10064, loaded +10068, open
// +10072) ---

// sub_822591B0 NodeScreen::OnEnter: +10068 = 0, slot 9 (+36), +10064 = 1
// when +10072 is set, else 9.
uint64_t NodeScreenOnEnter(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls);
// sub_822590D0 NodeScreen::OpenMovie: +10068 = 0, a tail call of slot 13
// (+52).
uint64_t NodeScreenOpenMovie(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                             const Calls& calls);
// sub_82259208 NodeScreen::WaitForMovie: while +10068 is 0, +10068 = slot 3
// (+12); once set, +10064 = 9 when +10072 is set, else 4; then slot 14
// (+56) with r4.
uint64_t NodeScreenWaitForMovie(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls);
// sub_822592D8 NodeScreen::Leave: slot 19 (+76), +10064 = 7.
uint64_t NodeScreenLeave(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls);
// sub_82259298 NodeScreen::OnLeft: slot 10 (+40), +10064 = 6.
uint64_t NodeScreenOnLeft(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls);
// sub_82236FD8 Type6NodeScreen::IsLoaded: UiMovies::IsLoaded([+10100]) as
// 0 or 1.
uint64_t NodeScreenIsLoaded(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82236EB0 Type6NodeScreen::OpenMovie: UiMovies::Open([+10100],
// 22566), a tail call.
uint64_t NodeScreenOpenAsset(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                             const Calls& calls);
// sub_82236EC0 Type6NodeScreen::Release: UiMovies::Delete([+10100]), then
// MenuList::Release on +10132 and +10204.
uint64_t NodeScreenRelease(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// --- The panel groups and the group pair (panel_parts.cpp; a group's +4
// its UI movie, the pair's +4 the current group, +8 group A, +12 group B) ---

// sub_82258FC8 PanelGroup::Movie: the movie of the group's id
// (UiMovies::Find on 0x826C5CCC, the id at its frame's +80).
uint64_t PanelGroupMovie(uint8_t* base, uint64_t group, uint32_t sp);
// sub_82259008 PanelGroup::Run: UiMovie::Run(the group's movie, 0).
uint64_t PanelGroupRun(uint8_t* base, uint64_t group, uint32_t sp, const Calls& calls);
// sub_82249C30 GroupPair::SelectA: +4 = +8. r3 left.
uint64_t GroupPairSelectA(uint8_t* base, uint64_t r3);
// sub_82249C40 GroupPair::Reset: +4 = +8, UiMovie::ResetPose on group A's
// movie.
uint64_t GroupPairReset(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82249BE0 GroupPair::Run: PanelGroup::Run on +4 (a tail call).
uint64_t GroupPairRun(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82249BA8 GroupPair::WaitForFade: [0x82815EB0] = 0 when it's set and
// both fade counts are 0. r3 left.
uint64_t GroupPairWaitForFade(uint8_t* base, uint64_t r3);
// sub_82249C70 GroupPair::FinishAll: UiMovie::FinishAll on group A's, then
// group B's movie.
uint64_t GroupPairFinishAll(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// --- The panel screens' slots (panel_parts.cpp) ---

// sub_82230790 PanelScreen::IsLoaded: bit 0 of UiMovies::IsLoaded(4) AND
// IsLoaded(28).
uint64_t PanelScreenIsLoaded(uint8_t* base, uint32_t sp);
// sub_82230758 Type6PanelScreen::OpenMovies (assets 22567, 22568) and
// sub_822401D0 Type7PanelScreen::OpenMovies (22571, 22572): UiMovies::Open
// of movies 4 and 28.
uint64_t PanelScreenOpenMovies(uint8_t* base, uint32_t sp, bool type7, const Calls& calls);
// sub_82240208 PanelScreen::DeleteMovies: UiMovies::Delete of 4 and 28.
uint64_t PanelScreenDeleteMovies(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82240860 Type7PanelScreen::Back (+9608 = 9, message 5),
// sub_82247058 Type6PanelScreen::OpenPowerGraph (message 21) and
// sub_822408B8 Type7PanelScreen::OpenPowerGraph (18), by `function`: the
// message queued in r4's ring. r3 left.
uint64_t PanelScreenPost(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t function);
// sub_82240238 Type7PanelScreen::CarList, sub_82240248 CarEntry,
// sub_82240258 CarSlot: the type 7 car's +1972, +1976 or +1980 (`offset`).
uint64_t Type7CarWord(uint8_t* base, uint32_t offset);

// --- The panel screens' handlers (panel_screens.cpp; state +9608, loaded
// +9612, open +9616, the car list +9644, the group pair +9648) ---

// sub_82249D18 PanelScreen::OnEnter: +9612 = 0, the car list's slot 2
// (+8), the pair's slot 3 (+12), +9608 = 1 when +9616 is set, else 9.
uint64_t PanelScreenOnEnter(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls);
// sub_82248CD0 PanelScreen::OpenMovie: a tail call of slot 13 (+52).
uint64_t PanelScreenOpenMovie(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                              const Calls& calls);
// sub_82249D88 PanelScreen::WaitForMovie: +9612 = slot 2 (+8); when set,
// +9608 = 3 when +9652 is set, else 9 when +9616 is set, else 4.
uint64_t PanelScreenWaitForMovie(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls);
// sub_82249DF8 PanelScreen::Open: with +9612 set, +9608 = 4 and a tail call
// of the car list's slot 13 (+52) with r4 = 1; else +9608 = 1.
uint64_t PanelScreenOpen(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                         const Calls& calls);
// sub_82248CE0 PanelScreen::Show: unless +9652 is set, the car list's
// slots 15 to 17 (+60 to +68) with the screen's slots 6 to 8 (+24 to +32);
// then the car list's slots 3 (+12) and 1 (+4), the pair's slot 5 (+20).
uint64_t PanelScreenShow(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls);
// sub_82248C48 PanelScreen::Update: the pair's slot 2 (+8), the car list's
// slot 6 (+24), the pair's slot 1 (+4), each when its pointer isn't 0.
uint64_t PanelScreenUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls);
// sub_82248DF8 PanelScreen::OnMessage24: a tail call of the car list's
// slot 11 (+44) with r4 = 0.
uint64_t PanelScreenOnMessage24(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                                const Calls& calls);
// sub_82249E38 PanelScreen::Leave: the car list's and the pair's slot 7
// (+28), the screen's slot 18 (+72), +9608 = 7.
uint64_t PanelScreenLeave(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls);
// sub_82248B08 PanelScreen::OnLeft: the car list's and the pair's slot 4
// (+16).
uint64_t PanelScreenOnLeft(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls);
// sub_82248B58 PanelScreen::DrawList: with the car list, its slot 5 (+20),
// then the screen's slot 3, 4 or 5 (+12 to +20) for the first of its slots
// 8, 9, 10 (+32 to +40) answering nonzero; then the screen's slot 10 (+40).
uint64_t PanelScreenDrawList(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls);
// sub_82240908 Type7PanelScreen::DrawList: PanelScreen::DrawList, then
// Type7PanelScreen::AskQuit (sub_82240518, never seen in play: the
// original) when +11700 is set.
uint64_t Type7PanelScreenDrawList(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                  const Calls& calls);
// sub_82240958 Type7PanelScreen::Draw: DrawList (a tail call) while the
// quit dialog +9656 is 0; at 2, +9608 = 10 once both fade counts are 0.
uint64_t Type7PanelScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls);

}  // namespace type67_screens
