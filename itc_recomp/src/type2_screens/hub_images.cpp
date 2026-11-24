// The hub's set-up, teardown and notice images
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "archive/archive.h"
#include "base/log.h"
#include "build_screen/build_screen.h"
#include "car_makers/car_makers.h"
#include "data_tables/data_tables.h"
#include "memory/handles.h"
#include "stand_models/stand_models.h"
#include "type2_screens/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kNone = 0xFFFFFFFFu;

uint64_t Alloc(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t bytes) {
  return calls.Call(kVirtualAlloc, frame, lr, 0, bytes, 4096, 4);
}

}  // namespace

uint32_t HubMarkNoticeImages(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218DCC0 Type2Hub::MarkNoticeImages");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  uint32_t r3 = save;
  const uint32_t list = SaveData(base, save).unlocks_shown_address();
  for (uint32_t i = 0; i < SaveData(base, save).unlocks_shown_count(); i++) {
    const uint32_t notice = list + 12 * i;
    const uint32_t table = Load(base, notice + 4);
    uint32_t entry = Load(base, notice + 8);
    Store(base, frame + 80, Load(base, notice + 0));
    for (;;) {
      r3 = data_tables::Entry56(base, table, entry);
      const schema::Type2HubData hub = Globals::hub_data(base);
      const int32_t image = int32_t(schema::CareerEntry56(base, r3).image());
      if (image >= 0) {
        // The bit set's word (image_bits, image_bits2) through the host view.
        const uint32_t word = hub.image_bits_address() - hub.address + 4 * uint32_t(image >> 5);
        hub.store_at(word, 4, (1u << (image & 31)) | uint32_t(hub.load_at(word, 4)));
      }
      const uint32_t next = schema::CareerEntry56(base, r3).next_page();
      if (next == 0) {
        break;
      }
      entry += next;
    }
  }
  return r3;
}

uint32_t HubCreate(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E078 Type2Hub::Create");
  const uint32_t frame = Frame(base, sp, 128);
  schema::Type2Hub(base, machine).set_loaded(0);
  // Host objects (E5 step 5) in place of the VirtualAlloc(28) at 0x8218E0A4
  // and the VirtualAlloc(68) at 0x8218E0E0; the globals hold their handles.
  const schema::PageObject pages = schema::PageObject::New(base);
  Globals::set_hub_pages(base, pages.address);
  pages.set_next_page(0);
  pages.set_page(0);
  pages.set_previous_page(0);
  pages.set_unknown_20(kNone);
  pages.set_step(kNone);
  pages.set_step_started(0);
  const schema::FrontEndMenu menu = schema::FrontEndMenu::New(base);
  Globals::set_hub_menu(base, menu.address);
  menu.set_first_item(0);
  menu.set_wraps(1);
  menu.set_cursor(0);
  menu.set_previous(0);
  menu.set_last_item(7);
  menu.set_moved(0);
  menu.set_wrapped(0);
  menu.set_visible_row(0);
  menu.set_previous_row(0);
  menu.set_side(0);
  menu.set_player(0);
  menu.set_up_down_locked(0);
  menu.set_left_right_steps(0);
  menu.set_shoulders_step(0);
  // The data: the original asks VirtualAlloc for 68 bytes (it gives a page)
  // and uses its words up to +447; a host object (E5 step 4) of 448 bytes
  // in place of the VirtualAlloc at 0x8218E140, [0x826D8418] its handle.
  const schema::Type2HubData data = schema::Type2HubData::New(base);
  Globals::set_hub_data(base, data.address);
  data.set_set_up(0);
  data.set_quitting(0);
  data.set_stand_model(0);
  data.set_busy_shown(0);
  data.set_unknown_20(0);
  data.set_record_string(kNone);
  data.set_notice_entry(kNone);
  data.set_image_count(0);
  data.set_image2_count(0);
  data.set_image_bits(0);
  data.set_image_bits2(0);
  Globals::hub_data(base).set_unknown_20(0);
  const uint32_t r3 = HubMarkNoticeImages(base, frame);
  schema::Type2Hub(base, machine).set_state(schema::Type2Hub(base, machine).reenter() != 0 ? 1 : 9);
  return r3;
}

uint64_t HubFreeImages(uint8_t* base, uint32_t data, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218DE88 Type2Hub::FreeImages");
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t r3 = data;
  const schema::Type2HubData hub(base, data);
  static constexpr struct {
    uint32_t (schema::Type2HubData::*images)() const;
    uint32_t (schema::Type2HubData::*count)() const;
    void (schema::Type2HubData::*set_count)(uint32_t) const;
    uint32_t lr;
  } kLists[2] = {{&schema::Type2HubData::images_address, &schema::Type2HubData::image_count,
                  &schema::Type2HubData::set_image_count, 0x8218DED8},
                 {&schema::Type2HubData::images2_address, &schema::Type2HubData::image2_count,
                  &schema::Type2HubData::set_image2_count, 0x8218DF24}};
  for (const auto& list : kLists) {
    while ((hub.*list.count)() != 0) {
      const uint32_t count = (hub.*list.count)() - 1;
      (hub.*list.set_count)(count);
      const uint32_t record =
          uint32_t(hub.load_at((hub.*list.images)() - hub.address + 4 * count, 4));
      HubFreeImage(base, record, frame, calls);
      // Deleted in place of the VirtualFree at list.lr (r3 = 1, as it
      // returned).
      schema::Type2HubImage(base, record).Delete();
      r3 = 1;
    }
  }
  return r3;
}

uint64_t HubDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E1A8 Type2Hub::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  Globals::hub_data(base).set_unknown_20(0);
  if (Globals::hub_data(base).quitting() != 0) {
    schema::Type2Hub(base, machine).set_state(14);
  }
  HubFreeImages(base, Globals::hub_data(base).address, frame, calls);
  calls.Call(kVirtualFree, frame, 0x8218E204, Globals::hub_pages(base).address, 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8218E218, Globals::hub_menu(base).address, 0, kMemRelease);
  // Deleted in place of the VirtualFree at 0x8218E228 (r3 = 1, as it
  // returned; with no data, the VirtualFree of 0 as the original).
  const schema::Type2HubData data = Globals::hub_data(base);
  const uint64_t r3 =
      data.address != 0 ? 1 : calls.Call(kVirtualFree, frame, 0x8218E228, 0, 0, kMemRelease);
  data.Delete();
  Globals::set_hub_pages(base, 0);
  Globals::set_hub_menu(base, 0);
  Globals::set_hub_data(base, 0);
  return r3;
}

uint64_t HubLoadNoticeImage(uint8_t* base, uint32_t data, uint32_t index, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218DF38 Type2Hub::LoadNoticeImage");
  const uint32_t frame = Frame(base, sp, 144);
  const schema::Type2HubData hub(base, data);
  static constexpr struct {
    uint32_t asset;
    uint32_t (schema::Type2HubData::*images)() const;
    uint32_t (schema::Type2HubData::*count)() const;
    void (schema::Type2HubData::*set_count)(uint32_t) const;
    uint32_t alloc, memset, record, load;
  } kImages[2] = {{22696, &schema::Type2HubData::images_address, &schema::Type2HubData::image_count,
                   &schema::Type2HubData::set_image_count, 0x8218DF70, 0x8218DF80, 0x8218DF94,
                   0x8218DFE0},
                  {22741, &schema::Type2HubData::images2_address,
                   &schema::Type2HubData::image2_count, &schema::Type2HubData::set_image2_count,
                   0x8218E004, 0x8218E014, 0x8218E028, 0x8218E070}};
  uint64_t r3 = 0;
  for (const auto& image : kImages) {
    const uint32_t asset = image.asset + index;
    const uint32_t size = archive::AssetSize(base, asset);
    // A blob (E5 step 7: the file in PC memory, in the guest's byte order)
    // in place of the VirtualAlloc(size) at image.alloc: an XPR2 pack only
    // the game's code reads (ImageLoaded's XPR::Create copies its header and
    // data out, the data into physical memory, then VirtualFree frees it);
    // whole pages, as the original's, for the archive's sectors.
    const uint32_t buffer = memory::NewBlob(size, "Type2HubImageFile");
    calls.Call(kXMemSet, frame, image.memset, buffer, 0, size);
    // A host object (E5 step 3) in place of the VirtualAlloc(16) at
    // image.record; the list holds its handle.
    const schema::Type2HubImage record = schema::Type2HubImage::New(base);
    record.set_index(index);
    record.set_loaded(0);
    record.set_texture(0);
    record.set_xpr(0);
    const uint32_t count = (hub.*image.count)();
    (hub.*image.set_count)(count + 1);
    hub.store_at((hub.*image.images)() - hub.address + 4 * count, 4, record.address);
    const uint64_t args[8] = {asset, buffer, kHubImageLoaded, record.address, 0, 0, 0, 0};
    r3 = calls.CallArgs(kArchiveLoad, frame, image.load, args);
  }
  return r3;
}

uint32_t HubLoadNoticeImages(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E240 Type2Hub::LoadNoticeImages");
  const uint32_t frame = Frame(base, sp, 112);
  schema::Type2HubData data = Globals::hub_data(base);
  for (uint32_t i = 0; i < 46; i++) {
    const uint32_t word = data.image_bits_address() - data.address + 4 * (i >> 5);
    if ((uint32_t(data.load_at(word, 4)) & (1u << (i & 31))) != 0) {
      HubLoadNoticeImage(base, data.address, i, frame, calls);
      data.address = Globals::hub_data(base).address;
    }
  }
  return data.address;
}

uint32_t HubOpenMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E368 Type2Hub::OpenMovie");
  const uint32_t frame = Frame(base, sp, 2064);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  uint32_t block = 0;
  if (SaveData(base, save).garage_count() != 0) {
    calls.Call(kMemcpy, frame, 0x8218E3D4, frame + 80, SelectedGarageCar(base, save) + 212,
               1968);
    block = car_makers::BlockOf(schema::CarMakersCar(base, frame + 80).car_id());
  }
  // GarageStage::MakeModel by its plain C++ (its adapter went, B7 loop,
  // 2026-11-13), ScenePools::SetUpB under it through `calls`.
  const stand_models::GuestCall call = [&](uint32_t function,
                                           std::initializer_list<uint64_t> args, uint32_t r1,
                                           uint32_t lr) {
    uint64_t regs[4] = {};
    size_t n = 0;
    for (const uint64_t v : args) {
      regs[n++] = v;
    }
    return calls.Call(function, r1, lr, regs[0], regs[1], regs[2], regs[3]);
  };
  stand_models::MakeModel(base, call, schema::StandHolder(base, Load(base, build_screen::kStandHolder)).stage_block_0(), block, 1,
                          frame);
  calls.Call(kMoviesOpen, frame, 0x8218E408, kHubMovie, 22545);
  return HubLoadNoticeImages(base, frame, calls);
}

uint64_t HubImageLoaded(uint8_t* base, uint32_t buffer, uint32_t record, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218D8E8 Type2Hub::ImageLoaded");
  const uint32_t frame = Frame(base, sp, 144);
  const schema::Type2HubImage image(base, record);
  const uint32_t xpr = uint32_t(Alloc(calls, frame, 0x8218D918, 24));
  image.set_xpr(xpr);
  calls.Call(kXprCreate, frame, 0x8218D924, xpr, buffer);
  // A host object (E5 step 4) in place of the VirtualAlloc(12) at
  // 0x8218D938; the image record holds its handle.
  const schema::Type2HubTexture texture = schema::Type2HubTexture::New(base);
  image.set_texture(texture.address);
  const uint64_t level = calls.Call(kXprGetTexture, frame, 0x8218D94C, image.xpr(), 0);
  image.texture().set_texture(uint32_t(level));
  calls.Call(kGetLevelDesc, frame, 0x8218D968, image.texture().texture(), 0, frame + 80);
  image.texture().set_width(Load(base, frame + 104));
  image.texture().set_height(Load(base, frame + 108));
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x8218D994, buffer, 0, kMemRelease);
  image.set_loaded(1);
  return r3;
}

}  // namespace type2_screens
