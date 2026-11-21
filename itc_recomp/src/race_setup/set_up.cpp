// Race::SetUp (sub_82115D98, docs/functions/race-setup.md) as plain C++; see
// race_setup.h. Its callees are the plain C++ of the set-up's functions,
// each with the frame the original's call gives it (below Race::SetUp's
// 11,984-byte frame); where a callee's plain C++ would hand back to its
// original (a copy memcpy's override wouldn't make natively, Race::SetUpRival's
// kind 1, Race::PlaceRacers' save data branch, Car::Refresh refusing), that
// original is called through the caller's guest call, from the same frame.

#include "race_setup/race_setup.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "car_specs/car_specs.h"
#include "data_tables/data_tables.h"
#include "guest.h"
#include "native/buffers.h"
#include "race_overlays/race_overlays.h"
#include "schema/race_setup.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace race_setup {

namespace {

using G = schema::RaceSetupGlobals;

constexpr uint32_t kSetUpFrameBytes = 11984;
// The racer records Race::SetUp builds on its stack: the player's, the
// event 1 rival's and the event's entries'.
constexpr uint32_t kPlayerRecord = 7952;
constexpr uint32_t kRivalRecord = 80;
constexpr uint32_t kEntryRecord = 4016;
// The callees' frames under Race::SetUp's.
constexpr uint32_t kResetFrame = 112;
constexpr uint32_t kRivalFrame = 4112;
constexpr uint32_t kPlaceFrame = 128;
constexpr uint32_t kSetStartFrame = 4096;

// The garage car the save data's list picks: its +0x19920-th node of the
// list at +6120 (4-byte nodes, +0 the next, +2 the car; the head halfword at
// +6324), followed as the original does. Returns the car's index (signed).
int32_t GarageCar(uint8_t* base, uint32_t save) {
  const uint32_t nodes = save + 6120;
  uint32_t n = guest::LoadU32(base, save + 0x19920);
  uint32_t node = uint32_t(int32_t(int16_t(guest::LoadU16(base, nodes + 204)))) * 4 + nodes;
  while (n != 0) {
    node = uint32_t(int32_t(int16_t(guest::LoadU16(base, node + 0)))) * 4 + nodes;
    n--;
  }
  return int32_t(int16_t(guest::LoadU16(base, node + 2)));
}

// The garage car's address (1,968-byte cars from the save's +6120 + 212).
uint32_t GarageCarAt(uint8_t* base, uint32_t save) {
  return uint32_t(uint64_t(int64_t(GarageCar(base, save))) * 1968) + save + 6120;
}

// Car::Refresh through the guest call where its plain C++ refuses, and the
// GuestRefresh the set-up's callees take.
struct Refresh {
  GuestCall call;
  void* user;
  uint32_t r1;
  uint32_t lr;
};

void RefreshThrough(void* user, uint32_t car) {
  const Refresh& r = *static_cast<const Refresh*>(user);
  const uint64_t args[4] = {car, 0, 0, 0};
  r.call(r.user, 0x820C4998, args, 0.0, r.r1, r.lr);
}

void RefreshCar(uint8_t* base, uint32_t car, GuestCall call, void* user, uint32_t r1,
                uint32_t lr) {
  uint32_t r3;
  if (!car_specs::CarRefresh(base, car, &r3)) {
    Refresh refresh{call, user, r1, lr};
    RefreshThrough(&refresh, car);
  }
}

// The racer record at `record` copied to the records at their count (memcpy's
// native copy), the count up. Returns the count before.
uint32_t AppendRecord(uint8_t* base, uint32_t records, uint32_t record, uint32_t caller) {
  const uint32_t count = records + kRacersCount;
  const uint32_t to =
      uint32_t(uint64_t(guest::LoadU32(base, count)) * kRacerRecordBytes + records);
  if (!native::Buffers_Memcpy(base, to, record, kRacerRecordBytes, caller)) {
    ITC_LOG_ERROR("race_setup: Race::SetUp's copy to {:08X} not made natively", to);
  }
  const uint32_t before = guest::LoadU32(base, count);
  guest::StoreU32(base, count, before + 1);
  return before;
}

void SetRoute(uint8_t* base, uint32_t location, uint32_t frame, GuestCall call, void* user,
              uint32_t lr) {
  if (!RaceSetRoute(base, 0, location, frame - kRouteFrameBytes)) {
    const uint64_t args[4] = {0, location, 0, 0};
    call(user, 0x82101D38, args, 0.0, frame, lr);
  }
}

void SetUpRivalFrom(uint8_t* base, uint32_t location, uint64_t rival, uint32_t add,
                    uint32_t frame, GuestCall call, void* user, uint32_t lr) {
  const uint32_t rival_frame = frame - kRivalFrame;
  if (!SetUpRivalNative(base, location, uint32_t(rival), add, rival_frame)) {
    const uint64_t args[4] = {location, rival, add, 0};
    call(user, 0x82112DB0, args, 0.0, frame, lr);
    return;
  }
  Refresh refresh{call, user, rival_frame, 0x82112EC4};
  SetUpRival(base, location, uint32_t(rival), add, rival_frame, RefreshThrough, &refresh);
}

}  // namespace

bool SetUpNative(uint8_t* base, uint32_t save, uint32_t frame) {
  if (!native::Buffers_NativeMemcpy()) {
    return false;
  }
  const uint32_t records = G::race_data(base).address + 48;
  const uint32_t all = 93 * kRacerRecordBytes;
  return !c_runtime::MayShare(frame + kPlayerRecord, 1884, GarageCarAt(base, save) + 212, 1884) &&
         !c_runtime::MayShare(records, all, frame + kPlayerRecord, kRacerRecordBytes) &&
         !c_runtime::MayShare(records, all, frame + kRivalRecord, kRacerRecordBytes) &&
         !c_runtime::MayShare(records, all, frame + kEntryRecord, kRacerRecordBytes);
}

uint32_t SetUp(uint8_t* base, uint32_t save, uint64_t rival, uint32_t frame, GuestCall call,
               void* user) {
  ITC_NATIVE_RUNNING("sub_82115D98 Race::SetUp");
  guest::StoreU32(base, save + 6112, 48);
  if (!RaceDataResetGlobal(base, frame - kResetFrame)) {
    const uint64_t args[4] = {0, 0, 0, 0};
    call(user, 0x821021A8, args, 0.0, frame, 0x82115DC0);
  }
  const int32_t r = int32_t(race_overlays::Rand(base));
  guest::StoreU32(base, G::race_data(base).address + 0xA3AE0, uint32_t(r - r / 10 * 10));
  guest::StoreU32(base, G::race_data(base).address + 0xA3AF0, 1);
  guest::StoreU32(base, G::race_data(base).address + 0xA3AF4, 2);
  vehicle_accessors::RaceDataSetWord32(base, guest::LoadU32(base, save + 6104));

  // The player's racer record: the garage car picked, Car::Refresh, added.
  const uint32_t player = frame + kPlayerRecord;
  CarInit(base, player);
  if (!native::Buffers_Memcpy(base, player, GarageCarAt(base, save) + 212, 1884, 0x82115E6C)) {
    ITC_LOG_ERROR("race_setup: Race::SetUp's garage car not copied natively");
  }
  schema::Car(base, player)
      .set_unknown_3892(guest::LoadU32(base, vehicle_accessors::SaveDataOf(base, 0) + 0x1F870));
  RefreshCar(base, player, call, user, frame, 0x82115E90);
  const uint32_t car = GarageCarAt(base, save);
  const uint32_t word = uint32_t(int32_t(int8_t(guest::LoadU8(base, save + 8))));
  const double value = double(guest::LoadFloat(base, car + 2104));
  if (AddRacerNative(base, player)) {
    AddRacer(base, player, 0, value, word);
  } else {
    const uint64_t args[4] = {player, 0, 0, uint64_t(int64_t(int32_t(word)))};
    call(user, 0x821017C0, args, value, frame, 0x82115ED8);
  }
  const int32_t mode = int32_t(guest::LoadU32(base, save + 6104));
  vehicle_accessors::RaceDataSetWord59618(base, mode == 1 ? 2 : mode == 2 ? 1 : 3);
  vehicle_accessors::RaceDataSetWord5961C(base, 5);
  SetRoute(base, guest::LoadU32(base, save + 6108), frame, call, user, 0x82115F18);

  const int32_t event = int32_t(guest::LoadU32(base, save + 6100));
  if (event <= 999 && event == 1) {
    // Event 1: the player and a fixed rival (car 11, its paint and the +1568
    // bytes set) in the first stage.
    StagesAddRacer(base, G::race_data(base).address + kStageList, 0, 2, -1);
    const uint32_t records = G::race_data(base).address + 48;
    const uint32_t rec = frame + kRivalRecord;
    const schema::Car car(base, rec);
    RacerRecordInit(base, rec);
    SetModel(base, rec, 11);
    car.set_paint_kind(1);
    car.set_unknown_1569(0);
    car.set_unknown_1571(7);
    car.set_extras_word_4(0);
    car.set_paint_pick(0xFFFE);
    car.set_unknown_1574(22);
    car.set_paint_hue(234);
    car.set_unknown_1575(25);
    car.set_paint_saturation(92);
    car.set_unknown_1578(24);
    car.set_paint_value(64);
    car.set_unknown_1570(3);
    car.set_unknown_1572(4);
    car.set_unknown_1573(14);
    car.set_unknown_1576(14);
    car.set_unknown_1577(27);
    car.set_unknown_1579(27);
    car.set_flags(car.flags() | 2);
    RefreshCar(base, rec, call, user, frame, 0x82115FF4);
    const uint32_t index = AppendRecord(base, records, rec, 0x82116014);
    StagesAddRacer(base, G::race_data(base).address + kStageList, index, 2, -1);
    vehicle_accessors::StagesSetWord808(base, G::race_data(base).address + kStageList, 1);
    const uint32_t flags = G::race_data(base).address + 0x59AE4;
    guest::StoreU32(base, flags, guest::LoadU32(base, flags) | 8);
    AddStage(base, 0);
  } else {
    const uint32_t data = G::race_data(base).address;
    const uint32_t records = data + 48;
    if (int32_t(rival) != -1) {
      SetUpRivalFrom(base, guest::LoadU32(base, save + 6108), rival, 1, frame, call, user,
                     0x82116088);
      vehicle_accessors::StagesSetWord808(base, data + kStageList, 1);
      const uint32_t flags = G::race_data(base).address + 0x59AE4;
      guest::StoreU32(base, flags, guest::LoadU32(base, flags) | 16);
    } else {
      // The event's entries, each a racer record from its item, then the
      // rival: the first owned item (or no stage when there's none).
      const int32_t first = EventCurrentFirstOwned(base);
      SetRoute(base, guest::LoadU32(base, save + 6108), frame, call, user, 0x821160C8);
      const uint32_t flags = G::race_data(base).address + 0x59AE4;
      guest::StoreU32(base, flags, guest::LoadU32(base, flags) | 8);
      const uint32_t event_object = EventCurrent(base);
      const int32_t entries = int32_t(int16_t(EventCount(base)));
      const uint32_t rec = frame + kEntryRecord;
      const schema::Car car(base, rec);
      for (int32_t i = 0; i < entries; i++) {
        const uint32_t entry = guest::LoadU32(base, event_object + 156) + 12 * uint32_t(i);
        const uint32_t item = data_tables::Item(base, guest::LoadU32(base, entry + 0));
        RacerRecordInit(base, rec);
        car.set_item_id(uint32_t(int32_t(int16_t(guest::LoadU16(base, item + 0)))));
        const uint8_t byte37 = guest::LoadU8(base, item + 37);
        car.set_entry_index(uint32_t(i));
        car.set_racer_word(uint32_t(int32_t(int8_t(byte37))));
        FromBuild(base, rec, item + 160);
        const uint32_t with2 = car.flags() | 2;
        car.set_flags(with2);
        car.set_start_position(guest::LoadU32(base, entry + 4));
        if (guest::LoadU32(base, entry + 8) == 0) {
          car.set_flags(with2 | 0x300);
        }
        RefreshCar(base, rec, call, user, frame, 0x82116174);
        AppendRecord(base, records, rec, 0x8211618C);
      }
      if (first == -1) {
        AddStage(base, 4);
      } else {
        SetUpRivalFrom(base, guest::LoadU32(base, save + 6108), uint64_t(int64_t(first)), 0,
                       frame, call, user, 0x821161CC);
      }
    }
  }

  // The race data's +20 and the event's two bits in +24.
  guest::StoreU32(base, G::race_data(base).address + 20,
                  guest::LoadU32(base, save + 0x199E0));
  const uint32_t bits = G::race_data(base).address + 24;
  const int32_t ev = int32_t(guest::LoadU32(base, save + 6100));
  guest::StoreU32(base, bits, 0);
  uint32_t at = uint32_t(ev - ev / 7 * 7) * 4 + G::event_bits_address();
  for (int k = 0; k < 2; k++, at += 2) {
    const int32_t bit = int32_t(int16_t(guest::LoadU16(base, at)));
    if (bit < 0) {
      continue;
    }
    const uint32_t word = bits + uint32_t(bit >> 5) * 4;
    guest::StoreU32(base, word, guest::LoadU32(base, word) | (1u << (bit & 31)));
  }

  const uint32_t place_frame = frame - kPlaceFrame;
  if (PlaceRacersNative(base, place_frame)) {
    Refresh refresh{call, user, place_frame - kSetStartFrame, 0x82101DC8};
    PlaceRacers(base, place_frame, RefreshThrough, &refresh);
  } else {
    const uint64_t args[4] = {0, 0, 0, 0};
    call(user, 0x82102048, args, 0.0, frame, 0x82116248);
  }

  // The races counted by location (14 to 19, then 0 to 13), to 9999.
  uint32_t location = guest::LoadU32(base, save + 6108);
  uint32_t which = location - 14;
  if (which > 5) {
    which = 7;
  }
  if (location > 13) {
    location = 15;
  }
  uint32_t index = 0;
  bool count = true;
  if (int32_t(which) != 7) {
    index = which + 2943;
  } else if (int32_t(location) != 15) {
    index = location + 2943;
  } else {
    count = false;
  }
  if (count) {
    const uint32_t half = index * 2 + save;
    const int32_t n = int32_t(int16_t(guest::LoadU16(base, half)));
    if (n < 9999) {
      guest::StoreU16(base, half, uint16_t(n + 1));
    }
  }
  CountRace(base, save + 48, guest::LoadU32(base, save + 6104));
  return save + 48;
}

}  // namespace race_setup
