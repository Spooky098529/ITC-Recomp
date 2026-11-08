// The car dealer natively (B7 in docs/rexglue-removal.md; the functions are
// in docs/functions/car-dealer.md, the globals in docs/known-data.md): the
// type 2 mode's screen machine at its block's +0x19978 where the player buys
// a car. Plain C++ over guest memory (the machine, the page object, the
// menus, the dealer's data and the movies are shared with the recompiled
// code), with the originals' loads and stores in their order and their
// frames' words. `sp` is the caller's stack pointer: the back chain of the
// original's frame and its locals are written below it as the original
// leaves them; its register saves aren't. Adapters: src/hooks/car_dealer.cpp
// (behind --native_car_dealer), register steps shared with the harness in
// src/hooks/car_dealer.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls: r3 to r6 as
// given, r7 to r10 0) by address, to the callees' overrides. UiMovies::Find,
// UiMovie::FindNode and UiMovie::Child are their plain C++. The varargs
// UiMovie::SetText calls pass 0 for the arguments the call sites leave unset
// (the user's call for the parking area, 2026-10-30: only dead words of the
// caller's parameter area differ).

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace car_dealer {

using Calls = race_hud_object::Calls;

// The globals: the stand's holder ([0x826D83E0]: +0 the garage stage), the
// page object, the two menus, the dealer's data, the spec sheet's object
// ([0x826D8438], another screen's: CheckLimits' words).
constexpr uint32_t kStandHolder = 0x826D83E0;
constexpr uint32_t kPages = 0x826D83E8;
constexpr uint32_t kListMenu = 0x826D83EC;
constexpr uint32_t kColorMenu = 0x826D83F0;
constexpr uint32_t kData = 0x826D83F4;
constexpr uint32_t kLimits = 0x826D8438;

// The machine's words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kScrollText = 0x8211C570;
constexpr uint32_t kScrollReset = 0x8211C5F8;
constexpr uint32_t kCarModelsBlockOf = 0x8215AAD0;
constexpr uint32_t kStageReloadModel = 0x8217F880;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kMenuListRelease = 0x8220BBB0;
constexpr uint32_t kSetUpEvent = 0x82114268;
constexpr uint32_t kRaceSetUp = 0x82115D98;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kGarageCarReset = 0x82110448;
constexpr uint32_t kSetModelAndClass = 0x821104D8;
constexpr uint32_t kGarageAdd = 0x82116CE0;
constexpr uint32_t kPickNext = 0x821159B0;
constexpr uint32_t kPickFromCar = 0x82115CC0;
constexpr uint32_t kSetChildNameText = 0x8211D930;
constexpr uint32_t kSetNumberText = 0x8211C740;
constexpr uint32_t kResetPose = 0x8211ABA8;
constexpr uint32_t kFadeOut = 0x8209D0C8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kEmpty = 0x82106540;  // the empty shared function
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kStageIsBusy = 0x82179BE0;
constexpr uint32_t kStageCommand = 0x82180108;
constexpr uint32_t kShowCarById = 0x82182C90;
constexpr uint32_t kStageDraw = 0x8217FE60;
constexpr uint32_t kStageSetDrawWord = 0x82178B40;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kShowDriveDialog = 0x82182990;
constexpr uint32_t kListUpdate = 0x8220C1E0;
constexpr uint32_t kListAccepted = 0x8220BD50;
constexpr uint32_t kListCancelled = 0x8220BDE0;
constexpr uint32_t kListSelection = 0x8220BC20;
constexpr uint32_t kListPressed166 = 0x8220BF90;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
constexpr uint32_t kShowFirstCarDialog = 0x82182A80;
constexpr uint32_t kSetUpSpecSheet = 0x821810E0;
// The boot movies' list (boot-mode.md).
constexpr uint32_t kBootList = 0x826C5BAC;

// --- The machine's set-up (init_states.cpp) ---

// sub_82185920 CarDealer::InitStates: the 23 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in its 1,632-byte
// frame (`frame`, the original's stores in order). Returns the last Add's
// r3 (an index when `replaced`, else the new transition).
constexpr uint32_t kInitStatesFrame = 1632;
uint32_t InitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

// --- The handlers and small helpers (handlers.cpp) ---

// sub_82180D68 CarDealer::Create (r3 the machine): +9608 = 0; the page
// object (28 bytes), the two menus (72 each) and the data (1,196)
// VirtualAlloc'ed into their globals with their first words; +9604 = 1 or
// 9 by +9612. The last VirtualAlloc's r3. 112-byte frame.
uint64_t Create(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82180C08 CarDealer::OpenMovie: UiMovies::Open(4, 22555) (a tail
// call). Its r3.
uint64_t OpenMovie(uint8_t* base, uint32_t sp, uint32_t lr, const Calls& calls);
// sub_82180F20 CarDealer::WaitForLoad (r3 the machine): +9608 = 1 once
// movie 4 is loaded, then +9604 = 9 or 4 by +9612. r3 IsLoaded's when
// called, else the machine (`r3`). 96-byte frame.
uint64_t WaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_82181B98 CarDealer::Page4 (r3 the mode): message 9 posted on its ring.
void Page4(uint8_t* base, uint32_t mode);
// Posts the message (id, 0, 0) on `mode`'s ring (+204: its count +600,
// start +604, 50 slots of 12 bytes), as the handlers do inline.
void Post(uint8_t* base, uint32_t mode, uint32_t id);
// sub_82180C48 CarDealer::AfterDetail: with the data's +12 0, transition 20
// (list +44 0) or transitions 10 then 11 to page 2.
void AfterDetail(uint8_t* base);
// sub_821812B8 CarDealer::CheckLimits: [0x826D8438] +48 = 1, +44 by the
// selected car's spec.
void CheckLimits(uint8_t* base);
// sub_82181330 CarDealer::ReloadStandModel: GarageStage::ReloadModel(stage,
// CarModels::BlockOf(the selected car's id)) into the data's +32, the color
// (+56, +60) 0. ReloadModel's r3. 96-byte frame.
uint64_t ReloadStandModel(uint8_t* base, uint32_t sp, const Calls& calls);

// --- The set-up, teardown and purchase (lifecycle.cpp) ---

// sub_821816B8 CarDealer::Unload (r3 the machine): both lists' and the
// colors' entries VirtualFree'd from the last; UiMovie::FinishAll(movie 4);
// with +9616, UiMovies::Delete(4) and +9608 = 0. The last call's r3.
// 128-byte frame.
uint64_t Unload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82181838 CarDealer::Close (r3 the machine, r4 the mode): +9604 = 7;
// FinishAll(movie 4), Delete(4); both lists' entries freed; message 4
// posted. The last call's r3. 112-byte frame.
uint64_t Close(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82180E70 CarDealer::Destroy (r3 the machine): +9604 = 14 or 20 by the
// data's +8 and +4; MenuList::Release(the list menu); the page object, both
// menus and the data freed and the three globals cleared. The last
// VirtualFree's r3. 128-byte frame.
uint64_t Destroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_82180CE8 CarDealer::StartFirstRace: the boot movies' list
// (0x826C5BAC) set, Career::SetUpEvent(SaveData::Of(0), 0, 0),
// Race::SetUp(SaveData::Of(0), -1). SetUp's r3. 96-byte frame.
uint64_t StartFirstRace(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821813B0 CarDealer::BuildLists: player 0's save data into the data's
// words, then an 8,448-byte entry (VirtualAlloc, the spec memcpy'd) for each
// car id 0 to 71 marked in the save's bit set, added to a list by the spec's
// byte +771 or freed. The last call's r3. 128-byte frame.
uint64_t BuildLists(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82182E68 CarDealer::Buy: the money less the price; a garage car on
// the frame (GarageCar::Reset, Car::SetModelAndClass) with the color,
// Garage::Add; the stats' cars bought; the new car's +1780 from
// [0x826D8438] +4; the selection and Career::PickFromCar or PickNext with
// SetChildNameText. The last call's r3. 2,112-byte frame.
uint64_t Buy(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82181968 CarDealer::OnYes: the data's +16 = 1; at step 2 the money
// shown less the price (SetNumberText). SetNumberText's r3, else `r3`.
// 112-byte frame.
uint64_t OnYes(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls);
// sub_82184AF0 CarDealer::UpdatePage (r4 the mode): the six page handlers'
// addresses at the frame's +80; unless the page is -1, the page's handler
// called (r3 and r4 the mode, r5 to r10 the handlers, as the original
// leaves them). The handler's r3, else `r3`. 112-byte frame.
uint64_t UpdatePage(uint8_t* base, uint64_t r3, uint64_t mode, uint32_t sp, const Calls& calls);

// --- The car's texts and the swatches (info.cpp) ---

// sub_82182388 CarDealer::SetNameTexts: movie 4's (-1, 20) string 1934 +
// `maker`, (-1, 21) 1646 + `id`, (-1, 22) 1718 + `id` (UiMovie::SetText,
// its unset varargs 0). The last SetText's r3. 112-byte frame.
uint64_t SetNameTexts(uint8_t* base, uint64_t id, uint64_t maker, uint32_t sp, const Calls& calls);
// sub_82182418 CarDealer::SetPriceTexts: (19, 2) SetNumberText(`price`),
// (19, 3) string 435, (19, 4) string 527 with the price. The last SetText's
// r3. 112-byte frame.
uint64_t SetPriceTexts(uint8_t* base, uint64_t price, uint32_t sp, const Calls& calls);
// sub_821824A8 CarDealer::SetSpecTexts: sub-scene 18's texts from the
// record at `record` (the doubles at 0x82009F00 and 0x82009F08 scaling two).
// The last SetText's r3. 112-byte frame.
uint64_t SetSpecTexts(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls);
// sub_82182620 CarDealer::ShowCarInfo: the shown list's entry `index`'s
// record on the frame (+80 to +100) for SetSpecTexts, then SetNameTexts and
// SetPriceTexts (their plain C++). SetPriceTexts' r3. 128-byte frame.
uint64_t ShowCarInfo(uint8_t* base, uint64_t index, uint32_t sp, const Calls& calls);
// sub_82181270 CarDealer::SetSwatchColor: the node's child 2 +56, +60, +64
// from the record's +0, +4, +8. The child (r3). 96-byte frame.
uint64_t SetSwatchColor(uint8_t* base, uint32_t node, uint32_t record, uint32_t sp);
// sub_821826C0 CarDealer::ShowSwatches: up to nine of sub-scene 26's
// swatches colored from the color records from the color menu's scroll.
// The last SetSwatchColor's r3, else FindNode(26, 1)'s. 144-byte frame.
uint64_t ShowSwatches(uint8_t* base, uint32_t sp);
// sub_82183AC8 CarDealer::FillStatus: movie 4's pose reset, the pick, the
// money, the day and the weekday; SetPrompts(0, 0, 1, 1, -1, -1) (its plain
// C++); the data's +8, +28, +32 = 0 and +0 = 1. SetPrompts' r3. 112-byte
// frame.
uint64_t FillStatus(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_821817C0 CarDealer::Leave (r3 the machine, r4 the mode): with an
// empty garage +9604 = 10 and StartFirstRace (its plain C++, on the caller's
// frame: a tail call), else +9604 = 9 and message 6. StartFirstRace's r3,
// else `r3`.
uint64_t Leave(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp, const Calls& calls);

// --- The transitions (transitions.cpp) ---

// sub_82181D68 CarDealer::Transition3 (r3 the started word): first call
// ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0) and the word 1; then
// UiMovie::Run(movie 4, 0). 1 when both fades' counts are 0. 112-byte
// frame.
uint64_t Transition3(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls);

// --- The update, the start and the dialogs' end (update.cpp) ---

// sub_82181558 CarDealer::ShowBusy, sub_821815D8 HideBusy and sub_82181640
// HideBusyNow: movie 4's busy marker (named animations 80, 79 and 81
// played, looped or finished). The last call's r3. 112-byte frames.
uint64_t ShowBusy(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t HideBusy(uint8_t* base, uint32_t sp, const Calls& calls);
uint64_t HideBusyNow(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82185600 CarDealer::Update (r3 the machine, not read): the empty
// sub_82106540
// with its arguments; the running transition's handler from the 22 on the
// frame (+96 to +180) called with the page object's +24 (r4 to r10 as the
// original leaves them) and the next started, or UiMovie::Run(movie 4, 0);
// then the stand: the busy marker by GarageStage::IsBusy, the model reloaded
// (ReloadModel, Command(55), ShowCarById) or drawn (GarageStage::Draw), or
// HideBusyNow. The last call's r3 (movie 4, the pair's word UiMovies::Find
// answers, with none). 320-byte frame.
uint64_t Update(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82184B80 CarDealer::Start: BuildLists and FillStatus (their plain C++),
// the page object to page 0 or 1 by its transitions, the stand drawn once
// (SetDrawWord 0, Draw, SetDrawWord 1) and ScreenFades::FadeIn. FadeIn's
// r3. 96-byte frame.
uint64_t Start(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82183160 CarDealer::OnDialogDone (r4 the mode): by the dialog's step
// (the data's +36) the page object's transitions, messages 32 and 34 and
// Buy (its plain C++), ShowDriveDialog (sub_82182990, never seen in play:
// the original, through the guest context) or SetPrompts. The last call's
// r3, else `r3`. 112-byte frame.
uint64_t OnDialogDone(uint8_t* base, uint64_t r3, uint32_t mode, uint32_t sp,
                      const Calls& calls);

// --- The pages (pages.cpp) ---
//
// The page handlers CarDealer::UpdatePage calls (r3 the mode): each updates
// its menus (MenuList::Update) and by their move, accept, cancel or press
// sets the page object's transitions and plays a menu sound
// (MenuSound::Play), all through their overrides. The last call's r3.

// sub_82180F90 CarDealer::Page0: the list's menu. 96-byte frame.
uint64_t Page0(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_82183428 CarDealer::Page1: the car list; with an empty garage,
// leaving shows ShowFirstCarDialog (sub_82182A80, never seen in play: the
// original, through the guest context) and posts message 34. 112-byte
// frame.
uint64_t Page1(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82183600 CarDealer::Page2: one car (both menus; buy, the spec sheet
// through SetUpSpecSheet sub_821810E0, never seen in play: the original;
// SetPrompts, CheckLimits, ReloadStandModel and HelpLine::Set their plain
// C++; UiMovie::SetText's unset varargs 0). 144-byte frame.
uint64_t Page2(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_82181A10 CarDealer::Page3: the color (CheckLimits' plain C++).
// 112-byte frame.
uint64_t Page3(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);

// --- The movie 4 setters (texts.cpp) ---

// sub_82182158 CarDealer::SetKindBar and sub_82182770 SetColorBar: node
// (8, 0) or (26, 0) +16 = `value` as a float (its 64-bit integer left at
// the frame's +80). The node (r3). 112-byte frames.
uint64_t SetKindBar(uint8_t* base, uint64_t value, uint32_t sp);
uint64_t SetColorBar(uint8_t* base, uint64_t value, uint32_t sp);
// sub_82182DD0 CarDealer::SetScrollArrows: (7, 3) and (7, 4) shown by the
// scroll and the count. The second node (r3). 128-byte frame.
uint64_t SetScrollArrows(uint8_t* base, uint64_t scroll, uint64_t count, uint32_t sp);
// sub_82182C28 CarDealer::SetHeader: (2, 1) and (2, 2) +108 = `a`, `b`.
// The second node (r3). 128-byte frame.
uint64_t SetHeader(uint8_t* base, uint32_t a, uint32_t b, uint32_t sp);
// sub_82182B30 CarDealer::SetPrompts: sub-scene 16's three prompts by the
// pairs (r3, r4), (r5, r6), (r7, r8). The last FindNode's r3. 176-byte
// frame.
uint64_t SetPrompts(uint8_t* base, const uint32_t args[6], uint32_t sp);
// sub_82199B38 HelpLine::Set: movie 4's (17, 0) text string `id`
// (UiMovie::SetText, its unset varargs 0), ScrollText(17, 0, 1) and the
// scroll reset sub_8211C5F8. The reset's r3. 112-byte frame.
uint64_t HelpLineSet(uint8_t* base, uint64_t id, uint32_t sp, const Calls& calls);

}  // namespace car_dealer
