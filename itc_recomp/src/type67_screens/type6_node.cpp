// The type 6 node screen's own slots and destructors (Type6NodeScreen, over
// NodeScreen; docs/functions/type67-screens.md): its menus' set-up, the
// course texts, the back it leaves through, the icons, the scroll text, the
// marks, NodeScreen::SetUpTexts (slot 1 of NodeScreen and PairedNodeTable)
// and the destructors. See type67_screens.h. UiMovies::Find, UiMovie::
// FindNode, SetNodeTexture and HideNode, MenuList::Selection and
// SetSelection through their plain C++; MenuList::Init's plain C++ where it
// frees natively, else a guest call; UiMovie::SetText through
// Calls::set_text; sub_8211C5F8, UiMovie::ScrollText and operator delete
// guest calls; NodeScreen's item loader, course records and destructor by
// their plain C++; the screen's slot 2 through Slot.

#include "type67_screens/type67_screens.h"

#include "base/log.h"
#include "car_list/car_list.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "save_data/save_data.h"
#include "type67_screens/list_pages.h"
#include "ui_movies/game.h"

namespace type67_screens {

namespace {

using namespace pages;
using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kListInit = 0x8220BAC0, kScrollText = 0x8211C570,
                   kResetTextScroll = 0x8211C5F8;
// The course texts' string ids (7 a row: the type 6 list screen's table).
constexpr uint32_t kCourseTexts = 0x826366FC;
// The node screen's movie id (the slots that don't read +10100).
constexpr uint32_t kNodeMovie = 15;

// The movie of the id at the frame's +80 (written first), by UiMovies::Find.
uint32_t MovieById(uint8_t* base, uint32_t id, uint32_t frame) {
  StoreU32(base, frame + 80, id);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

// UiMovie::HideNode (sub_8211AC48): the node's +80 cleared; the node.
uint32_t Hide(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  StoreU32(base, node + 80, 0);
  return node;
}

// MenuList::Init on `list` with the ids at the frame's +84 and +92 written
// first (the original's stack arguments): its plain C++ where it frees the
// old block natively, else a guest call.
void InitList(uint8_t* base, const Calls& calls, uint32_t list, uint32_t frame, uint32_t lr,
              const save_data::MenuListArgs& a) {
  StoreU32(base, frame + 92, a.id6);
  StoreU32(base, frame + 84, a.id5);
  if (calls.native_heap && save_data::MenuListInitNative(base, list)) {
    save_data::MenuListInit(base, list, a);
    return;
  }
  const uint64_t args[8] = {list, a.count, a.shown, a.r6, a.r7, a.r8, a.r9, a.r10};
  calls.call8(kListInit, args, frame, lr);
}

}  // namespace

uint64_t Type6NodeScreenSetUpMenus(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82236F00 Type6NodeScreen::SetUpMenus");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 144);
  InitList(base, calls, screen + 10132, frame, 0x82236F48, {8, 6, 1, 154, 153, 155, 156, 159, 158});
  InitList(base, calls, screen + 10204, frame, 0x82236F78, {2, 2, 1, 156, 155, 154, 153, 159, 158});
  race_screens::ListSetSelection(base, screen + 10204, 1);
  // Slot 2 (SetText) with r4 = 0.
  Slot(base, r3, 8, 0, frame, 0x82236F9C, calls);
  NodeScreenLoadItem(base, r3, frame, calls);
  const uint32_t traffic = race_screens::ListSelection(base, screen + 10204);
  const uint32_t selection = race_screens::ListSelection(base, screen + 10132);
  const uint64_t course = uint64_t(LoadU32(base, screen + 10136)) + uint64_t(selection);
  return NodeScreenShowRecords(base, r3, course, traffic, frame, calls);
}

uint64_t Type6NodeScreenSetText(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                                const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822370A0 Type6NodeScreen::SetText");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieById(base, LoadU32(base, uint32_t(r3) + 10100), frame);
  const uint32_t texts = (uint32_t(r4) << 2) + kCourseTexts;
  uint64_t result = 0;
  for (uint32_t i = 0; i < 7; i++) {
    result = calls.set_text(frame, 0x822370F8, movie, 5, 5 + i, LoadU32(base, texts + 4 * i));
  }
  return result;
}

uint64_t Type6NodeScreenBack(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82237110 Type6NodeScreen::Back");
  // Message 6: back to Type6Screen0.
  Post(base, uint32_t(r4), 6);
  return r3;
}

uint64_t Type6NodeScreenSetIcons(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82237160 Type6NodeScreen::SetIcons");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = MovieById(base, kNodeMovie, frame);
  // UiMovie::SetNodeTexture (sub_8211A708): the lookup, then the texture.
  ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, 2, 4), 0);
  ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, 2, 5), 1);
  uint32_t node = 0;
  for (const uint32_t child : {6u, 7u, 8u, 9u, 13u, 14u}) node = Hide(base, movie, 2, child);
  return node;
}

uint64_t Type6NodeScreenSetScrollText(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82237228 Type6NodeScreen::SetScrollText");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = MovieById(base, kNodeMovie, frame);
  const uint64_t reset[6] = {movie, 1, 0, 0, 0, 0};
  calls.call_set(kResetTextScroll, reset, 3, frame, 0x82237264);
  calls.set_text(frame, 0x82237278, movie, 1, 0, 934);
  const uint64_t scroll[6] = {movie, 1, 0, 1, 0, 0};
  return calls.call_set(kScrollText, scroll, 4, frame, 0x8223728C);
}

uint64_t Type6NodeScreenShowMarks(uint8_t* base, uint64_t r4, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822372A0 Type6NodeScreen::ShowMarks");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieById(base, kNodeMovie, frame);
  const uint32_t which = uint32_t(r4);
  uint32_t node = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 13);
  StoreU32(base, node + 80, which != 0 ? 1 : 0);
  node = ui_movies::FindNode(base, movie, 0xFFFFFFFFu, 14);
  StoreU32(base, node + 80, which + 6 - 8 != 0 ? 1 : 0);
  return node;
}

uint64_t NodeScreenSetUpTexts(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82247FE0 NodeScreen::SetUpTexts");
  const uint32_t table = uint32_t(r3), frame = Frame(base, sp, 128);
  StoreU32(base, frame + 80, LoadU32(base, table + 4));
  const uint32_t found = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = LoadU32(base, found);
  uint64_t result = found;
  for (uint32_t i = 0; i < 27; i++) {
    const uint32_t child = LoadU32(base, table + 156 + 4 * i);
    if (int32_t(child) >= 0) result = Hide(base, movie, LoadU32(base, table + 12), child);
    const uint32_t first = LoadU32(base, table + 420);
    if (int32_t(first) > -1) result = Hide(base, movie, LoadU32(base, table + 8), first + i);
  }
  for (uint32_t i = 0; i < 8; i++) {
    const uint32_t child = LoadU32(base, table + 264 + 4 * i);
    if (int32_t(child) >= 0) result = Hide(base, movie, LoadU32(base, table + 8), child);
  }
  const uint32_t texts = LoadU32(base, table + 424);
  if (int32_t(texts) > -1) {
    calls.set_text(frame, 0x822480A0, movie, LoadU32(base, table + 8), texts, 402);
    static constexpr uint32_t kIds[3] = {401, 407, 408};
    static constexpr uint32_t kLr[3] = {0x822480B8, 0x822480D0, 0x822480E8};
    for (uint32_t k = 0; k < 3; k++) {
      const uint32_t child = LoadU32(base, table + 424) + 1 + k;
      result = calls.set_text(frame, kLr[k], movie, LoadU32(base, table + 8), child, kIds[k]);
    }
  }
  return result;
}

uint64_t Type6NodeScreenDestruct(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82236EA0 Type6NodeScreen::~Type6NodeScreen");
  StoreU32(base, uint32_t(r3), kType6NodeScreenVtable);
  // A tail call of ~NodeScreen.
  return NodeScreenDestruct(base, r3, sp, calls);
}

uint64_t Type6NodeScreenDeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                         const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82237048 Type6NodeScreen::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, uint32_t(r3), kType6NodeScreenVtable);
  NodeScreenDestruct(base, r3, frame, calls);
  if ((flags & 1) != 0) {
    // operator delete, r4 as the destructor left it.
    Call1(calls, car_list::kOperatorDelete, r3, frame, 0x82237084);
  }
  return r3;
}

}  // namespace type67_screens
