// The race HUD machine natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/race-hud-machine.md): one of the race
// mode's state machines (race mode + 128664, RaceHudMachine::), whose
// handlers drive the race HUD and the in-race screens' holder
// ([0x826D84F0], race-hud-object.md) by the race kind. Plain C++ over guest
// memory (the machine and the holder are shared with the recompiled code),
// with the originals' loads and stores in their order. `sp` is the caller's
// stack pointer: the back chain of the original's frame is written there as
// the original leaves it; its register saves aren't. Adapters:
// src/hooks/race_hud_machine.cpp.
//
// The handlers' callees (the race HUD's and the alternate HUD's functions,
// all native) are called through `callees` by their addresses, with the
// original's registers (r3, r4), frame and return address: the adapters'
// implementation runs their plain C++ directly (src/hooks/race_hud_machine.cpp).
// The kinds whose
// screens are never seen in play (4: the holder's +656 object; 0: its +12)
// aren't ported: the handlers answer nullopt for them, with nothing written,
// and the adapters run the originals.

#pragma once

#include "race_hud_object/race_hud_object.h"

#include <cstdint>
#include <optional>

namespace race_hud_machine {

using Calls = race_hud_object::Calls;

// The machine's words.
constexpr uint32_t kState = 9604;   // the state (0 at set-up)
constexpr uint32_t kHolder = 9608;  // the in-race screens' holder
constexpr uint32_t kKind = 9612;    // the race kind
constexpr uint32_t kFlag = 9616;    // set by message 30
// The holder's: +8 bit 0 the replay bit; +16 the race HUD; +1004 the
// alternate HUD's; the HUD's +436 (the holder's +452) the countdown running.
constexpr uint32_t kHolderFlags = 8;
constexpr uint32_t kHud = 16;
constexpr uint32_t kAltHud = 1004;
constexpr uint32_t kCountdownRunning = 16 + 436;

// The callees, by address.
constexpr uint32_t kHudTick = 0x822153A0;          // RaceHud::Tick(r3, r4 = flag)
constexpr uint32_t kHudUpdateView = 0x82212B68;    // RaceHud::UpdateView
constexpr uint32_t kHudUpdateWarning = 0x82212C50; // RaceHud::UpdateWarning
constexpr uint32_t kHudUpdateItem = 0x82214238;    // RaceHud::UpdateItem
constexpr uint32_t kHudTickPanel = 0x82215488;     // RaceHud::TickPanel
constexpr uint32_t kHudDraw = 0x822128A8;          // RaceHud::Draw
constexpr uint32_t kHudRunWarning = 0x82212A48;    // RaceHud::RunWarning
constexpr uint32_t kHudRunItem = 0x82212B08;       // RaceHud::RunItem
constexpr uint32_t kHudRunMovie = 0x822141E8;      // RaceHud::RunMovie(r3, r4 = draw only)
constexpr uint32_t kHudRunCountdown = 0x822129A8;  // RaceHud::RunCountdown
constexpr uint32_t kHudStartMovie1 = 0x82212718;   // RaceHud::StartMovie1
constexpr uint32_t kHudFinishMovie1 = 0x82212878;  // RaceHud::FinishMovie1
constexpr uint32_t kHudRunMovie1 = 0x82212AF8;     // RaceHud::RunMovie1
constexpr uint32_t kHudSetPlayer = 0x82212690;     // RaceHud::SetPlayer(r3, r4 = player)
constexpr uint32_t kHudReset = 0x822121C0;
constexpr uint32_t kHudClearWarning = 0x822122A0;
constexpr uint32_t kHudSetUpGauges = 0x82213C90;
constexpr uint32_t kHudResetBar = 0x82212668;
constexpr uint32_t kHudSetUpExtraGauges = 0x82213DB0;
constexpr uint32_t kHudSetUpMirror = 0x822135B0;
constexpr uint32_t kHudApplyOptions = 0x82212768;
constexpr uint32_t kHudSetUpSigns = 0x822122B0;
constexpr uint32_t kAltSetUp = 0x82217C70;         // AltHud::SetUp(r3, r4 = player)
constexpr uint32_t kAltTick = 0x82217AC8;          // AltHud::Tick
constexpr uint32_t kAltRunMovie = 0x82216C00;      // AltHud::RunMovie
constexpr uint32_t kAfterRaceClose = 0x8221A498;   // AfterRaceMenu::Close (race-screens.md)
constexpr uint32_t kCareerResultsUpdate = 0x8221A090;  // CareerResults::Update (race-screens.md)
constexpr uint32_t kPauseMenuUpdate = 0x8220F638;      // PauseMenu::Update (race-screens.md)
constexpr uint32_t kPauseMenuClose = 0x8220D890;       // PauseMenu::Close (race-screens.md)
constexpr uint32_t kCareerResultsDraw = 0x8221A2F8;    // CareerResults::Draw (race-screens.md)
constexpr uint32_t kDrawAfterRaceMenu = 0x82211D10;    // RaceScreens::DrawAfterRaceMenu
constexpr uint32_t kDrawPauseMenu = 0x82211D08;        // RaceScreens::DrawPauseMenu
constexpr uint32_t kHudDrawWarning = 0x82212AC8;       // RaceHud::DrawWarning
constexpr uint32_t kScreensCreate = 0x82211F40;        // RaceScreens::Create (race-hud-object.md)
constexpr uint32_t kAfterRaceUpdate = 0x8221BD20;      // AfterRaceMenu::Update (race-screens.md)
// The in-race screens' holder ([0x826D84F0]), which the pause's update reads
// instead of +9608.
constexpr uint32_t kHolderGlobal = 0x826D84F0;

// --- The set-up (race_hud_machine_init.cpp) ---

// RaceHudMachine::Init's frame (its r1 less this).
constexpr uint32_t kInitFrame = 4400;
// sub_8216E6E8 RaceHudMachine::Init: the state = 0 and the 65 transitions
// added (StateMachine::Add), built in the original's stack frame at `frame`
// (its r1 less kInitFrame: see race_hud_machine_init.cpp). Returns the last
// StateMachine::Add's answer (`replaced` as its).
uint32_t MachineInit(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);

// --- The handlers (handlers.cpp) ---
//
// r3 = the machine; each returns r3 as the original leaves it (`r3` itself,
// the machine's register, where it calls nothing), or nullopt, with nothing
// done, for kind 4 (and kind 0 where the original draws the holder's +12).

// sub_8216C210 RaceHudMachine::PreRaceUpdate: kinds 1 to 3 and 6 the HUD's
// Tick(0), UpdateView, UpdateWarning, UpdateItem.
std::optional<uint64_t> PreRaceUpdate(uint8_t* base, uint64_t r3, uint32_t sp,
                                      const Calls& callees);
// sub_8216C2A0 RaceHudMachine::PreRaceDraw: kinds 1 to 3 and 6 the HUD's
// Draw, RunWarning, RunItem.
std::optional<uint64_t> PreRaceDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                    const Calls& callees);
// sub_8216C330 RaceHudMachine::CountdownDraw: kinds 1 to 3 and 6 the HUD's
// Draw, RunMovie(0), RunCountdown, RunWarning; 5 AltHud::RunMovie.
std::optional<uint64_t> CountdownDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                      const Calls& callees);
// sub_8216C510 RaceHudMachine::StartIntro and sub_8216C520 EndIntro: the
// HUD's StartMovie1 or FinishMovie1 (tail calls: `sp` and the caller's `lr`
// kept).
uint64_t StartIntro(uint8_t* base, uint32_t machine, uint32_t sp, uint32_t lr,
                    const Calls& callees);
uint64_t EndIntro(uint8_t* base, uint32_t machine, uint32_t sp, uint32_t lr,
                  const Calls& callees);
// sub_8216C530 RaceHudMachine::IntroUpdate: kinds 1 to 3 and 6 the HUD's
// Tick(0), UpdateView, UpdateWarning.
std::optional<uint64_t> IntroUpdate(uint8_t* base, uint64_t r3, uint32_t sp,
                                    const Calls& callees);
// sub_8216C5B8 RaceHudMachine::IntroDraw: kinds 1 to 3 and 6 the HUD's
// RunMovie1, Draw, RunWarning.
std::optional<uint64_t> IntroDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                  const Calls& callees);
// sub_8216CB98 RaceHudMachine::CountdownUpdate: kinds 1 to 3 and 6 the HUD's
// Tick(1), UpdateWarning, TickPanel, UpdateView, then the state 7 once the
// countdown isn't running; kind 5 the state 7 at once.
std::optional<uint64_t> CountdownUpdate(uint8_t* base, uint64_t r3, uint32_t sp,
                                        const Calls& callees);
// sub_8216CE00 RaceHudMachine::RaceDraw: kinds 1 to 3 and 6 without the
// replay bit the HUD's Draw, RunWarning, RunMovie(0); with it, and kind 5,
// AltHud::RunMovie.
std::optional<uint64_t> RaceDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                 const Calls& callees);
// sub_8216D918 RaceHudMachine::RaceUpdate: kinds 1 to 3 and 6 without the
// replay bit the HUD's Tick(1), UpdateView, UpdateWarning, TickPanel; with
// it, and kind 5, AltHud::Tick, posting 20 to `dispatcher` when it answers
// non-zero.
std::optional<uint64_t> RaceUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                   const Calls& callees);
// sub_8216D450 RaceHudMachine::SetUp: by the kind the alternate HUD's
// SetUp, the HUD's SetPlayer and its set-ups (see the row), then +9616 = 0.
std::optional<uint64_t> SetUp(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees);

// sub_8216DFD0 RaceHudMachine::ResultsUpdate: RacerRecords::At(the race
// data's +0x59AE8, -1) (unused); kinds 0 to 3 and 6 the HUD's Tick(0) and
// UpdateWarning; race kind 1 outside the first day CareerResults::Update
// (holder +1024); then with IsFirstDay 48 posted, else with the answer or
// the replay bit the state 22. Returns IsFirstDay's answer. nullopt, with
// nothing done, for kind 4 and the race kinds whose results are never seen
// in play (2, 4, 6, and 1 with +0xA3AF4 0).
std::optional<uint64_t> ResultsUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                      const Calls& callees);
// sub_8216DB70 RaceHudMachine::PauseUpdate: r4 the dispatcher. Kind 6 with
// the race data's +0xA3AF4 8 the HUD's Tick(1), UpdateWarning, TickPanel;
// kinds 1 to 3, 5 and 6 the answer PauseMenu::Update([0x826D84F0] + 1012),
// others 0; by it: 1 posts 29, 2, 6 and 7 post 48, 3 posts 45 and
// sub_82211D00(+9608) (never seen in play: the original), 4 posts 20, 5
// posts 21, 8 RaceHud::ApplyOptions([0x826D84F0] + 16), 9 posts 48 and
// with +0xA3AF4 7 [0x82816E3C] +240 = 1. nullopt, with nothing done, for
// kind 4 (sub_82211458, never seen in play).
std::optional<uint64_t> PauseUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                                    const Calls& callees);
// sub_8216DD88 RaceHudMachine::AfterRaceUpdate: r4 the dispatcher. The last
// result's +3916 (RacerRecords::At(the race data's +0x59AE8, -1)); kinds 1
// to 3 and 6 the HUD's Tick(0) and UpdateWarning; kinds 1 to 3 the answer
// AfterRaceMenu::Update(holder +1020): 1 and 8 post 40, 2 posts 42 and sets
// the replay bit, 4, 5, 7 and 9 post 48, 6 posts 45, 3 nothing, every post
// but 42's clearing the bit; kind 6 with the result's word 0 or 2 posts 40.
// nullopt, with nothing done, for kinds 0 and 4 (sub_8220B9E8,
// sub_82216850) and kind 6 with the word 1 or 3 (sub_8221F1E8), never seen
// in play.
std::optional<uint64_t> AfterRaceUpdate(uint8_t* base, uint64_t r3, uint32_t dispatcher,
                                        uint32_t sp, const Calls& callees);
// sub_8216D198 RaceHudMachine::CloseResults: by the race data's kind the
// results' screen passed to the empty sub_82106540 (nothing done), then
// AfterRaceMenu::Close(holder +1020) and the holder's +440 = 1; returns
// Close's r3. nullopt, with nothing done, for kind 6 (its sub_8221C168,
// never seen in play).
std::optional<uint64_t> CloseResults(uint8_t* base, uint64_t r3, uint32_t sp,
                                     const Calls& callees);

// sub_8216CA28 RaceHudMachine::CreateScreens: +9612 = the race data's kind
// (+0xA3AF0); RaceScreens::Create with the holder's kind by it and the
// current stage's type (Stages::Type(race data + 0x59620, -1)), none where
// the original makes none; then +9608 = [0x826D84F0]. Create's r3, or the
// type's where none is made.
uint64_t CreateScreens(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& callees);
// sub_8216C658 RaceHudMachine::ClosePause: kinds 1 to 3 and 6
// PauseMenu::Close(holder +1012) and RaceHud::ApplyOptions(holder +16); kind
// 4 the holder's +1016 passed to the empty sub_82106540 (only its load
// left); then +9616 = 0. The last call's r3 (the +1016 word for kind 4, the
// machine's r3 for the others).
uint64_t ClosePause(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees);
// sub_8216D6D0 RaceHudMachine::ResultsDraw: race kind 1 with the mode
// (+0xA3AF4) not 0, unless IsFirstDay, CareerResults::Draw(holder +1024);
// 3, 5 and above 6 nothing; then RaceHud::Draw(holder +16). Draw's r3.
// nullopt, with nothing done, for kind 4 (sub_82215D58) and the race kinds
// whose results' draws are never seen in play (0 sub_8220B9E8, 2 and 1 with
// the mode 0 sub_8221F7D0, 4 and 6 sub_822200E0).
std::optional<uint64_t> ResultsDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                    const Calls& callees);
// sub_8216C6D8 RaceHudMachine::AfterRaceDraw: the last result
// (RacerRecords::At(the race data's +0x59AE8, -1)); kinds 1 to 3
// RaceHud::Draw(holder +16) and DrawAfterRaceMenu(holder); 5, 6 (with the
// result's +3916 not 1 or 3) and above nothing (r3 the result). nullopt,
// with nothing done, for kinds 0 and 4 and kind 6 with the word 1 or 3
// (sub_8220B9E8, sub_82215D58, sub_8221EF60), never seen in play.
std::optional<uint64_t> AfterRaceDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                      const Calls& callees);
// sub_8216CF08 RaceHudMachine::PauseDraw: kinds 1 to 3 and 6 without the
// replay bit RaceHud::Draw(holder +16), DrawWarning(holder +16) and
// DrawPauseMenu(holder); with it, and kind 5, DrawPauseMenu(holder) and
// AltHud::RunMovie; above 6 nothing (r3 the machine). The last call's r3.
// nullopt, with nothing done, for kinds 0 and 4 (sub_8220B9E8; sub_82215D58
// and sub_82211590), never seen in play.
std::optional<uint64_t> PauseDraw(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& callees);
// sub_8216CFE0 RaceHudMachine::RacePauseDraw: as PauseDraw, with
// RaceHud::RunMovie(holder +16, 1) between Draw and DrawWarning.
std::optional<uint64_t> RacePauseDraw(uint8_t* base, uint64_t r3, uint32_t sp,
                                      const Calls& callees);

// The pause menu's openers (tail calls to PauseMenu::Open, sub_8220FA88,
// race-screens.md, from the caller's frame `sp` with its return address
// `lr`; r3 = the holder's +1012, r4 = the page, r5 = the flag). Kinds other
// than 1 to 6 return r3 (the machine) as the original's blr. nullopt, with
// nothing done, for kind 4 (sub_82211390) and the stage's
// (RaceHudMachine::OpenPauseByStage sub_8216C798), never seen in play.
// sub_8216D600 RaceHudMachine::OpenPause: 1 page 0 (8 with the replay bit),
// 2 page 3, 3 page 4, 5 page 8, 6 the stage's.
std::optional<uint64_t> OpenPause(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                                  const Calls& callees);
// sub_8216DA40 RaceHudMachine::OpenRacePause: 1 page 8 with the replay bit,
// else page 2 with the race data's +0xA3AF4 0, else OpenCareerPause; 2 page
// 3 (8 with the bit); 3 page 4; 5 page 8; 6 page 8 with the bit, else the
// stage's.
std::optional<uint64_t> OpenRacePause(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                                      const Calls& callees);
// sub_8216D8C8 RaceHudMachine::OpenCareerPause: PauseMenu::Open(holder +1012,
// 1, IsFirstDay) from its 96-byte frame. Open's r3.
uint64_t OpenCareerPause(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& callees);
constexpr uint32_t kPauseMenuOpen = 0x8220FA88;  // PauseMenu::Open (race-screens.md)

// --- The helpers (handlers.cpp) ---

// sub_8216D3F0 RaceHudMachine::IsFirstDay (probably): 1 when the race data's
// kind (+0xA3AF0) is 1 and player 0's save data's day (+6100) is 1, else 0.
uint32_t IsFirstDay(uint8_t* base, uint32_t sp);
// sub_8216C820 RaceHudMachine::OpponentPrize (probably): the prize for an
// opponent's item `id` (-1: 0).
uint32_t OpponentPrize(uint8_t* base, uint32_t id, uint32_t sp);
// sub_8216C938 RaceHudMachine::ItemBonus (probably), with sub_8216C9B8 (its
// jump table's ids 376 to 378) merged: the bonus for item `id` when player
// 0's save data's record of it has the halfword +2 1, else 0 (-1: 0).
uint32_t ItemBonus(uint8_t* base, uint32_t id, uint32_t sp);

}  // namespace race_hud_machine
