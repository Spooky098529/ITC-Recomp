// The hub's handlers and helpers (docs/functions/type2-screens.md): see
// type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "build_screen/build_screen.h"
#include "data_tables/data_tables.h"
#include "type2_screens/internal.h"
#include "type67_modes/type67_modes.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kNone = 0xFFFFFFFFu;
// The debug print's format WaitForLoad hands the empty sub_82106540.
constexpr uint32_t kWaitFormat = 0x8200A1F8;

schema::Type2HubData Data(uint8_t* base) { return Globals::hub_data(base); }

// Movie 0's object's two image words put back from the data's +440, +444.
void PutBackImages(uint8_t* base, uint32_t movie) {
  Store(base, MovieTextureSlot(base, movie, 9), Data(base).movie_image());
  Store(base, MovieTextureSlot(base, movie, 8), Data(base).movie_image2());
}

}  // namespace

void HubOnYes(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8218D6A8 Type2Hub::OnYes");
  Data(base).set_dialog_answer(0);
}

void HubOnNo(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_8218D6C0 Type2Hub::OnNo");
  Data(base).set_dialog_answer(1);
}

uint32_t HubImagesLoadedA(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_8218E2D8 Type2Hub::ImagesLoadedA");
  const schema::Type2HubData hub(base, data);
  const uint32_t count = hub.image_count();
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t record = uint32_t(hub.load_at(hub.images_address() - hub.address + 4 * i, 4));
    if (schema::Type2HubImage(base, record).loaded() == 0) {
      return 0;
    }
  }
  return 1;
}

uint32_t HubImagesLoadedB(uint8_t* base, uint32_t data) {
  ITC_NATIVE_RUNNING("sub_8218E320 Type2Hub::ImagesLoadedB");
  const schema::Type2HubData hub(base, data);
  const uint32_t count = hub.image2_count();
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t record = uint32_t(hub.load_at(hub.images2_address() - hub.address + 4 * i, 4));
    if (schema::Type2HubImage(base, record).loaded() == 0) {
      return 0;
    }
  }
  return 1;
}

uint64_t HubWaitForLoad(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E420 Type2Hub::WaitForLoad");
  const schema::Type2Hub machine(base, uint32_t(r3));
  const uint32_t frame = Frame(base, sp, 112);
  if (machine.loaded() == 0) {
    bool loaded = false;
    for (const uint32_t id : {0u, 9u, 10u}) {
      r3 = ui_movies::IsLoaded(base, id);
      if (uint32_t(r3) == 0) {
        break;
      }
      if (id == 10) {
        const schema::Type2HubData data = Data(base);
        r3 = HubImagesLoadedA(base, data.address);
        if (r3 != 0) {
          r3 = HubImagesLoadedB(base, data.address);
          loaded = r3 != 0;
        }
      }
    }
    if (loaded) {
      machine.set_loaded(1);
    }
  }
  uint32_t count = Globals::hub_counter(base);
  if (((count / 15) & 1) == 0) {
    r3 = calls.Call(kEmpty, frame, 0x8218E4D4, 100, 30, 0, kWaitFormat);
    count = Globals::hub_counter(base);
  }
  count += 1;
  const uint32_t loaded = machine.loaded();
  Globals::set_hub_counter(base, count);
  // The patched comparison: 1, not the original's 60 (the PATCH: in
  // generated/, Gliniak's Xenia patch for the black screen after quest
  // races).
  if (loaded != 0 && count >= 1) {
    const uint32_t reenter = machine.reenter();
    Globals::set_hub_counter(base, 0);
    machine.set_state(reenter != 0 ? 9 : 4);
  }
  return r3;
}

uint64_t HubFreeImage(uint8_t* base, uint32_t record, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218D650 Type2Hub::FreeImage");
  const uint32_t frame = Frame(base, sp, 96);
  const schema::Type2HubImage image(base, record);
  const schema::Type2HubTexture texture = image.texture();
  image.set_loaded(0);
  // Deleted in place of the VirtualFree at 0x8218D680 (its answer unused;
  // 0, a record never loaded, does nothing).
  texture.Delete();
  return calls.Call(kVirtualFree, frame, 0x8218D694, image.xpr(), 0, kMemRelease);
}

void HubOpenSelected(uint8_t* base, uint32_t machine, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8218E5B0 Type2Hub::OpenSelected");
  static constexpr uint32_t kMessages[7] = {8, 12, 18, 10, 23, 21, 14};
  const uint32_t item = Globals::hub_menu(base).cursor();
  Post(base, mode, item < 7 ? kMessages[item] : 51);
  schema::Type2Hub(base, machine).set_state(9);
}

void HubReenter(uint8_t* base, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8218D9B8 Type2Hub::Reenter");
  const schema::Type2Hub hub(base, machine);
  if (hub.loaded() == 0) {
    hub.set_state(1);
    return;
  }
  const uint32_t transition = Data(base).set_up() != 0 ? 4 : 0;
  PagesGo(base, Globals::hub_pages(base).address, 0, transition, 1);
  hub.set_state(4);
}

void HubPostMessage5(uint8_t* base, uint32_t mode) {
  ITC_NATIVE_RUNNING("sub_8218E7A4 Type2Hub::PostMessage5");
  Post(base, mode, 5);
}

void HubChoose(uint8_t* base, uint32_t mode, uint32_t machine) {
  ITC_NATIVE_RUNNING("sub_8218E768 Type2Hub::Choose");
  const uint32_t item = Globals::hub_menu(base).cursor();
  if (item > 7) {
    return;
  }
  if (item < 7) {
    HubPostMessage5(base, mode);
    return;
  }
  schema::Type2Hub(base, machine).set_state(10);
  Data(base).set_quitting(1);
}

uint64_t HubUnload(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E530 Type2Hub::Unload");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, frame + 80, kHubMovie);
  Data(base).set_unknown_20(0);
  const uint32_t movie = Load(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  uint64_t r3 = calls.Call(kFinishAll, frame, 0x8218E570, movie);
  PutBackImages(base, movie);
  const schema::Type2Hub hub(base, machine);
  if (hub.delete_movie() != 0) {
    r3 = calls.Call(kMoviesDelete, frame, 0x8218E5A4, kHubMovie);
    hub.set_loaded(0);
  }
  return r3;
}

uint64_t HubClose(uint8_t* base, uint32_t machine, uint32_t mode, uint32_t sp,
                  const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E688 Type2Hub::Close");
  const uint32_t frame = Frame(base, sp, 112);
  schema::Type2Hub(base, machine).set_state(7);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  if (movie != 0) {
    calls.Call(kFinishAll, frame, 0x8218E6D8, movie);
  }
  PutBackImages(base, movie);
  const uint64_t r3 = calls.Call(kMoviesDelete, frame, 0x8218E704, kHubMovie);
  Post(base, mode, 4);
  return r3;
}

uint32_t HubSetPrompts(uint8_t* base, const uint32_t args[6], uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218F530 Type2Hub::SetPrompts");
  const uint32_t frame = Frame(base, sp, 192);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  // The pairs' first values at +88 (r3, r5, r7), their second at +104 (r4,
  // r6, r8), stored in the original's order.
  Store(base, frame + 92, args[2]);
  Store(base, frame + 104, args[1]);
  Store(base, frame + 108, args[3]);
  Store(base, frame + 88, args[0]);
  Store(base, frame + 96, args[4]);
  Store(base, frame + 112, args[5]);
  uint32_t r3 = 0;
  for (uint32_t i = 0; i < 3; i++) {
    const uint32_t prompt = ui_movies::FindNode(base, movie, 10, i);
    const uint32_t first = ui_movies::FindNode(base, movie, 10, FirstChild(base, prompt));
    r3 = ui_movies::FindNode(base, movie, 10, FirstChild(base, prompt) + 1);
    const uint32_t a = Load(base, frame + 88 + 4 * i);
    if (a != kNone) {
      const uint32_t b = Load(base, frame + 104 + 4 * i);
      if (b != kNone) {
        Node(base, prompt).set_visible(1);
        Node(base, first).set_texture_entry(a);
        Node(base, r3).set_texture_entry(b);
        continue;
      }
    }
    Node(base, prompt).set_visible(0);
  }
  return r3;
}

uint32_t HubBack(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218F698 Type2Hub::Back");
  const schema::PageObject pages = Globals::hub_pages(base);
  const uint32_t before = pages.page();
  if (before != 0) {
    pages.set_previous_page(before);
  }
  pages.set_step(kNone);
  pages.set_page(0);
  pages.set_queued_step(kNone);
  static constexpr uint32_t kPrompts[6] = {0, 0, 6, 2, 7, 3};
  return HubSetPrompts(base, kPrompts, sp);
}

uint64_t HubSetUpStatus(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E810 Type2Hub::SetUpStatus");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  const SaveData save(base, vehicle_accessors::SaveDataOf(base, 0));
  const int32_t day = int32_t(save.day());
  calls.Call(kSetChildNameText, frame, 0x8218E860, movie, 8, 6, save.pick_head_address());
  calls.Call(kSetNumberText, frame, 0x8218E874, movie, 8, 9, save.money());
  const uint64_t args[8] = {movie, 8, 13, 8, uint32_t(day > 999 ? 999 : day), 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8218E898, args);
  const uint32_t node = ui_movies::FindNode(base, movie, 8, 11);
  Node(base, node).set_texture_entry(uint32_t(day % 7));
  const uint64_t r3 = type67_modes::GarageStageSetPlates(
      base, Load(base, Load(base, build_screen::kStandHolder)), 1);
  Data(base).set_set_up(1);
  Data(base).set_quitting(0);
  Data(base).set_stand_model(0);
  return r3;
}

uint32_t HubNextNotice(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218DBD0 Type2Hub::NextNotice");
  Frame(base, sp, 96);
  const schema::Type2HubData data = Data(base);
  const uint32_t entry = data.notice_entry();
  if (entry != kNone) {
    const uint32_t next =
        schema::CareerEntry56(base, data_tables::Entry56(base, data.notice_table(), entry)).next_page();
    if (next != 0) {
      const schema::Type2HubData now = Data(base);
      now.set_notice_entry(next + now.notice_entry());
      return 0;
    }
  }
  const SaveData save(base, vehicle_accessors::SaveDataOf(base, 0));
  const uint32_t list = save.unlocks_shown_address();
  const uint32_t count = save.unlocks_shown_count();
  for (uint32_t i = 0; i < count; i++) {
    const uint32_t at = list + 12 * i;
    if (Load(base, at + 0) == 0) {
      Store(base, at + 0, 1);
      const schema::Type2HubData to = Data(base);
      to.set_notice_read(Load(base, at + 0));
      to.set_notice_table(Load(base, at + 4));
      to.set_notice_entry(Load(base, at + 8));
      return 1;
    }
  }
  SaveData(base, vehicle_accessors::SaveDataOf(base, 0)).set_unlocks_shown_count(0);
  return 2;
}

uint32_t HubNextNotice2(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218DD78 Type2Hub::NextNotice2");
  Frame(base, sp, 96);
  const SaveData save(base, vehicle_accessors::SaveDataOf(base, 0));
  const uint32_t list = save.rewards_shown_address();
  for (uint32_t i = 0; i < save.rewards_shown_count(); i++) {
    const uint32_t at = list + 12 * i;
    if (Load(base, at + 0) == 0) {
      Store(base, at + 0, 1);
      const schema::Type2HubData to = Data(base);
      to.set_notice2_read(Load(base, at + 0));
      to.set_notice2_table(Load(base, at + 4));
      to.set_record_string(Load(base, at + 8));
      return 1;
    }
  }
  save.set_rewards_shown_count(0);
  return 0;
}

uint32_t HubStart(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218F6E0 Type2Hub::Start");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kHubMovie, frame + 80);
  Data(base).set_movie_image(Load(base, MovieTextureSlot(base, movie, 9)));
  Data(base).set_movie_image2(Load(base, MovieTextureSlot(base, movie, 8)));
  HubSetUpStatus(base, frame, calls);
  uint32_t r3 = HubNextNotice(base, frame);
  if (r3 != 2) {
    PagesGo(base, Globals::hub_pages(base).address, 4, 5, 1);
  } else {
    r3 = HubNextNotice2(base, frame);
    PagesGo(base, Globals::hub_pages(base).address, r3 != 0 ? 5 : 0, 0, 1);
  }
  Data(base).set_unknown_20(0);
  return r3;
}

}  // namespace type2_screens
