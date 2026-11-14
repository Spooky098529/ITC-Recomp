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
#include <optional>

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


// --- The color list screen's list part (color_part.cpp): Type8ColorListPart
// (788 bytes, +20 its inner part) over ListPartBase, Type8ColorPartInner
// (768 bytes, a Type8ColorBlock of 364 bytes a player at +40 and +404: +0
// the player, +4 ready, +8 backed out, +12 (3 items), +84 (2) and +156 (5)
// MenuLists, +228 to +360 node, string and named animation ids) over
// ListPartInnerBase. Its step 1 and pages 1 and 2 are the shared ones ---

constexpr uint32_t kColorPartVtable = 0x820113F4;
constexpr uint32_t kColorInnerVtable = 0x820113D8;
constexpr uint32_t kColorInnerPages = 0x82637A4C;
constexpr uint32_t kColorInnerSteps = 0x82637A44;

// sub_82252EA0 Type8ColorBlock::Type8ColorBlock (a leaf; r4 the player, 0
// or 1: its ids from the tables at 0x82011348 to 0x820113D4). r3 left.
uint64_t ColorBlockConstruct(uint8_t* base, uint64_t r3, uint64_t player);
// sub_82253308 Type8ColorBlock::SetUp (176): the three menus' Init and
// selections (0, 0, 2), their pad player ({0, 1} on the frame, by the
// player), two SetText and three SetNodeText (the last with r6 the +156
// menu's selection + 1), the movie's +40960 = 1, +4 = +8 = 0. The last
// SetNodeText's r3.
uint64_t ColorBlockSetUp(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                         const Calls& calls);
// sub_82253728 Type8ColorBlock::PlaceCursorA (112; r5 0 shows): the live
// scene's child +228, its +236, its +240: +80 = (r5 == 0). That node.
uint64_t ColorBlockPlaceCursorA(uint8_t* base, uint64_t r3, uint64_t id, uint64_t shown,
                                uint32_t sp);
// sub_82253690 Type8ColorBlock::PlaceCursorB (144): the live scene's child
// +228, its +232: children +244 and +248's +68 = 0.0, +252 and +256's +108
// = 1. The last.
uint64_t ColorBlockPlaceCursorB(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp);
// sub_82253600 Type8ColorBlock::PlaceCursorC (128; r5 the +84 menu's
// item): as PlayerBlockPlaceCursor over (+228, +232) and +356, +360.
uint64_t ColorBlockPlaceCursorC(uint8_t* base, uint64_t r3, uint64_t id, uint64_t selection,
                                uint32_t sp);
// sub_822539A8 Type8ColorBlock::PlayOpening (128): PlayNamed on +304, +308,
// +312, PlaceCursorA(1), PlaceCursorB (its r3).
uint64_t ColorBlockPlayOpening(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp);
// sub_82253A50 Type8ColorBlock::ShowSelections (128): the +12 item's
// (+280), +332's, +344's, +336's and +348's animations restarted and
// looped, PlaceCursorC, the +84 item's (+316). The last LoopNamed's r3.
uint64_t ColorBlockShowSelections(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                  const Calls& calls);
// sub_82253BF8 Type8ColorBlock::ShowMenuMove (144): as
// PlayerBlockShowMenuMove over +280, with +324 at item 1 (PlaceCursorA)
// and +328 at item 2. The last call's r3.
uint64_t ColorBlockShowMenuMove(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                const Calls& calls);
// sub_82253570 Type8ColorBlock::PlayReady (112): FinishNamed on +328,
// +312, +300, PlayNamed +300.
uint64_t ColorBlockPlayReady(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                             const Calls& calls);
// sub_822534F0 Type8ColorBlock::SecondMoved (128): the node (+228, +236)'s
// child +276: UiMovie::ClearNodeText, SetNodeText 8 with the +156 menu's
// item + 1 (its r3).
uint64_t ColorBlockSecondMoved(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                               const Calls& calls);
// sub_82253948 Type8ColorBlock::UpdateSecond (112): MenuList::Update on
// +156; moved: sound 4, SecondMoved.
uint64_t ColorBlockUpdateSecond(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                                const Calls& calls);
// sub_82253D90 Type8ColorBlock::ThirdMoved (144): the +84 menu's previous
// item's (+316) animation finished, its item's restarted and looped,
// PlaceCursorC (its r3).
uint64_t ColorBlockThirdMoved(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                              const Calls& calls);
// sub_82253F60 Type8ColorBlock::UpdateThird (112): MenuList::Update on
// +84; moved: sound 4, ThirdMoved.
uint64_t ColorBlockUpdateThird(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                               const Calls& calls);
// sub_82253EF8 Type8ColorBlock::WhileReady (112): as PlayerBlockWhileReady.
uint64_t ColorBlockWhileReady(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                              const Calls& calls);
// sub_82254070 Type8ColorBlock::Input (128): MenuList::Update on +12;
// accepted: sound 0, items 0 and 1 → item 2 and ShowMenuMove, item 2
// ready (+4 = 1, PlayReady); cancelled: sound 2 or 3, +8 = 1; moved: sound
// 4, ShowMenuMove; else item 0 UpdateThird, item 1 UpdateSecond. Answers 0.
uint64_t ColorBlockInput(uint8_t* base, uint64_t r3, uint64_t id, uint32_t sp,
                         const Calls& calls);

// sub_82253030 Type8ColorPartInner::Type8ColorPartInner (96), sub_82253190
// Type8ColorListPart::Type8ColorListPart (112), sub_82253088
// ~Type8ColorPartInner (112; the six menus released), sub_822531F0
// ~Type8ColorListPart (96), their deleting destructors sub_82253268 and
// sub_822532B8 (112). r3 left.
uint64_t ColorInnerConstruct(uint8_t* base, uint64_t r3, uint32_t sp);
uint64_t ColorPartConstruct(uint8_t* base, uint64_t r3, uint32_t sp);
uint64_t ColorInnerDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorInnerDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls);
uint64_t ColorPartDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorPartDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls);
// sub_82253148 Type8ColorPartInner::IsFirstChoice (96; r4 the player): 1
// when the player's block's +84 menu is at item 0. sub_82253238
// Type8ColorListPart::IsFirstChoice (slot 11): its tail call on +20.
// sub_82253240 Type8ColorListPart::ThirdChoice (slot 12): the player's
// block's +156 menu's item.
uint64_t ColorInnerIsFirstChoice(uint8_t* base, uint64_t r3, uint64_t player, uint32_t sp);
uint64_t ColorPartIsFirstChoice(uint8_t* base, uint64_t r3, uint64_t player, uint32_t sp);
uint64_t ColorPartThirdChoice(uint8_t* base, uint64_t r3, uint64_t player);
// sub_822537A0 Open (128), sub_82254280 Pick, sub_82253FC0 Input (128),
// sub_82253E48 Step0 (128; named 46) and sub_82254178 Page0 (128): as the
// list part's inner part's, over the color blocks and tables.
uint64_t ColorInnerOpen(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls);
uint64_t ColorInnerPick(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                        uint32_t lr, const Calls& calls);
uint64_t ColorInnerInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls);
uint64_t ColorInnerStep0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls);
uint64_t ColorInnerPage0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls);


// --- The list screen (list_screen.cpp): Type8ListScreen (11,368 bytes with
// its part, vtable 0x82010E18) over Type8ListBase (vtable 0x82011638, a
// sibling of TitledListScreen) over NodeListScreen: the race kind list
// +10144 (3 items), the course list +10216 (8, 6 shown), the traffic toggle
// +10288, the node table +10360, the scroll texts +10820 to +10840, the
// list part +10844. The page block as the titled list's (src/type67_screens/
// titled_list.cpp); Draw and Update build their tables on first use, the
// entries run through type67_screens::SlotNative. Names prefixed ListBase
// and ListScreen8 (type67_screens has ListScreen* functions ADL finds) ---

constexpr uint32_t kListBaseVtable = 0x82011638;
constexpr uint32_t kListScreenVtable = 0x82010E18;
constexpr uint32_t kListScreenPart = 10844;
// The page table (10 entries: the empty handler for pages 0, 1, 7; Page2
// to Page6, Page8 and ListScreen::LeavePage for 9) and the step table
// (Step0 to Step3, TitledListScreen::Step2, ColorListScreen::Step0), with
// their guards.
constexpr uint32_t kListBasePages = 0x82815FF0;
constexpr uint32_t kListBasePageGuard = 0x82816040;
constexpr uint32_t kListBaseSteps = 0x82816044;
constexpr uint32_t kListBaseStepGuard = 0x82816074;

// sub_822574D0 Type8ListBase::Type8ListBase (96-byte frame):
// NodeListScreen::NodeListScreen's plain C++, its vtable, +10820 to +10836
// = 545 to 549, +10840 = -1. r3 left.
uint64_t ListBaseConstruct(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223F140 Type8ListScreen::Type8ListScreen (112): ListBaseConstruct,
// its vtable, Type8ListPart::Type8ListPart on +10844 (+9676 its address),
// then its words, those of the node table through [+9672] (read again for
// each store as the original does). r3 left.
uint64_t ListScreen8Construct(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82257548 Type8ListBase::~Type8ListBase: its vtable, then
// ~NodeListScreen (a tail call from `sp`). Its r3.
uint64_t ListBaseDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82257B20 Type8ListBase::DeletingDestructor and sub_8223F388
// Type8ListScreen::DeletingDestructor (112): the destructors (the screen's:
// ~Type8ListPart on +10844, ~Type8ListBase), operator delete (a guest call)
// when bit 0 of `flags` is set. r3 left.
uint64_t ListBaseDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                  const Calls& calls);
uint64_t ListScreen8DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                     const Calls& calls);
// sub_8223F590 Type8ListScreen::IsLoaded (slot 1 of both type 8 list
// screens): UiMovies::IsLoaded([+9680])'s plain C++ (a tail call), its
// answer as is. sub_8223F3E0 OpenMovie (slot 12) and sub_8223F588 Release
// (slot 18 of both): UiMovies::Open([+9680], 22584) and Delete([+9680]),
// tail calls from `sp` returning to `lr`.
uint64_t ListScreen8IsLoaded(uint8_t* base, uint64_t r3);
uint64_t ListScreen8OpenMovie(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                              const Calls& calls);
uint64_t ListScreen8Release(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                            const Calls& calls);
// sub_8223F5D8 Type8ListScreen::SetItemTexts (slot 2; 128; r4 the course
// list's scroll): SetText(m, +9744, 5 + i, [0x82010D70 + 4 r4 + 4 i]) for i
// 0 to 5. The last SetText's r3.
uint64_t ListScreen8SetItemTexts(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls);
// sub_8223F598 Type8ListScreen::ItemInfo (slot 25; a leaf): item r4 (0 to
// 7, else nothing)'s 16 bytes from 0x82010D90 to `info`. r3 left.
uint64_t ListScreen8ItemInfo(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t info);
// sub_8223F6C8 Type8ListScreen::ShowMarks (slot 26; 128): FindNode(m, -1,
// 7)'s +80 = 0 for a negative r4 (that node), else 1, then FindNode(m, -1,
// 17)'s +80 = (r4 != 0) and FindNode(m, -1, 18)'s = (r4 != 2) (the last
// node).
uint64_t ListScreen8ShowMarks(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp);
// sub_8223F648 Type8ListScreen::SetLabels (slot 27; 112): SetText(m, 19,
// 5 + i, 128 + i) for i 0 to 2. The last SetText's r3.
uint64_t ListScreen8SetLabels(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_8223F3F0 Type8ListScreen::SetUpMenus (slot 14; 176): MenuList::Init
// on +10216, +10288 (selection 1) and +10144, slot 2 with r4 0, slot 27,
// NodeListScreen::LoadItem(0), ListScreen::OnToggleMoved(+10288), then the
// music: the sound manager's slot 14 on (1, 2) and, for a nonzero byte,
// slot 13 on (1, 2); slot 14 on (1, 0) and, for a zero byte,
// Sound::Play(1, 0) (guest calls but Sound::Play's plain C++). The last
// call's r3.
uint64_t ListScreen8SetUpMenus(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_8223F778 Type8ListScreen::SetUpRace (slot 7; 4,096-byte frame, with
// its switch piece AddRacers sub_8223F818; r4 the dispatcher): the race
// data reset, its stages cleared, +0xA3AF0 = 4, +0xA3AF4 = 5, its +32 =
// the kind; the course (the course list's +4 plus its selection, 0 to 7)
// set on the stages and the race; the traffic (3 for the toggle's first
// item); the two players' garage cars' racer records at the frame's +80
// (RacerRecord::Init, Car::SetModel, +3892 whether the player's first menu
// is at its first item, memcpy of 1,884 bytes, Car::Refresh), each added to
// the race and stage; the stage's type 1, the racers placed, message 4
// posted. The race set-up's plain C++, guest calls where it declines.
// PlaceRacers' r3.
uint64_t ListScreen8SetUpRace(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls);

// sub_82258D60 Type8ListBase::Draw (slot 23; 224): the page table built
// on first use (its pairs left in the frame's +80 to +159), then page
// +9644's entry with r4 unless it's -1 (then r3 left). The page's r3.
uint64_t ListBaseDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_82258EB0 Type8ListBase::Update (slot 24; 160): the step table built
// on first use, then step +9656's entry with r4 the guard's word as loaded
// (and r5 the table's second entry when it was just built, else `r5`). The
// step's answer.
uint64_t ListBaseUpdate(uint8_t* base, uint64_t r3, std::optional<uint64_t> r5, uint32_t sp,
                        const Calls& calls);
// The pages: sub_82257588 Page2 (112; the kind list), sub_82257768 Page3
// (112; the course list), sub_82258A40 Page4 (96; the list part opened),
// sub_82258AD0 Page5 (96; r4 to the part's Pick; the prompt), sub_822578D8
// Page6 (page 9) and sub_82257900 Page8 (page 9 once both fade counts are
// 0; r3 left).
uint64_t ListBasePage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ListBasePage3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ListBasePage4(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ListBasePage5(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
uint64_t ListBasePage6(uint8_t* base, uint64_t r3);
uint64_t ListBasePage8(uint8_t* base, uint64_t r3);
// The steps, each answering whether it's done: sub_82258158 Step0 (128;
// the opening), sub_82258648 Step1 (144; the kind moved, 1), sub_82258BE0
// Step2 (128; into the course list) and sub_82258438 Step3 (128; back to
// the kind list).
uint64_t ListBaseStep0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ListBaseStep1(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ListBaseStep2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ListBaseStep3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822587C8 Type8ListBase::DimList (128; r4 dim): the toggle's and the
// course's named animations finished (dim) or played and looped, then the
// nodes +9820 (under -1) and +9824 (under +9728) hidden (dim) or shown. The
// last call's r3.
uint64_t ListBaseDimList(uint8_t* base, uint64_t r3, uint64_t dim, uint32_t sp,
                         const Calls& calls);
// sub_82248A20 PairedNodeTable::Clear (128; r3 the table): the third
// pair's named animation finished, the first and second pairs' items shown
// off (NodeTable::ShowNode's plain C++) and their named animations
// finished. The last call's r3.
uint64_t PairedNodeTableClear(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// --- The color list screen (color_list_screen.cpp): Type8ColorListScreen
// (11,828 bytes with its part, vtable 0x82010CF0) over ColorListScreen: the
// item list +10144 (9 items, 3 shown), the course list +10216 (2), the
// traffic toggle +10288, the race kind list +10360 (3), the color node
// table +10432, the color list part +11040. Names prefixed ColorList8 ---

constexpr uint32_t kColorListVtable = 0x82010CF0;
constexpr uint32_t kColorListPart = 11040;

// sub_8223DFC0 Type8ColorListScreen::Type8ColorListScreen (144-byte frame):
// ColorListScreen::ColorListScreen's plain C++, its vtable, the color list
// part on +11040 (+9676 its address), then its words in the original's
// order (the node table's through [+9672], read again for each store; its
// 15 items' +0, +140 and color masks from the item records at 0x82010BE0).
// r3 left.
uint64_t ColorList8Construct(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8223E2B8 Type8ColorListScreen::DeletingDestructor (112):
// ~Type8ColorListPart on +11040, ~ColorListScreen, operator delete (a guest
// call) when bit 0 of `flags` is set. r3 left.
uint64_t ColorList8DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls);
// sub_8223E310 Type8ColorListScreen::OpenMovie (slot 12):
// UiMovies::Open([+9680], 22585), a tail call from `sp` returning to `lr`.
uint64_t ColorList8OpenMovie(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                             const Calls& calls);
// sub_8223E4D0 Type8ColorListScreen::ItemInfo (slot 25; a leaf): r3 = r5;
// item r4 (0 to 8, unsigned, else nothing)'s 28 bytes from 0x82010BE0 to
// r5 (memcpy's plain C++, or its tail call from `sp` returning to `lr`).
uint64_t ColorList8ItemInfo(uint8_t* base, uint64_t r4, uint64_t r5, uint32_t sp, uint32_t lr,
                            const Calls& calls);
// sub_8223E9D8 Type8ColorListScreen::InitMachine (slot 5; a 992-byte
// frame): its 13 transitions on the machine at +4, as InitMachine above
// (its step table in init_table.h).
constexpr uint32_t kColorListInitMachineFrame = 992;
uint32_t ColorList8InitMachine(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t* machine,
                               bool* replaced);

}  // namespace type8_screens
