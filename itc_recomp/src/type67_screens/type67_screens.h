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
  // UiMovie::SetText(movie, sub_scene, child, id) with r7 to r10 0 (the
  // user's call for its unset varargs, 2026-10-30) from the frame at `frame`
  // (its varargs stored in that frame's +48 to +79) returning to `lr`: its
  // plain C++ while the UI movies' text setters are native and it doesn't
  // decline, else a guest call. Its r3.
  std::function<uint64_t(uint32_t frame, uint32_t lr, uint32_t movie, uint32_t sub_scene,
                         uint32_t child, uint32_t id)>
      set_text;
  // UiMovie::SetText as set_text with r7 to r10 from `varargs` (the calls
  // that set some of them; the user's call: 0 for those they leave unset).
  std::function<uint64_t(uint32_t frame, uint32_t lr, uint32_t movie, uint32_t sub_scene,
                         uint32_t child, uint32_t id, const uint64_t varargs[4])>
      set_text_args;
  // A guest call with r3 to r10 from `args` (MenuList::Init's adapter where
  // its plain C++ can't free the old block), from the frame at `r1`
  // returning to `lr`. Its r3.
  std::function<uint64_t(uint32_t function, const uint64_t args[8], uint32_t r1, uint32_t lr)>
      call8;
  // A guest call with r3 to r6 from `args` and f1 (Race::AddRacer's where its
  // plain C++ declines), from the frame at `r1` returning to `lr`. Its r3.
  std::function<uint64_t(uint32_t function, const uint64_t args[4], double f1, uint32_t r1,
                         uint32_t lr)>
      call_f1;
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
// Whether Slot runs the plain C++ for a slot holding `target` (given
// whether the call sets r4, and r5), and that plain C++ (from the frame at
// `sp`, the callee's entry r1, returning to `lr`; nullopt where it isn't one
// of ours).
bool SlotIsNative(uint32_t target, bool has_r4, bool has_r5 = false);
std::optional<uint64_t> SlotNative(uint8_t* base, uint32_t target, uint64_t r3,
                                   std::optional<uint64_t> r4, uint32_t sp, uint32_t lr,
                                   const Calls& calls,
                                   std::optional<uint64_t> r5 = std::nullopt);
// Slot with r5 set too (the list screens' slot 25, ItemInfo, takes the
// info's address in r5): the plain C++ where the target is one of ours, else
// a guest call with r3 to r5.
uint64_t Slot5(uint8_t* base, uint64_t object_r3, uint32_t slot, uint64_t r4, uint64_t r5,
               uint32_t frame, uint32_t lr, const Calls& calls);

// MenuList::Release on the list at `list_r3` (r3 both halves) from `frame`
// returning to `lr`: its plain C++ where it frees natively, else its
// adapter (the original where it declines). Its r3.
uint64_t ReleaseList(uint8_t* base, uint64_t list_r3, uint32_t frame, uint32_t lr,
                     const Calls& calls);

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

// The vtables the destructors and the one-group holder's constructor store:
// the group pair's base's, the one-group holder's, MachineScreen's.
constexpr uint32_t kGroupPairBaseVtable = 0x82011064;
constexpr uint32_t kSingleGroupVtable = 0x820110A4;
constexpr uint32_t kMachineScreenVtable = 0x82011168;
// ScreenFades::FadeIn (its override).
constexpr uint32_t kScreenFadeIn = 0x8209D038;

// sub_82249C20 GroupPair::~GroupPair: +0 = the base's vtable. r3 left.
uint64_t GroupPairDestruct(uint8_t* base, uint64_t r3);
// sub_82249CB0 SingleGroup::SingleGroup (r4 the group): +4 = it, +0 =
// 0x820110A4. r3 left.
uint64_t SingleGroupConstruct(uint8_t* base, uint64_t r3, uint32_t group);
// sub_82249CC8 SingleGroup::Reset: UiMovie::ResetPose on the movie of the
// group at +4.
uint64_t SingleGroupReset(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82249BE8 PanelFade::In (r3 the passes): [0x82815EB0] = 1, then a tail
// call of ScreenFades::FadeIn(passes, 0, 0, 0, 0) from `sp` returning to
// `lr`.
uint64_t PanelFadeIn(uint8_t* base, uint64_t passes, uint32_t sp, uint32_t lr,
                     const Calls& calls);
// sub_8224CB18 MachineScreen::~MachineScreen: +0 = MachineScreen's vtable.
// r3 left.
uint64_t MachineScreenDestruct(uint8_t* base, uint64_t r3);

// The panel screens' destructors (panel_parts.cpp): the vtables they store.
constexpr uint32_t kPanelGroupVtable = 0x82010E8C;
constexpr uint32_t kType6PanelScreenVtable = 0x82010920;
constexpr uint32_t kType7PanelScreenVtable = 0x82010E90;
// sub_82231CC0 PanelGroup::~PanelGroup: +0 = PanelGroup's vtable, then
// PanelGroupBase::~PanelGroupBase (its plain C++, a tail call). r3 left.
uint64_t PanelGroupDestruct(uint8_t* base, uint64_t group);
// sub_82240348 PanelGroup::DeletingDestructor: the same from a 112-byte
// frame, then operator delete (a guest call) when bit 0 of `flags` is set.
// r3 left.
uint64_t PanelGroupDeletingDestruct(uint8_t* base, uint64_t group, uint64_t flags, uint32_t sp,
                                    const Calls& calls);
// sub_82230668 Type6PanelScreen::~Type6PanelScreen and sub_82240168
// Type7PanelScreen::~Type7PanelScreen (96-byte frame): +0 = the screen's
// vtable; GroupPair::~GroupPair on +11728, PanelSet::~PanelSet on +9752 (the
// car list's plain C++, its MenuList::Release guest calls through `calls`),
// PanelGroup::~PanelGroup on +9708, +9664 the same inline, then
// MachineScreen::~MachineScreen. r3 the screen.
uint64_t PanelScreenDestruct(uint8_t* base, uint64_t screen, bool type7, uint32_t sp,
                             const Calls& calls);
// sub_82230B00 Type6PanelScreen::DeletingDestructor (112-byte frame): the
// destructor, then operator delete (a guest call) when bit 0 of `flags` is
// set. r3 the screen.
uint64_t Type6PanelScreenDeletingDestruct(uint8_t* base, uint64_t screen, uint64_t flags,
                                          uint32_t sp, const Calls& calls);

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
// sub_8223E708 Type6PanelScreen::Back (+9608 = 9, message 6),
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
// sub_822405F8 Type7PanelScreen::PickCar (pick_car.cpp; r4 the
// dispatcher): GarageCar::Reset on the type 7 car (a guest call), then from
// the car list's selection (its slot 18, +72, called before each read): the
// garage's car at its step copied in (memcpy's plain C++, or a guest call
// where it declines) when its tab (+28) is 2, else Car::SetModelAndClass's
// plain C++ with the list entry's car id and the car's halfwords (+4 when it
// was 0xFFFF, +2 = +0, +0 = the selection's +40 + +44); then the car's
// +1972, +1980, +1976, +9608 = 9 and message 5 posted. The last selection
// call's r3.
uint64_t Type7PanelScreenPickCar(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls);

// --- The node tables (node_tables.cpp): the list and node screens' tables
// of their items' nodes in a UI movie (the table's +4 the movie's id, +8 and
// +12 the node sets, +16 + 4i item i's named animation, +156 + 4i its node,
// +296 to +308 the four children, +312 + 4i its marks, +420 and +424 extra
// nodes, +428 to +476 the pairs (now, before), +480 and +516 the color
// pairs' animations, +552 and +568 two colors). The functions that call
// none of the UI movie functions leave r3; the others leave the r3 of the
// last call the original makes (UiMovies::Find's address when nothing
// else ran). UiMovies::Find, FindNode, ShowNode, HideNode and Child by
// their plain C++; FinishNamed, PlayNamed and LoopNamed guest calls from
// the original's frame with its return addresses; SetText through
// Calls::set_text. Each takes the r1 the original is entered with. ---

// The base's vtable (NodeTable::~NodeTable and the deleting destructor
// store it).
constexpr uint32_t kNodeTableVtable = 0x82010FF0;

// sub_82247DC0 NodeTable::~NodeTable: +0 = the base's vtable.
uint64_t NodeTableDestruct(uint8_t* base, uint64_t r3);
// sub_82247E88 NodeTable::DeletingDestructor (slot 0 of the three
// vtables; 96-byte frame): +0 = the base's vtable, operator delete (a guest
// call) when bit 0 of `flags` is set. r3 the table.
uint64_t NodeTableDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls);
// sub_82247CB8 NodeTable::SetPairs: +428, +436, +444, +452 = the 16-byte
// info's +4, +8, +0, +12.
uint64_t NodeTableSetPairs(uint8_t* base, uint64_t r3, uint32_t info);
// sub_82247CE0 PairedNodeTable::SavePairs: +432, +440, +448, +456 = +428,
// +436, +444, +452.
uint64_t PairedNodeTableSavePairs(uint8_t* base, uint64_t r3);
// sub_82247DD0 ColorNodeTable::Clear: the pairs 0 now and -1 before, +476
// -1.
uint64_t ColorNodeTableClear(uint8_t* base, uint64_t r3);
// sub_82247E00 ColorNodeTable::Set: from the 28-byte info at `info`.
uint64_t ColorNodeTableSet(uint8_t* base, uint64_t r3, uint32_t info);
// sub_82247E50 ColorNodeTable::SavePairs: the six pairs' previous values.
uint64_t ColorNodeTableSavePairs(uint8_t* base, uint64_t r3);
// sub_82248300 NodeTable::Show (128-byte frame): item `item`'s node (and
// +420's) shown or hidden.
uint64_t NodeTableShow(uint8_t* base, uint64_t r3, uint32_t item, uint32_t on, uint32_t sp);
// sub_82247ED0 NodeTable::ShowNode (160): as Show, and when on the four
// children's +80 = the bits of `mask`.
uint64_t NodeTableShowNode(uint8_t* base, uint64_t r3, uint32_t item, uint32_t on, uint32_t mask,
                           uint32_t sp);
// sub_822480F0 NodeTable::SetMarks (144): the four children's +80 = the
// bits of `mask`.
uint64_t NodeTableSetMarks(uint8_t* base, uint64_t r3, uint32_t item, uint32_t mask, uint32_t sp);
// sub_82248198 ColorNodeTable::SetColors (144) and sub_82248260
// ClearColors (128): the four children's colors.
uint64_t ColorNodeTableSetColors(uint8_t* base, uint64_t r3, uint32_t item, uint32_t k,
                                 uint32_t sp);
uint64_t ColorNodeTableClearColors(uint8_t* base, uint64_t r3, uint32_t item, uint32_t sp);
// sub_822487B8 ColorNodeTable::MarkSelected (no frame: a tail call of
// SetMarks from `sp`): +428's marks by `which` (0: bit +460, 1: bit +468,
// else the item's own mask).
uint64_t ColorNodeTableMarkSelected(uint8_t* base, uint64_t r3, uint32_t which, uint32_t sp);
// sub_822483B0 ColorNodeTable::Refresh (128): the old and new items, the
// color pair, +424's four nodes; `finish` finishes the played animations
// again.
uint64_t ColorNodeTableRefresh(uint8_t* base, uint64_t r3, uint32_t finish, uint32_t sp,
                               const Calls& calls);
// sub_82248650 ColorNodeTable::Reset (slot 1 of 0x82011000; 128): every
// item hidden, marked and uncolored, +264's eight nodes hidden, Refresh(1),
// the pairs saved, +424's four texts (strings 402, 401, 407, 408) set and
// their nodes hidden.
uint64_t ColorNodeTableReset(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82248800 PairedNodeTable::Refresh (128): the pairs' old items shown
// off, the new ones shown with their animations replayed.
uint64_t PairedNodeTableRefresh(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// --- The list part (list_part.cpp): ListPart (140 bytes; +4 yes, +8 back,
// +12 its UI movie's id, +16 the inner part, +20 the inner part in place)
// over ListPartBase, and its inner part ListPartInner (120 bytes; +4 the
// page, +8 the page after, +12 the page before, +16 the step, +20 the step
// queued, +24, +28 the step's first-call word, +32 yes, +36 back, +40 a
// two-item MenuList, +112 the prompt's state: 0 opening, 1 shown, 2
// closing, 3 closed, 4 asked to leave; +116 leave asked) over
// ListPartInnerBase. The part's and the inner's slot calls, and the inner's
// page and step tables in .data, run the plain C++ here where the target is
// one of these (SlotNative), else guest calls (r4 and r5 as the original
// sets them). r3 as the originals leave it. ---

constexpr uint32_t kListPartVtable = 0x82011314;
constexpr uint32_t kListPartBaseVtable = 0x8201127C;
constexpr uint32_t kListPartInnerVtable = 0x820112F8;
constexpr uint32_t kListPartInnerBaseVtable = 0x820112CC;
// The inner's page table (Page0 to Page2), step table (Step0 to Step2) and
// the named animations of its two items (1 and 2).
constexpr uint32_t kListPartPages = 0x82637A20;
constexpr uint32_t kListPartSteps = 0x82637A2C;
constexpr uint32_t kListPartNamed = 0x820112F0;

// sub_82252420 ListPartBase::~ListPartBase and sub_82252600
// ListPartInnerBase::~ListPartInnerBase: +0 = the base's vtable.
uint64_t ListPartBaseDestruct(uint8_t* base, uint64_t r3);
uint64_t ListPartInnerBaseDestruct(uint8_t* base, uint64_t r3);
// sub_82252610 ListPartBase::DeletingDestructor and sub_82252658
// ListPartInnerBase::DeletingDestructor (96-byte frames): the same, then
// operator delete (a guest call) when bit 0 of `flags` is set.
uint64_t ListPartBaseDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                      const Calls& calls);
uint64_t ListPartInnerBaseDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags,
                                           uint32_t sp, const Calls& calls);
// sub_822526F8 ListPartInner::~ListPartInner (96): +0 = its vtable,
// MenuList::Release on +40, ~ListPartInnerBase.
uint64_t ListPartInnerDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822529B0 ListPartInner::DeletingDestructor (112): the destructor,
// operator delete when bit 0 of `flags` is set.
uint64_t ListPartInnerDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                       const Calls& calls);
// sub_82252968 ListPart::~ListPart (96): +0 = its vtable, ~ListPartInner on
// +20, ~ListPartBase.
uint64_t ListPartDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82252A38 ListPart::DeletingDestructor (112).
uint64_t ListPartDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                  const Calls& calls);

// sub_82252430 ListPartBase::Open (96): the inner's slot 1 (r4 = +12), then
// its own slot 9 (+36). Slot 9's r3.
uint64_t ListPartOpen(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82252490 ListPartBase::Pick (96): 1 without an inner part; else the
// inner's slot 2 (r4 = +12, r5 = `r4`) and, when it answers nonzero, +4 =
// the inner's +32 and +8 = its +36. The slot's r3.
uint64_t ListPartPick(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_82252500 ListPartBase::Input: the inner's slot 3 (r4 = +12, r5 =
// `r4`), a tail call from `sp` returning to `lr`; r3 left without one.
uint64_t ListPartInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                       const Calls& calls);
// sub_82252530 ListPartBase::Close (96): the inner's slot 5, then its own
// slot 10 (+40). Slot 10's r3.
uint64_t ListPartClose(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82252590 ListPartBase::IsIdle and sub_822525B8 CanLeave: the inner's
// slot 4 or 6 (a tail call), or 1 without one.
uint64_t ListPartIsIdle(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                        const Calls& calls);
uint64_t ListPartCanLeave(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                          const Calls& calls);

// sub_82252A88 ListPartInner::Open (144; r4 the movie's id): the page
// block reset to step 0, the movie's pose reset and its three texts set,
// its +40960 = 1, the two-item MenuList made (MenuList::Init's plain C++
// where it can free the old block, else a guest call), its selection 1
// when player 0's options' +20 (save data +0x1F870) is 0, else 0; +112 and
// +116 = 0. r3 + 40.
uint64_t ListPartInnerOpen(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                           const Calls& calls);
// sub_82252A00 ListPartInner::Pick: 0 when the page (+4) is -1, else the
// page table's entry (a tail call from `sp` returning to `lr`, r4 and r5
// passed on).
uint64_t ListPartInnerPick(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5, uint32_t sp,
                           uint32_t lr, const Calls& calls);
// sub_82252DF0 ListPartInner::Input (128; r4 the movie's id): UiMovie::Run
// without a step (+16 -1), else the step table's entry and, when it answers
// nonzero, the page block moved on. Its r3.
uint64_t ListPartInnerInput(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls);
// sub_82252748 ListPartInner::IsIdle (96): 1 when the list's selection is
// 0.
uint64_t ListPartInnerIsIdle(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82252740 ListPartInner::Close: MenuList::Release on +40 (a tail
// call).
uint64_t ListPartInnerClose(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                            const Calls& calls);
// sub_822528C8 ListPartInner::CanLeave: +116 = 1; 1 when +112 is 3, else
// +112 = 4 unless it's 2 or 4, and 0.
uint64_t ListPartInnerCanLeave(uint8_t* base, uint64_t r3);
// sub_82252778 ListPartInner::Page0 (128): the asked-to-leave check, then
// MenuList::Update, its move, accept and cancel (MenuSound::Play's plain
// C++). 0.
uint64_t ListPartInnerPage0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82252878 ListPartInner::Page1 and sub_822528A8 Page2: +32 and +36.
// 1.
uint64_t ListPartInnerPage1(uint8_t* base, uint64_t r3);
uint64_t ListPartInnerPage2(uint8_t* base, uint64_t r3);
// sub_82252BB8 ListPartInner::Step0, sub_82252C98 Step1 and sub_82252D38
// Step2 (128-byte frames; r4 the movie's id): the opening, the closing and
// the list moved. Done (Step2: 1).
uint64_t ListPartInnerStep0(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls);
uint64_t ListPartInnerStep1(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls);
uint64_t ListPartInnerStep2(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                            const Calls& calls);

// --- The list screens' shared helpers, item loaders and handlers
// (list_helpers.cpp; r3 the screen, m its UI movie: UiMovies::Find on
// [+9680], the id at the function's frame's +80). UiMovies::Find,
// UiMovie::FindNode, ShowNode, HideNode, SetNodeTexture and MenuList::
// Selection and GetPreviousSelection through their plain C++;
// UiMovie::FinishNamed, PlayNamed, LoopNamed, ResetPose, Run, ClearText,
// ScrollText and sub_8211C5F8 guest calls to their overrides from the
// original's frame with its return addresses; SetText through
// Calls::set_text (r4 set to the sub-scene before it, as the original
// leaves it for the calls after). The helpers call each other's plain C++.
// r3 as the originals leave it. ---

constexpr uint32_t kListScreenVtable = 0x82011218;

// sub_82251EB8 ListScreen::~ListScreen: +0 = ListScreen's vtable.
uint64_t ListScreenDestruct(uint8_t* base, uint64_t r3);
// sub_82252248 ListScreen::DeletingDestructor (96-byte frame): the same,
// then operator delete (a guest call) when bit 0 of `flags` is set. r3 the
// screen.
uint64_t ListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls);
// sub_82256FE8 NodeListScreen::ItemInfo (slot 25): the three words at
// `info` = -1. r3 left.
uint64_t NodeListScreenItemInfo(uint8_t* base, uint64_t r3, uint32_t info);
// sub_82257060 NodeListScreen::LoadItem (112; r4 the item): slot 25 with
// r5 = its frame's +80, NodeTable::SetPairs on +10360 when +9672 is set,
// then +9700 and +9704 = bit 0 of the info's +12 word (both: the
// original's `(1 << i) != 0` test). SetPairs' r3, else slot 25's.
uint64_t NodeListScreenLoadItem(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls);
// sub_82255238 ColorListScreen::LoadItem (144; r4 the item, r5 an index):
// slot 25 into its frame's +80, ColorNodeTable::Set on +10432, +10908 = the
// info's +20 + 4 r5 word when 0 <= r5 < the info's +8, else -1. Set's r3.
uint64_t ColorListScreenLoadItem(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5,
                                 uint32_t sp, const Calls& calls);

// sub_822518B0 ListScreen::ShowSelection (144; r4 a menu list): +9900
// replayed (FinishNamed, PlayNamed); the +108 words of +9724's nodes +9764
// (the previous selection), +9768 (the selection), the +9784 entries of the
// arrays at [+9772] and [+9776], and -1's +9760 (the selection).
uint64_t ListScreenShowSelection(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls);
// sub_82251A20 ListScreen::LoopItemNode, sub_82251B80 LoopSecondNode and
// sub_82251AD0 LoopThirdNode (112; r4 a menu list): the named animation at
// +9936, +9976 or +10016 (`table`) + 4 × the selection finished, played and
// looped.
uint64_t ListScreenLoopNode(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t table, uint32_t sp,
                            const Calls& calls);
// sub_82251D00 ListScreen::SetScrollText (128; r4 a string id, r5
// scrolling): unless r4 is +10140, the text +9740/+9816 reset, set and
// scrolled when both and r4 are >= 0, then +10140 = r4.
uint64_t ListScreenSetScrollText(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5,
                                 uint32_t sp, const Calls& calls);
// sub_82251DA0 ListScreen::ClearScrollText (112): ClearText(m, +9740,
// +9816) when both are >= 0; +10136 = 0, +10140 = -1.
uint64_t ListScreenClearScrollText(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82251E10 ListScreen::SetArrows (128; `which` 0: +9752, else +9756;
// `on`): the node's +108 = +9800 (on) or +9804.
uint64_t ListScreenSetArrows(uint8_t* base, uint64_t r3, uint32_t which, uint32_t on,
                             uint32_t sp);
// sub_82252068 ListScreen::OnToggleMoved (128; r4 the two-item list).
uint64_t ListScreenOnToggleMoved(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                 const Calls& calls);
// sub_82252198 ListScreen::ShowToggle (128; r4 the two-item list).
uint64_t ListScreenShowToggle(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls);
// sub_82251C30 ListScreen::SetIcons (112): four textures and two nodes
// hidden at +9732/+9812 + 0 to 5.
uint64_t ListScreenSetIcons(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82251EC8 ListScreen::SetUpMovie (112): ResetPose, the node table's
// slot 1, +9748 shown or hidden, SetArrows(0, 0), (1, 0), SetIcons.
uint64_t ListScreenSetUpMovie(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);

// sub_82251F98 ListScreen::Show (128; r4 the dispatcher): SetUpMovie, the
// three titles, +9696 = 0, slot 22 (+88), slot 14 (+56) with r4.
uint64_t ListScreenShow(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_82251520 ListScreen::Draw (112): slot 23 (+92), slot 10 (+40), both
// with r4.
uint64_t ListScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls);
// sub_822517B8 ListScreen::Update (112): with a step (+9656 not -1) slot 24
// (+96, r4 its frame's +80) and the page block moved on when it answers
// nonzero; else UiMovie::Run(m, 0) and, when +9696 is set, the list part's
// (+9676) slot 3 (+12) with r4.
uint64_t ListScreenUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                          const Calls& calls);
// sub_82251580 ListScreen::Suspend, sub_82251590 Resume (tail calls of
// slots 16 and 17, `slot` +64 or +68) and sub_822517A0 Open (+9608 = 4,
// then slot 15, +60): from `sp` returning to `lr`, r4 passed on.
uint64_t ListScreenTail(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t slot, uint32_t sp,
                        uint32_t lr, const Calls& calls);

// --- The titled list screen (titled_list.cpp; TitledListScreen, the type 6
// list screen's base, over NodeListScreen). The page block: +9644 the page,
// +9648 the page after, +9652 the page before, +9656 the step, +9660 the
// step queued, +9668 the step's first-call word. Draw and Update build
// their tables of member functions (8 bytes an entry: the function, the
// this offset) on first use, the guard's bit 0 set as the originals do, so
// the recompiled and native sides share them; the entries run the plain
// C++ through SlotNative. ---

constexpr uint32_t kTitledListScreenVtable = 0x820115C8;
constexpr uint32_t kNodeListScreenVtable = 0x82011558;
// The page table (9 entries: the empty handler for pages 0, 1, 5, 6; Page2,
// Page3, Page4, Page7 and ListScreen::LeavePage for 2, 3, 4, 7, 8) and the
// step table (Step0 to Step2), with their guards.
constexpr uint32_t kTitledPages = 0x82815F88;
constexpr uint32_t kTitledPageGuard = 0x82815FD0;
constexpr uint32_t kTitledSteps = 0x82815FD4;
constexpr uint32_t kTitledStepGuard = 0x82815FEC;

// sub_82257558 TitledListScreen::Reset (slot 22): page 2 after step 0. r3
// left.
uint64_t TitledListScreenReset(uint8_t* base, uint64_t r3);
// sub_822579E8 TitledListScreen::Draw (slot 23; 208-byte frame): the page
// table built on first use (its pairs left in the frame's +80 to +151), then
// page +9644's entry with r4 unless it's -1 (then r3 left). The page's r3.
uint64_t TitledListScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls);
// sub_82258998 TitledListScreen::Update (slot 24; 112): the step table built
// on first use (the frame's +80 to +103), then step +9656's entry (r4
// passed on, r5 the step). The step's answer.
uint64_t TitledListScreenUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls);
// sub_82257148 TitledListScreen::Page2 (112): the item list's update, move,
// accept and cancel, then the two other lists'.
uint64_t TitledListScreenPage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822572F0 Page3 (96): the list part's slot 1 (r4 left) and +9696 = 1
// when there's one; page 4. Slot 1's r3, else 0.
uint64_t TitledListScreenPage3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82257360 Page4 (112; r4 passed to the part's Pick): the part's answer.
uint64_t TitledListScreenPage4(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                               const Calls& calls);
// sub_82257490 Page7: page 8 once both fade counts are 0. r3 left.
uint64_t TitledListScreenPage7(uint8_t* base, uint64_t r3);
// sub_82255818 ListScreen::LeavePage (the titled list's page 8, the color
// list's page 10): a tail call (from `sp`, returning to `lr`) of slot 7
// (+28) when +9688 is set, else slot 6 (+24), r4 passed on.
uint64_t ListScreenLeavePage(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, uint32_t lr,
                             const Calls& calls);
// sub_82257D48 Step0 (128; the opening), sub_82257FF8 Step1 (112; the
// leaving, its first-call quirk kept) and sub_822580E0 Step2 (112; the item
// moved, always 1). Done.
uint64_t TitledListScreenStep0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t TitledListScreenStep1(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t TitledListScreenStep2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82257B78 TitledListScreen::OnItemMoved (176): the item nodes'
// animations, the arrows or the cursor, the scroll, slot 26. Slot 26's r3.
uint64_t TitledListScreenOnItemMoved(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82257000 NodeListScreen::~NodeListScreen (96): its vtable,
// NodeTable::~NodeTable on +10360, MenuList::Release on +10288, +10216,
// +10144, ~ListScreen. r3 the screen.
uint64_t NodeListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82257138 TitledListScreen::~TitledListScreen: its vtable, then
// ~NodeListScreen (a tail call from `sp`).
uint64_t TitledListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82257990 TitledListScreen::DeletingDestructor (`titled`) and
// sub_82257940 NodeListScreen::DeletingDestructor (112): the destructor
// (TitledListScreen's vtable stored first, then ~NodeListScreen), operator
// delete (a guest call) when bit 0 of `flags` is set. r3 the screen.
uint64_t NodeListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, bool titled,
                                        uint32_t sp, const Calls& calls);

// --- The color list screen (color_list.cpp; ColorListScreen, the type 7
// list screen's base, over ListScreen: the item list +10144, the color list
// +10216, the toggle +10288, the first list +10360, the color node table
// +10432). The page block as the titled list's; Draw and Update build their
// 11-entry tables on first use (the same words and guards as the
// originals), the entries run through SlotNative. ---

constexpr uint32_t kColorListScreenVtable = 0x820114E0;
// The page table (the empty handler for pages 0, 1, 8; Page2 to Page7,
// Page9 and ListScreen::LeavePage for 10) and the step table (Step0 to
// Step4, RunMovie for 5 and 9, Step6 to Step8, Step10), with their guards.
constexpr uint32_t kColorPages = 0x82815EC8;
constexpr uint32_t kColorPageGuard = 0x82815F20;
constexpr uint32_t kColorSteps = 0x82815F28;
constexpr uint32_t kColorStepGuard = 0x82815F80;

// sub_82255200 ColorListScreen::Reset (slot 22): page 2 after step 1, then
// ColorNodeTable::Clear on +10432 (a tail call). Clear's r3.
uint64_t ColorListScreenReset(uint8_t* base, uint64_t r3);
// sub_82256CA0 ColorListScreen::Draw (slot 23; 240-byte frame): the page
// table built on first use (its pairs left in the frame's +80 to +167), then
// page +9644's entry with r4 unless it's -1 (then r3 left). The page's r3.
uint64_t ColorListScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls);
// sub_82256E08 ColorListScreen::Update (slot 24; 240): the step table built
// on first use, then step +9656's entry (r4 passed on, or the table's sixth
// entry when it was just built; r5 left). The step's answer.
uint64_t ColorListScreenUpdate(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                               const Calls& calls);
// sub_822552B8 Page2 (144): the first list's update, move (step 10),
// accept (the item list's first info, step 2 then page 3) and cancel (page
// 9, the fade-out), else the toggle's.
uint64_t ColorListScreenPage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82255490 Page3 (160): the item list's update, move (step 7), accept
// (the color list from 0, step 4 then page 4) and cancel (step 3 then page
// 2), else the toggle's.
uint64_t ColorListScreenPage3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82255658 Page4 (144): the color list's update and move (step 8), the
// item list's accept (page 5) and cancel (step 6 then page 3), else the
// toggle's.
uint64_t ColorListScreenPage4(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82256508 Page5 (96): the list part's slot 1 (r4 left) and +9696 = 1
// when there's one; SetScrollText(+11036, 1); page 6; ShowPartNodes(1) when
// +9708 is set.
uint64_t ColorListScreenPage5(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822565A0 Page6 (112; r4 passed to the part's Pick): the list part's
// prompt. The part's answer.
uint64_t ColorListScreenPage6(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                              const Calls& calls);
// sub_822557F0 Page7: page 10. r3 left.
uint64_t ColorListScreenPage7(uint8_t* base, uint64_t r3);
// sub_822562E0 Page9 (96): once both fade counts are 0, page 10 and
// UiMovie::FinishAll on the screen's movie (its r3); else r3 left.
uint64_t ColorListScreenPage9(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822561D0 PlaceItemCursor and sub_82256258 PlaceColorCursor (128; r4 a
// selection): the cursor's child (+9744's node and +9828, or +9736 and
// +9808) +16 = r4 × 40 as a float, the 64-bit product left in the frame's
// +80. The child, or r3 left when either word is negative.
uint64_t ColorListScreenPlaceItemCursor(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp);
uint64_t ColorListScreenPlaceColorCursor(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp);
// sub_82256360 ShowPartNodes (128; `hide` r4): the toggle's and the color's
// nodes finished (hide) or played and looped, then +9824 hidden or shown.
// The last call's r3.
uint64_t ColorListScreenShowPartNodes(uint8_t* base, uint64_t r3, uint64_t hide, uint32_t sp,
                                      const Calls& calls);
// The step table's steps (the page block's +9668 the first-call word),
// each answering whether it's done: sub_82258578 Step0 (112; the leaving,
// the first-call quirk kept), sub_82255890 Step1 (128; the opening),
// sub_82256740 Step2 (128; page 2 to 3), sub_82255B58 Step3 (128; page 3
// back to 2), sub_82256908 Step4 (128; page 3 to 4), sub_82255C88 RunMovie
// (96; steps 5 and 9: UiMovie::Run's answer), sub_82255CD0 Step6 (128; page
// 4 back to 3), sub_82256AC0 Step7 (176; the item list moved, 1),
// sub_82256030 Step8 (144; the color list moved, 1) and sub_82255EB0 Step10
// (144; the first list moved, 1).
uint64_t ColorListScreenStep0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenStep1(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenStep2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenStep3(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenStep4(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenRunMovie(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenStep6(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenStep7(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenStep8(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t ColorListScreenStep10(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82255198 ColorListScreen::~ColorListScreen (96): its vtable,
// NodeTable::~NodeTable on +10432, MenuList::Release on +10360, +10288,
// +10216, +10144, ~ListScreen. r3 the screen.
uint64_t ColorListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82255840 ColorListScreen::DeletingDestructor (112): the destructor,
// operator delete (a guest call) when bit 0 of `flags` is set. r3 the
// screen.
uint64_t ColorListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls);

// --- The type 6 list screen's own slots and destructors, and the panel and
// node screens' leaves (type6_list.cpp) ---

constexpr uint32_t kType6ListScreenVtable = 0x820109A8;

// sub_822336C0 Type6ListScreen::SetItemTexts (slot 2; 128-byte frame; r4 a
// row): SetText(m, 8, 5 + i, [0x826366FC + 4 r4 + 4 i]) for i 0 to 6. The
// last SetText's r3.
uint64_t Type6ListScreenSetItemTexts(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                     const Calls& calls);
// sub_82233730 Type6ListScreen::ShowMarks (slot 26; 128): FindNode(m, -1,
// 17)'s +80 = (r4 != 0), FindNode(m, -1, 18)'s +80 = (r4 != 2). The second
// node.
uint64_t Type6ListScreenShowMarks(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp);
// sub_82237008 Type6ListScreen::ItemInfo (slot 25, and slot 4 of the node
// screen): item r4 (0 to 7, else nothing)'s 16 bytes from 0x82636720 to
// `info`. r3 left.
uint64_t Type6ListScreenItemInfo(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t info);
// sub_82259998 NodeScreen::ItemInfo: the four words at `info` = -1. r3
// left.
uint64_t NodeScreenItemInfo(uint8_t* base, uint64_t r3, uint32_t info);
// sub_82233378 Type6ListScreen::SetUpMenus (slot 14; 144): MenuList::Init
// on +10216, +10288 (selection 1) and +10144 (selection 0), slot 2 with r4
// 0, NodeListScreen::LoadItem(0). LoadItem's r3.
uint64_t Type6ListScreenSetUpMenus(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822337B0 Type6ListScreen::Back (slot 6; 112; r4 the dispatcher):
// +9608 = 9, message 6 posted, the sound manager's slot 13 (r4, r5 = the
// frame's +84 (1) and +80 (0)), Sound::Play(+92 (1), +88 (2)). Its r3.
uint64_t Type6ListScreenBack(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls);
// sub_822334C8 Type6ListScreen::SetUpRace (slot 7; 4,080-byte frame, with
// its switch piece sub_8223356C): the race data reset, its stages cleared,
// its +32 = the +10144 list's selection, +0xA3AF0 = 2, +0xA3AF4 = 4; the
// route (the +10216 list's +4 plus its selection, 0 to 7, else the frame's
// unset +80) set on the stages and the race; the race data's +0x59618 from
// the +10144 list's selection when the +10288 list's is 0; the list part's
// slot 5 (its answer: the record's +3892 and the save data's +0x1F870); the
// type 6 car's racer record at the frame's +96 (RacerRecord::Init,
// Car::SetModel, memcpy of 1,884 bytes, Car::Refresh), added to the race and
// the stage; stage 1 added, the racers placed, +9608 = 10. PlaceRacers' r3.
uint64_t Type6ListScreenSetUpRace(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822332D8 Type6ListScreen::~Type6ListScreen (96): its vtable,
// ~ListPart on +10824, ~TitledListScreen. r3 the screen.
uint64_t Type6ListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82233478 Type6ListScreen::DeletingDestructor (112): the destructor,
// operator delete (a guest call) when bit 0 of `flags` is set. r3.
uint64_t Type6ListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls);
// sub_822609E0 PanelScreen::OnYes, sub_822609D0 OnNo and sub_822306D0
// OnBack: +9660 = `answer` (0, 1, 2). r3 left.
uint64_t PanelScreenSetAnswer(uint8_t* base, uint64_t r3, uint32_t answer);
// sub_822306E0 PanelScreen::OnDialogClosed (96): with +9660 0,
// ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0) and +9656 = 2, else +9656
// = 0. FadeOut's r3, else r3 left.
uint64_t PanelScreenOnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82248DE0 PanelScreen::Suspend: a tail call of the pair's (+9648) slot
// 7 (+28).
uint64_t PanelScreenSuspend(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                            const Calls& calls);
// sub_822404D8 PanelScreen::OpenEmpty: PanelScreen::Open with r4 = 0.
uint64_t PanelScreenOpenEmpty(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                              const Calls& calls);
// sub_822307D0 Type6PanelScreen::CarList, sub_822307E0 CarEntry,
// sub_822307F0 CarSlot: the type 6 car's +1972, +1976 or +1980.
uint64_t Type6CarWord(uint8_t* base, uint32_t offset);
// sub_8224ACE0 GroupPair::DeletingDestructor (96): +0 = the base's vtable,
// operator delete when bit 0 of `flags` is set. r3.
uint64_t GroupPairDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                   const Calls& calls);
// sub_82259318 NodeScreen::Open: +10064 = 4. r3 left.
uint64_t NodeScreenOpen(uint8_t* base, uint64_t r3);

// --- The type 7 list screen's own slots and destructors (type7_list.cpp) ---

constexpr uint32_t kType7ListScreenVtable = 0x82010F18;

// sub_82243238 Type7ListScreen::SetItemText (slot 2; 128-byte frame; r4 an
// index): SetText(m, 21, 5 + i, [0x82637054 + 4 r4 + 4 i]) for i 0 to 3.
// The last SetText's r3.
uint64_t Type7ListScreenSetItemText(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                    const Calls& calls);
// sub_822432A8 Type7ListScreen::ClearItemText (slot 3; 128): ClearText(m,
// 21, 5 + i) for i 0 to 2 but r4 (a guest call). The last one's r3.
uint64_t Type7ListScreenClearItemText(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                      const Calls& calls);
// sub_822433D8 Type7ListScreen::SetTitle (128; from SetUpMenus only):
// SetText(m, 20, 5 + i, [0x82637194 + 4 i]) for i 0 to 2. The last r3.
uint64_t Type7ListScreenSetTitle(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82243308 Type7ListScreen::SetDetails (slot 26; 160; r4 an item, 0 to
// 8): the item's record (0x82637098 + 28 r4): MenuList::Init on +10216 with
// its +8 count, then for i 0 and 1, SetText(m, 9, 5 + i, [0x82637078 + 4
// [record + 12 + 4 i]]) while i < the count, else ClearText(m, 9, 5 + i).
// The last call's r3.
uint64_t Type7ListScreenSetDetails(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                   const Calls& calls);
// sub_82243448 Type7ListScreen::ShowMarks (slot 27; 128): FindNode(m, -1,
// 18)'s +80 = (r4 != 0), FindNode(m, -1, 19)'s +80 = (r4 != 6). The second
// node.
uint64_t Type7ListScreenShowMarks(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp);
// sub_82242FA0 Type7ListScreen::ItemInfo (slot 25; no frame): item r4 (0 to
// 8, else nothing)'s 28-byte record from 0x82637098 to r5 (memcpy, a tail
// call from `sp` returning to `lr`: its plain C++, else a guest call). r5.
uint64_t Type7ListScreenItemInfo(uint8_t* base, uint64_t r4, uint64_t r5, uint32_t sp,
                                 uint32_t lr, const Calls& calls);
// sub_82243588 Type7ListScreen::SetUpMenus (slot 14; 144): MenuList::Init
// on +10144, +10288 (selection 1), +10360 (selection 0) and +10216,
// SetTitle, ColorListScreen::LoadItem(Selection(+10144), -1). LoadItem's r3.
uint64_t Type7ListScreenSetUpMenus(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822434C8 Type7ListScreen::Back (slot 6; 112; r4 the dispatcher):
// +9608 = 9, message 5 posted, the sound manager's slot 13, Sound::Play(1,
// 2), as Type6ListScreen::Back. Its r3.
uint64_t Type7ListScreenBack(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                             const Calls& calls);
// sub_82243018 Type7ListScreen::SetUpRace (slot 7; 4,064-byte frame, with
// its switch piece sub_822430BC): the race data reset, its stages cleared,
// its +32 = the +10360 list's selection, +0xA3AF0 = 3, +0xA3AF4 = 6; the
// route by +10148 plus the +10144 list's selection (0 to 14, else route 0)
// set on the race (Race::SetRoute(0, route)); +0x59618 = 4 when the +10288
// list's selection is 0; +0x5961C left; the list part's slot 5 (its answer:
// the record's +3892 and the save data's +0x1F870); the type 7 car's racer
// record at the frame's +80, added to the race and the stage; AddStage(4)
// (nothing), the racers placed, +9608 = 10. PlaceRacers' r3.
uint64_t Type7ListScreenSetUpRace(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82242EC8 Type7ListScreen::~Type7ListScreen (96): its vtable,
// ~ListPart on +11040, ~ColorListScreen. r3 the screen.
uint64_t Type7ListScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82242FC8 Type7ListScreen::DeletingDestructor (112): the destructor,
// operator delete (a guest call) when bit 0 of `flags` is set. r3.
uint64_t Type7ListScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls);

// --- The node screen (node_screen.cpp; NodeScreen, Type6NodeScreen's base:
// state +10064, movie +10100, the page block +10104 (page, page after, page
// before, step, step queued, +10128 the step's first-call word), the list
// +10132 and the toggle +10204; the paired node table at +0) ---

constexpr uint32_t kNodeScreenVtable = 0x820116B8;

// sub_82259CA0 NodeScreen::Show ((4, 1); 128): Refresh, the title and two
// texts on [+10100] (UiMovie::SetText with 322, 0 and 1), page 2 after step
// 0, slot 15 (+60) with r4. Slot 15's r3.
uint64_t NodeScreenShow(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls);
// sub_8225A1B8 NodeScreen::Draw ((5, 1); 160): the page table at 0x82816078
// (guard 0x828160A8) built on first use, the page +10104 called with r4
// unless it's -1. The page's r3.
uint64_t NodeScreenDraw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                        const Calls& calls);
// sub_8225A2C0 NodeScreen::Update ((5, 0); 144): nothing when [+10100] < 0;
// UiMovie::Run(m, 0) when the step is -1; else the step table at 0x828160AC
// (guard 0x828160C4) built on first use and the step +10116 called; when
// done, the page block moves on. Its r3.
uint64_t NodeScreenUpdate(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82259D80 NodeScreen::Page2 (112), sub_822590E8 Page4 (a leaf: page 5
// once the screen fades' counts are 0), sub_82259328 Page5 (96: slot 6 with
// r4, then +10064 = 9).
uint64_t NodeScreenPage2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t NodeScreenPage4(uint8_t* base, uint64_t r3);
uint64_t NodeScreenPage5(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                         const Calls& calls);
// sub_82259EC0 NodeScreen::Step0 (128, the opening), sub_82259680 Step1
// (112, the leaving: +10128 never set), sub_82259750 Step2 (176, the list
// moved). Whether done.
uint64_t NodeScreenStep0(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t NodeScreenStep1(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t NodeScreenStep2(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82259A78 NodeScreen::ShowSelection (128), sub_82259B70 OnToggleMoved
// (128), sub_822599B0 Refresh (112), sub_82259410 ShowRecords (160; r4 the
// course, r5 the traffic: the ten records' times and texts from
// CourseRecords::Of, the course's texture), sub_82259050 LoadItem (128: slot
// 4 into its frame, the pairs at +428 to +452). Their r3.
uint64_t NodeScreenShowSelection(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t NodeScreenOnToggleMoved(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t NodeScreenRefresh(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t NodeScreenShowRecords(uint8_t* base, uint64_t r3, uint64_t r4, uint64_t r5,
                               uint32_t sp, const Calls& calls);
uint64_t NodeScreenLoadItem(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82259368 NodeScreen::SetArrows (128): the arrow `which` (0: +10396,
// else +10400) on sub-scene +10276 gets +108 = +10536 (`on`) or +10540.
uint64_t NodeScreenSetArrows(uint8_t* base, uint64_t r3, uint32_t which, uint32_t on,
                             uint32_t sp);
// sub_82259948 NodeScreen::~NodeScreen (96): its vtable, MenuList::Release
// on +10204 and +10132, ~NodeTable. r3. sub_8225A168 DeletingDestructor
// (112): the destructor, operator delete when bit 0 of `flags` is set. r3.
uint64_t NodeScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
uint64_t NodeScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                    const Calls& calls);

// --- The type 6 node screen's own slots and destructors (type6_node.cpp) ---

constexpr uint32_t kType6NodeScreenVtable = 0x82010A80;

// sub_82236F00 Type6NodeScreen::SetUpMenus (slot 15; r4 not read):
// MenuList::Init on +10132 (8, 6) and +10204 (2, 2), +10204's selection 1,
// slot 2 (SetText) with r4 = 0, NodeScreen::LoadItem, then
// NodeScreen::ShowRecords([+10136] + +10132's selection, +10204's).
uint64_t Type6NodeScreenSetUpMenus(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_822370A0 Type6NodeScreen::SetText (slot 2): UiMovie::SetText(m, 5,
// 5 + i, [0x826366FC + 4 × (r4 + i)]) for i 0 to 6.
uint64_t Type6NodeScreenSetText(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls);
// sub_82237110 Type6NodeScreen::Back (slot 6, r4 the dispatcher): message 6
// posted. A leaf; r3 left.
uint64_t Type6NodeScreenBack(uint8_t* base, uint64_t r3, uint64_t r4);
// sub_82237160 Type6NodeScreen::SetIcons (slot 20; r3 and r4 not read):
// movie 15's (2, 4) and (2, 5) textures 0 and 1, (2, 6 to 9, 13, 14)
// hidden. The last node.
uint64_t Type6NodeScreenSetIcons(uint8_t* base, uint32_t sp);
// sub_82237228 Type6NodeScreen::SetScrollText (slot 21; r3 and r4 not
// read): movie 15's (1, 0): sub_8211C5F8, SetText 934, ScrollText 1.
uint64_t Type6NodeScreenSetScrollText(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_822372A0 Type6NodeScreen::ShowMarks (slot 22): movie 15's nodes 13
// and 14 shown by r4 != 0 and r4 != 2. The second node.
uint64_t Type6NodeScreenShowMarks(uint8_t* base, uint64_t r4, uint32_t sp);
// sub_82247FE0 NodeScreen::SetUpTexts (slot 1 of NodeScreen and
// PairedNodeTable; r3 the node table): the table's nodes hidden, the texts
// 402, 401, 407, 408 set at +424 to +427 when +424 is set.
uint64_t NodeScreenSetUpTexts(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82236EA0 Type6NodeScreen::~Type6NodeScreen: +0 = 0x82010A80, a tail
// call of ~NodeScreen.
uint64_t Type6NodeScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82237048 Type6NodeScreen::DeletingDestructor (slot 0): +0 =
// 0x82010A80, ~NodeScreen, operator delete when bit 0 of `flags` is set.
// r3.
uint64_t Type6NodeScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls);

}  // namespace type67_screens
