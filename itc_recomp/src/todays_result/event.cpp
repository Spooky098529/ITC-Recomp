// Today's event's machine (docs/functions/todays-result.md): see
// todays_result.h. Most of it never ran in play (no unlock was made at the
// end of a day in the sessions measured): ported with its callers, as the
// garage screen's paths were (the user's call of 2026-11-09).

#include "base/log.h"
#include "career/career.h"
#include "data_tables/data_tables.h"
#include "memory/handles.h"
#include "todays_result/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace todays_result {

namespace {

using Machine = schema::TodaysEventMachine;
using Data_ = schema::TodaysEventData;
using List = schema::TodaysEventImageList;
using Image = schema::Type2HubImage;  // the type 2 hub's image record

Pages EventPages(uint8_t* base) { return G::event_pages(base); }
Data_ Data(uint8_t* base) { return G::event_data(base); }

// The image lists: list A at the data's +4 (count +188), list B at +192
// (count +376).
// (Embedded in the data, a host object since E5 step 4.)
List ListA(uint8_t* base, uint32_t data) { return Data_(base, data).images_a(); }
List ListB(uint8_t* base, uint32_t data) { return Data_(base, data).images_b(); }

// A list's record `k` (its words kept in guest order).
uint32_t Record(const List& list, uint32_t k) {
  return schema::host::LoadU32(list.records() + 4 * k);
}

// The image bits' word `word` (+380 on; only words 0 and 1 are the data's,
// any other read where the original reads it).
uint32_t BitsWord(uint8_t* base, const Data_& data, uint32_t word) {
  if (word == 0) {
    return data.image_bits_0();
  }
  if (word == 1) {
    return data.image_bits_1();
  }
  return Load(base, data.image_bits_0_address() + 4 * word);
}
void SetBitsWord(uint8_t* base, const Data_& data, uint32_t word, uint32_t value) {
  if (word == 0) {
    data.set_image_bits_0(value);
  } else if (word == 1) {
    data.set_image_bits_1(value);
  } else {
    Store(base, data.image_bits_0_address() + 4 * word, value);
  }
}

// Whether bit `index` of the data's image bits is set (a negative index
// none).
bool BitSet(uint8_t* base, const Data_& data, int32_t index) {
  if (index < 0) {
    return false;
  }
  return (BitsWord(base, data, uint32_t(index >> 5)) & (1u << (index & 31))) != 0;
}

uint32_t ImagesLoaded(uint8_t* base, const List& list) {
  const uint32_t count = list.count();
  for (uint32_t k = 0; k < count; k++) {
    if (Image(base, Record(list, k)).loaded() == 0) {
      return 0;
    }
  }
  return 1;
}

// The movie's texture words 9 and 8 (its textures' vector data, the
// movie's +28, at +36 and +32).
uint32_t Texture9(uint8_t* base, uint32_t movie) {
  return schema::UiMovie(base, movie).textures() + 36;
}
uint32_t Texture8(uint8_t* base, uint32_t movie) {
  return schema::UiMovie(base, movie).textures() + 32;
}

// The movie's texture words 9 and 8 put back from the data's +400 and +404
// (Unload's and Leave's).
void PutBackImages(uint8_t* base, uint32_t movie) {
  Store(base, Texture9(base, movie), Data(base).movie_object_36());
  Store(base, Texture8(base, movie), Data(base).movie_object_32());
}

}  // namespace

uint32_t ImagesLoadedA(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_8225C0A0 TodaysEvent::ImagesLoadedA");
  return ImagesLoaded(base, ListA(base, data));
}

uint32_t ImagesLoadedB(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_8225C0E8 TodaysEvent::ImagesLoadedB");
  return ImagesLoaded(base, ListB(base, data));
}

uint64_t MarkImages(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225BAB8 TodaysEvent::MarkImages");
  const uint32_t frame = Frame(base, sp, 160);
  const Save save(base, vehicle_accessors::SaveDataOf(base, 0));
  uint64_t r3 = save.address;
  // The unlocks just made: 12-byte entries (+0 shown, +4 the table, +8 the
  // entry).
  const uint32_t unlocks = save.unlocks_shown_address();
  for (uint32_t k = 0; k < save.unlocks_shown_count(); k++) {
    const uint32_t unlock = unlocks + 12 * k;
    const uint32_t table = Load(base, unlock + 4);
    uint32_t entry = Load(base, unlock + 8);
    Store(base, frame + 80, Load(base, unlock + 0));
    while (true) {
      const uint32_t record = data_tables::Entry56(base, table, entry);
      r3 = record;
      const Data_ data = Data(base);
      const int32_t image = int32_t(Load(base, record + 16));
      if (image >= 0) {
        const uint32_t word = uint32_t(image >> 5);
        SetBitsWord(base, data, word, BitsWord(base, data, word) | (1u << (image & 31)));
      }
      const uint32_t next = Load(base, record + 8);
      if (next == 0) {
        break;
      }
      entry += next;
    }
  }
  return r3;
}

uint64_t EventCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225BE50 TodaysEvent::Create");
  const uint32_t frame = Frame(base, sp, 112);
  const Machine m(base, machine);
  m.set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x8225BE7C
  // and the VirtualAlloc(68) at 0x8225BEB8; the globals hold their handles.
  const uint32_t pages_at = schema::PageObject::New(base).address;
  G::set_event_pages(base, pages_at);
  const Pages pages(base, pages_at);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  pages.set_step_started(0);
  const uint32_t menu = schema::FrontEndMenu::New(base).address;
  G::set_event_menu(base, menu);
  InitMenu(schema::FrontEndMenu(base, menu));
  // A host object (E5 step 4) in place of the VirtualAlloc(68) at
  // 0x8225BF14 (zeroed as the page was; 412 bytes, as far as it's used).
  const Data_ data = Data_::New(base);
  G::set_event_data(base, data);
  data.set_ready(0);
  data.set_unlock_entry(kNone);
  data.set_image_count_a(0);
  data.set_image_count_b(0);
  data.set_image_bits_0(0);
  data.set_image_bits_1(0);
  const uint32_t kind = Kind(base, frame);
  if (kind == 0) {
    m.set_loaded(1);
    m.set_state(9);
    return kind;
  }
  const uint64_t r3 = MarkImages(base, frame);
  m.set_state(m.reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t LoadImagePair(uint8_t* base, uint32_t data, uint32_t index, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225BD10 TodaysEvent::LoadImagePair");
  const uint32_t frame = Frame(base, sp, 144);
  uint64_t r3 = 0;
  struct Pair {
    uint32_t asset;
    bool b;
    uint32_t lrs[5];
  };
  static constexpr Pair kPairs[2] = {
      {kImageAssetA, false, {0x8225BD30, 0x8225BD48, 0x8225BD58, 0x8225BD6C, 0x8225BDB8}},
      {kImageAssetB, true, {0x8225BDC4, 0x8225BDDC, 0x8225BDEC, 0x8225BE00, 0x8225BE48}},
  };
  for (const Pair& pair : kPairs) {
    const uint32_t asset = pair.asset + index;
    const uint64_t size = calls.Call(kAssetSize, frame, pair.lrs[0], asset);
    // A blob (E5 step 7: the file in PC memory, in the guest's byte order)
    // in place of the VirtualAlloc(size) at pair.lrs[1]: an XPR2 pack only
    // the game's code reads (Type2Hub::ImageLoaded's XPR::Create copies its
    // header and data out, the data into physical memory, then VirtualFree
    // frees it); whole pages, as the original's, for the archive's sectors.
    const uint32_t buffer = memory::NewBlob(uint32_t(size), "TodaysEventImageFile");
    calls.Call(kXMemSet, frame, pair.lrs[2], buffer, 0, size);
    // A host object (E5 step 3) in place of the VirtualAlloc(16) at
    // pair.lrs[3]; the list and the callback's context hold its handle.
    const Image image = Image::New(base);
    const uint32_t record = image.address;
    image.set_index(index);
    image.set_loaded(0);
    image.set_texture(0);
    image.set_xpr(0);
    const List list = pair.b ? ListB(base, data) : ListA(base, data);
    const uint32_t count = list.count();
    list.set_count(count + 1);
    schema::host::StoreU32(list.records() + (count << 2), record);
    const uint64_t args[8] = {asset, buffer, kImageLoaded, record, 0, 0, 0, 0};
    r3 = calls.CallArgs(kArchiveLoad, frame, pair.lrs[4], args);
  }
  return r3;
}

uint64_t LoadImages(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C008 TodaysEvent::LoadImages");
  const uint32_t frame = Frame(base, sp, 112);
  uint32_t data = Data(base).address;
  for (uint32_t image = 0; image < kImages; image++) {
    if (BitSet(base, Data_(base, data), int32_t(image))) {
      LoadImagePair(base, data, image, frame, calls);
      data = Data(base).address;
    }
  }
  return data;
}

uint64_t EventOpenMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C130 TodaysEvent::OpenMovie");
  const uint32_t frame = Frame(base, sp, 96);
  calls.Call(kMoviesOpen, frame, 0x8225C148, kEventMovie, kEventAsset);
  return LoadImages(base, frame, calls);
}

uint64_t EventWaitForMovie(uint8_t* base, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225C160 TodaysEvent::WaitForMovie");
  const uint32_t machine = uint32_t(r3);
  Frame(base, sp, 96);
  const Machine m(base, machine);
  if (m.loaded() == 0) {
    r3 = ui_movies::IsLoaded(base, kEventMovie);
    if (uint32_t(r3) != 0) {
      const uint32_t data = Data(base).address;
      r3 = ImagesLoadedA(base, data);
      if (uint32_t(r3) != 0) {
        r3 = ImagesLoadedB(base, data);
        if (uint32_t(r3) != 0) {
          m.set_loaded(1);
        }
      }
    }
  }
  if (m.loaded() != 0) {
    m.set_state(m.reenter() != 0 ? 9 : 4);
  }
  return r3;
}

uint32_t NextUnlockEntry(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225BB70 TodaysEvent::NextUnlockEntry");
  Frame(base, sp, 96);
  const Data_ data = Data(base);
  const uint32_t entry = data.unlock_entry();
  if (entry != kNone) {
    const uint32_t record = data_tables::Entry56(base, data.unlock_table(), entry);
    const uint32_t next = Load(base, record + 8);
    if (next != 0) {
      const Data_ d = Data(base);
      d.set_unlock_entry(next + d.unlock_entry());
      return 0;
    }
  }
  const Save save(base, vehicle_accessors::SaveDataOf(base, 0));
  const uint32_t unlocks = save.unlocks_shown_address();
  const uint32_t count = save.unlocks_shown_count();
  for (uint32_t k = 0; k < count; k++) {
    const uint32_t unlock = unlocks + 12 * k;
    if (Load(base, unlock + 0) == 0) {
      Store(base, unlock + 0, 1);
      const Data_ shown = Data(base);
      shown.set_unlock_word(Load(base, unlock + 0));
      shown.set_unlock_table(Load(base, unlock + 4));
      shown.set_unlock_entry(Load(base, unlock + 8));
      return 1;
    }
  }
  Save(base, vehicle_accessors::SaveDataOf(base, 0)).set_unlocks_shown_count(0);
  return 2;
}

uint64_t SetUpTexts(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C648 TodaysEvent::SetUpTexts");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  const Save save(base, vehicle_accessors::SaveDataOf(base, 0));
  const int32_t day = int32_t(save.day());
  calls.Call(kSetChildNameText, frame, 0x8225C698, movie, 8, 6,
             schema::CareerSave(base, save.address).pick_address());
  calls.Call(kSetNumberText, frame, 0x8225C6AC, movie, 8, 9, save.money());
  // SetText's varargs past the day 0 (the user's call of 2026-10-30).
  const uint64_t args[8] = {movie, 8, 13, 8, uint64_t(uint32_t(day > 999 ? 999 : day)), 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8225C6D0, args);
  Node(base, ui_movies::FindNode(base, movie, 8, 11))
      .set_texture_entry(uint32_t(day - (day / 7) * 7));
  Node(base, ui_movies::FindNode(base, movie, 10, 6)).set_visible(0);
  Node(base, ui_movies::FindNode(base, movie, 10, 8)).set_visible(0);
  Node(base, ui_movies::FindNode(base, movie, 10, 7)).set_visible(0);
  const uint32_t last = ui_movies::FindNode(base, movie, 10, 9);
  Node(base, last).set_visible(0);
  Data(base).set_ready(1);
  return last;
}

uint64_t EventStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C990 TodaysEvent::Start");
  const uint32_t frame = Frame(base, sp, 112);
  // Sound::Play(&bank 1, &cue 2).
  Store(base, frame + 80, 2);
  Store(base, frame + 84, 1);
  calls.Call(kSoundPlay, frame, 0x8225C9BC, frame + 84, frame + 80);
  const uint32_t movie = FindMovie(base, frame + 84, kEventMovie);
  Data(base).set_movie_object_36(Load(base, Texture9(base, movie)));
  Data(base).set_movie_object_32(Load(base, Texture8(base, movie)));
  SetUpTexts(base, frame, calls);
  const uint32_t answer = NextUnlockEntry(base, frame);
  if (answer == 2) {
    return answer;
  }
  const Pages pages = EventPages(base);
  const uint32_t page = pages.page();
  if (page != 0) {
    pages.set_previous_page(page);
  }
  pages.set_step(0);
  pages.set_next_page(0);
  pages.set_queued_step(kNone);
  pages.set_page(kNone);
  return answer;
}

uint64_t EventUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C1F8 TodaysEvent::Unload");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, frame + 80, kEventMovie);
  uint64_t r3 = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = Load(base, uint32_t(r3));
  if (movie != 0) {
    r3 = calls.Call(kFinishAll, frame, 0x8225C234, movie);
    PutBackImages(base, movie);
  }
  const Machine m(base, machine);
  if (m.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8225C26C, kEventMovie);
    m.set_loaded(0);
  }
  return r3;
}

uint64_t EventUnloaded(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C278 TodaysEvent::Unloaded");
  const uint32_t frame = Frame(base, sp, 144);
  // The sound manager's IsCuePlaying (slot 14) for &bank 1, &cue 2, and when
  // it answers a nonzero byte its StopCue (slot 13), as the original calls
  // them.
  uint32_t manager = G::sound_manager(base);
  Store(base, frame + 80, 2);
  Store(base, frame + 84, 1);
  uint64_t r3 = calls.Call(schema::AudioManager(base, manager).vtable().is_cue_playing(), frame,
                           0x8225C2BC, manager, frame + 84, frame + 80);
  if ((r3 & 0xFF) != 0) {
    manager = G::sound_manager(base);
    Store(base, frame + 88, 2);
    Store(base, frame + 92, 1);
    r3 = calls.Call(schema::AudioManager(base, manager).vtable().stop_cue(), frame, 0x8225C2E8,
                    manager, frame + 92, frame + 88);
  }
  Post(base, dispatcher, 6);
  Machine(base, machine).set_state(9);
  return r3;
}

uint64_t EventLeave(uint8_t* base, uint32_t machine, uint32_t dispatcher, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C348 TodaysEvent::Leave");
  const uint32_t frame = Frame(base, sp, 112);
  Machine(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x8225C398, movie);
    PutBackImages(base, movie);
  }
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8225C3C4, kEventMovie);
  Post(base, dispatcher, 4);
  return r3;
}

uint64_t FreeImages(uint8_t* base, uint32_t data, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225BC60 TodaysEvent::FreeImages");
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t r3 = data;
  struct Lrs {
    bool b;
    uint32_t free_image, free;
  };
  static constexpr Lrs kLists[2] = {{false, 0x8225BC9C, 0x8225BCB0},
                                    {true, 0x8225BCE8, 0x8225BCFC}};
  for (const Lrs& l : kLists) {
    const List list = l.b ? ListB(base, data) : ListA(base, data);
    while (list.count() != 0) {
      const uint32_t count = list.count() - 1;
      list.set_count(count);
      const uint32_t record = Record(list, count);
      calls.Call(kFreeImage, frame, l.free_image, record);
      // Deleted in place of the VirtualFree at l.free (r3 = 1, as it
      // returned).
      Image(base, record).Delete();
      r3 = 1;
    }
  }
  return r3;
}

uint64_t EventDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225BF78 TodaysEvent::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  if (Data(base).movies_queued() != 0) {
    Machine(base, machine).set_state(20);
  }
  FreeImages(base, Data(base).address, frame, calls);
  calls.Call(kVirtualFree, frame, 0x8225BFC8, G::event_pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8225BFDC, G::event_menu(base).address, 0, kMemRelease);
  // Deleted in place of the VirtualFree at 0x8225BFEC (r3 = 1, as it
  // returned).
  G::event_data(base).Delete();
  const uint64_t r3 = 1;
  G::set_event_pages(base, 0u);
  G::set_event_menu(base, 0u);
  G::set_event_data(base, 0u);
  return r3;
}

}  // namespace todays_result

namespace todays_result {

namespace {

// The pages' table in Draw's frame: EventPage, EndPage.
constexpr uint32_t kEventPageTable[2] = {0x8225C428, 0x8225C590};
// The transitions' table in Update's frame (+96 on): Intro, FadeOut,
// NextUnlock, Type2Hub::Transition9, NextPage, Type2Hub::Transition11.
constexpr uint32_t kEventTransitionTable[6] = {0x8225CB18, 0x8225C770, 0x8225CC80,
                                               0x8218F150, 0x8225CD50, 0x8218F220};
constexpr uint64_t kAll = ~uint64_t(0);

schema::TodaysEventData EventData(uint8_t* base) { return G::event_data(base); }

// UiMovie::Play(movie, a, b) with r6 to r9 0, as the originals pass them.
void Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t a,
          uint64_t b) {
  const uint64_t args[8] = {movie, a, b, 0, 0, 0, 0, 0};
  calls.CallArgs(kPlay, frame, lr, args);
}
uint64_t RunMovie(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}
uint64_t FadesDone(uint8_t* base) {
  return (G::fade_count_0(base) | G::fade_count_1(base)) == 0 ? 1 : 0;
}

}  // namespace

uint64_t EventDraw(uint8_t* base, uint64_t machine, uint64_t dispatcher, uint32_t sp,
                   const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C928 TodaysEvent::Draw");
  const uint32_t frame = Frame(base, sp, 96);
  Store(base, frame + 80, kEventPageTable[0]);
  Store(base, frame + 84, kEventPageTable[1]);
  const uint32_t page = EventPages(base).page();
  if (page == kNone) {
    return dispatcher;
  }
  // No bounds check, as the original: the word at the table's index.
  return calls.Call(Load(base, frame + 80 + (page << 2)), frame, 0x8225C980, dispatcher,
                    machine);
}

uint64_t EventUpdate(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CDE8 TodaysEvent::Update");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  const Pages pages = EventPages(base);
  Store(base, frame + 116, kEventTransitionTable[5]);
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, frame + 96 + 4 * k, kEventTransitionTable[k]);
  }
  const uint32_t running = pages.step();
  if (running == kNone) {
    return RunMovie(calls, frame, 0x8225CF14, movie);
  }
  uint64_t r3 = movie;
  Pages p = pages;
  if (int32_t(running) >= 0) {
    // No bounds check: the word at the table's index.
    r3 = calls.Call(Load(base, frame + 96 + (running << 2)), frame, 0x8225CEAC,
                    pages.step_started_address());
    p = EventPages(base);
    if (uint32_t(r3) == 0) {
      return r3;
    }
  }
  const uint32_t queued = p.queued_step();
  if (queued == kNone) {
    if (int32_t(running) >= 0) {
      p.set_page(p.next_page());
      p.set_step(queued);
    } else {
      const uint32_t next = p.next_page();
      p.set_step(kNone);
      p.set_page(next);
    }
  } else {
    p.set_queued_step(kNone);
    p.set_unknown_20(0);
    p.set_step(queued);
  }
  p.set_step_started(0);
  return r3;
}

uint64_t EventPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C428 TodaysEvent::EventPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t pressed =
      calls.Call(kMenuPressed, frame, 0x8225C44C, G::event_menu(base).address, kAccept);
  if (uint32_t(pressed) == 0) {
    return pressed;
  }
  calls.Call(kMenuSoundPlay, frame, 0x8225C45C, 0);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  const uint64_t revealed = calls.Call(kRevealText, frame, 0x8225C488, movie, 0, 2);
  if (uint32_t(revealed) != 0) {
    const uint64_t more = calls.Call(kHasMoreText, frame, 0x8225C548, movie, 0, 2);
    const uint32_t node = ui_movies::FindNode(base, movie, 0, 0);
    Node(base, node).set_visible(uint32_t(more) != 0 ? 1 : 0);
    return node;
  }
  const uint32_t answer = NextUnlockEntry(base, frame);
  if (answer >= 3) {
    return answer;
  }
  const Pages pages = EventPages(base);
  const uint32_t page = pages.page();
  if (answer == 2) {
    if (page != 1) {
      pages.set_previous_page(page);
    }
    pages.set_step(1);
    pages.set_queued_step(kNone);
    pages.set_next_page(1);
    pages.set_page(kNone);
    return answer;
  }
  if (page != 0) {
    pages.set_previous_page(page);
  }
  // 0: the entry's next page through Transition11, then NextPage; 1: the
  // next unlock through Transition9, then NextUnlock.
  pages.set_step(answer == 0 ? 5 : 3);
  pages.set_queued_step(answer == 0 ? 4 : 2);
  pages.set_next_page(0);
  pages.set_page(kNone);
  return answer;
}

uint64_t QueueEventMovies(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225B9B8 TodaysEvent::QueueEventMovies");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t queued = Save(base, save).movie_after_results_address();
  const uint32_t ids = G::boot_movie_ids_address();
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, ids + 4 * k, 11);
  }
  G::set_boot_next_type(base, 13);
  G::set_boot_movie_index(base, 0);
  G::set_boot_skippable(base, 1);
  const uint32_t movie = Load(base, queued);
  Store(base, ids + 0, movie);
  if (int32_t(movie) == 6) {
    Store(base, ids + 4, 7);
  }
  Store(base, queued, kNone);
  G::set_boot_next_type(base, 9);
  // Progress::CheckUnlocks: its plain C++ where Condition::Test's copies fit,
  // else the original through the guest.
  if (career::CheckUnlocksNative(base, frame)) {
    return career::ProgressCheckUnlocks(base, save, frame);
  }
  return calls.Call(kCheckUnlocks, frame, 0x8225BA38, save);
}

uint64_t EndPage(uint8_t* base, uint32_t dispatcher, uint32_t machine, uint32_t sp,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C590 TodaysEvent::EndPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t kind = Kind(base, frame);
  if (kind == 1) {
    Post(base, dispatcher, 7);
    EventData(base).set_movies_queued(0);
    return kind;
  }
  Machine(base, machine).set_state(10);
  EventData(base).set_movies_queued(1);
  return QueueEventMovies(base, frame, calls);
}

uint64_t ShowImages(uint8_t* base, uint32_t index, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8225C848 TodaysEvent::ShowImages");
  const uint32_t frame = Frame(base, sp, 112);
  Store(base, frame + 80, kEventMovie);
  const uint32_t pair = ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80);
  const uint32_t movie = Load(base, pair);
  // The texture record of the list's first record for the image, 0 none.
  auto texture = [&](const List& list) {
    const uint32_t count = list.count();
    for (uint32_t k = 0; k < count; k++) {
      const Image record(base, Record(list, k));
      if (record.index() == index) {
        return record.texture().address;
      }
    }
    return 0u;
  };
  Store(base, Texture9(base, movie), texture(EventData(base).images_a()));
  Store(base, Texture8(base, movie), texture(EventData(base).images_b()));
  return pair;
}

uint64_t ShowUnlock(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CA50 TodaysEvent::ShowUnlock");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  const schema::TodaysEventData data = EventData(base);
  const uint32_t record = data_tables::Entry56(base, data.unlock_table(), data.unlock_entry());
  // SetText's varargs 0 (the user's call of 2026-10-30).
  const uint64_t title[8] = {movie, 0, 1, uint64_t(Load(base, record + 12)) + 4650, 0, 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8225CAAC, title);
  const uint64_t body[8] = {
      movie, 0, 2, uint64_t(EventData(base).unlock_entry()) + 6683, 0, 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8225CAC8, body);
  ShowImages(base, Load(base, record + 16), frame);
  const uint64_t more = calls.Call(kHasMoreText, frame, 0x8225CAE0, movie, 0, 2);
  const uint32_t node = ui_movies::FindNode(base, movie, 0, 0);
  Node(base, node).set_visible(uint32_t(more) != 0 ? 1 : 0);
  return node;
}

uint64_t EventIntro(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CB18 TodaysEvent::Intro");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x8225CB78, movie, kAll, 2);
    Play(calls, frame, 0x8225CB98, movie, 8, 0);
    calls.Call(kLoopLast, frame, 0x8225CBA0, movie);
    Play(calls, frame, 0x8225CBC0, movie, 15, 0);
    calls.Call(kLoopLast, frame, 0x8225CBC8, movie);
    calls.Call(kPlayNamed, frame, 0x8225CBE4, movie, 2);
    calls.Call(kLoopNamed, frame, 0x8225CBF0, movie, 2);
    calls.Call(kFadeIn, frame, 0x8225CC0C, G::fade_in_passes(base), 0, 0, 0);
    ShowUnlock(base, frame, calls);
    Store(base, started, 1);
  }
  const uint64_t done = uint32_t(RunMovie(calls, frame, 0x8225CC24, movie)) == 0 ? 1 : 0;
  if (done != 0) {
    Play(calls, frame, 0x8225CC50, movie, 12, 1);
    calls.Call(kLoop, frame, 0x8225CC60, movie, 12, 1);
  }
  return done;
}

uint64_t EventFadeOut(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225C770 TodaysEvent::FadeOut");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (Load(base, started) == 0) {
    calls.Call(kFinish, frame, 0x8225C7C0, movie, 8, 0);
    calls.Call(kFinish, frame, 0x8225C7D0, movie, 15, 0);
    calls.Call(kFinishNamed, frame, 0x8225C7DC, movie, 2);
    calls.Call(kFadeOut, frame, 0x8225C7F4, 60, 0, 0, 0);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8225C808, movie);
  return FadesDone(base);
}

uint64_t NextUnlock(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CC80 TodaysEvent::NextUnlock");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (Load(base, started) == 0) {
    calls.Call(kFadeIn, frame, 0x8225CCD8, 60, 0, 0, 0);
    Play(calls, frame, 0x8225CCF8, movie, kAll, 4);
    ShowUnlock(base, frame, calls);
    Store(base, started, 1);
  }
  RunMovie(calls, frame, 0x8225CD10, movie);
  return FadesDone(base);
}

uint64_t NextPage(uint8_t* base, uint32_t started, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8225CD50 TodaysEvent::NextPage");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, frame + 80, kEventMovie);
  if (Load(base, started) == 0) {
    Play(calls, frame, 0x8225CDB0, movie, kAll, 4);
    ShowUnlock(base, frame, calls);
    Store(base, started, 1);
  }
  return uint32_t(RunMovie(calls, frame, 0x8225CDC8, movie)) == 0 ? 1 : 0;
}

}  // namespace todays_result
