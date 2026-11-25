// The screens' vtable calls and the ring post (docs/functions/type67-screens.md):
// see type67_screens.h.

#include "type67_screens/type67_screens.h"

#include "game_modes/game_modes.h"
#include "guest.h"
#include "schema/boot_mode.h"
#include "schema/game_modes.h"
#include "type8_screens/type8_screens.h"

namespace type67_screens {

using guest::LoadU32;
using guest::StoreU32;

void Post(uint32_t dispatcher, uint32_t message) {
  const schema::ModeDispatcher d(dispatcher);  // a dual view (E5 step 4)
  uint8_t* const ring = d.ring();
  const uint32_t count = d.ring_count() + 1;
  const uint32_t end = d.ring_start() + count - 1;
  d.set_ring_count(count);
  const game_modes::RingSlot slot{ring + (end - (end / 50) * 50) * 12};
  slot.set_id(message);
  slot.set_arg0(0);
  slot.set_arg1(0);
}

// The plain C++ for a slot's target, from the frame at `sp` (the callee's
// entry r1) returning to `lr`; nullopt where the target isn't one of ours
// (or needs an r4 the call leaves unset).
std::optional<uint64_t> SlotNative(uint32_t target, uint64_t r3,
                                   std::optional<uint64_t> r4, uint32_t sp, uint32_t lr,
                                   const Calls& calls, std::optional<uint64_t> r5) {
  // The list screens' slot 25 (list_helpers.cpp), with the info's address
  // in r5.
  if (target == 0x82256FE8) {
    if (!r5) return std::nullopt;
    return NodeListScreenItemInfo(r3, uint32_t(*r5));
  }
  // The type 6 list screen's slot 25 (also the node screen's slot 4; r4 the
  // item) and the node screen's own (type6_list.cpp).
  if (target == 0x82237008) {
    if (!r4 || !r5) return std::nullopt;
    return Type6ListScreenItemInfo(r3, *r4, uint32_t(*r5));
  }
  if (target == 0x82259998) {
    if (!r5) return std::nullopt;
    return NodeScreenItemInfo(r3, uint32_t(*r5));
  }
  // The type 7 list screen's slot 25 (r4 the item; type7_list.cpp).
  if (target == 0x82242FA0) {
    if (!r4 || !r5) return std::nullopt;
    return Type7ListScreenItemInfo(*r4, *r5, sp, lr, calls);
  }
  // The type 8 list screen's slot 25 (r4 the item;
  // src/type8_screens/list_screen.cpp).
  if (target == 0x8223F598) {
    if (!r4 || !r5) return std::nullopt;
    return type8_screens::ListScreen8ItemInfo(r3, *r4, uint32_t(*r5));
  }
  // The type 8 color list screen's slot 25 (r4 the item;
  // src/type8_screens/color_list_screen.cpp).
  if (target == 0x8223E4D0) {
    if (!r4 || !r5) return std::nullopt;
    return type8_screens::ColorList8ItemInfo(*r4, *r5, sp, lr, calls);
  }
  // The list part's and its inner part's slots, and the inner's pages and
  // steps (list_part.cpp); r5, which only the guest fallbacks see, isn't
  // passed on (the plain C++ here doesn't read it).
  switch (target) {
    case 0x82252420:
      return ListPartBaseDestruct(r3);
    case 0x82252600:
      return ListPartInnerBaseDestruct(r3);
    case 0x82252430:
      return ListPartOpen(r3, sp, calls);
    case 0x82252530:
      return ListPartClose(r3, sp, calls);
    case 0x82252590:
      return ListPartIsIdle(r3, sp, lr, calls);
    case 0x822525B8:
      return ListPartCanLeave(r3, sp, lr, calls);
    case 0x822526F8:
      return ListPartInnerDestruct(r3, sp, calls);
    case 0x82252968:
      return ListPartDestruct(r3, sp, calls);
    case 0x82252748:
      return ListPartInnerIsIdle(r3, sp);
    case 0x82252740:
      return ListPartInnerClose(r3, sp, lr, calls);
    case 0x822528C8:
      return ListPartInnerCanLeave(r3);
    case 0x82252778:
      return ListPartInnerPage0(r3, sp, calls);
    case 0x82252878:
      return ListPartInnerPage1(r3);
    case 0x822528A8:
      return ListPartInnerPage2(r3);
    // The titled list screen's slot 22 and its tables' pages and steps
    // (titled_list.cpp).
    case 0x82257558:
      return TitledListScreenReset(r3);
    case 0x82257148:
      return TitledListScreenPage2(r3, sp, calls);
    case 0x822572F0:
      return TitledListScreenPage3(r3, sp, calls);
    case 0x82257490:
      return TitledListScreenPage7(r3);
    case 0x82257D48:
      return TitledListScreenStep0(r3, sp, calls);
    case 0x82257FF8:
      return TitledListScreenStep1(r3, sp, calls);
    case 0x822580E0:
      return TitledListScreenStep2(r3, sp, calls);
    // The color list screen's slot 22 and its page table's pages
    // (color_list.cpp).
    case 0x82255200:
      return ColorListScreenReset(r3);
    case 0x822552B8:
      return ColorListScreenPage2(r3, sp, calls);
    case 0x82255490:
      return ColorListScreenPage3(r3, sp, calls);
    case 0x82255658:
      return ColorListScreenPage4(r3, sp, calls);
    case 0x82256508:
      return ColorListScreenPage5(r3, sp, calls);
    case 0x822557F0:
      return ColorListScreenPage7(r3);
    case 0x822562E0:
      return ColorListScreenPage9(r3, sp, calls);
    // Its step table's steps (none reads r4).
    case 0x82258578:
      return ColorListScreenStep0(r3, sp, calls);
    case 0x82255890:
      return ColorListScreenStep1(r3, sp, calls);
    case 0x82256740:
      return ColorListScreenStep2(r3, sp, calls);
    case 0x82255B58:
      return ColorListScreenStep3(r3, sp, calls);
    case 0x82256908:
      return ColorListScreenStep4(r3, sp, calls);
    case 0x82255C88:
      return ColorListScreenRunMovie(r3, sp, calls);
    case 0x82255CD0:
      return ColorListScreenStep6(r3, sp, calls);
    case 0x82256AC0:
      return ColorListScreenStep7(r3, sp, calls);
    case 0x82256030:
      return ColorListScreenStep8(r3, sp, calls);
    case 0x82255EB0:
      return ColorListScreenStep10(r3, sp, calls);
    case 0x82259D80:
      return NodeScreenPage2(r3, sp, calls);
    case 0x822590E8:
      return NodeScreenPage4(r3);
    case 0x82259EC0:
      return NodeScreenStep0(r3, sp, calls);
    case 0x82259680:
      return NodeScreenStep1(r3, sp, calls);
    case 0x82259750:
      return NodeScreenStep2(r3, sp, calls);
    // Type6NodeScreen's slots that set r4 before reading it (or don't read
    // it): SetUpMenus, SetIcons, SetScrollText, and NodeScreen::SetUpTexts.
    case 0x82236F00:
      return Type6NodeScreenSetUpMenus(r3, sp, calls);
    case 0x82237160:
      return Type6NodeScreenSetIcons(sp);
    case 0x82237228:
      return Type6NodeScreenSetScrollText(sp, calls);
    case 0x82247FE0:
      return NodeScreenSetUpTexts(r3, sp, calls);
    default:
      break;
  }
  if (r4) {
    switch (target) {
      case 0x82252610:
        return ListPartBaseDeletingDestruct(r3, *r4, sp, calls);
      case 0x82252658:
        return ListPartInnerBaseDeletingDestruct(r3, *r4, sp, calls);
      case 0x822529B0:
        return ListPartInnerDeletingDestruct(r3, *r4, sp, calls);
      case 0x82252A38:
        return ListPartDeletingDestruct(r3, *r4, sp, calls);
      case 0x82252490:
        return ListPartPick(r3, *r4, sp, calls);
      case 0x82252500:
        return ListPartInput(r3, *r4, sp, lr, calls);
      case 0x82252A88:
        return ListPartInnerOpen(r3, *r4, sp, calls);
      case 0x82252A00:
        // r5 as the inner's pages leave it unread.
        return ListPartInnerPick(r3, *r4, *r4, sp, lr, calls);
      case 0x82252DF0:
        return ListPartInnerInput(r3, *r4, sp, calls);
      case 0x82252BB8:
        return ListPartInnerStep0(r3, *r4, sp, calls);
      case 0x82252C98:
        return ListPartInnerStep1(r3, *r4, sp, calls);
      case 0x82252D38:
        return ListPartInnerStep2(r3, *r4, sp, calls);
      case 0x82252248:
        return ListScreenDeletingDestruct(r3, *r4, sp, calls);
      case 0x822579E8:
        return TitledListScreenDraw(r3, *r4, sp, calls);
      case 0x82258998:
        return TitledListScreenUpdate(r3, *r4, sp, calls);
      case 0x82257360:
        return TitledListScreenPage4(r3, *r4, sp, calls);
      case 0x82255818:
        return ListScreenLeavePage(r3, *r4, sp, lr, calls);
      case 0x82257990:
        return NodeListScreenDeletingDestruct(r3, *r4, true, sp, calls);
      case 0x82257940:
        return NodeListScreenDeletingDestruct(r3, *r4, false, sp, calls);
      // The color list screen's slots 23, 24 and 0 and its page 6.
      case 0x82256CA0:
        return ColorListScreenDraw(r3, *r4, sp, calls);
      case 0x82256E08:
        return ColorListScreenUpdate(r3, *r4, sp, calls);
      case 0x822565A0:
        return ColorListScreenPage6(r3, *r4, sp, calls);
      case 0x82255840:
        return ColorListScreenDeletingDestruct(r3, *r4, sp, calls);
      // The type 6 list screen's slots 2, 6, 26 and 0, Type6PanelScreen's
      // slot 4 and the group pair's slot 0 (type6_list.cpp).
      case 0x822336C0:
        return Type6ListScreenSetItemTexts(r3, *r4, sp, calls);
      case 0x822337B0:
        return Type6ListScreenBack(r3, *r4, sp, calls);
      case 0x82233730:
        return Type6ListScreenShowMarks(r3, *r4, sp);
      case 0x82233478:
        return Type6ListScreenDeletingDestruct(r3, *r4, sp, calls);
      case 0x8223E708:
        return PanelScreenPost(r3, *r4, target);
      case 0x8224ACE0:
        return GroupPairDeletingDestruct(r3, *r4, sp, calls);
      // The type 7 list screen's slots 2, 3, 6, 26, 27 and 0
      // (type7_list.cpp).
      case 0x82243238:
        return Type7ListScreenSetItemText(r3, *r4, sp, calls);
      case 0x822432A8:
        return Type7ListScreenClearItemText(r3, *r4, sp, calls);
      case 0x822434C8:
        return Type7ListScreenBack(r3, *r4, sp, calls);
      case 0x82243308:
        return Type7ListScreenSetDetails(r3, *r4, sp, calls);
      case 0x82243448:
        return Type7ListScreenShowMarks(r3, *r4, sp);
      case 0x82242FC8:
        return Type7ListScreenDeletingDestruct(r3, *r4, sp, calls);
      case 0x82259328:
        return NodeScreenPage5(r3, *r4, sp, calls);
      case 0x8225A168:
        return NodeScreenDeletingDestruct(r3, *r4, sp, calls);
      case 0x822370A0:
        return Type6NodeScreenSetText(r3, *r4, sp, calls);
      case 0x822372A0:
        return Type6NodeScreenShowMarks(*r4, sp);
      case 0x82237110:
        return Type6NodeScreenBack(r3, *r4);
      case 0x82237048:
        return Type6NodeScreenDeletingDestruct(r3, *r4, sp, calls);
      // The type 8 list part's and its inner part's slots, the inner's
      // pages and steps and the shared ones (src/type8_screens/list_part.cpp).
      case 0x82254580:
        return type8_screens::ListPartDeletingDestruct(r3, *r4, sp, calls);
      case 0x82254530:
        return type8_screens::ListInnerDeletingDestruct(r3, *r4, sp, calls);
      case 0x822549D8:
        return type8_screens::ListInnerOpen(r3, *r4, sp, calls);
      case 0x822550C0:
        // r5 as the inner's pages leave it unread.
        return type8_screens::ListInnerPick(r3, *r4, r5.value_or(*r4), sp, lr, calls);
      case 0x82254E18:
        return type8_screens::ListInnerInput(r3, *r4, sp, calls);
      case 0x82254D08:
        return type8_screens::ListInnerStep0(r3, *r4, sp, calls);
      case 0x82254FB8:
        return type8_screens::ListInnerPage0(r3, *r4, sp, calls);
      case 0x822538A8:
        return type8_screens::PartInnerStep1(r3, *r4, sp, calls);
      case 0x82253838:
        return type8_screens::PartInnerPage2(r3, *r4, sp);
      // The type 8 color list part's (src/type8_screens/color_part.cpp).
      case 0x822532B8:
        return type8_screens::ColorPartDeletingDestruct(r3, *r4, sp, calls);
      case 0x82253268:
        return type8_screens::ColorInnerDeletingDestruct(r3, *r4, sp, calls);
      case 0x822537A0:
        return type8_screens::ColorInnerOpen(r3, *r4, sp, calls);
      case 0x82254280:
        return type8_screens::ColorInnerPick(r3, *r4, r5.value_or(*r4), sp, lr, calls);
      case 0x82253FC0:
        return type8_screens::ColorInnerInput(r3, *r4, sp, calls);
      case 0x82253E48:
        return type8_screens::ColorInnerStep0(r3, *r4, sp, calls);
      case 0x82254178:
        return type8_screens::ColorInnerPage0(r3, *r4, sp, calls);
      case 0x82253238:
        return type8_screens::ColorPartIsFirstChoice(r3, *r4, sp);
      case 0x82253240:
        return type8_screens::ColorPartThirdChoice(r3, *r4);
      // The type 8 list screen's slots 0, 2, 23, 24 and 26 (both vtables'
      // slot 0) and its page 5 (src/type8_screens/list_screen.cpp).
      case 0x82257B20:
        return type8_screens::ListBaseDeletingDestruct(r3, *r4, sp, calls);
      case 0x8223F388:
        return type8_screens::ListScreen8DeletingDestruct(r3, *r4, sp, calls);
      case 0x8223F5D8:
        return type8_screens::ListScreen8SetItemTexts(r3, *r4, sp, calls);
      case 0x8223F6C8:
        return type8_screens::ListScreen8ShowMarks(r3, *r4, sp);
      case 0x82258D60:
        return type8_screens::ListBaseDraw(r3, *r4, sp, calls);
      case 0x82258EB0:
        // r4 not read (the step table's guard word replaces it).
        return type8_screens::ListBaseUpdate(r3, r5, sp, calls);
      case 0x82258AD0:
        return type8_screens::ListBasePage5(r3, *r4, sp, calls);
      case 0x8223F778:
        return type8_screens::ListScreen8SetUpRace(r3, *r4, sp, calls);
      // The type 8 color list screen's slots 2, 3, 26 and 7
      // (src/type8_screens/color_list_screen.cpp).
      case 0x8223E4F8:
        return type8_screens::ColorList8SetItemText(r3, *r4, sp, calls);
      case 0x8223E568:
        return type8_screens::ColorList8ClearItemText(r3, *r4, sp, calls);
      case 0x8223E5C8:
        return type8_screens::ColorList8SetDetails(r3, *r4, sp, calls);
      case 0x8223E760:
        return type8_screens::ColorList8SetUpRace(r3, *r4, sp, calls);
      default:
        break;
    }
  }
  switch (target) {
    case kEmpty:
      return r3;
    case 0x82242F70:
      return ListScreenIsLoaded(r3, sp);
    case 0x82233320:
      return ListScreenOpenAsset(r3, false, sp, lr, calls);
    case 0x82242F10:
      return ListScreenOpenAsset(r3, true, sp, lr, calls);
    case 0x82233330:
      return ListScreenRelease(r3, false, sp, calls);
    case 0x82242F20:
      return ListScreenRelease(r3, true, sp, calls);
    case 0x82236FD8:
      return NodeScreenIsLoaded(r3, sp);
    case 0x82236EB0:
      return NodeScreenOpenAsset(r3, sp, lr, calls);
    case 0x82236EC0:
      return NodeScreenRelease(r3, sp, calls);
    case 0x82230790:
      return PanelScreenIsLoaded(sp);
    case 0x82230758:
      return PanelScreenOpenMovies(sp, false, calls);
    case 0x822401D0:
      return PanelScreenOpenMovies(sp, true, calls);
    case 0x82240208:
      return PanelScreenDeleteMovies(sp, calls);
    case 0x82240860:
    case 0x82247058:
    case 0x822408B8:
      if (!r4) return std::nullopt;
      return PanelScreenPost(r3, *r4, target);
    case 0x822405F8:
      if (!r4) return std::nullopt;
      return Type7PanelScreenPickCar(r3, *r4, sp, calls);
    case 0x82230C88:
      if (!r4) return std::nullopt;
      return Type6PanelScreenPickCar(r3, *r4, sp, calls);
    case 0x82247E88:
      if (!r4) return std::nullopt;
      return NodeTableDeletingDestruct(r3, *r4, sp, calls);
    case 0x82248650:
      return ColorNodeTableReset(r3, sp, calls);
    case 0x82240238:
      return Type7CarWord(1972);
    case 0x82240248:
      return Type7CarWord(1976);
    case 0x82240258:
      return Type7CarWord(1980);
    // The type 6 list screen's slots 7 and 14 and Type6PanelScreen's slots 6
    // to 8 (type6_list.cpp).
    case 0x822334C8:
      return Type6ListScreenSetUpRace(r3, sp, calls);
    case 0x82233378:
      return Type6ListScreenSetUpMenus(r3, sp, calls);
    case 0x822307D0:
      return Type6CarWord(1972);
    case 0x822307E0:
      return Type6CarWord(1976);
    case 0x822307F0:
      return Type6CarWord(1980);
    // The type 7 list screen's slots 7 and 14 (type7_list.cpp).
    case 0x82243018:
      return Type7ListScreenSetUpRace(r3, sp, calls);
    case 0x82243588:
      return Type7ListScreenSetUpMenus(r3, sp, calls);
    case 0x82249C30:
      return GroupPairSelectA(r3);
    case 0x82249C40:
      return GroupPairReset(r3, sp, calls);
    case 0x82249BE0:
      return GroupPairRun(r3, sp, calls);
    case 0x82249BA8:
      return GroupPairWaitForFade(r3);
    case 0x82249C70:
      return GroupPairFinishAll(r3, sp, calls);
    // The type 8 panel screen's slots 2, 13 and 18 (src/type8_screens/).
    case 0x82238418:
      return type8_screens::IsLoaded();
    case 0x82238440:
      return type8_screens::OpenMovies(sp, lr, calls);
    case 0x82238450:
      return type8_screens::DeleteMovies(sp, lr, calls);
    // Its slot 3 (PickCar; r4, the dispatcher, not read).
    case 0x82238C88:
      return type8_screens::PickCar(r3, sp, calls);
    // The type 8 inner parts' shared page 1 (list_part.cpp).
    case 0x82253108:
      return type8_screens::PartInnerPage1(r3);
    // The type 8 list screen's slots 1, 12, 14, 18 and 27, its pages and
    // steps (list_screen.cpp; none reads r4).
    case 0x8223F590:
      return type8_screens::ListScreen8IsLoaded(r3);
    case 0x8223F3E0:
      return type8_screens::ListScreen8OpenMovie(r3, sp, lr, calls);
    case 0x8223F588:
      return type8_screens::ListScreen8Release(r3, sp, lr, calls);
    case 0x8223F3F0:
      return type8_screens::ListScreen8SetUpMenus(r3, sp, calls);
    case 0x8223F648:
      return type8_screens::ListScreen8SetLabels(r3, sp, calls);
    case 0x82257588:
      return type8_screens::ListBasePage2(r3, sp, calls);
    case 0x82257768:
      return type8_screens::ListBasePage3(r3, sp, calls);
    case 0x82258A40:
      return type8_screens::ListBasePage4(r3, sp, calls);
    case 0x822578D8:
      return type8_screens::ListBasePage6(r3);
    case 0x82257900:
      return type8_screens::ListBasePage8(r3);
    case 0x82258158:
      return type8_screens::ListBaseStep0(r3, sp, calls);
    case 0x82258648:
      return type8_screens::ListBaseStep1(r3, sp, calls);
    case 0x82258BE0:
      return type8_screens::ListBaseStep2(r3, sp, calls);
    case 0x82258438:
      return type8_screens::ListBaseStep3(r3, sp, calls);
    // The type 8 color list screen's slots 12, 14 and 28
    // (color_list_screen.cpp; none reads r4).
    case 0x8223E310:
      return type8_screens::ColorList8OpenMovie(r3, sp, lr, calls);
    case 0x8223E320:
      return type8_screens::ColorList8SetUpMenus(r3, sp, calls);
    case 0x8223E698:
      return type8_screens::ColorList8SetLabels(r3, sp, calls);
    default:
      return std::nullopt;
  }
}

// Whether SlotNative runs `target` (given whether the call sets r4 and r5).
bool SlotIsNative(uint32_t target, bool has_r4, bool has_r5) {
  switch (target) {
    case 0x82256FE8:
      return has_r5;
    case 0x82237008:
      return has_r4 && has_r5;
    case 0x82259998:
      return has_r5;
    case 0x82242FA0:
    case 0x8223F598:
    case 0x8223E4D0:
      return has_r4 && has_r5;
    case 0x82243238:
    case 0x822432A8:
    case 0x822434C8:
    case 0x82243308:
    case 0x82243448:
    case 0x82242FC8:
    case 0x82259328:
    case 0x8225A168:
    case 0x822370A0:
    case 0x822372A0:
    case 0x82237110:
    case 0x82237048:
    case 0x82254580:
    case 0x82254530:
    case 0x822549D8:
    case 0x822550C0:
    case 0x82254E18:
    case 0x82254D08:
    case 0x82254FB8:
    case 0x822538A8:
    case 0x82253838:
    case 0x822532B8:
    case 0x82253268:
    case 0x822537A0:
    case 0x82254280:
    case 0x82253FC0:
    case 0x82253E48:
    case 0x82254178:
    case 0x82253238:
    case 0x82253240:
    case 0x82257B20:
    case 0x8223F388:
    case 0x8223F5D8:
    case 0x8223F6C8:
    case 0x82258D60:
    case 0x82258EB0:
    case 0x82258AD0:
    case 0x8223F778:
    case 0x8223E4F8:
    case 0x8223E568:
    case 0x8223E5C8:
    case 0x8223E760:
      return has_r4;
    case 0x82253108:
    case 0x8223F590:
    case 0x8223E310:
    case 0x8223E320:
    case 0x8223E698:
    case 0x8223F3E0:
    case 0x8223F588:
    case 0x8223F3F0:
    case 0x8223F648:
    case 0x82257588:
    case 0x82257768:
    case 0x82258A40:
    case 0x822578D8:
    case 0x82257900:
    case 0x82258158:
    case 0x82258648:
    case 0x82258BE0:
    case 0x82258438:
      return true;
    case 0x82236F00:
    case 0x82237160:
    case 0x82237228:
    case 0x82247FE0:
      return true;
    case 0x82243018:
    case 0x82243588:
      return true;
    case 0x822336C0:
    case 0x822337B0:
    case 0x82233730:
    case 0x82233478:
    case 0x8223E708:
    case 0x8224ACE0:
      return has_r4;
    case 0x822334C8:
    case 0x82233378:
    case 0x822307D0:
    case 0x822307E0:
    case 0x822307F0:
      return true;
    case 0x82252610:
    case 0x82252658:
    case 0x822529B0:
    case 0x82252A38:
    case 0x82252490:
    case 0x82252500:
    case 0x82252A88:
    case 0x82252A00:
    case 0x82252DF0:
    case 0x82252248:
    case 0x822579E8:
    case 0x82258998:
    case 0x82257360:
    case 0x82255818:
    case 0x82257990:
    case 0x82257940:
    case 0x82256CA0:
    case 0x82256E08:
    case 0x822565A0:
    case 0x82255840:
    case 0x82252BB8:
    case 0x82252C98:
    case 0x82252D38:
    case 0x82240860:
    case 0x82247058:
    case 0x822408B8:
    case 0x822405F8:
    case 0x82230C88:
    case 0x82247E88:
      return has_r4;
    case kEmpty:
    case 0x82242F70:
    case 0x82233320:
    case 0x82242F10:
    case 0x82233330:
    case 0x82242F20:
    case 0x82236FD8:
    case 0x82236EB0:
    case 0x82236EC0:
    case 0x82230790:
    case 0x82230758:
    case 0x822401D0:
    case 0x82240208:
    case 0x82240238:
    case 0x82240248:
    case 0x82240258:
    case 0x82249C30:
    case 0x82249C40:
    case 0x82249BE0:
    case 0x82249BA8:
    case 0x82249C70:
    case 0x82238418:
    case 0x82238440:
    case 0x82238450:
    case 0x82238C88:
    case 0x82248650:
    case 0x82252420:
    case 0x82252600:
    case 0x82252430:
    case 0x82252530:
    case 0x82252590:
    case 0x822525B8:
    case 0x822526F8:
    case 0x82252968:
    case 0x82252748:
    case 0x82252740:
    case 0x822528C8:
    case 0x82252778:
    case 0x82252878:
    case 0x822528A8:
    case 0x82257558:
    case 0x82257148:
    case 0x822572F0:
    case 0x82257490:
    case 0x82257D48:
    case 0x82257FF8:
    case 0x822580E0:
    case 0x82255200:
    case 0x822552B8:
    case 0x82255490:
    case 0x82255658:
    case 0x82256508:
    case 0x822557F0:
    case 0x822562E0:
    case 0x82258578:
    case 0x82255890:
    case 0x82256740:
    case 0x82255B58:
    case 0x82256908:
    case 0x82255C88:
    case 0x82255CD0:
    case 0x82256AC0:
    case 0x82256030:
    case 0x82255EB0:
    case 0x82259D80:
    case 0x822590E8:
    case 0x82259EC0:
    case 0x82259680:
    case 0x82259750:
      return true;
    default:
      return false;
  }
}

uint64_t Slot(uint64_t object_r3, uint32_t slot, std::optional<uint64_t> r4,
              uint32_t frame, uint32_t lr, const Calls& calls) {
  const uint32_t target = LoadU32(LoadU32(uint32_t(object_r3)) + slot);
  if (SlotIsNative(target, r4.has_value())) {
    // The original sets r4 before the call; the callee may leave another.
    if (r4) {
      calls.set_r4(*r4);
    }
    return *SlotNative(target, object_r3, r4, frame, lr, calls);
  }
  if (r4) {
    return calls.call(target, frame, lr, object_r3, *r4);
  }
  const uint64_t args[6] = {object_r3, 0, 0, 0, 0, 0};
  return calls.call_set(target, args, 1, frame, lr);
}

uint64_t Slot5(uint64_t object_r3, uint32_t slot, uint64_t r4, uint64_t r5,
               uint32_t frame, uint32_t lr, const Calls& calls) {
  const uint32_t target = LoadU32(LoadU32(uint32_t(object_r3)) + slot);
  if (SlotIsNative(target, true, true)) {
    calls.set_r4(r4);
    return *SlotNative(target, object_r3, r4, frame, lr, calls, r5);
  }
  const uint64_t args[6] = {object_r3, r4, r5, 0, 0, 0};
  return calls.call_set(target, args, 3, frame, lr);
}

}  // namespace type67_screens
