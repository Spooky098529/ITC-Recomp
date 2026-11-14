// The type 8 mode's first screen (docs/functions/type8-mode.md):
// Type8Screen0's constructor, destructors and its simpler handlers and
// pages, over movie 24 (the menu) and movie 16 (the yes/no dialog). See
// type8_mode.h. UiMovies::Find, IsLoaded, UiMovie::FindNode, the node
// setters, MenuList::SetSelection and Selection and ScreenMachine's
// constructor through their plain C++; UiMovie::SetText through the calls'
// set_text, MenuList::Init and Release through their plain C++ where they
// free natively; UiMovies::Open and Delete through the calls; FinishAll,
// operator delete and the pages not ours as guest calls.

#include "type8_mode/type8_mode.h"

#include "base/log.h"
#include "boot_mode/boot_mode.h"
#include "guest.h"
#include "race_screens/race_screens.h"
#include "save_data/save_data.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace type8_mode {

namespace {

using guest::LoadU32;
using guest::StoreU32;

constexpr uint32_t kMinusOne = 0xFFFFFFFF;
constexpr uint32_t kVtable = 0x82010B74;
constexpr uint32_t kFinishAll = 0x8211DFF8;
constexpr uint32_t kOperatorDelete = 0x825A70E8;
constexpr uint32_t kListInit = 0x8220BAC0;
constexpr uint32_t kModeBefore = 0x826032E0;
constexpr uint32_t kList = 9680;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  const uint32_t frame = sp - bytes;
  StoreU32(base, frame, sp);
  return frame;
}

// The movie with id `id`: the id at the frame's +80, then UiMovies::Find.
uint32_t MovieAt(uint8_t* base, uint32_t frame, uint32_t id) {
  StoreU32(base, frame + 80, id);
  return LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
}

// A message (`message`, 0, 0) posted in the dispatcher's ring (+204), the
// words in the original's order (`id_last`: +8, +4, then the id).
void Post(uint8_t* base, uint32_t dispatcher, uint32_t message, bool id_last = false) {
  const uint32_t ring = dispatcher + 204;
  const uint32_t count = LoadU32(base, ring + 600) + 1;
  const uint32_t end = LoadU32(base, ring + 604) + count - 1;
  StoreU32(base, ring + 600, count);
  const uint32_t slot = ring + (end - (end / 50) * 50) * 12;
  if (id_last) {
    StoreU32(base, slot + 8, 0);
    StoreU32(base, slot + 4, 0);
    StoreU32(base, slot + 0, message);
  } else {
    StoreU32(base, slot + 0, message);
    StoreU32(base, slot + 4, 0);
    StoreU32(base, slot + 8, 0);
  }
}

// UiMovie::ShowNode and HideNode's work (+80 = 1 or 0); the node.
uint32_t SetShown(uint8_t* base, uint32_t movie, uint32_t sub_scene, uint32_t child,
                  uint32_t shown) {
  const uint32_t node = ui_movies::FindNode(base, movie, sub_scene, child);
  StoreU32(base, node + 80, shown);
  return node;
}

uint32_t Texture(uint8_t* base, uint32_t movie, uint32_t child, uint32_t texture) {
  return ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, movie, 6, child), texture);
}

}  // namespace

uint64_t Screen0Construct(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223D2F0 Type8Screen0::Type8Screen0");
  const uint32_t screen = uint32_t(r3);
  Frame(base, sp, 96);
  boot_mode::ScreenMachineConstruct(base, screen + 4);
  StoreU32(base, screen + 0, kVtable);
  StoreU32(base, screen + 9748, 0);
  StoreU32(base, screen + 9740, 0);
  for (const uint32_t block : {screen + 9752, screen + 9772}) {
    StoreU32(base, block + 0, 0);
    StoreU32(base, block + 12, kMinusOne);
    StoreU32(base, block + 8, kMinusOne);
    StoreU32(base, block + 4, kMinusOne);
    StoreU32(base, block + 16, 0);
  }
  StoreU32(base, screen + 9752, 0);
  StoreU32(base, screen + 9768, 0);
  StoreU32(base, screen + 9788, 0);
  StoreU32(base, screen + 9772, 0);
  StoreU32(base, screen + 9756, 45);
  StoreU32(base, screen + 9760, 44);
  StoreU32(base, screen + 9764, 46);
  StoreU32(base, screen + 9644, 1);
  StoreU32(base, screen + 9776, 48);
  StoreU32(base, screen + 9780, 47);
  StoreU32(base, screen + 9784, 49);
  return r3;
}

uint64_t Screen0Destruct(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                         const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BF00 Type8Screen0::~Type8Screen0");
  StoreU32(base, uint32_t(r3), kVtable);
  return type67_screens::ReleaseList(base, r3 + kList, sp, lr, calls);
}

uint64_t Screen0DeletingDestruct(uint8_t* base, uint64_t r3, uint64_t flags, uint32_t sp,
                                 const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C528 Type8Screen0::DeletingDestructor");
  const uint32_t frame = Frame(base, sp, 112);
  StoreU32(base, uint32_t(r3), kVtable);
  type67_screens::ReleaseList(base, r3 + kList, frame, 0x8223C558, calls);
  if ((flags & 1) != 0) {
    const uint64_t args[6] = {r3, 0, 0, 0, 0, 0};
    calls.call_set(kOperatorDelete, args, 1, frame, 0x8223C568);
  }
  return r3;
}

uint64_t Screen0OnEnter(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B630 Type8Screen0::OnEnter");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 144);
  StoreU32(base, frame + 92, 158);
  StoreU32(base, frame + 84, 159);
  StoreU32(base, screen + 9648, kMinusOne);
  StoreU32(base, screen + 9644, 1);
  StoreU32(base, screen + 9656, 0);
  StoreU32(base, screen + 9652, 0);
  StoreU32(base, screen + 9660, 0);
  StoreU32(base, screen + 9672, kMinusOne);
  StoreU32(base, screen + 9664, kMinusOne);
  StoreU32(base, screen + 9676, 0);
  const uint32_t list = screen + kList;
  const save_data::MenuListArgs args{5, 5, 1, 154, 153, 155, 156, 159, 158};
  if (calls.native_heap && save_data::MenuListInitNative(base, list)) {
    save_data::MenuListInit(base, list, args);
  } else {
    const uint64_t regs[8] = {r3 + kList, 5, 5, 1, 154, 153, 155, 156};
    calls.call8(kListInit, regs, frame, 0x8223B6A4);
  }
  race_screens::ListSetSelection(base, list, 0);
  StoreU32(base, screen + 9608, LoadU32(base, screen + 9616) != 0 ? 1 : 9);
  return r3 + kList;
}

uint64_t Screen0OpenMovie(uint32_t sp, uint32_t lr, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B530 Type8Screen0::OpenMovie");
  return calls.open(24, 22583, sp, lr);
}

uint64_t Screen0WaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223B6D8 Type8Screen0::WaitForMovie");
  const uint32_t screen = uint32_t(r3);
  Frame(base, sp, 96);
  if (LoadU32(base, screen + 9612) != 0) {
    return r3;
  }
  const uint64_t loaded = ui_movies::IsLoaded(base, 24);
  if (uint32_t(loaded) == 0) {
    return loaded;
  }
  StoreU32(base, screen + 9612, 1);
  StoreU32(base, screen + 9608, LoadU32(base, kModeBefore) == 1 ? 4 : 9);
  return loaded;
}

uint64_t Screen0Suspend(uint8_t* base, uint64_t r3, uint32_t sp, const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B540 Type8Screen0::Suspend");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 96);
  if (LoadU32(base, screen + 9620) == 0) {
    return r3;
  }
  const uint64_t result = calls.remove(24, frame, 0x8223B568);
  StoreU32(base, screen + 9612, 0);
  return result;
}

uint64_t Screen0Pick(uint8_t* base, uint64_t r3, uint32_t answer) {
  switch (answer) {
    case 1:
      ITC_NATIVE_RUNNING("sub_8223B590 Type8Screen0::PickYes");
      break;
    case 0:
      ITC_NATIVE_RUNNING("sub_8223B5A0 Type8Screen0::PickNo");
      break;
    default:
      ITC_NATIVE_RUNNING("sub_8223B5B0 Type8Screen0::PickBack");
      break;
  }
  StoreU32(base, uint32_t(r3) + 9648, answer);
  return r3;
}

uint64_t Screen0ShowPage(uint8_t* base, uint64_t a, uint64_t b, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223BD20 Type8Screen0::ShowPage");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame, 24);
  const bool first = int32_t(a) != 0, second = int32_t(b) != 0;
  if (!first && !second) {
    SetShown(base, movie, kMinusOne, 6, 0);
  } else {
    SetShown(base, movie, kMinusOne, 6, 1);
  }
  if (first) {
    SetShown(base, movie, 6, 4, 1);
    SetShown(base, movie, 6, 5, 1);
    Texture(base, movie, 4, 0);
    Texture(base, movie, 5, 0);
  } else {
    SetShown(base, movie, 6, 4, 0);
    SetShown(base, movie, 6, 5, 0);
  }
  if (second) {
    SetShown(base, movie, 6, 6, 1);
    SetShown(base, movie, 6, 7, 1);
    Texture(base, movie, 6, 1);
    Texture(base, movie, 7, 1);
  } else {
    SetShown(base, movie, 6, 6, 0);
    SetShown(base, movie, 6, 7, 0);
  }
  SetShown(base, movie, 6, 8, 0);
  return SetShown(base, movie, 6, 9, 0);
}

uint64_t Screen0OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223C098 Type8Screen0::OnDialogClosed");
  const uint32_t screen = uint32_t(r3);
  const uint32_t answer = LoadU32(base, screen + 9648);
  StoreU32(base, screen + 9644, 1);
  if (answer == 1) {
    return r3;
  }
  return Screen0ShowPage(base, 1, 0, sp);
}

uint64_t Screen0Leave(uint8_t* base, uint32_t dispatcher, uint32_t sp,
                      const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223BA48 Type8Screen0::Leave");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = MovieAt(base, frame, 24);
  uint64_t r3 = movie;
  if (movie != 0) {
    calls.call(kFinishAll, frame, 0x8223BA84, movie, frame + 80);
    r3 = calls.remove(24, frame, 0x8223BA8C);
  }
  Post(base, dispatcher, 4);
  return r3;
}

uint64_t Screen0OnLeft(uint8_t* base, uint64_t r3, uint32_t sp, uint32_t lr,
                       const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223B588 Type8Screen0::OnLeft");
  return type67_screens::ReleaseList(base, r3 + kList, sp, lr, calls);
}

uint64_t Screen0Confirm(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp,
                        const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C430 Type8Screen0::Confirm");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = MovieAt(base, frame, 16);
  calls.set_text(frame, 0x8223C474, movie, kMinusOne, 0, 27);
  calls.set_text(frame, 0x8223C488, movie, 1, 3, 2);
  calls.set_text(frame, 0x8223C49C, movie, 1, 4, 3);
  StoreU32(base, movie + 40960, 1);
  StoreU32(base, kDialogPlayer, 0);
  Post(base, dispatcher, 7);
  StoreU32(base, screen + 9648, kMinusOne);
  StoreU32(base, screen + 9644, 0);
  return Screen0ShowPage(base, 1, 1, frame);
}

uint64_t Screen0ConfirmPage(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_8223B5C0 Type8Screen0::ConfirmPage");
  const uint32_t screen = uint32_t(r3);
  const uint32_t answer = LoadU32(base, screen + 9648);
  if (answer == 0 || answer == 2) {
    const uint32_t page = LoadU32(base, screen + 9652);
    if (page != 0) {
      StoreU32(base, screen + 9660, page);
    }
    StoreU32(base, screen + 9664, kMinusOne);
    StoreU32(base, screen + 9652, 0);
    StoreU32(base, screen + 9668, kMinusOne);
    StoreU32(base, screen + 9648, kMinusOne);
  } else if (answer == 1) {
    const uint32_t page = LoadU32(base, screen + 9652);
    if (page != 3) {
      StoreU32(base, screen + 9660, page);
    }
    StoreU32(base, screen + 9664, 3);
    StoreU32(base, screen + 9668, kMinusOne);
    StoreU32(base, screen + 9656, 3);
    StoreU32(base, screen + 9652, kMinusOne);
  }
  return r3;
}

uint64_t Screen0ChosenPage(uint8_t* base, uint64_t r3, uint32_t dispatcher, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223C0C0 Type8Screen0::ChosenPage");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t selection = race_screens::ListSelection(base, screen + kList);
  static constexpr uint32_t kMessages[4] = {16, 14, 19, 21};
  if (selection >= 4) {
    const uint64_t result = Screen0ShowPage(base, 0, 0, frame);
    StoreU32(base, screen + 9608, 6);
    return result;
  }
  StoreU32(base, screen + 9608, 9);
  Post(base, dispatcher, kMessages[selection], true);
  return selection;
}

uint64_t Screen0PlaceCursor(uint8_t* base, uint64_t item, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8223BE88 Type8Screen0::PlaceCursor");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = MovieAt(base, frame, 24);
  // mulli and extsw, the doubleword through the frame's +80 (std, lfd,
  // fcfid, frsp).
  const int64_t value = int64_t(int32_t(uint32_t(item * 40)));
  StoreU32(base, frame + 80, uint32_t(uint64_t(value) >> 32));
  StoreU32(base, frame + 84, uint32_t(value));
  const uint32_t node = ui_movies::FindNode(base, movie, 0, 0);
  guest::StoreFloat(base, node + 16, float(double(value)));
  return node;
}

uint64_t Screen0Draw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                     const ScreenCalls& calls) {
  ITC_NATIVE_RUNNING("sub_8223C888 Type8Screen0::Draw");
  constexpr uint32_t kTable = 0x82814E94, kGuard = 0x82814EB4;
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t guard = LoadU32(base, kGuard);
  if ((guard & 1) == 0) {
    StoreU32(base, frame + 80, 0x8223C6B8);   // MenuPage
    StoreU32(base, frame + 88, 0x8223B5C0);   // ConfirmPage
    StoreU32(base, frame + 96, 0x82106540);   // the empty handler
    StoreU32(base, frame + 84, 0);
    StoreU32(base, frame + 92, 0);
    StoreU32(base, frame + 100, 0);
    StoreU32(base, frame + 108, 0);
    StoreU32(base, frame + 104, 0x8223C0C0);  // ChosenPage
    StoreU32(base, kGuard, guard | 1);
    for (uint32_t i = 0; i < 8; i++) {
      StoreU32(base, kTable + 4 * i, LoadU32(base, frame + 80 + 4 * i));
    }
  }
  const uint32_t page = LoadU32(base, uint32_t(r3) + 9652);
  if (page == kMinusOne) {
    return r3;
  }
  const uint32_t target = LoadU32(base, kTable + (page << 3));
  const uint32_t adjustment = LoadU32(base, kTable + 4 + (page << 3));
  const uint64_t object = uint64_t(adjustment) + r3;
  switch (target) {
    case 0x8223B5C0:
      return Screen0ConfirmPage(base, object);
    case 0x8223C0C0:
      return Screen0ChosenPage(base, object, uint32_t(r4), frame);
    default:
      return calls.call(target, frame, 0x8223C940, object, r4);
  }
}

}  // namespace type8_mode
