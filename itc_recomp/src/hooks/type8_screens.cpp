// The type 8 mode's panel screen natively: adapters in front of
// src/type8_screens/ (docs/functions/type8-screens.md), with the register
// steps the harness shares (hooks/type8_screens.h). Never seen in play
// (ported by the user's 2026-11-12 call); the slots are reached through the
// vtables (the screen's, the group's and Panel8C's), the handlers through
// the screen's machine, so their adapters stay.

#include "hooks/type8_screens.h"

#include "base/flags.h"
#include "hooks/type67_screens.h"
#include "type8_screens/type8_screens.h"

ITC_FLAG_BOOL(native_type8_screens, true, "ITC",
              "Run the type 8 mode's panel screen (docs/functions/type8-screens.md: "
              "PanelGroup8::PanelGroup8 sub_82239C10 and DeletingDestructor sub_8223A2A8, "
              "Panel8F::Panel8F sub_82239C70, Panel8E::Panel8E sub_82239D58, Panel8C::Panel8C "
              "sub_82239E78, DeletingDestructor sub_8223A348, SetStats sub_8223A090 and "
              "ShowArrows sub_8223A098, Panel8D::Panel8D sub_8223A100, "
              "ConfirmPanel8::ConfirmPanel8 sub_8223A208 (the panels' bases, "
              "Panel6C::SetStats and UiMovie::ShowNode and HideNode through the car list's and "
              "the screen constructors' plain C++ or guest calls, operator delete a guest "
              "call); Type8PanelScreen::InitWords sub_82238420, IsLoaded sub_82238418, "
              "OpenMovies sub_82238440, DeleteMovies sub_82238450, OnMessage2 sub_82238458, "
              "OnSaveLoaded sub_82238530, OnAnswer0 to 2 sub_82238540, sub_82238550 and "
              "sub_82238560, Leave sub_82238B10, FadeInGroup sub_82238468 (PanelFade::In's plain "
              "C++, UiMovie::FinishNamed, PlayNamed and LoopNamed guest calls), UpdateOpening "
              "sub_82238A00, Show sub_82238928, Draw sub_82238E78 with DrawList sub_82238DC8 "
              "and AskQuit sub_82238BA0 (UiMovie::SetText's plain C++ where the text setters "
              "are native), OnDialogClosed sub_82238990 and ShowPlayerNodes sub_82238570 "
              "(UiMovie::FindNode, ShowNode, HideNode and SetNodeTexture's and ScreenFades' "
              "plain C++; the shared panel handlers' and the holder's slots through "
              "type67_screens::Slot), Type8PanelScreen::Type8PanelScreen sub_822386F0 "
              "(MachineScreen's, the panels', PanelSetPair's and the holder's constructors by "
              "their plain C++), ~Type8PanelScreen sub_82238368 and DeletingDestructor "
              "sub_822388D8 (the members' destructors by their plain C++, operator delete and "
              "the car list's MenuList::Release guest calls), InitMachine sub_82238E98 (its "
              "step table, StateMachine::Add's plain C++) and PickCar sub_82238C88 "
              "(GarageCar::Reset a guest call, memcpy's, Car::SetModelAndClass's, "
              "SaveData::TwoPlayerBlock's and ScreenFades::FadeOut's plain C++), "
              "Type8ListPart::Type8ListPart sub_82254488, ~Type8ListPart sub_822544E8 and "
              "DeletingDestructor sub_82254580, Type8ListPartInner::Type8ListPartInner "
              "sub_822543C0, ~Type8ListPartInner sub_82254418, DeletingDestructor sub_82254530, "
              "Open sub_822549D8, Pick sub_822550C0, Input sub_82254E18, Step0 sub_82254D08 and "
              "Page0 sub_82254FB8, Type8PlayerBlock::Type8PlayerBlock sub_822542B8, SetUp "
              "sub_822545D0, PlayOpening sub_822546F8, ShowMenuMove sub_82254798, PlayReady "
              "sub_822548B8, PlaceCursor sub_82254948, ShowSelections sub_82254AD8, WhileReady "
              "sub_82254A70, Input sub_82254EC8, UpdateSetting sub_82254DB8 and SettingMoved "
              "sub_82254C50, Type8PartInner::Step1 sub_822538A8, Page1 sub_82253108 and Page2 "
              "sub_82253838, Type8ColorListPart::Type8ColorListPart sub_82253190, "
              "~Type8ColorListPart sub_822531F0, DeletingDestructor sub_822532B8, IsFirstChoice "
              "sub_82253238 and ThirdChoice sub_82253240, Type8ColorPartInner "
              "sub_82253030, ~Type8ColorPartInner sub_82253088, DeletingDestructor sub_82253268, "
              "IsFirstChoice sub_82253148, Open sub_822537A0, Pick sub_82254280, Input "
              "sub_82253FC0, Step0 sub_82253E48 and Page0 sub_82254178, Type8ColorBlock "
              "sub_82252EA0, SetUp sub_82253308, PlaceCursorA to C sub_82253728, sub_82253690 "
              "and sub_82253600, PlayOpening sub_822539A8, ShowSelections sub_82253A50, "
              "ShowMenuMove sub_82253BF8, PlayReady sub_82253570, UpdateSecond sub_82253948, "
              "SecondMoved sub_822534F0, UpdateThird sub_82253F60, ThirdMoved sub_82253D90, "
              "WhileReady sub_82253EF8 and Input sub_82254070 (UiMovie::FinishNamed, LoopNamed, SetNodeText, ClearNodeText, Run, ResetPose and "
              "the menus' guest calls, MenuList::Init and Release by their plain C++ where they "
              "free natively), Type8ListScreen::Type8ListScreen sub_8223F140, DeletingDestructor "
              "sub_8223F388, IsLoaded sub_8223F590, OpenMovie sub_8223F3E0, Release "
              "sub_8223F588, SetItemTexts sub_8223F5D8, ItemInfo sub_8223F598, ShowMarks "
              "sub_8223F6C8, SetLabels sub_8223F648, SetUpMenus sub_8223F3F0 (the sound "
              "manager's slots 13 and 14 guest calls) and SetUpRace sub_8223F778 with its switch "
              "piece sub_8223F818 (the race set-up's plain C++, guest calls where it declines), Type8ListBase::Type8ListBase "
              "sub_822574D0, ~Type8ListBase sub_82257548, DeletingDestructor sub_82257B20, Draw "
              "sub_82258D60 and Update sub_82258EB0 (their tables built on first use), Page2 "
              "sub_82257588, Page3 sub_82257768, Page4 sub_82258A40, Page5 sub_82258AD0, Page6 "
              "sub_822578D8, Page8 sub_82257900, Step0 sub_82258158, Step1 sub_82258648, Step2 "
              "sub_82258BE0, Step3 sub_82258438 and DimList sub_822587C8, "
              "PairedNodeTable::Clear sub_82248A20 (the list helpers', node tables' and "
              "NodeListScreen's plain C++, the UI movies' named animations and Run and the menus' "
              "guest calls), Type8ColorListScreen::Type8ColorListScreen sub_8223DFC0, "
              "DeletingDestructor sub_8223E2B8, OpenMovie sub_8223E310, ItemInfo sub_8223E4D0 "
              "and InitMachine sub_8223E9D8 (its step table), SetItemText sub_8223E4F8, "
              "ClearItemText sub_8223E568, SetDetails sub_8223E5C8, SetLabels sub_8223E698 and "
              "SetUpMenus sub_8223E320 (UiMovie::ClearText and the sound manager's slots guest "
              "calls)) natively instead of through the translated "
              "code (the same writes; checked by --run_harnesses=type8_screens)");

namespace hooks {

namespace t8 = type8_screens;

#define ITC_T8_STEP(name, expression)                          \
  void name(PPCContext& ctx, uint8_t* base) {                  \
    const t8::Calls calls = Type67ScreensCalls(ctx, base);     \
    (void)calls;                                               \
    const uint32_t sp = ctx.r1.u32;                            \
    (void)sp;                                                  \
    ctx.r3.u64 = (expression);                                 \
  }

ITC_T8_STEP(Type8PanelGroupConstruct, t8::PanelGroup8Construct(base, ctx.r3.u64, ctx.r4.u32, sp))
ITC_T8_STEP(Type8Panel8FConstruct,
            t8::Panel8FConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8Panel8EConstruct,
            t8::Panel8EConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8Panel8CConstruct,
            t8::Panel8CConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8Panel8DConstruct,
            t8::Panel8DConstruct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8ConfirmPanelConstruct,
            t8::ConfirmPanel8Construct(base, ctx.r3.u64, ctx.r4.u32, ctx.r5.u32, sp))
ITC_T8_STEP(Type8PanelGroupDeletingDestruct,
            t8::PanelGroup8DeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8Panel8CDeletingDestruct,
            t8::Panel8CDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8Panel8CSetStats,
            t8::Panel8CSetStats(base, ctx.r3.u64, ctx.r4.u32, sp, t8::CarListCalls(calls)))
ITC_T8_STEP(Type8Panel8CShowArrows,
            t8::Panel8CShowArrows(base, ctx.r3.u64, ctx.r4.u64, sp, t8::CarListCalls(calls)))

ITC_T8_STEP(Type8InitWords, t8::InitWords(base, ctx.r3.u64))
ITC_T8_STEP(Type8IsLoaded, t8::IsLoaded(base))
ITC_T8_STEP(Type8OpenMovies, t8::OpenMovies(sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8DeleteMovies, t8::DeleteMovies(sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8OnMessage2, t8::SetWord(base, ctx.r3.u64, 0x82238458))
ITC_T8_STEP(Type8OnSaveLoaded, t8::SetWord(base, ctx.r3.u64, 0x82238530))
ITC_T8_STEP(Type8OnAnswer0, t8::SetWord(base, ctx.r3.u64, 0x82238540))
ITC_T8_STEP(Type8OnAnswer1, t8::SetWord(base, ctx.r3.u64, 0x82238550))
ITC_T8_STEP(Type8OnAnswer2, t8::SetWord(base, ctx.r3.u64, 0x82238560))
ITC_T8_STEP(Type8Leave, t8::Leave(base, ctx.r3.u64, ctx.r4.u64))
ITC_T8_STEP(Type8FadeInGroup, t8::FadeInGroup(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8UpdateOpening, t8::UpdateOpening(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8Show, t8::Show(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8Draw, t8::Draw(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8OnDialogClosed, t8::OnDialogClosed(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8Construct, t8::Construct(base, ctx.r3.u64, sp))
ITC_T8_STEP(Type8Destruct, t8::Destruct(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8DeletingDestruct, t8::DeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8PickCar, t8::PickCar(base, ctx.r3.u64, sp, calls))

ITC_T8_STEP(Type8BlockConstruct, t8::PlayerBlockConstruct(base, ctx.r3.u64, ctx.r4.u64))
ITC_T8_STEP(Type8BlockSetUp, t8::PlayerBlockSetUp(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8BlockPlayOpening, t8::PlayerBlockPlayOpening(base, ctx.r3.u64, ctx.r4.u64, sp))
ITC_T8_STEP(Type8BlockShowMenuMove,
            t8::PlayerBlockShowMenuMove(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8BlockPlayReady, t8::PlayerBlockPlayReady(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8BlockPlaceCursor,
            t8::PlayerBlockPlaceCursor(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, sp))
ITC_T8_STEP(Type8BlockShowSelections,
            t8::PlayerBlockShowSelections(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8BlockWhileReady,
            t8::PlayerBlockWhileReady(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8BlockInput, t8::PlayerBlockInput(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8BlockUpdateSetting,
            t8::PlayerBlockUpdateSetting(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8BlockSettingMoved,
            t8::PlayerBlockSettingMoved(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListInnerConstruct, t8::ListInnerConstruct(base, ctx.r3.u64, sp))
ITC_T8_STEP(Type8ListPartConstruct, t8::ListPartConstruct(base, ctx.r3.u64, sp))
ITC_T8_STEP(Type8ListInnerDestruct, t8::ListInnerDestruct(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListInnerDeletingDestruct,
            t8::ListInnerDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListPartDestruct, t8::ListPartDestruct(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListPartDeletingDestruct,
            t8::ListPartDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListInnerOpen, t8::ListInnerOpen(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListInnerPick, t8::ListInnerPick(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, sp,
                                                  uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8ListInnerInput, t8::ListInnerInput(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListInnerStep0, t8::ListInnerStep0(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListInnerPage0, t8::ListInnerPage0(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8PartInnerStep1, t8::PartInnerStep1(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8PartInnerPage1, t8::PartInnerPage1(base, ctx.r3.u64))
ITC_T8_STEP(Type8PartInnerPage2, t8::PartInnerPage2(base, ctx.r3.u64, ctx.r4.u64, sp))

ITC_T8_STEP(Type8ColorBlockConstruct, t8::ColorBlockConstruct(base, ctx.r3.u64, ctx.r4.u64))
ITC_T8_STEP(Type8ColorBlockSetUp, t8::ColorBlockSetUp(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockPlaceCursorA, t8::ColorBlockPlaceCursorA(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, sp))
ITC_T8_STEP(Type8ColorBlockPlaceCursorB, t8::ColorBlockPlaceCursorB(base, ctx.r3.u64, ctx.r4.u64, sp))
ITC_T8_STEP(Type8ColorBlockPlaceCursorC, t8::ColorBlockPlaceCursorC(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, sp))
ITC_T8_STEP(Type8ColorBlockPlayOpening, t8::ColorBlockPlayOpening(base, ctx.r3.u64, ctx.r4.u64, sp))
ITC_T8_STEP(Type8ColorBlockShowSelections, t8::ColorBlockShowSelections(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockShowMenuMove, t8::ColorBlockShowMenuMove(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockPlayReady, t8::ColorBlockPlayReady(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockSecondMoved, t8::ColorBlockSecondMoved(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockUpdateSecond, t8::ColorBlockUpdateSecond(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockThirdMoved, t8::ColorBlockThirdMoved(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockUpdateThird, t8::ColorBlockUpdateThird(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockWhileReady, t8::ColorBlockWhileReady(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorBlockInput, t8::ColorBlockInput(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorInnerConstruct, t8::ColorInnerConstruct(base, ctx.r3.u64, sp))
ITC_T8_STEP(Type8ColorPartConstruct, t8::ColorPartConstruct(base, ctx.r3.u64, sp))
ITC_T8_STEP(Type8ColorInnerDestruct, t8::ColorInnerDestruct(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ColorInnerDeletingDestruct, t8::ColorInnerDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorPartDestruct, t8::ColorPartDestruct(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ColorPartDeletingDestruct, t8::ColorPartDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorInnerIsFirstChoice, t8::ColorInnerIsFirstChoice(base, ctx.r3.u64, ctx.r4.u64, sp))
ITC_T8_STEP(Type8ColorPartIsFirstChoice, t8::ColorPartIsFirstChoice(base, ctx.r3.u64, ctx.r4.u64, sp))
ITC_T8_STEP(Type8ColorPartThirdChoice, t8::ColorPartThirdChoice(base, ctx.r3.u64, ctx.r4.u64))
ITC_T8_STEP(Type8ColorInnerOpen, t8::ColorInnerOpen(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorInnerPick, t8::ColorInnerPick(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u64, sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8ColorInnerInput, t8::ColorInnerInput(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorInnerStep0, t8::ColorInnerStep0(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorInnerPage0, t8::ColorInnerPage0(base, ctx.r3.u64, ctx.r4.u64, sp, calls))

ITC_T8_STEP(Type8ListBaseConstruct, t8::ListBaseConstruct(base, ctx.r3.u64, sp))
ITC_T8_STEP(Type8ListScreenConstruct, t8::ListScreen8Construct(base, ctx.r3.u64, sp))
ITC_T8_STEP(Type8ListBaseDestruct, t8::ListBaseDestruct(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListBaseDeletingDestruct,
            t8::ListBaseDeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListScreenDeletingDestruct,
            t8::ListScreen8DeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListIsLoaded, t8::ListScreen8IsLoaded(base, ctx.r3.u64))
ITC_T8_STEP(Type8ListOpenMovie, t8::ListScreen8OpenMovie(base, ctx.r3.u64, sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8ListRelease, t8::ListScreen8Release(base, ctx.r3.u64, sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8ListSetItemTexts,
            t8::ListScreen8SetItemTexts(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListItemInfo, t8::ListScreen8ItemInfo(base, ctx.r3.u64, ctx.r4.u64, ctx.r5.u32))
ITC_T8_STEP(Type8ListShowMarks, t8::ListScreen8ShowMarks(base, ctx.r3.u64, ctx.r4.u64, sp))
ITC_T8_STEP(Type8ListSetLabels, t8::ListScreen8SetLabels(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListSetUpMenus, t8::ListScreen8SetUpMenus(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListSetUpRace,
            t8::ListScreen8SetUpRace(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListDraw, t8::ListBaseDraw(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListUpdate, t8::ListBaseUpdate(base, ctx.r3.u64, ctx.r5.u64, sp, calls))
ITC_T8_STEP(Type8ListPage2, t8::ListBasePage2(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListPage3, t8::ListBasePage3(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListPage4, t8::ListBasePage4(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListPage5, t8::ListBasePage5(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ListPage6, t8::ListBasePage6(base, ctx.r3.u64))
ITC_T8_STEP(Type8ListPage8, t8::ListBasePage8(base, ctx.r3.u64))
ITC_T8_STEP(Type8ListStep0, t8::ListBaseStep0(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListStep1, t8::ListBaseStep1(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListStep2, t8::ListBaseStep2(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ListStep3, t8::ListBaseStep3(base, ctx.r3.u64, sp, calls))

ITC_T8_STEP(Type8ColorListConstruct, t8::ColorList8Construct(base, ctx.r3.u64, sp))
ITC_T8_STEP(Type8ColorListDeletingDestruct,
            t8::ColorList8DeletingDestruct(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorListOpenMovie,
            t8::ColorList8OpenMovie(base, ctx.r3.u64, sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8ColorListItemInfo,
            t8::ColorList8ItemInfo(base, ctx.r4.u64, ctx.r5.u64, sp, uint32_t(ctx.lr), calls))
ITC_T8_STEP(Type8ColorListSetItemText,
            t8::ColorList8SetItemText(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorListClearItemText,
            t8::ColorList8ClearItemText(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorListSetDetails,
            t8::ColorList8SetDetails(base, ctx.r3.u64, ctx.r4.u64, sp, calls))
ITC_T8_STEP(Type8ColorListSetLabels, t8::ColorList8SetLabels(base, ctx.r3.u64, sp, calls))
ITC_T8_STEP(Type8ColorListSetUpMenus, t8::ColorList8SetUpMenus(base, ctx.r3.u64, sp, calls))

#undef ITC_T8_STEP

void Type8InitMachine(PPCContext& ctx, uint8_t* base) {
  const uint64_t object_r3 = ctx.r3.u64;
  uint32_t machine = 0;
  bool replaced = false;
  const uint32_t r3 = t8::InitMachine(base, ctx.r3.u32, ctx.r1.u32 - t8::kInitMachineFrame,
                                      &machine, &replaced);
  // StateMachine::Add's adapter's r3 (src/hooks/game_modes.cpp): a new
  // transition's address from the machine's 64-bit r3.
  const uint64_t machine_r3 = object_r3 + (machine - ctx.r3.u32);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

void Type8ColorListInitMachine(PPCContext& ctx, uint8_t* base) {
  const uint64_t object_r3 = ctx.r3.u64;
  uint32_t machine = 0;
  bool replaced = false;
  const uint32_t r3 = t8::ColorList8InitMachine(
      base, ctx.r3.u32, ctx.r1.u32 - t8::kColorListInitMachineFrame, &machine, &replaced);
  const uint64_t machine_r3 = object_r3 + (machine - ctx.r3.u32);
  ctx.r3.u64 = replaced ? r3 : machine_r3 + (r3 - machine);
}

}  // namespace hooks

#define ITC_TYPE8_SCREENS(address, step)    \
  extern "C" REX_FUNC(sub_##address) {      \
    REX_FUNC_PROLOGUE();                    \
    if (!ITC_FLAG(native_type8_screens)) {  \
      __imp__sub_##address(ctx, base);      \
      return;                               \
    }                                       \
    hooks::step(ctx, base);                 \
  }

// The panels' and the group's constructors (from the screen's constructor,
// recompiled) and their vtables' slots.
ITC_TYPE8_SCREENS(82239C10, Type8PanelGroupConstruct)
ITC_TYPE8_SCREENS(82239C70, Type8Panel8FConstruct)
ITC_TYPE8_SCREENS(82239D58, Type8Panel8EConstruct)
ITC_TYPE8_SCREENS(82239E78, Type8Panel8CConstruct)
ITC_TYPE8_SCREENS(8223A100, Type8Panel8DConstruct)
ITC_TYPE8_SCREENS(8223A208, Type8ConfirmPanelConstruct)
ITC_TYPE8_SCREENS(8223A2A8, Type8PanelGroupDeletingDestruct)
ITC_TYPE8_SCREENS(8223A348, Type8Panel8CDeletingDestruct)
ITC_TYPE8_SCREENS(8223A090, Type8Panel8CSetStats)
ITC_TYPE8_SCREENS(8223A098, Type8Panel8CShowArrows)
// The screen's vtable slots and its machine's leaf handlers.
ITC_TYPE8_SCREENS(82238420, Type8InitWords)
ITC_TYPE8_SCREENS(82238418, Type8IsLoaded)
ITC_TYPE8_SCREENS(82238440, Type8OpenMovies)
ITC_TYPE8_SCREENS(82238450, Type8DeleteMovies)
ITC_TYPE8_SCREENS(82238458, Type8OnMessage2)
ITC_TYPE8_SCREENS(82238530, Type8OnSaveLoaded)
ITC_TYPE8_SCREENS(82238540, Type8OnAnswer0)
ITC_TYPE8_SCREENS(82238550, Type8OnAnswer1)
ITC_TYPE8_SCREENS(82238560, Type8OnAnswer2)
ITC_TYPE8_SCREENS(82238B10, Type8Leave)
// The machine's other handlers. ShowPlayerNodes sub_82238570, AskQuit
// sub_82238BA0 and DrawList sub_82238DC8 have no adapters: their only
// callers are these handlers, native (with the flag off, the originals call
// the originals).
ITC_TYPE8_SCREENS(82238468, Type8FadeInGroup)
ITC_TYPE8_SCREENS(82238A00, Type8UpdateOpening)
ITC_TYPE8_SCREENS(82238928, Type8Show)
ITC_TYPE8_SCREENS(82238E78, Type8Draw)
ITC_TYPE8_SCREENS(82238990, Type8OnDialogClosed)
// The screen's constructor and destructor (from the type 8 mode's member's,
// recompiled: a lead) and its vtable's slots 1, 3 and 20.
ITC_TYPE8_SCREENS(822386F0, Type8Construct)
ITC_TYPE8_SCREENS(82238368, Type8Destruct)
ITC_TYPE8_SCREENS(822388D8, Type8DeletingDestruct)
ITC_TYPE8_SCREENS(82238E98, Type8InitMachine)
ITC_TYPE8_SCREENS(82238C88, Type8PickCar)
// The list screen's list part: its constructor (from the list screen's,
// recompiled), destructor (from the member's and the list screen's deleting
// destructor, recompiled), and the vtables' and tables' slots. The inner
// part's constructor and destructor and the player block have no adapters:
// only these call them.
ITC_TYPE8_SCREENS(82254488, Type8ListPartConstruct)
ITC_TYPE8_SCREENS(822544E8, Type8ListPartDestruct)
ITC_TYPE8_SCREENS(82254580, Type8ListPartDeletingDestruct)
ITC_TYPE8_SCREENS(82254530, Type8ListInnerDeletingDestruct)
ITC_TYPE8_SCREENS(822549D8, Type8ListInnerOpen)
ITC_TYPE8_SCREENS(822550C0, Type8ListInnerPick)
ITC_TYPE8_SCREENS(82254E18, Type8ListInnerInput)
ITC_TYPE8_SCREENS(82254D08, Type8ListInnerStep0)
ITC_TYPE8_SCREENS(82254FB8, Type8ListInnerPage0)
ITC_TYPE8_SCREENS(822538A8, Type8PartInnerStep1)
ITC_TYPE8_SCREENS(82253108, Type8PartInnerPage1)
ITC_TYPE8_SCREENS(82253838, Type8PartInnerPage2)
// The color list screen's list part: its constructor and destructor (from
// the color list screen's and the member's, recompiled), the vtables' and
// tables' slots, and the part's slots 11 and 12 (from the color list
// screen's SetUpRace, recompiled).
ITC_TYPE8_SCREENS(82253190, Type8ColorPartConstruct)
ITC_TYPE8_SCREENS(822531F0, Type8ColorPartDestruct)
ITC_TYPE8_SCREENS(822532B8, Type8ColorPartDeletingDestruct)
ITC_TYPE8_SCREENS(82253268, Type8ColorInnerDeletingDestruct)
ITC_TYPE8_SCREENS(822537A0, Type8ColorInnerOpen)
ITC_TYPE8_SCREENS(82254280, Type8ColorInnerPick)
ITC_TYPE8_SCREENS(82253FC0, Type8ColorInnerInput)
ITC_TYPE8_SCREENS(82253E48, Type8ColorInnerStep0)
ITC_TYPE8_SCREENS(82254178, Type8ColorInnerPage0)
ITC_TYPE8_SCREENS(82253238, Type8ColorPartIsFirstChoice)
ITC_TYPE8_SCREENS(82253240, Type8ColorPartThirdChoice)
// The list screen: its constructors and destructor (from the type 8 mode's
// member's, recompiled), its vtables' slots and its tables' pages and steps
// (reached through the vtables and tables).
ITC_TYPE8_SCREENS(822574D0, Type8ListBaseConstruct)
ITC_TYPE8_SCREENS(8223F140, Type8ListScreenConstruct)
ITC_TYPE8_SCREENS(82257548, Type8ListBaseDestruct)
ITC_TYPE8_SCREENS(82257B20, Type8ListBaseDeletingDestruct)
ITC_TYPE8_SCREENS(8223F388, Type8ListScreenDeletingDestruct)
ITC_TYPE8_SCREENS(8223F590, Type8ListIsLoaded)
ITC_TYPE8_SCREENS(8223F3E0, Type8ListOpenMovie)
ITC_TYPE8_SCREENS(8223F588, Type8ListRelease)
ITC_TYPE8_SCREENS(8223F5D8, Type8ListSetItemTexts)
ITC_TYPE8_SCREENS(8223F598, Type8ListItemInfo)
ITC_TYPE8_SCREENS(8223F6C8, Type8ListShowMarks)
ITC_TYPE8_SCREENS(8223F648, Type8ListSetLabels)
ITC_TYPE8_SCREENS(8223F3F0, Type8ListSetUpMenus)
ITC_TYPE8_SCREENS(8223F778, Type8ListSetUpRace)
ITC_TYPE8_SCREENS(82258D60, Type8ListDraw)
ITC_TYPE8_SCREENS(82258EB0, Type8ListUpdate)
ITC_TYPE8_SCREENS(82257588, Type8ListPage2)
ITC_TYPE8_SCREENS(82257768, Type8ListPage3)
ITC_TYPE8_SCREENS(82258A40, Type8ListPage4)
ITC_TYPE8_SCREENS(82258AD0, Type8ListPage5)
ITC_TYPE8_SCREENS(822578D8, Type8ListPage6)
ITC_TYPE8_SCREENS(82257900, Type8ListPage8)
ITC_TYPE8_SCREENS(82258158, Type8ListStep0)
ITC_TYPE8_SCREENS(82258648, Type8ListStep1)
ITC_TYPE8_SCREENS(82258BE0, Type8ListStep2)
ITC_TYPE8_SCREENS(82258438, Type8ListStep3)
// The color list screen: its constructor and destructor (from the type 8
// mode's member's, recompiled), its InitMachine (slot 5, from
// ListScreen::Init's plain C++ through a guest call) and its vtable's slots.
ITC_TYPE8_SCREENS(8223DFC0, Type8ColorListConstruct)
ITC_TYPE8_SCREENS(8223E2B8, Type8ColorListDeletingDestruct)
ITC_TYPE8_SCREENS(8223E310, Type8ColorListOpenMovie)
ITC_TYPE8_SCREENS(8223E4D0, Type8ColorListItemInfo)
ITC_TYPE8_SCREENS(8223E9D8, Type8ColorListInitMachine)
ITC_TYPE8_SCREENS(8223E4F8, Type8ColorListSetItemText)
ITC_TYPE8_SCREENS(8223E568, Type8ColorListClearItemText)
ITC_TYPE8_SCREENS(8223E5C8, Type8ColorListSetDetails)
ITC_TYPE8_SCREENS(8223E698, Type8ColorListSetLabels)
ITC_TYPE8_SCREENS(8223E320, Type8ColorListSetUpMenus)
