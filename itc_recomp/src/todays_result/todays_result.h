// The type 9 front-end mode natively (B7 in docs/rexglue-removal.md; the
// functions are in docs/functions/todays-result.md, the globals in
// docs/known-data.md): the career's end of the day. The mode's Init and
// delegate functions, its +32 IsLoaded, today's result's machine (the type 9
// block's +0, UI movie 27) and today's event's (+9640, UI movie 0) with
// their pages and transitions. Plain C++ over guest memory (the machines,
// the screens' blocks, the mode's message ring and the movies are shared
// with the recompiled code), with the originals' loads and stores in their
// order and their frames' words. `sp` is the caller's stack pointer: the
// back chain of the original's frame and its locals are written below it as
// the original leaves them; its register saves aren't.
// Adapters: src/hooks/todays_result.cpp (behind --native_todays_result),
// register steps shared with the harness in src/hooks/todays_result.h.
//
// The guest calls go through `Calls` (race_hud_object::Calls) by address, to
// the callees' overrides (the UI movie calls, VirtualAlloc and VirtualFree,
// the loading screen, the fades, the screen start, Strings::Load,
// GameMessages::Dispatch, and the pages and transitions the tables hold).
// SaveData::Of, UiMovies::Find and IsLoaded are their plain C++. The empty
// sub_82106540 (a debug print) the Updates call first is left out: it
// returns at once.

#pragma once

#include <cstdint>

#include "race_hud_object/race_hud_object.h"

namespace todays_result {

using Calls = race_hud_object::Calls;

// The machines' words.
constexpr uint32_t kMachineState = 9604;
constexpr uint32_t kMachineLoaded = 9608;
constexpr uint32_t kMachineReenter = 9612;
constexpr uint32_t kMachineDelete = 9616;

// The mode's fields: the delegate's holder (+4) and the delegate (+8), the
// dispatcher (+36: the handlers, the count +200; its ring +204 is the
// mode's +240), the message ring (+240) and the block (+848).
constexpr uint32_t kDispatcher = 36;
constexpr uint32_t kRing = 240;
constexpr uint32_t kBlock = 848;
// The block (19,300 bytes): today's result at +0, today's event at +9640,
// the shown machine's index (+19280: 27 today's result, 0 today's event),
// +19284.
constexpr uint32_t kBlockBytes = 19300;
constexpr uint32_t kEventAt = 9640;
constexpr uint32_t kShown = 19280;
constexpr uint32_t kResultIndex = 27;

// The save data's fields Kind reads: the unlocks just made's count and the
// movie queued (-1 none).
constexpr uint32_t kUnlockCount = 0x1CA78;
constexpr uint32_t kQueuedMovie = 0x1D524;

// The callees by address.
constexpr uint32_t kVirtualAlloc = 0x82295398;
constexpr uint32_t kVirtualFree = 0x82295330;
constexpr uint32_t kDispatch = 0x821062E8;
constexpr uint32_t kLoadingShow = 0x822478D0;
constexpr uint32_t kLoadingRun = 0x82247898;
constexpr uint32_t kLoadingFinish = 0x82247A60;
constexpr uint32_t kScreenStart = 0x8211AD00;
constexpr uint32_t kStringsLoad = 0x8210A8D0;
constexpr uint32_t kFadeIn = 0x8209D038;
constexpr uint32_t kFadeOut = 0x8209D0C8;
constexpr uint32_t kMoviesOpen = 0x8211E808;
constexpr uint32_t kMoviesDelete = 0x8211E258;
constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kLoopNamed = 0x8211DDE0;
constexpr uint32_t kFinishNamed = 0x8211DF20;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kRun = 0x8211E1B8;
constexpr uint32_t kPlay = 0x8211D978;
constexpr uint32_t kLoop = 0x8211DDD8;
constexpr uint32_t kLoopLast = 0x8211DDE8;
constexpr uint32_t kFinish = 0x8211DE38;
constexpr uint32_t kRevealText = 0x8211D880;
constexpr uint32_t kHasMoreText = 0x8211D8D8;
constexpr uint32_t kSetText = 0x8211C1F8;
constexpr uint32_t kSetNumberText = 0x8211C740;
constexpr uint32_t kSetChildNameText = 0x8211D930;
constexpr uint32_t kMemcpy = 0x825A7410;
constexpr uint32_t kWinStreak = 0x82110580;
constexpr uint32_t kWord4832 = 0x821105A0;
constexpr uint32_t kCareerNextDay = 0x82116AF0;
constexpr uint32_t kCheckUnlocks = 0x821162B0;
constexpr uint32_t kRaceEventDestroy = 0x82144DF0;
constexpr uint32_t kMenuPressed = 0x821864C8;
constexpr uint32_t kMenuSoundPlay = 0x821103F0;
// The fades' passes FadeIn and FadeOut get, and the fades' counts the
// FadeOut transitions wait on.
constexpr uint32_t kFadesIn = 0x82001BEC;
constexpr uint32_t kFadesOut = 0x82001BF0;
constexpr uint32_t kFadeCount0 = 0x826A50E4;
constexpr uint32_t kFadeCount1 = 0x826A50DC;
// The boot movies' list (boot-mode.md): five ids, +20 the mode after, +24
// skippable, +28 the index.
constexpr uint32_t kBootMovies = 0x826C5BAC;

// Today's result's blocks: the page object (28 bytes: +0 the page Draw
// runs, -1 while a transition runs; +4 the page after; +8 the page before;
// +12 the transition Update runs, -1 none; +16 the one queued after it; +20;
// +24 the transition's started word), the menu (68 bytes) and the screen's
// data (68 bytes asked, used to +87: +0 set up, +4 a movie queued, +8 to +52
// the counts, +56 to +64 the day, +68 to +87 the next pick's record).
constexpr uint32_t kResultPages = 0x828160C8;
constexpr uint32_t kResultMenu = 0x828160CC;
constexpr uint32_t kResultData = 0x828160D0;
constexpr uint32_t kResultMovie = 27;
constexpr uint32_t kResultAsset = 22578;
// Today's event's blocks: the page object (28 bytes, as today's result's),
// the menu (68 bytes) and the data (68 bytes asked, used to +411: +0 set
// up, +4 the first image list (46 records at most) and +188 its count,
// +192 the second and +376 its count, +380 and +384 the images' bits, +388
// to +396 the unlock shown (+392 its table, +396 its entry, -1 none), +400
// and +404 the movie's +28 object's +36 and +32 kept, +408 movies queued).
constexpr uint32_t kEventPages = 0x828160D4;
constexpr uint32_t kEventMenu = 0x828160D8;
constexpr uint32_t kEventData = 0x828160DC;
constexpr uint32_t kEventMovie = 0;
constexpr uint32_t kEventAsset = 22545;
// The save's unlocks just made: 12-byte entries (+0 shown, +4 the table,
// +8 the entry) from +0x1C778, their count at kUnlockCount.
constexpr uint32_t kUnlocks = 0x1C778;
// The images' assets: 22696 + i and 22741 + i.
constexpr uint32_t kImageAssetA = 22696;
constexpr uint32_t kImageAssetB = 22741;
constexpr uint32_t kImages = 46;
constexpr uint32_t kAssetSize = 0x820A0678;
constexpr uint32_t kXMemSet = 0x822962B0;
constexpr uint32_t kArchiveLoad = 0x8209DE48;
constexpr uint32_t kImageLoaded = 0x8218D8E8;
constexpr uint32_t kFreeImage = 0x8218D650;
constexpr uint32_t kSoundPlay = 0x82175850;
constexpr uint32_t kSoundManager = 0x826C5C80;

// The accept action Menu::Pressed is asked for.
constexpr uint32_t kAccept = 159;

// --- The mode (mode.cpp) ---

// sub_8225D6D0 ModeType9::Init (r3 the mode): see the row; both machines'
// InitStates as plain C++, the other callees through their overrides.
// ScreenFades::FadeIn's r3. 112-byte frame.
uint64_t ModeInit(uint8_t* base, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8225B358 TodaysResult::InitStates and sub_8225CF30
// TodaysEvent::InitStates: their 13 transitions added to `machine`
// (StateMachine::Add's plain C++) from functors built in their frames
// (`frame`, the originals' stores in order). The last Add's r3 (an index
// when `replaced`, else the new transition).
constexpr uint32_t kInitStatesFrame = 976;
uint32_t ResultInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
uint32_t EventInitStates(uint8_t* base, uint32_t machine, uint32_t frame, bool* replaced);
// sub_8225D658 ModeType9::FindShown: of the indexes 0 to 46, 0 (today's
// event, +9640) and 27 (today's result, +0): the last in state 5 or 4 into
// the block's +19280. A leaf.
void FindShown(uint8_t* base, uint32_t block);
// sub_8225BA48 TodaysEvent::Kind: SaveData::Of(0)'s unlocks just made
// counted and a movie 2 to 5 queued: 1; 6: 2; else 0. 96-byte frame.
uint32_t Kind(uint8_t* base, uint32_t sp);
// The delegate functions (r3 the 8-byte answer, r4 the mode; each returns
// the answer): sub_8225D918 WaitForLoads (128-byte frame), sub_8225DC38
// NextMode (144), sub_8225DB70 LeaveScreen (128), sub_8225DA68 WaitForShown
// (112) and sub_8225DE78 Leaving (128, never seen in play).
void WaitForLoads(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void NextMode(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void LeaveScreen(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void WaitForShown(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
void Leaving(uint8_t* base, uint32_t answer, uint32_t mode, uint32_t sp, const Calls& calls);
// sub_8225D6A8 ModeType9::IsLoaded (r3 the mode + 32): 1 when both
// machines' movies are loaded. A leaf, never seen in play.
uint32_t IsLoaded(uint8_t* base, uint32_t object);

// --- Today's result (result.cpp; r3 the machine, r4 the dispatcher where
// read) ---

// sub_8225A7A0 Create: +9608 = 0; the page object (28), the menu (68) and
// the data (68) VirtualAlloc'ed into their globals with their first words;
// +9604 = 1 or 9 by +9612. 112-byte frame.
uint64_t ResultCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8225A510 OpenMovie: UiMovies::Open(27, 22578) (a tail call, from the
// caller's frame with its return address). Its r3.
uint64_t ResultOpenMovie(uint32_t sp, uint32_t lr, const Calls& calls);
// sub_8225A930 WaitForMovie: while +9608 is 0, +9608 = 1 once movie 27 is
// loaded; once set, +9604 = 9 or 4 by +9612. IsLoaded's r3 when called,
// else `r3`. 96-byte frame.
uint64_t ResultWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8225B300 Start: ReadDay, SetUpMovie; the page object to page 0
// through transition 0 with 1 queued. SetUpMovie's r3. 96-byte frame.
uint64_t ResultStart(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225A520 ReadDay: see the row (11,040-byte frame; memcpy,
// DayBlock::WinStreak and Word4832 and Career::NextDay through their
// overrides). NextDay's r3.
constexpr uint32_t kReadDayFrame = 11040;
uint64_t ReadDay(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225B258 SetUpMovie: SetTexts, the data's +0 = 1, four nodes hidden.
// The last FindNode. 128-byte frame.
uint64_t SetUpMovie(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225ADE0 SetTexts: movie 27's texts (the varargs setters' unset
// arguments 0, the user's call of 2026-10-30). SetChildNameText's r3.
// 112-byte frame.
uint64_t SetTexts(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225B0C8 Draw: the two pages' addresses at the frame's +80; unless
// the page (+0) is -1, the page called with r3 the dispatcher (through the
// guest). Its r3, else `r3`. 96-byte frame.
uint64_t ResultDraw(uint8_t* base, uint64_t r3, uint64_t dispatcher, uint32_t sp,
                    const Calls& calls);
// sub_8225B128 Update (the transitions): movie 27; the transition (+12) from
// the 3-entry table at the frame's +88 called with the page object's +24
// (through the guest), the queued one or the page after started on a
// nonzero answer; with none (-1), UiMovie::Run(movie, 0) unless the page is
// 1. The transition's or Run's r3, or the movie. 128-byte frame.
uint64_t ResultUpdate(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225A9B0 WaitPage: on Menu::Pressed(menu, 159), to page 1 through
// transition 2, MenuSound::Play(0). The sound's r3, else Pressed's.
// 96-byte frame.
uint64_t ResultWaitPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225AB30 LeavePage (r3 the dispatcher): message 5; QueueMovie when the
// save's movie is set (the data's +4 = 1, else 0); RaceEvent::Destroy. Its
// r3. 96-byte frame.
uint64_t ResultLeavePage(uint8_t* base, uint32_t dispatcher, uint32_t sp, const Calls& calls);
// sub_8225A720 QueueMovie (never seen in play): the boot movies' list from
// the save's movie (then -1), the garage after; Progress::CheckUnlocks'
// plain C++ (the original through the guest where Condition::Test's copies
// can't be native). Its r3. 96-byte frame.
uint64_t ResultQueueMovie(uint8_t* base, uint32_t sp, const Calls& calls);
// The transitions (r3 the started word, the page object's +24; 112-byte
// frames): sub_8225ABD8 Intro (PlayNamed 5, the fade in; 1 when Run answers
// 0), sub_8225AC88 ShowLoop (PlayNamed and LoopNamed 4; 1) and sub_8225AD20
// FadeOut (FinishNamed 4, the fade out; 1 once the fades' counts are 0).
uint64_t ResultIntro(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t ResultShowLoop(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t ResultFadeOut(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_8225AA20 Unload: UiMovie::FinishAll(movie 27); with +9616 set,
// UiMovies::Delete(27) and +9608 = 0. The last call's r3. 112-byte frame.
uint64_t ResultUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8225A9A0 Unloaded: +9604 = 10. A leaf.
void ResultUnloaded(uint8_t* base, uint32_t machine);
// sub_8225AA88 Leave: +9604 = 7; FinishAll, UiMovies::Delete(27); message 4
// posted. Delete's r3. 112-byte frame.
uint64_t ResultLeave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                     const Calls& calls);
// sub_8225A8A8 Destroy: +9604 = 20 (a movie queued) or 13; the three blocks
// VirtualFree'd and their globals 0. The last VirtualFree's r3. 128-byte
// frame.
uint64_t ResultDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);

// --- Today's event (event.cpp; r3 the machine, r4 the dispatcher where
// read) ---

// sub_8225BE50 Create: +9608 = 0; the page object (28), the menu (68) and
// the data (68) VirtualAlloc'ed into their globals with their first words;
// Kind not 0: MarkImages, +9604 = 1 or 9 by +9612; else +9608 = 1, +9604 =
// 9. MarkImages' or Kind's r3. 112-byte frame.
uint64_t EventCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8225BAB8 MarkImages (never seen in play): the images of each unlock's
// pages marked in the data's +380 bits. Its r3 (the last Entry56's, or the
// save). 160-byte frame.
uint64_t MarkImages(uint8_t* base, uint32_t sp);
// sub_8225C130 OpenMovie (never seen in play): UiMovies::Open(0, 22545),
// LoadImages. LoadImages' r3. 96-byte frame.
uint64_t EventOpenMovie(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225C008 LoadImages (never seen in play): LoadImagePair for each
// marked image. r3 the data, or as LoadImagePair leaves it. 112-byte frame.
uint64_t LoadImages(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225BD10 LoadImagePair (never seen in play; r3 the data, r4 an image
// index): both images' buffers and records, Archive::Load. Its r3.
// 144-byte frame.
uint64_t LoadImagePair(uint8_t* base, uint32_t data, uint32_t index, uint32_t sp,
                       const Calls& calls);
// sub_8225C160 WaitForMovie (never seen in play): while +9608 is 0, +9608 =
// 1 once movie 0 and both image lists are loaded; once set, +9604 = 9 or 4
// by +9612. The last test's r3, else `r3`. 96-byte frame.
uint64_t EventWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp);
// sub_8225C0A0 ImagesLoadedA and sub_8225C0E8 ImagesLoadedB (leaves; r3 the
// data): 1 when every record of the list has +0 set.
uint32_t ImagesLoadedA(uint8_t* base, uint32_t data);
uint32_t ImagesLoadedB(uint8_t* base, uint32_t data);
// sub_8225C990 Start (never seen in play): Sound::Play(1, 2), the movie's
// +28 object's words kept, SetUpTexts, NextUnlockEntry; unless it answers
// 2, the page object to page 0 through transition 0. NextUnlockEntry's r3.
// 112-byte frame.
uint64_t EventStart(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225C648 SetUpTexts (never seen in play): movie 0's header texts and
// nodes. The last FindNode. 128-byte frame.
uint64_t SetUpTexts(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225BB70 NextUnlockEntry (never seen in play): 0 the entry's next
// page, 1 the next unlock, 2 none left. 96-byte frame.
uint32_t NextUnlockEntry(uint8_t* base, uint32_t sp);
// sub_8225C1F8 Unload (never seen in play): FinishAll and the +28 object's
// words put back when movie 0 is there; with +9616 set, UiMovies::Delete(0)
// and +9608 = 0. The last call's r3. 128-byte frame.
uint64_t EventUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8225C278 Unloaded (never seen in play): the screen's sound stopped
// when playing (the sound manager's slots 14 and 13 through the guest);
// message 6 posted; +9604 = 9. The last slot's r3. 144-byte frame.
uint64_t EventUnloaded(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                       const Calls& calls);
// sub_8225C348 Leave: +9604 = 7; FinishAll and the +28 object's words put
// back when movie 0 is there; UiMovies::Delete(0); message 4 posted.
// Delete's r3. 112-byte frame.
uint64_t EventLeave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                    const Calls& calls);
// sub_8225BF78 Destroy: +9604 = 20 when movies were queued; FreeImages; the
// three blocks VirtualFree'd and their globals 0. The last VirtualFree's
// r3. 128-byte frame.
uint64_t EventDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls);
// sub_8225BC60 FreeImages (r3 the data): each list emptied from its end,
// Type2Hub::FreeImage and VirtualFree of each record. The last VirtualFree's
// r3, else `r3`. 112-byte frame.
uint64_t FreeImages(uint8_t* base, uint32_t data, uint32_t sp, const Calls& calls);

// sub_8225C928 Draw (never seen in play): the two pages' addresses at the
// frame's +80; unless the page (+0) is -1, the page called with r3 the
// dispatcher and r4 the machine (through the guest). Its r3, else the
// dispatcher. 96-byte frame.
uint64_t EventDraw(uint8_t* base, uint64_t machine, uint64_t dispatcher, uint32_t sp,
                   const Calls& calls);
// sub_8225CDE8 Update (never seen in play): movie 0; the transition (+12)
// from the 6-entry table at the frame's +96 called with the page object's
// +24 (through the guest), the queued one or the page after started on a
// nonzero answer; with none (-1), UiMovie::Run(movie, 0). The transition's
// or Run's r3, or the movie. 144-byte frame.
uint64_t EventUpdate(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225C428 EventPage (never seen in play; r3 the dispatcher): on
// Menu::Pressed, MenuSound::Play(0) and the text revealed (RevealText) or
// the next page, unlock or end (NextUnlockEntry). See the row. 112-byte
// frame.
uint64_t EventPage(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225C590 EndPage (never seen in play; r3 the dispatcher, r4 the
// machine): Kind 1: message 7, the data's +408 = 0; else +9604 = 10, +408 =
// 1, QueueEventMovies. 112-byte frame.
uint64_t EndPage(uint8_t* base, uint32_t dispatcher, uint32_t machine, uint32_t sp,
                 const Calls& calls);
// sub_8225B9B8 QueueEventMovies (never seen in play): the boot movies' list
// from the save's movie (7 after 6), this mode after;
// Progress::CheckUnlocks' plain C++ (the original through the guest where
// Condition::Test's copies can't be native). Its r3. 96-byte frame.
uint64_t QueueEventMovies(uint8_t* base, uint32_t sp, const Calls& calls);
// The transitions (never seen in play; r3 the started word; 112-byte
// frames): sub_8225CB18 Intro, sub_8225C770 FadeOut, sub_8225CC80
// NextUnlock and sub_8225CD50 NextPage.
uint64_t EventIntro(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t EventFadeOut(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t NextUnlock(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
uint64_t NextPage(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls);
// sub_8225CA50 ShowUnlock (never seen in play): the unlock shown's title
// and body texts, its images, the more-text arrow. The last FindNode.
// 128-byte frame.
uint64_t ShowUnlock(uint8_t* base, uint32_t sp, const Calls& calls);
// sub_8225C848 ShowImages (never seen in play; r3 an image index): movie
// 0's +28 object's +36 and +32 the texture records of the lists' first
// records for the index (0 none). UiMovies::Find's r3. 112-byte frame.
uint64_t ShowImages(uint8_t* base, uint32_t index, uint32_t sp);

}  // namespace todays_result
