// The hub's set-up, teardown and notice images
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "archive/archive.h"
#include "base/log.h"
#include "build_screen/build_screen.h"
#include "car_makers/car_makers.h"
#include "data_tables/data_tables.h"
#include "stand_models/stand_models.h"
#include "type2_screens/internal.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kMemRelease = 0x8000;
constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kNotices = 0x1C778;  // player 0's first notice list

uint64_t Alloc(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t bytes) {
  return calls.Call(kVirtualAlloc, frame, lr, 0, bytes, 4096, 4);
}

}  // namespace

uint32_t HubMarkNoticeImages(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_8218DCC0 Type2Hub::MarkNoticeImages");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  uint32_t r3 = save;
  const uint32_t list = save + kNotices;
  for (uint32_t i = 0; i < Load(base, list + 768); i++) {
    const uint32_t notice = list + 12 * i;
    const uint32_t table = Load(base, notice + 4);
    uint32_t entry = Load(base, notice + 8);
    Store(base, frame + 80, Load(base, notice + 0));
    for (;;) {
      r3 = data_tables::Entry56(base, table, entry);
      const uint32_t bits = Load(base, kHubData) + 408;
      const int32_t image = int32_t(Load(base, r3 + 16));
      if (image >= 0) {
        const uint32_t word = bits + 4 * uint32_t(image >> 5);
        Store(base, word, (1u << (image & 31)) | Load(base, word));
      }
      const uint32_t next = Load(base, r3 + 8);
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
  Store(base, machine + kMachineLoaded, 0);
  const uint32_t pages = uint32_t(Alloc(calls, frame, 0x8218E0A4, 28));
  Store(base, kHubPages, pages);
  Store(base, pages + 4, 0);
  Store(base, pages + 0, 0);
  Store(base, pages + 8, 0);
  Store(base, pages + 20, kNone);
  Store(base, pages + 12, kNone);
  Store(base, pages + 24, 0);
  const uint32_t menu = uint32_t(Alloc(calls, frame, 0x8218E0E0, 68));
  Store(base, kHubMenu, menu);
  Store(base, menu + 12, 0);
  Store(base, menu + 20, 1);
  Store(base, menu + 0, 0);
  Store(base, menu + 4, 0);
  Store(base, menu + 16, 7);
  for (const uint32_t at : {8u, 40u, 48u, 52u, 36u, 64u, 24u, 28u, 32u}) {
    Store(base, menu + at, 0);
  }
  // The data: 68 bytes asked for, as the original (VirtualAlloc gives a
  // page), its words up to +447 used.
  const uint32_t data = uint32_t(Alloc(calls, frame, 0x8218E140, 68));
  Store(base, kHubData, data);
  for (const uint32_t at : {0u, 4u, 8u, 16u, 20u}) {
    Store(base, data + at, 0);
  }
  Store(base, data + 424, kNone);
  Store(base, data + 436, kNone);
  for (const uint32_t at : {216u, 404u, 408u, 412u}) {
    Store(base, data + at, 0);
  }
  Store(base, Load(base, kHubData) + 20, 0);
  const uint32_t r3 = HubMarkNoticeImages(base, frame);
  Store(base, machine + kMachineState, Load(base, machine + kMachineReenter) != 0 ? 1 : 9);
  return r3;
}

uint64_t HubFreeImages(uint8_t* base, uint32_t data, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218DE88 Type2Hub::FreeImages");
  const uint32_t frame = Frame(base, sp, 112);
  uint64_t r3 = data;
  static constexpr struct {
    uint32_t list, lr;
  } kLists[2] = {{32, 0x8218DED8}, {220, 0x8218DF24}};
  for (const auto& list : kLists) {
    const uint32_t at = data + list.list;
    while (Load(base, at + 184) != 0) {
      const uint32_t count = Load(base, at + 184) - 1;
      Store(base, at + 184, count);
      const uint32_t record = Load(base, at + 4 * count);
      HubFreeImage(base, record, frame, calls);
      r3 = calls.Call(kVirtualFree, frame, list.lr, record, 0, kMemRelease);
    }
  }
  return r3;
}

uint64_t HubDestroy(uint8_t* base, uint32_t machine, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E1A8 Type2Hub::Destroy");
  const uint32_t frame = Frame(base, sp, 128);
  Store(base, Load(base, kHubData) + 20, 0);
  if (Load(base, Load(base, kHubData) + 4) != 0) {
    Store(base, machine + kMachineState, 14);
  }
  HubFreeImages(base, Load(base, kHubData), frame, calls);
  calls.Call(kVirtualFree, frame, 0x8218E204, Load(base, kHubPages), 0, kMemRelease);
  calls.Call(kVirtualFree, frame, 0x8218E218, Load(base, kHubMenu), 0, kMemRelease);
  const uint64_t r3 =
      calls.Call(kVirtualFree, frame, 0x8218E228, Load(base, kHubData), 0, kMemRelease);
  Store(base, kHubPages, 0);
  Store(base, kHubMenu, 0);
  Store(base, kHubData, 0);
  return r3;
}

uint64_t HubLoadNoticeImage(uint8_t* base, uint32_t data, uint32_t index, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218DF38 Type2Hub::LoadNoticeImage");
  const uint32_t frame = Frame(base, sp, 144);
  static constexpr struct {
    uint32_t asset, list, alloc, memset, record, load;
  } kImages[2] = {{22696, 32, 0x8218DF70, 0x8218DF80, 0x8218DF94, 0x8218DFE0},
                  {22741, 220, 0x8218E004, 0x8218E014, 0x8218E028, 0x8218E070}};
  uint64_t r3 = 0;
  for (const auto& image : kImages) {
    const uint32_t asset = image.asset + index;
    const uint32_t size = archive::AssetSize(base, asset);
    const uint32_t buffer = uint32_t(Alloc(calls, frame, image.alloc, size));
    calls.Call(kXMemSet, frame, image.memset, buffer, 0, size);
    const uint32_t record = uint32_t(Alloc(calls, frame, image.record, 16));
    Store(base, record + 4, index);
    Store(base, record + 0, 0);
    Store(base, record + 8, 0);
    Store(base, record + 12, 0);
    const uint32_t list = data + image.list;
    const uint32_t count = Load(base, list + 184);
    Store(base, list + 184, count + 1);
    Store(base, list + 4 * count, record);
    const uint64_t args[8] = {asset, buffer, kHubImageLoaded, record, 0, 0, 0, 0};
    r3 = calls.CallArgs(kArchiveLoad, frame, image.load, args);
  }
  return r3;
}

uint32_t HubLoadNoticeImages(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E240 Type2Hub::LoadNoticeImages");
  const uint32_t frame = Frame(base, sp, 112);
  uint32_t data = Load(base, kHubData);
  for (uint32_t i = 0; i < 46; i++) {
    if ((Load(base, data + 408 + 4 * (i >> 5)) & (1u << (i & 31))) != 0) {
      HubLoadNoticeImage(base, data, i, frame, calls);
      data = Load(base, kHubData);
    }
  }
  return data;
}

uint32_t HubOpenMovie(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218E368 Type2Hub::OpenMovie");
  const uint32_t frame = Frame(base, sp, 2064);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  uint32_t block = 0;
  if (guest::LoadU16(base, save + 6328) != 0) {
    calls.Call(kMemcpy, frame, 0x8218E3D4, frame + 80, SelectedGarageCar(base, save) + 212,
               1968);
    block = car_makers::BlockOf(Load(base, frame + 92));
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
  stand_models::MakeModel(base, call, Load(base, Load(base, build_screen::kStandHolder)), block, 1,
                          frame);
  calls.Call(kMoviesOpen, frame, 0x8218E408, kHubMovie, 22545);
  return HubLoadNoticeImages(base, frame, calls);
}

uint64_t HubImageLoaded(uint8_t* base, uint32_t buffer, uint32_t record, uint32_t sp,
                        const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8218D8E8 Type2Hub::ImageLoaded");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t xpr = uint32_t(Alloc(calls, frame, 0x8218D918, 24));
  Store(base, record + 12, xpr);
  calls.Call(kXprCreate, frame, 0x8218D924, xpr, buffer);
  const uint32_t texture = uint32_t(Alloc(calls, frame, 0x8218D938, 12));
  Store(base, record + 8, texture);
  const uint64_t level = calls.Call(kXprGetTexture, frame, 0x8218D94C, Load(base, record + 12), 0);
  Store(base, Load(base, record + 8) + 0, uint32_t(level));
  calls.Call(kGetLevelDesc, frame, 0x8218D968, Load(base, Load(base, record + 8) + 0), 0,
             frame + 80);
  Store(base, Load(base, record + 8) + 4, Load(base, frame + 104));
  Store(base, Load(base, record + 8) + 8, Load(base, frame + 108));
  const uint64_t r3 = calls.Call(kVirtualFree, frame, 0x8218D994, buffer, 0, kMemRelease);
  Store(base, record + 0, 1);
  return r3;
}

}  // namespace type2_screens
