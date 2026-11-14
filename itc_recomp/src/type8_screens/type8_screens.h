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

// --- The screen's handlers (panel_screen.cpp; r4 the dispatcher where the
// original reads it) ---

// sub_82238570 Type8PanelScreen::ShowPlayerNodes (128-byte frame; r4, r5
// two flags): on the group's movie, the node +11256 shown with either flag,
// its children +11260 and the next shown with SetNodeTexture 0 with the
// first, the two after with texture 1 with the second, else hidden, the
// two after those hidden (UiMovie::FindNode, ShowNode, HideNode and
// SetNodeTexture's plain C++). The last node.
uint64_t ShowPlayerNodes(uint8_t* base, uint64_t r3, uint64_t first, uint64_t second,
                         uint32_t sp);
// sub_82238468 Type8PanelScreen::FadeInGroup ((3, 1); 112-byte frame):
// once +15228 is set and +15236 isn't, PanelFade::In, the group's named
// animations +11264 restarted and +11268 played and looped unless active
// (FinishNamed, PlayNamed and LoopNamed guest calls), +15236 = 1.
uint64_t FadeInGroup(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82238A00 Type8PanelScreen::UpdateOpening ((3, 0); 112-byte frame):
// with +15228 set, ShowPlayerNodes(1, 0), the holder's slots 2 and 1; then
// once +15232 is 0, +11264 isn't active and [0x82815EB0] is 0: message 29
// posted and +15232 = +15240 = 1, or +9608 = 9 and +15240 = 0 after the
// race.
uint64_t UpdateOpening(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                       const Calls& calls);
// sub_82238928 Type8PanelScreen::Show ((4, 1); 96-byte frame):
// PanelScreen::Show, ScreenFades::FadeIn unless +15240 is set,
// ShowPlayerNodes(0, 0).
uint64_t Show(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_82238BA0 Type8PanelScreen::AskQuit (128-byte frame; r5 the player):
// movie 16's texts (-1, 0) 27, (1, 3) 2 and (1, 4) 3 (Calls::set_text), its
// +40960 = 1, [0x82814E6C] = the player, message 7 posted, +9656 = 1,
// +15244 = -1. Answers -1.
uint64_t AskQuit(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t player, uint32_t sp,
                 const Calls& calls);
// sub_82238DC8 Type8PanelScreen::DrawList (112-byte frame):
// PanelScreen::DrawList, then for the first player whose car list base's
// +1944 is set: AskQuit and ShowPlayerNodes(1, 1) with +15240 set, else
// ScreenFades::FadeOut and +9656 = 2.
uint64_t DrawList(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_82238E78 Type8PanelScreen::Draw ((5, 1)): DrawList at +9656 0, Leave
// at 2 (tail calls), else r3 left.
uint64_t Draw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_82238990 Type8PanelScreen::OnDialogClosed ((5, 13); 96-byte frame):
// +9656 = 0 when +15244 is set, else ScreenFades::FadeOut and +9656 = 2;
// then ShowPlayerNodes(0, 0).
uint64_t OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// --- The screen's constructor, destructors, machine and pick (screen.cpp,
// init_machine.cpp) ---

// sub_822386F0 Type8PanelScreen::Type8PanelScreen (224-byte frame):
// MachineScreen::MachineScreen, its vtable, the ten panels (players 0 then
// 1) and the group, PanelSetPair's constructor (its stack words at the
// frame's +84 to +108), the holder, then its words. r3 left.
uint64_t Construct(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82238368 Type8PanelScreen::~Type8PanelScreen (112-byte frame): its
// vtable, the holder's, PanelSetPair's and the group's destructors, the
// panels' (the confirm panels' vtables, ListNode::~ListNode on the F and D
// panels, CarPanelBase::~CarPanelBase on the C panels), then
// MachineScreen::~MachineScreen. r3 left.
uint64_t Destruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822388D8 Type8PanelScreen::DeletingDestructor (slot 20; 112-byte
// frame): the destructor, then operator delete (a guest call) when bit 0 of
// `flags` is set. r3 left.
uint64_t DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                          const Calls& calls);
// sub_82238C88 Type8PanelScreen::PickCar (slot 3; 160-byte frame): for both
// players, GarageCar::Reset (a guest call) on the player's garage car, then
// by the car list base's tab (+232): the two-player save block's garage car
// at the entry's step copied in (memcpy's plain C++, or a guest call where
// it declines) at 2, Car::SetModelAndClass's plain C++ with the entry's car
// id and the car's halfwords at 0 or 1; then ScreenFades::FadeOut, +15240 =
// 0, +9656 = 2. FadeOut's r3.
uint64_t PickCar(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// sub_82238E98 Type8PanelScreen::InitMachine (slot 1; a 1712-byte frame):
// its 24 transitions on the machine at +4, the frame's words as the
// original leaves them, from its step table (init_table.h). `machine` gets
// the machine, `replaced` whether the last StateMachine::Add replaced a
// transition; the last Add's answer.
constexpr uint32_t kInitMachineFrame = 1712;
uint32_t InitMachine(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t* machine,
                     bool* replaced);


// --- The list screen's list part (list_part.cpp): Type8ListPart (524
// bytes, +20 its inner part) over ListPartBase, Type8ListPartInner (504
// bytes, a Type8PlayerBlock of 232 bytes a player at +40 and +272: +0 the
// player, +4 ready, +8 backed out, +12 and +84 two MenuLists, +156 to +228
// node, string and named animation ids) over ListPartInnerBase; and the
// inner parts' shared step 1 and pages 1 and 2. r4 (`id`) the part's UI
// movie id where the original passes it on ---

// The vtables, the inner's page and step tables (.data) and the pad players
// the blocks' menus take (by the player).
constexpr uint32_t kListPartVtable = 0x820114AC;
constexpr uint32_t kListInnerVtable = 0x82011490;
constexpr uint32_t kListInnerPages = 0x82637A74;
constexpr uint32_t kListInnerSteps = 0x82637A6C;
constexpr uint32_t kPadPlayers = 0x82637A64;
// The UI movie calls made as guest calls.
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kSetNodeText = 0x8211C7E8;

// sub_822542B8 Type8PlayerBlock::Type8PlayerBlock (a leaf; r4 the player,
// 0 or 1: its ids from the tables at 0x82011430 to 0x82011488). r3 left.
uint64_t PlayerBlockConstruct(uint8_t* base, uint64_t r3, uint64_t player);
// sub_822545D0 Type8PlayerBlock::SetUp (160-byte frame): the two menus'
// MenuList::Init, their pad player, the texts (UiMovie::SetText, two
// SetNodeText), the movie's +40960 = 1, +4 = +8 = 0. The last
// SetNodeText's r3.
uint64_t PlayerBlockSetUp(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                          const Calls& calls);
// sub_822546F8 Type8PlayerBlock::PlayOpening (112): PlayNamed on +196,
// +200, +204. The last PlayNamed's r3.
uint64_t PlayerBlockPlayOpening(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp);
// sub_82254798 Type8PlayerBlock::ShowMenuMove (144): the +12 menu's
// previous item's animation finished, its item's restarted and looped, the
// setting's finished when the previous was 0, +212's restarted at item 1 or
// finished when leaving it. The last call's r3.
uint64_t PlayerBlockShowMenuMove(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                 const Calls& calls);
// sub_822548B8 Type8PlayerBlock::PlayReady (112): FinishNamed on +212,
// +200, +192, PlayNamed +192. Its r3.
uint64_t PlayerBlockPlayReady(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                              const Calls& calls);
// sub_82254948 Type8PlayerBlock::PlaceCursor (128; r5 the setting): the
// node (+156, +160)'s children +224 and +228: +108 = 0 on one and 1 on the
// other by the setting. The second child.
uint64_t PlayerBlockPlaceCursor(uint8_t* base, uint64_t r3, uint64_t id, uint64_t selection,
                                uint32_t sp);
// sub_82254AD8 Type8PlayerBlock::ShowSelections (128): the item's, +216's,
// +220's and the setting's animations restarted and looped, PlaceCursor.
// The last LoopNamed's r3.
uint64_t PlayerBlockShowSelections(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                   const Calls& calls);
// sub_82254A70 Type8PlayerBlock::WhileReady (112): when the +12 menu is
// cancelled, sound 2, +4 = 0, ShowMenuMove. Answers 1.
uint64_t PlayerBlockWhileReady(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                               const Calls& calls);
// sub_82254EC8 Type8PlayerBlock::Input (128): MenuList::Update on +12;
// accepted: sound 0, item 1 ready (+4 = 1, PlayReady), else item 1 selected
// and ShowMenuMove; cancelled: sound 2 (player 0) or 3, +8 = 1; moved:
// sound 4, ShowMenuMove; else at item 0 UpdateSetting. Answers 0.
uint64_t PlayerBlockInput(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                          const Calls& calls);
// sub_82254DB8 Type8PlayerBlock::UpdateSetting (112): MenuList::Update on
// +84; when it moved, sound 4 and SettingMoved (its r3), else Update's r3.
uint64_t PlayerBlockUpdateSetting(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                  const Calls& calls);
// sub_82254C50 Type8PlayerBlock::SettingMoved (144): the +84 menu's
// previous item's animation finished, its item's restarted and looped,
// PlaceCursor (its r3).
uint64_t PlayerBlockSettingMoved(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                 const Calls& calls);

// sub_822543C0 Type8ListPartInner::Type8ListPartInner (96) and sub_82254488
// Type8ListPart::Type8ListPart (112): the bases, the vtables, the blocks
// (players 0, 1), the part's +16 = +20 and +12 = 20. r3 left.
uint64_t ListInnerConstruct(uint8_t* base, uint64_t r3, uint32_t sp);
uint64_t ListPartConstruct(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82254418 Type8ListPartInner::~Type8ListPartInner (112; the four
// menus released) and sub_822544E8 Type8ListPart::~Type8ListPart (96), and
// their deleting destructors sub_82254530 and sub_82254580 (112; operator
// delete a guest call when bit 0 of `flags` is set). r3 left.
uint64_t ListInnerDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ListInnerDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls);
uint64_t ListPartDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ListPartDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                  const Calls& calls);
// sub_822549D8 Type8ListPartInner::Open (128): the page block reset,
// UiMovie::ResetPose, both blocks' SetUp (the second's r3), +32 = +36 = 0.
uint64_t ListInnerOpen(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                       const Calls& calls);
// sub_822550C0 Type8ListPartInner::Pick: 0 at page -1, else a tail call of
// the page table's entry with r4 and r5 (from `sp`, returning to `lr`).
uint64_t ListInnerPick(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                       uint32_t lr, const Calls& calls);
// sub_82254E18 Type8ListPartInner::Input (128): UiMovie::Run without a
// step, else the step table's entry and, when it's done, the queued page or
// step. The step's or Run's r3.
uint64_t ListInnerInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls);
// sub_82254D08 Type8ListPartInner::Step0 (128): on the first call both
// blocks' PlayOpening and PlayNamed 47; done when Run answers 0, then both
// blocks' ShowSelections. Whether done.
uint64_t ListInnerStep0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls);
// sub_82254FB8 Type8ListPartInner::Page0 (128): each block's Input (state
// 0) or WhileReady (1); both 1: page 1 and ScreenFades::FadeOut; else
// player 0 backed out: page 2 after step 1. Answers 0.
uint64_t ListInnerPage0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls);
// sub_822538A8 Type8PartInner::Step1 (112): on the first call PlayNamed 48;
// done when Run answers 0, then UiMovie::FinishAll. Whether done.
uint64_t PartInnerStep1(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls);
// sub_82253108 Type8PartInner::Page1 (a leaf): once both fade counts are 0,
// +32 = 1, +36 = 0, answers 1; else 0.
uint64_t PartInnerPage1(uint8_t* base, uint64_t r3);
// sub_82253838 Type8PartInner::Page2 (112): once named animation 48 isn't
// active, +32 = 0, +36 = 1, answers 1; else 0.
uint64_t PartInnerPage2(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp);

}  // namespace type8_screens
