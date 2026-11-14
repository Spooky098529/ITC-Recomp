// The type 6 and 7 modes' list, panel and node screens natively: adapters
// in front of src/type67_screens/ (docs/functions/type67-screens.md), with
// the register steps the harness shares (hooks/type67_screens.h). The
// handlers are reached through their machines' transitions (functors) and
// the slots through the screens' vtables, so their adapters stay.

#include "hooks/type67_screens.h"

#include "base/flags.h"
#include "hooks/heap.h"
#include "hooks/strings_and_font.h"
#include "hooks/type67_modes.h"
#include "ui_movies/game.h"

ITC_FLAG_BOOL(native_type67_screens, true, "ITC",
              "Run the type 6 and 7 modes' list, panel and node screens "
              "(docs/functions/type67-screens.md: the list screens' ListScreen::OnEnter "
              "sub_82251618, OpenMovie sub_82251508, WaitForMovie sub_82251678, Leave "
              "sub_82251760, OnLeft sub_82251710 and IsLoaded sub_82242F70, "
              "Type6ListScreen::OpenMovie sub_82233320 and Release sub_82233330, "
              "Type7ListScreen::OpenMovie sub_82242F10 and Release sub_82242F20; the node "
              "screen's NodeScreen::OnEnter sub_822591B0, OpenMovie sub_822590D0, WaitForMovie "
              "sub_82259208, Leave sub_822592D8 and OnLeft sub_82259298, "
              "Type6NodeScreen::IsLoaded sub_82236FD8, OpenMovie sub_82236EB0 and Release "
              "sub_82236EC0; the panel screens' PanelScreen::OnEnter sub_82249D18, OpenMovie "
              "sub_82248CD0, WaitForMovie sub_82249D88, Open sub_82249DF8, Show sub_82248CE0, "
              "Update sub_82248C48, OnMessage24 sub_82248DF8, Leave sub_82249E38, OnLeft "
              "sub_82248B08, DrawList sub_82248B58, IsLoaded sub_82230790 and DeleteMovies "
              "sub_82240208, Type6PanelScreen::OpenMovies sub_82230758 and OpenPowerGraph "
              "sub_82247058, Type7PanelScreen::Draw sub_82240958, DrawList sub_82240908, "
              "OpenMovies sub_822401D0, PickCar sub_822405F8 (GarageCar::Reset a guest call, "
              "memcpy's and Car::SetModelAndClass's plain C++), Back sub_82240860, "
              "OpenPowerGraph sub_822408B8, CarList sub_82240238, CarEntry sub_82240248 and "
              "CarSlot sub_82240258; the group pair's GroupPair::SelectA sub_82249C30, Reset "
              "sub_82249C40, Run sub_82249BE0, WaitForFade sub_82249BA8 and FinishAll "
              "sub_82249C70, GroupPair::~GroupPair sub_82249C20, SingleGroup::SingleGroup "
              "sub_82249CB0 and Reset sub_82249CC8, PanelFade::In sub_82249BE8 (ScreenFades::FadeIn "
              "a guest call), MachineScreen::~MachineScreen sub_8224CB18, PanelGroup::Movie sub_82258FC8 and Run sub_82259008 "
              "(UiMovie::ResetPose, FinishAll and Run guest calls to their overrides); their "
              "vtable calls to the plain C++ where a slot holds one, else guest calls (the car "
              "list's); UiMovies::Open and Delete natively where their adapters would, "
              "MenuList::Release's plain C++ over the allocation layer; the node tables' "
              "NodeTable::~NodeTable sub_82247DC0, DeletingDestructor sub_82247E88 (operator "
              "delete a guest call), SetPairs sub_82247CB8, Show sub_82248300, ShowNode "
              "sub_82247ED0 and SetMarks sub_822480F0, PairedNodeTable::SavePairs sub_82247CE0 and "
              "Refresh sub_82248800, ColorNodeTable::Clear sub_82247DD0, Set sub_82247E00, "
              "SavePairs sub_82247E50, Reset sub_82248650 (UiMovie::SetText's plain C++ where the "
              "text setters are native), Refresh sub_822483B0, MarkSelected sub_822487B8, "
              "SetColors sub_82248198 and ClearColors sub_82248260, UiMovie::FinishNamed, "
              "PlayNamed and LoopNamed guest calls; the list part's ListPartBase::Open "
              "sub_82252430, Pick sub_82252490, Input sub_82252500, Close sub_82252530, IsIdle "
              "sub_82252590, CanLeave sub_822525B8, ~ListPartBase sub_82252420 and "
              "DeletingDestructor sub_82252610, ListPart::~ListPart sub_82252968 and "
              "DeletingDestructor sub_82252A38, ListPartInnerBase::~ListPartInnerBase "
              "sub_82252600 and DeletingDestructor sub_82252658, and its inner part's "
              "ListPartInner::~ListPartInner sub_822526F8, DeletingDestructor sub_822529B0, Open "
              "sub_82252A88, Pick sub_82252A00, Input sub_82252DF0, IsIdle sub_82252748, Close "
              "sub_82252740, CanLeave sub_822528C8, Page0 sub_82252778, Page1 sub_82252878, Page2 "
              "sub_822528A8, Step0 sub_82252BB8, Step1 sub_82252C98 and Step2 sub_82252D38 "
              "(their slots and page and step tables to the plain C++, MenuList::Init's and "
              "Release's plain C++ where they free natively, MenuList::Update, Accepted and "
              "Cancelled, UiMovie::ResetPose, Run, FinishAll and FinishNamed guest calls, "
              "PlayNamed, LoopLast and MenuSound::Play's plain C++); the list screens' "
              "ListScreen::ShowSelection sub_822518B0, LoopItemNode sub_82251A20, "
              "LoopSecondNode sub_82251B80, LoopThirdNode sub_82251AD0, SetScrollText "
              "sub_82251D00, ClearScrollText sub_82251DA0, SetArrows sub_82251E10, OnToggleMoved "
              "sub_82252068, ShowToggle sub_82252198, SetIcons sub_82251C30, SetUpMovie "
              "sub_82251EC8, ~ListScreen sub_82251EB8, DeletingDestructor sub_82252248, Show "
              "sub_82251F98, Draw sub_82251520, Update sub_822517B8, Suspend sub_82251580, Resume "
              "sub_82251590 and Open sub_822517A0, NodeListScreen::ItemInfo sub_82256FE8 and "
              "LoadItem sub_82257060, ColorListScreen::LoadItem sub_82255238 (UiMovie::FindNode, "
              "ShowNode, HideNode and SetNodeTexture's plain C++, FinishNamed, PlayNamed, "
              "LoopNamed, ResetPose, Run, ClearText, ScrollText and sub_8211C5F8 guest calls, "
              "SetText's plain C++ where the text setters are native, their slots to the plain "
              "C++ where a slot holds one); the titled list screen's TitledListScreen::Reset "
              "sub_82257558, Draw sub_822579E8 and Update sub_82258998 (their page and step "
              "tables built on first use, the same words and guards), Page2 sub_82257148, Page3 "
              "sub_822572F0, Page4 sub_82257360, Page7 sub_82257490, Step0 sub_82257D48, Step1 "
              "sub_82257FF8, Step2 sub_822580E0, OnItemMoved sub_82257B78, ~TitledListScreen "
              "sub_82257138 and DeletingDestructor sub_82257990, ListScreen::LeavePage "
              "sub_82255818, NodeListScreen::~NodeListScreen sub_82257000 and DeletingDestructor "
              "sub_82257940 (ScreenFades::FadeIn and FadeOut, MenuSound::Play, UiMovie::FindNode "
              "and Child's plain C++, MenuList::Update, Accepted and Cancelled, UiMovie::Run, "
              "FinishAll, FinishNamed, PlayNamed, LoopNamed and UnloopNamed guest calls, the "
              "table entries and slots to the plain C++ where they hold one); the color list "
              "screen's ColorListScreen::Reset sub_82255200, Draw sub_82256CA0 and Update "
              "sub_82256E08 (their tables built on first use, the same words and guards), Page2 "
              "sub_822552B8, Page3 sub_82255490, Page4 sub_82255658, Page5 sub_82256508, Page6 "
              "sub_822565A0, Page7 sub_822557F0, Page9 sub_822562E0, PlaceItemCursor "
              "sub_822561D0, PlaceColorCursor sub_82256258, ShowPartNodes sub_82256360, "
              "~ColorListScreen sub_82255198 and DeletingDestructor sub_82255840, its steps "
              "Step0 sub_82258578, Step1 sub_82255890, Step2 sub_82256740, Step3 sub_82255B58, "
              "Step4 sub_82256908, RunMovie sub_82255C88, Step6 sub_82255CD0, Step7 "
              "sub_82256AC0, Step8 sub_82256030 and Step10 sub_82255EB0 (the same "
              "callees, with MenuList::SetSelection, UiMovie::ShowNode and HideNode's plain "
              "C++); the type 6 "
              "list screen's Type6ListScreen::SetItemTexts sub_822336C0, Back sub_822337B0, "
              "SetUpRace sub_822334C8 (with its switch piece AddRacers sub_8223356C; the race "
              "set-up's plain C++: RaceData::ResetGlobal, Stages::Clear, SetRoute and AddRacer, "
              "Race::SetRoute, AddRacer, AddStage and PlaceRacers, RacerRecord::Init, "
              "Car::SetModel and Refresh, memcpy, the race data's setters; guest calls where "
              "they decline), SetUpMenus sub_82233378 (MenuList::Init's plain C++ where it "
              "frees natively), ShowMarks sub_82233730, ItemInfo sub_82237008, "
              "~Type6ListScreen sub_822332D8 and DeletingDestructor sub_82233478; "
              "NodeScreen::Open sub_82259318 and ItemInfo sub_82259998; PanelScreen::OnYes "
              "sub_822609E0, OnNo sub_822609D0, OnBack sub_822306D0, OnDialogClosed "
              "sub_822306E0 (ScreenFades::FadeOut's plain C++), Suspend sub_82248DE0 and "
              "OpenEmpty sub_822404D8; Type6PanelScreen::Back sub_8223E708, CarList "
              "sub_822307D0, CarEntry sub_822307E0 and CarSlot sub_822307F0; "
              "GroupPair::DeletingDestructor sub_8224ACE0 (the sound manager's slot 13 and "
              "operator delete guest calls); the type 7 list screen's "
              "Type7ListScreen::SetItemText sub_82243238, ClearItemText sub_822432A8, Back "
              "sub_822434C8, SetUpRace sub_82243018 (with its switch piece AddRacers "
              "sub_822430BC; the race set-up's plain C++ as type 6's), SetUpMenus sub_82243588, "
              "SetTitle sub_822433D8, SetDetails sub_82243308 (MenuList::Init's plain C++ where "
              "it frees natively; UiMovie::ClearText guest calls), ShowMarks sub_82243448, "
              "ItemInfo sub_82242FA0 (memcpy's plain C++), ~Type7ListScreen sub_82242EC8 and "
              "DeletingDestructor sub_82242FC8; the node screen's NodeScreen::Show "
              "sub_82259CA0, Draw sub_8225A1B8 and Update sub_8225A2C0 (their page and step "
              "tables built in .data as the originals do), Page2 sub_82259D80, Page4 "
              "sub_822590E8, Page5 sub_82259328, Step0 sub_82259EC0, Step1 sub_82259680, Step2 "
              "sub_82259750, ShowSelection sub_82259A78, OnToggleMoved sub_82259B70, Refresh "
              "sub_822599B0, ShowRecords sub_82259410 (SaveData::Of's and CourseRecords::Of's "
              "plain C++), SetArrows sub_82259368, LoadItem sub_82259050, ~NodeScreen "
              "sub_82259948 and DeletingDestructor sub_8225A168; the type 6 node screen's "
              "Type6NodeScreen::SetUpMenus sub_82236F00, SetText sub_822370A0, Back "
              "sub_82237110, SetIcons sub_82237160, SetScrollText sub_82237228, ShowMarks "
              "sub_822372A0, ~Type6NodeScreen sub_82236EA0 and DeletingDestructor "
              "sub_82237048, NodeScreen::SetUpTexts sub_82247FE0; Type6PanelScreen::Draw "
              "sub_82230F40 (with DrawList sub_82230EF0 and AskQuit sub_82230BA8) and PickCar "
              "sub_82230C88, Type7PanelScreen::AskQuit sub_82240518; the screen blocks' "
              "Type6Screens::~Type6Screens sub_822367C0 and DeletingDestructor sub_82236830, "
              "Type7Screens::~Type7Screens sub_82246BE0 and DeletingDestructor sub_82246C40 "
              "natively instead of "
              "through the translated code (the same writes; checked by "
              "--run_harnesses=type67_screens)");

namespace hooks {

namespace ts = type67_screens;

ts::Calls Type67ScreensCalls(PPCContext& ctx, uint8_t* base) {
  ts::Calls calls;
  static_cast<type67_modes::Calls&>(calls) = Type67Calls(ctx, base);
  calls.native_heap = NativeHeapActive();
  calls.set_r4 = [&ctx](uint64_t r4) { ctx.r4.u64 = r4; };
  const auto call_set = calls.call_set;
  calls.set_text_args = [&ctx, base, call_set](uint32_t frame, uint32_t lr, uint32_t movie,
                                               uint32_t sub_scene, uint32_t child, uint32_t id,
                                               const uint64_t varargs[4]) -> uint64_t {
    if (Type67TextsNative(base)) {
      const uint64_t stack = ctx.r1.u64, link = ctx.lr;
      ctx.fpscr.disableFlushMode();
      std::optional<uint32_t> r3;
      {
        FormatGuestCalls format(ctx, base);
        r3 = ui_movies::SetText(base, format.get(), frame, movie, sub_scene, child, id, varargs);
      }
      ctx.r1.u64 = stack;
      ctx.lr = link;
      if (r3) return *r3;
    }
    // The original (through its adapter) with r7 to r10 from `varargs`.
    ctx.r9.u64 = varargs[2];
    ctx.r10.u64 = varargs[3];
    const uint64_t args[6] = {movie, sub_scene, child, id, varargs[0], varargs[1]};
    return call_set(0x8211C1F8, args, 6, frame, lr);
  };
  calls.set_text = [set_text_args = calls.set_text_args](uint32_t frame, uint32_t lr,
                                                         uint32_t movie, uint32_t sub_scene,
                                                         uint32_t child,
                                                         uint32_t id) -> uint64_t {
    static constexpr uint64_t kNoArguments[4] = {0, 0, 0, 0};
    return set_text_args(frame, lr, movie, sub_scene, child, id, kNoArguments);
  };
  calls.call8 = [&ctx, call_set](uint32_t function, const uint64_t args[8], uint32_t r1,
                                 uint32_t lr) -> uint64_t {
    ctx.r9.u64 = args[6];
    ctx.r10.u64 = args[7];
    return call_set(function, args, 6, r1, lr);
  };
  calls.call_f1 = [&ctx, call_set](uint32_t function, const uint64_t args[4], double f1,
                                   uint32_t r1, uint32_t lr) -> uint64_t {
    ctx.f1.f64 = f1;
    const uint64_t set[6] = {args[0], args[1], args[2], args[3], 0, 0};
    return call_set(function, set, 4, r1, lr);
  };
  return calls;
}

#define ITC_TS_STEP(name, expression)                \
  void name(PPCContext& ctx, uint8_t* base) {        \
    const ts::Calls calls = Type67ScreensCalls(ctx, base); \
    (void)calls;                                     \
    ctx.r3.u64 = (expression);                       \
  }

ITC_TS_STEP(ListScreenOnEnter, ts::ListScreenOnEnter(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenOpenMovie,
            ts::ListScreenOpenMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListScreenWaitForMovie,
            ts::ListScreenWaitForMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenLeave, ts::ListScreenLeave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenOnLeft, ts::ListScreenOnLeft(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenIsLoaded, ts::ListScreenIsLoaded(base, ctx.r3.u64, ctx.r1.u32))
ITC_TS_STEP(Type6ListScreenOpenMovie,
            ts::ListScreenOpenAsset(base, ctx.r3.u64, false, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(Type7ListScreenOpenMovie,
            ts::ListScreenOpenAsset(base, ctx.r3.u64, true, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(Type6ListScreenRelease, ts::ListScreenRelease(base, ctx.r3.u64, false, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ListScreenRelease, ts::ListScreenRelease(base, ctx.r3.u64, true, ctx.r1.u32, calls))

ITC_TS_STEP(NodeScreenOnEnter, ts::NodeScreenOnEnter(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenOpenMovie,
            ts::NodeScreenOpenMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(NodeScreenWaitForMovie,
            ts::NodeScreenWaitForMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenLeave, ts::NodeScreenLeave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenOnLeft, ts::NodeScreenOnLeft(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6NodeScreenIsLoaded, ts::NodeScreenIsLoaded(base, ctx.r3.u64, ctx.r1.u32))
ITC_TS_STEP(Type6NodeScreenOpenMovie,
            ts::NodeScreenOpenAsset(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(Type6NodeScreenRelease, ts::NodeScreenRelease(base, ctx.r3.u64, ctx.r1.u32, calls))

ITC_TS_STEP(PanelGroupMovie, ts::PanelGroupMovie(base, ctx.r3.u64, ctx.r1.u32))
ITC_TS_STEP(GroupPairSelectA, ts::GroupPairSelectA(base, ctx.r3.u64))
ITC_TS_STEP(GroupPairReset, ts::GroupPairReset(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(GroupPairRun, ts::GroupPairRun(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(GroupPairWaitForFade, ts::GroupPairWaitForFade(base, ctx.r3.u64))
ITC_TS_STEP(GroupPairFinishAll, ts::GroupPairFinishAll(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(GroupPairDestruct, ts::GroupPairDestruct(base, ctx.r3.u64))
ITC_TS_STEP(SingleGroupConstruct, ts::SingleGroupConstruct(base, ctx.r3.u64, ctx.r4.u32))
ITC_TS_STEP(SingleGroupReset, ts::SingleGroupReset(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelFadeIn, ts::PanelFadeIn(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(MachineScreenDestruct, ts::MachineScreenDestruct(base, ctx.r3.u64))
ITC_TS_STEP(PanelGroupDestruct, ts::PanelGroupDestruct(base, ctx.r3.u64))
ITC_TS_STEP(PanelGroupDeletingDestruct,
            ts::PanelGroupDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6PanelScreenDestruct,
            ts::PanelScreenDestruct(base, ctx.r3.u64, false, ctx.r1.u32, calls))
ITC_TS_STEP(Type7PanelScreenDestruct,
            ts::PanelScreenDestruct(base, ctx.r3.u64, true, ctx.r1.u32, calls))
ITC_TS_STEP(Type6PanelScreenDeletingDestruct,
            ts::Type6PanelScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

ITC_TS_STEP(PanelScreenIsLoaded, ts::PanelScreenIsLoaded(base, ctx.r1.u32))
ITC_TS_STEP(Type6PanelScreenOpenMovies, ts::PanelScreenOpenMovies(base, ctx.r1.u32, false, calls))
ITC_TS_STEP(Type7PanelScreenOpenMovies, ts::PanelScreenOpenMovies(base, ctx.r1.u32, true, calls))
ITC_TS_STEP(PanelScreenDeleteMovies, ts::PanelScreenDeleteMovies(base, ctx.r1.u32, calls))
ITC_TS_STEP(Type7PanelScreenBack, ts::PanelScreenPost(base, ctx.r3.u64, ctx.r4.u64, 0x82240860))
ITC_TS_STEP(Type6PanelScreenOpenPowerGraph,
            ts::PanelScreenPost(base, ctx.r3.u64, ctx.r4.u64, 0x82247058))
ITC_TS_STEP(Type7PanelScreenOpenPowerGraph,
            ts::PanelScreenPost(base, ctx.r3.u64, ctx.r4.u64, 0x822408B8))
ITC_TS_STEP(Type7PanelScreenCarList, ts::Type7CarWord(base, 1972))
ITC_TS_STEP(Type7PanelScreenCarEntry, ts::Type7CarWord(base, 1976))
ITC_TS_STEP(Type7PanelScreenCarSlot, ts::Type7CarWord(base, 1980))
ITC_TS_STEP(PanelScreenOnEnter, ts::PanelScreenOnEnter(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOpenMovie,
            ts::PanelScreenOpenMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(PanelScreenWaitForMovie,
            ts::PanelScreenWaitForMovie(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOpen, ts::PanelScreenOpen(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(PanelScreenShow, ts::PanelScreenShow(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenUpdate, ts::PanelScreenUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOnMessage24,
            ts::PanelScreenOnMessage24(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(PanelScreenLeave, ts::PanelScreenLeave(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOnLeft, ts::PanelScreenOnLeft(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7PanelScreenPickCar,
            ts::Type7PanelScreenPickCar(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7PanelScreenDraw,
            ts::Type7PanelScreenDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

ITC_TS_STEP(NodeTableDestruct, ts::NodeTableDestruct(base, ctx.r3.u64))
ITC_TS_STEP(NodeTableDeletingDestruct,
            ts::NodeTableDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeTableSetPairs, ts::NodeTableSetPairs(base, ctx.r3.u64, ctx.r4.u32))
ITC_TS_STEP(PairedNodeTableSavePairs, ts::PairedNodeTableSavePairs(base, ctx.r3.u64))
ITC_TS_STEP(ColorNodeTableClear, ts::ColorNodeTableClear(base, ctx.r3.u64))
ITC_TS_STEP(ColorNodeTableSet, ts::ColorNodeTableSet(base, ctx.r3.u64, ctx.r4.u32))
ITC_TS_STEP(ColorNodeTableSavePairs, ts::ColorNodeTableSavePairs(base, ctx.r3.u64))
ITC_TS_STEP(NodeTableShow,
            ts::NodeTableShow(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32))
ITC_TS_STEP(NodeTableShowNode, ts::NodeTableShowNode(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32,
                                                     ctx.r6.u32, ctx.r1.u32))
ITC_TS_STEP(NodeTableSetMarks,
            ts::NodeTableSetMarks(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32))
ITC_TS_STEP(ColorNodeTableSetColors,
            ts::ColorNodeTableSetColors(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32))
ITC_TS_STEP(ColorNodeTableClearColors,
            ts::ColorNodeTableClearColors(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32))
ITC_TS_STEP(ColorNodeTableMarkSelected,
            ts::ColorNodeTableMarkSelected(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32))
ITC_TS_STEP(ColorNodeTableRefresh,
            ts::ColorNodeTableRefresh(base, ctx.r3.u64, ctx.r4.u32, ctx.r1.u32, calls))
ITC_TS_STEP(ColorNodeTableReset, ts::ColorNodeTableReset(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PairedNodeTableRefresh,
            ts::PairedNodeTableRefresh(base, ctx.r3.u64, ctx.r1.u32, calls))

ITC_TS_STEP(ListPartBaseDestruct, ts::ListPartBaseDestruct(base, ctx.r3.u64))
ITC_TS_STEP(ListPartInnerBaseDestruct, ts::ListPartInnerBaseDestruct(base, ctx.r3.u64))
ITC_TS_STEP(ListPartBaseDeletingDestruct,
            ts::ListPartBaseDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInnerBaseDeletingDestruct,
            ts::ListPartInnerBaseDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInnerDestruct, ts::ListPartInnerDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInnerDeletingDestruct,
            ts::ListPartInnerDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartDestruct, ts::ListPartDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartDeletingDestruct,
            ts::ListPartDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartOpen, ts::ListPartOpen(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartPick, ts::ListPartPick(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInput,
            ts::ListPartInput(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListPartClose, ts::ListPartClose(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartIsIdle,
            ts::ListPartIsIdle(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListPartCanLeave,
            ts::ListPartCanLeave(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListPartInnerOpen,
            ts::ListPartInnerOpen(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInnerPick, ts::ListPartInnerPick(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64,
                                                     ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListPartInnerInput,
            ts::ListPartInnerInput(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInnerIsIdle, ts::ListPartInnerIsIdle(base, ctx.r3.u64, ctx.r1.u32))
ITC_TS_STEP(ListPartInnerClose,
            ts::ListPartInnerClose(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListPartInnerCanLeave, ts::ListPartInnerCanLeave(base, ctx.r3.u64))
ITC_TS_STEP(ListPartInnerPage0, ts::ListPartInnerPage0(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInnerPage1, ts::ListPartInnerPage1(base, ctx.r3.u64))
ITC_TS_STEP(ListPartInnerPage2, ts::ListPartInnerPage2(base, ctx.r3.u64))
ITC_TS_STEP(ListPartInnerStep0,
            ts::ListPartInnerStep0(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInnerStep1,
            ts::ListPartInnerStep1(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListPartInnerStep2,
            ts::ListPartInnerStep2(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

ITC_TS_STEP(ListScreenDestruct, ts::ListScreenDestruct(base, ctx.r3.u64))
ITC_TS_STEP(ListScreenDeletingDestruct,
            ts::ListScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeListScreenItemInfo, ts::NodeListScreenItemInfo(base, ctx.r3.u64, ctx.r5.u32))
ITC_TS_STEP(NodeListScreenLoadItem,
            ts::NodeListScreenLoadItem(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenLoadItem, ts::ColorListScreenLoadItem(base, ctx.r3.u64, ctx.r4.u64,
                                                                 ctx.r5.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenShowSelection,
            ts::ListScreenShowSelection(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenLoopItemNode,
            ts::ListScreenLoopNode(base, ctx.r3.u64, ctx.r4.u64, 9936, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenLoopSecondNode,
            ts::ListScreenLoopNode(base, ctx.r3.u64, ctx.r4.u64, 9976, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenLoopThirdNode,
            ts::ListScreenLoopNode(base, ctx.r3.u64, ctx.r4.u64, 10016, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenSetScrollText, ts::ListScreenSetScrollText(base, ctx.r3.u64, ctx.r4.u64,
                                                                 ctx.r5.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenClearScrollText,
            ts::ListScreenClearScrollText(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenSetArrows,
            ts::ListScreenSetArrows(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32))
ITC_TS_STEP(ListScreenOnToggleMoved,
            ts::ListScreenOnToggleMoved(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenShowToggle,
            ts::ListScreenShowToggle(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenSetIcons, ts::ListScreenSetIcons(base, ctx.r3.u64, ctx.r1.u32))
ITC_TS_STEP(ListScreenSetUpMovie, ts::ListScreenSetUpMovie(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenShow, ts::ListScreenShow(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenDraw, ts::ListScreenDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenUpdate, ts::ListScreenUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ListScreenSuspend, ts::ListScreenTail(base, ctx.r3.u64, ctx.r4.u64, 64, ctx.r1.u32,
                                                  uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListScreenResume, ts::ListScreenTail(base, ctx.r3.u64, ctx.r4.u64, 68, ctx.r1.u32,
                                                 uint32_t(ctx.lr), calls))
ITC_TS_STEP(ListScreenOpen, ts::ListScreenTail(base, ctx.r3.u64, ctx.r4.u64, 60, ctx.r1.u32,
                                               uint32_t(ctx.lr), calls))
ITC_TS_STEP(TitledListScreenReset, ts::TitledListScreenReset(base, ctx.r3.u64))
ITC_TS_STEP(TitledListScreenDraw,
            ts::TitledListScreenDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenUpdate,
            ts::TitledListScreenUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenPage2, ts::TitledListScreenPage2(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenPage3, ts::TitledListScreenPage3(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenPage4,
            ts::TitledListScreenPage4(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenPage7, ts::TitledListScreenPage7(base, ctx.r3.u64))
ITC_TS_STEP(ListScreenLeavePage, ts::ListScreenLeavePage(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32,
                                                         uint32_t(ctx.lr), calls))
ITC_TS_STEP(TitledListScreenStep0, ts::TitledListScreenStep0(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenStep1, ts::TitledListScreenStep1(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenStep2, ts::TitledListScreenStep2(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenOnItemMoved,
            ts::TitledListScreenOnItemMoved(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenDestruct,
            ts::TitledListScreenDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeListScreenDestruct, ts::NodeListScreenDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(TitledListScreenDeletingDestruct,
            ts::NodeListScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, true, ctx.r1.u32,
                                               calls))
ITC_TS_STEP(NodeListScreenDeletingDestruct,
            ts::NodeListScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, false, ctx.r1.u32,
                                               calls))
ITC_TS_STEP(ColorListScreenReset, ts::ColorListScreenReset(base, ctx.r3.u64))
ITC_TS_STEP(ColorListScreenDraw,
            ts::ColorListScreenDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenUpdate,
            ts::ColorListScreenUpdate(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenPage2, ts::ColorListScreenPage2(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenPage3, ts::ColorListScreenPage3(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenPage4, ts::ColorListScreenPage4(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenPage5, ts::ColorListScreenPage5(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenPage6,
            ts::ColorListScreenPage6(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenPage7, ts::ColorListScreenPage7(base, ctx.r3.u64))
ITC_TS_STEP(ColorListScreenPage9, ts::ColorListScreenPage9(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenPlaceItemCursor,
            ts::ColorListScreenPlaceItemCursor(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32))
ITC_TS_STEP(ColorListScreenPlaceColorCursor,
            ts::ColorListScreenPlaceColorCursor(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32))
ITC_TS_STEP(ColorListScreenShowPartNodes,
            ts::ColorListScreenShowPartNodes(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenDestruct,
            ts::ColorListScreenDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenDeletingDestruct,
            ts::ColorListScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep0, ts::ColorListScreenStep0(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep1, ts::ColorListScreenStep1(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep2, ts::ColorListScreenStep2(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep3, ts::ColorListScreenStep3(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep4, ts::ColorListScreenStep4(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenRunMovie, ts::ColorListScreenRunMovie(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep6, ts::ColorListScreenStep6(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep7, ts::ColorListScreenStep7(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep8, ts::ColorListScreenStep8(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(ColorListScreenStep10, ts::ColorListScreenStep10(base, ctx.r3.u64, ctx.r1.u32, calls))

// The type 6 list screen's slots and destructors, the panel and node
// screens' leaves.
ITC_TS_STEP(Type6ListScreenSetItemTexts,
            ts::Type6ListScreenSetItemTexts(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6ListScreenShowMarks,
            ts::Type6ListScreenShowMarks(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32))
ITC_TS_STEP(Type6ListScreenItemInfo,
            ts::Type6ListScreenItemInfo(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u32))
ITC_TS_STEP(NodeScreenItemInfo, ts::NodeScreenItemInfo(base, ctx.r3.u64, ctx.r5.u32))
ITC_TS_STEP(Type6ListScreenSetUpMenus,
            ts::Type6ListScreenSetUpMenus(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6ListScreenBack,
            ts::Type6ListScreenBack(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
void Type6ListScreenSetUpRace(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const ts::Calls calls = Type67ScreensCalls(ctx, base);
  ctx.r3.u64 = ts::Type6ListScreenSetUpRace(base, ctx.r3.u64, ctx.r1.u32, calls);
}
ITC_TS_STEP(Type6ListScreenDestruct,
            ts::Type6ListScreenDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6ListScreenDeletingDestruct,
            ts::Type6ListScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenOnYes, ts::PanelScreenSetAnswer(base, ctx.r3.u64, 0))
ITC_TS_STEP(PanelScreenOnNo, ts::PanelScreenSetAnswer(base, ctx.r3.u64, 1))
ITC_TS_STEP(PanelScreenOnBack, ts::PanelScreenSetAnswer(base, ctx.r3.u64, 2))
ITC_TS_STEP(PanelScreenOnDialogClosed,
            ts::PanelScreenOnDialogClosed(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(PanelScreenSuspend,
            ts::PanelScreenSuspend(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(PanelScreenOpenEmpty,
            ts::PanelScreenOpenEmpty(base, ctx.r3.u64, ctx.r1.u32, uint32_t(ctx.lr), calls))
ITC_TS_STEP(Type6PanelScreenBack, ts::PanelScreenPost(base, ctx.r3.u64, ctx.r4.u64, 0x8223E708))
ITC_TS_STEP(Type6PanelScreenCarList, ts::Type6CarWord(base, 1972))
ITC_TS_STEP(Type6PanelScreenCarEntry, ts::Type6CarWord(base, 1976))
ITC_TS_STEP(Type6PanelScreenCarSlot, ts::Type6CarWord(base, 1980))
ITC_TS_STEP(GroupPairDeletingDestruct,
            ts::GroupPairDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenOpen, ts::NodeScreenOpen(base, ctx.r3.u64))

// The type 7 list screen's slots and destructors.
ITC_TS_STEP(Type7ListScreenSetItemText,
            ts::Type7ListScreenSetItemText(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ListScreenClearItemText,
            ts::Type7ListScreenClearItemText(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ListScreenSetTitle,
            ts::Type7ListScreenSetTitle(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ListScreenSetDetails,
            ts::Type7ListScreenSetDetails(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ListScreenShowMarks,
            ts::Type7ListScreenShowMarks(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32))
ITC_TS_STEP(Type7ListScreenItemInfo,
            ts::Type7ListScreenItemInfo(base, ctx.r4.u64, ctx.r5.u64, ctx.r1.u32,
                                        uint32_t(ctx.lr), calls))
ITC_TS_STEP(Type7ListScreenSetUpMenus,
            ts::Type7ListScreenSetUpMenus(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ListScreenBack,
            ts::Type7ListScreenBack(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
void Type7ListScreenSetUpRace(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const ts::Calls calls = Type67ScreensCalls(ctx, base);
  ctx.r3.u64 = ts::Type7ListScreenSetUpRace(base, ctx.r3.u64, ctx.r1.u32, calls);
}
ITC_TS_STEP(Type7ListScreenDestruct,
            ts::Type7ListScreenDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ListScreenDeletingDestruct,
            ts::Type7ListScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

// The node screen's show handler, tables, pages, steps, helpers and
// destructors.
ITC_TS_STEP(NodeScreenShow, ts::NodeScreenShow(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenDraw, ts::NodeScreenDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenUpdate, ts::NodeScreenUpdate(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenPage2, ts::NodeScreenPage2(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenPage4, ts::NodeScreenPage4(base, ctx.r3.u64))
ITC_TS_STEP(NodeScreenPage5,
            ts::NodeScreenPage5(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenStep0, ts::NodeScreenStep0(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenStep1, ts::NodeScreenStep1(base, ctx.r3.u64, ctx.r1.u32, calls))
void NodeScreenStep2(PPCContext& ctx, uint8_t* base) {
  ctx.fpscr.disableFlushMode();
  const ts::Calls calls = Type67ScreensCalls(ctx, base);
  ctx.r3.u64 = ts::NodeScreenStep2(base, ctx.r3.u64, ctx.r1.u32, calls);
}
ITC_TS_STEP(NodeScreenShowSelection,
            ts::NodeScreenShowSelection(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenOnToggleMoved,
            ts::NodeScreenOnToggleMoved(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenRefresh, ts::NodeScreenRefresh(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenShowRecords, ts::NodeScreenShowRecords(base, ctx.r3.u64, ctx.r4.u64,
                                                            ctx.r5.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenSetArrows,
            ts::NodeScreenSetArrows(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, ctx.r1.u32))
ITC_TS_STEP(NodeScreenLoadItem, ts::NodeScreenLoadItem(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenDestruct, ts::NodeScreenDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(NodeScreenDeletingDestruct,
            ts::NodeScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

// The type 6 node screen's own slots and destructors.
ITC_TS_STEP(Type6NodeScreenSetUpMenus,
            ts::Type6NodeScreenSetUpMenus(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6NodeScreenSetText,
            ts::Type6NodeScreenSetText(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6NodeScreenBack, ts::Type6NodeScreenBack(base, ctx.r3.u64, ctx.r4.u64))
ITC_TS_STEP(Type6NodeScreenSetIcons, ts::Type6NodeScreenSetIcons(base, ctx.r1.u32))
ITC_TS_STEP(Type6NodeScreenSetScrollText,
            ts::Type6NodeScreenSetScrollText(base, ctx.r1.u32, calls))
ITC_TS_STEP(Type6NodeScreenShowMarks,
            ts::Type6NodeScreenShowMarks(base, ctx.r4.u64, ctx.r1.u32))
ITC_TS_STEP(NodeScreenSetUpTexts, ts::NodeScreenSetUpTexts(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6NodeScreenDestruct,
            ts::Type6NodeScreenDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6NodeScreenDeletingDestruct,
            ts::Type6NodeScreenDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

// The type 6 panel screen's Draw and PickCar.
ITC_TS_STEP(Type6PanelScreenDraw,
            ts::Type6PanelScreenDraw(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6PanelScreenPickCar,
            ts::Type6PanelScreenPickCar(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

// The screen blocks' destructors.
ITC_TS_STEP(Type6ScreensDestruct, ts::Type6ScreensDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ScreensDestruct, ts::Type7ScreensDestruct(base, ctx.r3.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type6ScreensDeletingDestruct,
            ts::Type6ScreensDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))
ITC_TS_STEP(Type7ScreensDeletingDestruct,
            ts::Type7ScreensDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, ctx.r1.u32, calls))

#undef ITC_TS_STEP

}  // namespace hooks

#define ITC_TYPE67_SCREENS(address, step)     \
  extern "C" REX_FUNC(sub_##address) {        \
    REX_FUNC_PROLOGUE();                      \
    if (!ITC_FLAG(native_type67_screens)) {   \
      __imp__sub_##address(ctx, base);        \
      return;                                 \
    }                                         \
    hooks::step(ctx, base);                   \
  }

// The list screens' handlers (through their machines' transitions) and
// vtable slots.
ITC_TYPE67_SCREENS(82251618, ListScreenOnEnter)
ITC_TYPE67_SCREENS(82251508, ListScreenOpenMovie)
ITC_TYPE67_SCREENS(82251678, ListScreenWaitForMovie)
ITC_TYPE67_SCREENS(82251760, ListScreenLeave)
ITC_TYPE67_SCREENS(82251710, ListScreenOnLeft)
ITC_TYPE67_SCREENS(82242F70, ListScreenIsLoaded)
ITC_TYPE67_SCREENS(82233320, Type6ListScreenOpenMovie)
ITC_TYPE67_SCREENS(82242F10, Type7ListScreenOpenMovie)
ITC_TYPE67_SCREENS(82233330, Type6ListScreenRelease)
ITC_TYPE67_SCREENS(82242F20, Type7ListScreenRelease)
// The node screen's handlers and vtable slots.
ITC_TYPE67_SCREENS(822591B0, NodeScreenOnEnter)
ITC_TYPE67_SCREENS(822590D0, NodeScreenOpenMovie)
ITC_TYPE67_SCREENS(82259208, NodeScreenWaitForMovie)
ITC_TYPE67_SCREENS(822592D8, NodeScreenLeave)
ITC_TYPE67_SCREENS(82259298, NodeScreenOnLeft)
ITC_TYPE67_SCREENS(82236FD8, Type6NodeScreenIsLoaded)
ITC_TYPE67_SCREENS(82236EB0, Type6NodeScreenOpenMovie)
ITC_TYPE67_SCREENS(82236EC0, Type6NodeScreenRelease)
// The group pair's slots (through its vtable) and PanelGroup::Movie (from
// the car list's functions, recompiled).
ITC_TYPE67_SCREENS(82258FC8, PanelGroupMovie)
ITC_TYPE67_SCREENS(82249C30, GroupPairSelectA)
ITC_TYPE67_SCREENS(82249C40, GroupPairReset)
ITC_TYPE67_SCREENS(82249BE0, GroupPairRun)
ITC_TYPE67_SCREENS(82249BA8, GroupPairWaitForFade)
ITC_TYPE67_SCREENS(82249C70, GroupPairFinishAll)
// The group pair's destructor, the one-group holder's constructor and
// Reset (its vtable's slot 5), the panel fade in and MachineScreen's
// destructor (from the type 6, 7 and 8 panel screens' functions,
// recompiled; never seen in play).
ITC_TYPE67_SCREENS(82249C20, GroupPairDestruct)
ITC_TYPE67_SCREENS(82249CB0, SingleGroupConstruct)
ITC_TYPE67_SCREENS(82249CC8, SingleGroupReset)
ITC_TYPE67_SCREENS(82249BE8, PanelFadeIn)
ITC_TYPE67_SCREENS(8224CB18, MachineScreenDestruct)
// The panel screens' destructors and their panel groups' (from the type 6
// and 7 blocks' destructors and the vtables' slot 0, recompiled; never seen
// in play): the car list's PanelSet::~PanelSet and PanelGroupBase's by
// their plain C++.
ITC_TYPE67_SCREENS(82231CC0, PanelGroupDestruct)
ITC_TYPE67_SCREENS(82240348, PanelGroupDeletingDestruct)
ITC_TYPE67_SCREENS(82230668, Type6PanelScreenDestruct)
ITC_TYPE67_SCREENS(82240168, Type7PanelScreenDestruct)
ITC_TYPE67_SCREENS(82230B00, Type6PanelScreenDeletingDestruct)
// The panel screens' vtable slots.
ITC_TYPE67_SCREENS(82230790, PanelScreenIsLoaded)
ITC_TYPE67_SCREENS(82230758, Type6PanelScreenOpenMovies)
ITC_TYPE67_SCREENS(822401D0, Type7PanelScreenOpenMovies)
ITC_TYPE67_SCREENS(82240208, PanelScreenDeleteMovies)
ITC_TYPE67_SCREENS(82240860, Type7PanelScreenBack)
ITC_TYPE67_SCREENS(82247058, Type6PanelScreenOpenPowerGraph)
ITC_TYPE67_SCREENS(822408B8, Type7PanelScreenOpenPowerGraph)
ITC_TYPE67_SCREENS(822405F8, Type7PanelScreenPickCar)
ITC_TYPE67_SCREENS(82240238, Type7PanelScreenCarList)
ITC_TYPE67_SCREENS(82240248, Type7PanelScreenCarEntry)
ITC_TYPE67_SCREENS(82240258, Type7PanelScreenCarSlot)
// The panel screens' handlers (through their machines' transitions).
// PanelScreen::DrawList sub_82248B58 has no adapter (removed, B7 loop,
// 2026-11-08): in play only Type7PanelScreen::DrawList (native) calls it;
// the other screens' draws that call it (sub_82230EF0, sub_82238DC8, never
// seen in play) run the original.
ITC_TYPE67_SCREENS(82249D18, PanelScreenOnEnter)
ITC_TYPE67_SCREENS(82248CD0, PanelScreenOpenMovie)
ITC_TYPE67_SCREENS(82249D88, PanelScreenWaitForMovie)
ITC_TYPE67_SCREENS(82249DF8, PanelScreenOpen)
ITC_TYPE67_SCREENS(82248CE0, PanelScreenShow)
ITC_TYPE67_SCREENS(82248C48, PanelScreenUpdate)
ITC_TYPE67_SCREENS(82248DF8, PanelScreenOnMessage24)
ITC_TYPE67_SCREENS(82249E38, PanelScreenLeave)
ITC_TYPE67_SCREENS(82248B08, PanelScreenOnLeft)
ITC_TYPE67_SCREENS(82240958, Type7PanelScreenDraw)
// The node tables (from the list and node screens' functions, recompiled
// where not yet native, and slots 0 and 1 of their vtables; never seen in
// play).
ITC_TYPE67_SCREENS(82247DC0, NodeTableDestruct)
ITC_TYPE67_SCREENS(82247E88, NodeTableDeletingDestruct)
ITC_TYPE67_SCREENS(82247CB8, NodeTableSetPairs)
ITC_TYPE67_SCREENS(82247CE0, PairedNodeTableSavePairs)
ITC_TYPE67_SCREENS(82247DD0, ColorNodeTableClear)
ITC_TYPE67_SCREENS(82247E00, ColorNodeTableSet)
ITC_TYPE67_SCREENS(82247E50, ColorNodeTableSavePairs)
ITC_TYPE67_SCREENS(82248300, NodeTableShow)
ITC_TYPE67_SCREENS(82247ED0, NodeTableShowNode)
ITC_TYPE67_SCREENS(822480F0, NodeTableSetMarks)
ITC_TYPE67_SCREENS(82248198, ColorNodeTableSetColors)
ITC_TYPE67_SCREENS(82248260, ColorNodeTableClearColors)
ITC_TYPE67_SCREENS(822487B8, ColorNodeTableMarkSelected)
ITC_TYPE67_SCREENS(822483B0, ColorNodeTableRefresh)
ITC_TYPE67_SCREENS(82248650, ColorNodeTableReset)
ITC_TYPE67_SCREENS(82248800, PairedNodeTableRefresh)
// The list part and its inner part (the list screens' slots and
// destructors, recompiled where not yet native, and the part's and the
// inner's vtables and the inner's page and step tables; never seen in play).
ITC_TYPE67_SCREENS(82252420, ListPartBaseDestruct)
ITC_TYPE67_SCREENS(82252600, ListPartInnerBaseDestruct)
ITC_TYPE67_SCREENS(82252610, ListPartBaseDeletingDestruct)
ITC_TYPE67_SCREENS(82252658, ListPartInnerBaseDeletingDestruct)
ITC_TYPE67_SCREENS(822526F8, ListPartInnerDestruct)
ITC_TYPE67_SCREENS(822529B0, ListPartInnerDeletingDestruct)
ITC_TYPE67_SCREENS(82252968, ListPartDestruct)
ITC_TYPE67_SCREENS(82252A38, ListPartDeletingDestruct)
ITC_TYPE67_SCREENS(82252430, ListPartOpen)
ITC_TYPE67_SCREENS(82252490, ListPartPick)
ITC_TYPE67_SCREENS(82252500, ListPartInput)
ITC_TYPE67_SCREENS(82252530, ListPartClose)
ITC_TYPE67_SCREENS(82252590, ListPartIsIdle)
ITC_TYPE67_SCREENS(822525B8, ListPartCanLeave)
ITC_TYPE67_SCREENS(82252A88, ListPartInnerOpen)
ITC_TYPE67_SCREENS(82252A00, ListPartInnerPick)
ITC_TYPE67_SCREENS(82252DF0, ListPartInnerInput)
ITC_TYPE67_SCREENS(82252748, ListPartInnerIsIdle)
ITC_TYPE67_SCREENS(82252740, ListPartInnerClose)
ITC_TYPE67_SCREENS(822528C8, ListPartInnerCanLeave)
ITC_TYPE67_SCREENS(82252778, ListPartInnerPage0)
ITC_TYPE67_SCREENS(82252878, ListPartInnerPage1)
ITC_TYPE67_SCREENS(822528A8, ListPartInnerPage2)
ITC_TYPE67_SCREENS(82252BB8, ListPartInnerStep0)
ITC_TYPE67_SCREENS(82252C98, ListPartInnerStep1)
ITC_TYPE67_SCREENS(82252D38, ListPartInnerStep2)
// The list screens' shared helpers and item loaders (from the list screens'
// pages, steps and slots, recompiled where not yet native, and the type 8
// mode's lists), slot 0 and slot 25 of their vtables, and their handlers
// (through their machines' transitions); never seen in play.
ITC_TYPE67_SCREENS(82251EB8, ListScreenDestruct)
ITC_TYPE67_SCREENS(82252248, ListScreenDeletingDestruct)
ITC_TYPE67_SCREENS(82256FE8, NodeListScreenItemInfo)
ITC_TYPE67_SCREENS(82257060, NodeListScreenLoadItem)
ITC_TYPE67_SCREENS(82255238, ColorListScreenLoadItem)
ITC_TYPE67_SCREENS(822518B0, ListScreenShowSelection)
ITC_TYPE67_SCREENS(82251A20, ListScreenLoopItemNode)
ITC_TYPE67_SCREENS(82251B80, ListScreenLoopSecondNode)
ITC_TYPE67_SCREENS(82251AD0, ListScreenLoopThirdNode)
ITC_TYPE67_SCREENS(82251D00, ListScreenSetScrollText)
ITC_TYPE67_SCREENS(82251DA0, ListScreenClearScrollText)
ITC_TYPE67_SCREENS(82251E10, ListScreenSetArrows)
ITC_TYPE67_SCREENS(82252068, ListScreenOnToggleMoved)
ITC_TYPE67_SCREENS(82252198, ListScreenShowToggle)
ITC_TYPE67_SCREENS(82251C30, ListScreenSetIcons)
ITC_TYPE67_SCREENS(82251EC8, ListScreenSetUpMovie)
ITC_TYPE67_SCREENS(82251F98, ListScreenShow)
ITC_TYPE67_SCREENS(82251520, ListScreenDraw)
ITC_TYPE67_SCREENS(822517B8, ListScreenUpdate)
ITC_TYPE67_SCREENS(82251580, ListScreenSuspend)
ITC_TYPE67_SCREENS(82251590, ListScreenResume)
ITC_TYPE67_SCREENS(822517A0, ListScreenOpen)
// The titled list screen (TitledListScreen, the type 6 list screen's base):
// its slots 22 to 24 and 0, its tables' pages and steps (through the tables,
// which the recompiled Draw and Update build with the same words),
// OnItemMoved (from Step2) and the destructors (from ~Type6ListScreen and the
// deleting destructors, recompiled; never seen in play).
ITC_TYPE67_SCREENS(82257558, TitledListScreenReset)
ITC_TYPE67_SCREENS(822579E8, TitledListScreenDraw)
ITC_TYPE67_SCREENS(82258998, TitledListScreenUpdate)
ITC_TYPE67_SCREENS(82257148, TitledListScreenPage2)
ITC_TYPE67_SCREENS(822572F0, TitledListScreenPage3)
ITC_TYPE67_SCREENS(82257360, TitledListScreenPage4)
ITC_TYPE67_SCREENS(82257490, TitledListScreenPage7)
ITC_TYPE67_SCREENS(82255818, ListScreenLeavePage)
ITC_TYPE67_SCREENS(82257D48, TitledListScreenStep0)
ITC_TYPE67_SCREENS(82257FF8, TitledListScreenStep1)
ITC_TYPE67_SCREENS(822580E0, TitledListScreenStep2)
ITC_TYPE67_SCREENS(82257B78, TitledListScreenOnItemMoved)
ITC_TYPE67_SCREENS(82257138, TitledListScreenDestruct)
ITC_TYPE67_SCREENS(82257990, TitledListScreenDeletingDestruct)
ITC_TYPE67_SCREENS(82257000, NodeListScreenDestruct)
ITC_TYPE67_SCREENS(82257940, NodeListScreenDeletingDestruct)
// The color list screen (ColorListScreen, the type 7 list screen's base):
// its slots 22 to 24 and 0, its page table's pages (through the table, which
// the recompiled Draw builds with the same words), the cursors and the list
// part's nodes (from the pages and steps) and the destructors (from
// ~Type7ListScreen and the deleting destructor, recompiled; never seen in
// play).
ITC_TYPE67_SCREENS(82255200, ColorListScreenReset)
ITC_TYPE67_SCREENS(82256CA0, ColorListScreenDraw)
ITC_TYPE67_SCREENS(82256E08, ColorListScreenUpdate)
ITC_TYPE67_SCREENS(822552B8, ColorListScreenPage2)
ITC_TYPE67_SCREENS(82255490, ColorListScreenPage3)
ITC_TYPE67_SCREENS(82255658, ColorListScreenPage4)
ITC_TYPE67_SCREENS(82256508, ColorListScreenPage5)
ITC_TYPE67_SCREENS(822565A0, ColorListScreenPage6)
ITC_TYPE67_SCREENS(822557F0, ColorListScreenPage7)
ITC_TYPE67_SCREENS(822562E0, ColorListScreenPage9)
ITC_TYPE67_SCREENS(822561D0, ColorListScreenPlaceItemCursor)
ITC_TYPE67_SCREENS(82256258, ColorListScreenPlaceColorCursor)
ITC_TYPE67_SCREENS(82256360, ColorListScreenShowPartNodes)
ITC_TYPE67_SCREENS(82255198, ColorListScreenDestruct)
ITC_TYPE67_SCREENS(82255840, ColorListScreenDeletingDestruct)
// Its step table's steps (through the table, which the recompiled Update
// builds with the same words).
ITC_TYPE67_SCREENS(82258578, ColorListScreenStep0)
ITC_TYPE67_SCREENS(82255890, ColorListScreenStep1)
ITC_TYPE67_SCREENS(82256740, ColorListScreenStep2)
ITC_TYPE67_SCREENS(82255B58, ColorListScreenStep3)
ITC_TYPE67_SCREENS(82256908, ColorListScreenStep4)
ITC_TYPE67_SCREENS(82255C88, ColorListScreenRunMovie)
ITC_TYPE67_SCREENS(82255CD0, ColorListScreenStep6)
ITC_TYPE67_SCREENS(82256AC0, ColorListScreenStep7)
ITC_TYPE67_SCREENS(82256030, ColorListScreenStep8)
ITC_TYPE67_SCREENS(82255EB0, ColorListScreenStep10)
// The type 6 list screen's slots 2, 6, 7, 14, 25, 26 and 0 and its
// destructor (from the deleting destructor and the type 6 block's, never
// seen in play; Type6ListScreen::AddRacers sub_8223356C, a piece of
// SetUpRace's switch in its frame, has no adapter), the node screen's slot 4
// and its open handler, the panel screens' dialog handlers, Suspend and
// OpenEmpty, Type6PanelScreen's slots 4 and 6 to 8, the group pair's slot 0.
ITC_TYPE67_SCREENS(822336C0, Type6ListScreenSetItemTexts)
ITC_TYPE67_SCREENS(822337B0, Type6ListScreenBack)
ITC_TYPE67_SCREENS(822334C8, Type6ListScreenSetUpRace)
ITC_TYPE67_SCREENS(82233378, Type6ListScreenSetUpMenus)
ITC_TYPE67_SCREENS(82233730, Type6ListScreenShowMarks)
ITC_TYPE67_SCREENS(82237008, Type6ListScreenItemInfo)
ITC_TYPE67_SCREENS(822332D8, Type6ListScreenDestruct)
ITC_TYPE67_SCREENS(82233478, Type6ListScreenDeletingDestruct)
ITC_TYPE67_SCREENS(822609E0, PanelScreenOnYes)
ITC_TYPE67_SCREENS(822609D0, PanelScreenOnNo)
ITC_TYPE67_SCREENS(822306D0, PanelScreenOnBack)
ITC_TYPE67_SCREENS(822306E0, PanelScreenOnDialogClosed)
ITC_TYPE67_SCREENS(82248DE0, PanelScreenSuspend)
ITC_TYPE67_SCREENS(822404D8, PanelScreenOpenEmpty)
ITC_TYPE67_SCREENS(8223E708, Type6PanelScreenBack)
ITC_TYPE67_SCREENS(822307D0, Type6PanelScreenCarList)
ITC_TYPE67_SCREENS(822307E0, Type6PanelScreenCarEntry)
ITC_TYPE67_SCREENS(822307F0, Type6PanelScreenCarSlot)
ITC_TYPE67_SCREENS(8224ACE0, GroupPairDeletingDestruct)
ITC_TYPE67_SCREENS(82259318, NodeScreenOpen)
ITC_TYPE67_SCREENS(82259998, NodeScreenItemInfo)
// The type 7 list screen's slots 2, 3, 6, 7, 14, 25, 26, 27 and 0 and its
// destructor (from the deleting destructor and the type 7 block's, never
// seen in play) and SetTitle (from SetUpMenus only, kept for the recompiled
// SetUpMenus behind the flag; Type7ListScreen::AddRacers sub_822430BC, a
// piece of SetUpRace's switch in its frame, has no adapter).
ITC_TYPE67_SCREENS(82243238, Type7ListScreenSetItemText)
ITC_TYPE67_SCREENS(822432A8, Type7ListScreenClearItemText)
ITC_TYPE67_SCREENS(822434C8, Type7ListScreenBack)
ITC_TYPE67_SCREENS(82243018, Type7ListScreenSetUpRace)
ITC_TYPE67_SCREENS(82243588, Type7ListScreenSetUpMenus)
ITC_TYPE67_SCREENS(822433D8, Type7ListScreenSetTitle)
ITC_TYPE67_SCREENS(82243308, Type7ListScreenSetDetails)
ITC_TYPE67_SCREENS(82243448, Type7ListScreenShowMarks)
ITC_TYPE67_SCREENS(82242FA0, Type7ListScreenItemInfo)
ITC_TYPE67_SCREENS(82242EC8, Type7ListScreenDestruct)
ITC_TYPE67_SCREENS(82242FC8, Type7ListScreenDeletingDestruct)
// The node screen: its show, draw and update handlers (through its
// machine's transitions), its page and step tables' entries (through the
// tables, which the recompiled Draw and Update build with the same words),
// the helpers (from Type6NodeScreen::SetUpMenus, recompiled, and each
// other) and the destructors (slot 0; ~NodeScreen from
// Type6NodeScreen's destructors, recompiled). None seen in play.
ITC_TYPE67_SCREENS(82259CA0, NodeScreenShow)
ITC_TYPE67_SCREENS(8225A1B8, NodeScreenDraw)
ITC_TYPE67_SCREENS(8225A2C0, NodeScreenUpdate)
ITC_TYPE67_SCREENS(82259D80, NodeScreenPage2)
ITC_TYPE67_SCREENS(822590E8, NodeScreenPage4)
ITC_TYPE67_SCREENS(82259328, NodeScreenPage5)
ITC_TYPE67_SCREENS(82259EC0, NodeScreenStep0)
ITC_TYPE67_SCREENS(82259680, NodeScreenStep1)
ITC_TYPE67_SCREENS(82259750, NodeScreenStep2)
ITC_TYPE67_SCREENS(82259A78, NodeScreenShowSelection)
ITC_TYPE67_SCREENS(82259B70, NodeScreenOnToggleMoved)
ITC_TYPE67_SCREENS(822599B0, NodeScreenRefresh)
ITC_TYPE67_SCREENS(82259410, NodeScreenShowRecords)
ITC_TYPE67_SCREENS(82259368, NodeScreenSetArrows)
ITC_TYPE67_SCREENS(82259050, NodeScreenLoadItem)
ITC_TYPE67_SCREENS(82259948, NodeScreenDestruct)
ITC_TYPE67_SCREENS(8225A168, NodeScreenDeletingDestruct)
// The type 6 node screen's own slots and destructors.
ITC_TYPE67_SCREENS(82236F00, Type6NodeScreenSetUpMenus)
ITC_TYPE67_SCREENS(822370A0, Type6NodeScreenSetText)
ITC_TYPE67_SCREENS(82237110, Type6NodeScreenBack)
ITC_TYPE67_SCREENS(82237160, Type6NodeScreenSetIcons)
ITC_TYPE67_SCREENS(82237228, Type6NodeScreenSetScrollText)
ITC_TYPE67_SCREENS(822372A0, Type6NodeScreenShowMarks)
ITC_TYPE67_SCREENS(82247FE0, NodeScreenSetUpTexts)
ITC_TYPE67_SCREENS(82236EA0, Type6NodeScreenDestruct)
ITC_TYPE67_SCREENS(82237048, Type6NodeScreenDeletingDestruct)
// The type 6 panel screen's Draw ((5, 1), a transition) and PickCar (slot 3).
ITC_TYPE67_SCREENS(82230F40, Type6PanelScreenDraw)
ITC_TYPE67_SCREENS(82230C88, Type6PanelScreenPickCar)
// The screen blocks' destructors (no callers found: StandMode::Release frees
// the blocks without them).
ITC_TYPE67_SCREENS(822367C0, Type6ScreensDestruct)
ITC_TYPE67_SCREENS(82236830, Type6ScreensDeletingDestruct)
ITC_TYPE67_SCREENS(82246BE0, Type7ScreensDestruct)
ITC_TYPE67_SCREENS(82246C40, Type7ScreensDeletingDestruct)
