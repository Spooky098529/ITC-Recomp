// The type 8 mode's panel screen natively (B7 in docs/rexglue-removal.md;
// the functions are in docs/functions/type8-screens.md): the two players'
// car pick, built as the type 6 and 7 panel screens (src/type67_screens/)
// with its panels inline, five a player, one panel group (+11212) held by
// the one-group holder (+15220) and the two-list car list PanelSetPair
// (+11272, src/car_list/). Plain C++ over guest memory (the screen is
// shared with the recompiled code), with the originals' loads and stores in
// their order. Each function takes the stack pointer the original is
// entered with (`sp`); the back chain of an original's frame is written as
// it leaves it, its register saves aren't. Guest calls go through
// type67_screens::Calls (the car list's plain C++ through car_list::Calls
// made from them); the screen's, the holder's and the car list's vtable
// calls through type67_screens::Slot, which runs the plain C++ here for the
// screen's slots 2, 3, 13 and 18. Each returns r3 as the original leaves
// it. Adapters: src/hooks/type8_screens.cpp (behind --native_type8_screens),
// register steps shared with the harness in src/hooks/type8_screens.h.

#pragma once

#include "car_list/car_list.h"
#include "type67_screens/type67_screens.h"

#include <cstdint>

namespace type8_screens {

using Calls = type67_screens::Calls;

// The vtables the constructors and destructors store: the screen's, the
// panel group's, Panel8C's (one word after the group's), and the empty base
// the confirm panels' destructor leaves (ListNode's, ConfirmPanel's).
constexpr uint32_t kScreenVtable = 0x82010AF0;
constexpr uint32_t kGroupVtable = 0x82010B48;
constexpr uint32_t kPanel8CVtable = 0x82010B4C;
constexpr uint32_t kEmptyBaseVtable = 0x82010B5C;
// The vtable Panel8F and Panel8D store (its slot 0 RequestSlot's deleting
// destructor, archive.md).
constexpr uint32_t kListNodeVtable = 0x82001DD8;

// The screen's layout: the panel group, the car list, the holder, the two
// players' panels (F, E, C, D, the confirm panel), the car list's bases.
constexpr uint32_t kGroup = 11212;
constexpr uint32_t kCarList = 11272;
constexpr uint32_t kHolder = 15220;
constexpr uint32_t kPanelsF[2] = {9660, 10436};
constexpr uint32_t kPanelsE[2] = {9808, 10584};
constexpr uint32_t kPanelsC[2] = {9972, 10748};
constexpr uint32_t kPanelsD[2] = {10296, 11072};
constexpr uint32_t kConfirms[2] = {10392, 11168};
constexpr uint32_t kScreenBytes = 15252;
// The two players' garage cars PickCar fills (1,972 bytes each) and the
// player the quit dialog answers.
constexpr uint32_t kGarageCars = 0x82814EB8;
constexpr uint32_t kDialogPlayer = 0x82814E6C;
// The mode before ([0x826032E0]: 1 the race) and the screen fades' passes.
constexpr uint32_t kModeBefore = 0x826032E0;
constexpr uint32_t kFadeInPasses = 0x82001BEC;
constexpr uint32_t kFadeOutPasses = 0x82001BF0;

// The car list's plain C++ guest calls through `calls` (r3 to r10, as
// car_list::Calls sets them; f1 isn't passed).
car_list::Calls CarListCalls(const Calls& calls);

// --- The panels and the panel group (panels.cpp); r4 the player (0, or
// any other value for player 1's words), r5 the group ---

// sub_82239C10 PanelGroup8::PanelGroup8 (r4 its UI movie):
// PanelGroupBase::PanelGroupBase, +4 = r4, +12 = 10, +0 = its vtable, +32 =
// 0. r3 left.
uint64_t PanelGroup8Construct(uint8_t* base, uint64_t r3, uint32_t movie, uint32_t sp);
// sub_82239C70 Panel8F::Panel8F (148 bytes): PanelFBase::PanelFBase, then
// its words. r3 left.
uint64_t Panel8FConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp);
// sub_82239D58 Panel8E::Panel8E (164 bytes): PanelEBase::PanelEBase, then
// its words (+0 the group, no vtable). r3 left.
uint64_t Panel8EConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp);
// sub_82239E78 Panel8C::Panel8C (324 bytes): PanelCBase::PanelCBase, then
// its words. r3 left.
uint64_t Panel8CConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp);
// sub_8223A100 Panel8D::Panel8D (96 bytes): PanelDBase::PanelDBase, then
// its words. r3 left.
uint64_t Panel8DConstruct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                          uint32_t sp);
// sub_8223A208 ConfirmPanel8::ConfirmPanel8 (44 bytes):
// ConfirmPanel::ConfirmPanel, then its words. r3 left.
uint64_t ConfirmPanel8Construct(uint8_t* base, uint64_t r3, uint32_t player, uint32_t group,
                                uint32_t sp);
// sub_8223A2A8 PanelGroup8::DeletingDestructor and sub_8223A348
// Panel8C::DeletingDestructor (112-byte frames): PanelGroupBase's or
// CarPanelBase's destructor, then operator delete (a guest call) when bit 0
// of `flags` is set. r3 left.
uint64_t PanelGroup8DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                     const Calls& calls);
uint64_t Panel8CDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                 const Calls& calls);
// sub_8223A090 Panel8C::SetStats (slot 1; r4 the stats record): a tail call
// of Panel6C::SetStats.
uint64_t Panel8CSetStats(uint8_t* base, uint64_t r3, uint32_t record, uint32_t sp,
                         const car_list::Calls& calls);
// sub_8223A098 Panel8C::ShowArrows (slot 3; r4 shown): when +156 isn't
// negative, UiMovie::ShowNode or HideNode(group movie, +16, +156) (guest
// calls). Its r3, or r3 left.
uint64_t Panel8CShowArrows(uint8_t* base, uint64_t r3, uint64_t shown, uint32_t sp,
                           const car_list::Calls& calls);

// --- The screen's leaves (panel_screen.cpp) ---

// sub_82238420 Type8PanelScreen::InitWords (slot 0): both car list bases'
// +1916 = 3 and +1924 = 4. r3 left.
uint64_t InitWords(uint8_t* base, uint64_t r3);
// sub_82238418 Type8PanelScreen::IsLoaded (slot 2): UiMovies::IsLoaded(4)
// (a tail call; its plain C++).
uint64_t IsLoaded(uint8_t* base);
// sub_82238440 Type8PanelScreen::OpenMovies (slot 13) and sub_82238450
// DeleteMovies (slot 18): UiMovies::Open(4, 22586) and Delete(4), tail
// calls from `sp` returning to `lr`.
uint64_t OpenMovies(uint32_t sp, uint32_t lr, const Calls& calls);
uint64_t DeleteMovies(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_82238458 OnMessage2 (+15228 = 1), sub_82238530 OnSaveLoaded (+15232
// = 0), sub_82238540, sub_82238550 and sub_82238560 OnAnswer0 to 2 (+15244
// = 0, 1, 2), by `function`. r3 left.
uint64_t SetWord(uint8_t* base, uint64_t r3, uint32_t function);
// sub_82238B10 Type8PanelScreen::Leave (r4 the dispatcher): once both fade
// counts are 0, message 4 posted with +15240 set, else +9608 = 9 and
// message 6; then +9656 = 0. r3 left.
uint64_t Leave(uint8_t* base, uint64_t r3, uint64_t r4);

}  // namespace type8_screens
