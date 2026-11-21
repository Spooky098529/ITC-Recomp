// The build screen's entry (docs/functions/type2-screens.md): see
// type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "data_tables/data_tables.h"
#include "type2_screens/internal.h"
#include "ui_movies/game.h"
#include "vehicle_accessors/vehicle_accessors.h"

namespace type2_screens {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;
constexpr uint32_t kListsEnd = 0x2C760;    // 56 groups of 3,252 bytes (BuildGroup)
constexpr uint32_t kListBytes = schema::BuildGroup::kSize;
constexpr uint32_t kSaveBuilds = 0x199F8;  // player 0's build states, 12 bytes each
constexpr uint32_t kSaveSeen = 0x1ACE4;    // a bit a build
constexpr uint32_t kPercent = 0x82002968;  // the float the state-2 count is scaled by
constexpr uint32_t kRatioScale = 0x82002A0C;
constexpr uint32_t kFadeInLayer = 0x82001BEC;

schema::BuildScreenData Screen(uint8_t* base) { return Globals::build_screen(base); }

uint16_t Half(uint8_t* base, uint32_t address) { return guest::LoadU16(base, address); }
int32_t SignedHalf(uint8_t* base, uint32_t address) { return int16_t(Half(base, address)); }

// The list's link `steps` from its head, by the halfword links.
uint32_t Walk(uint8_t* base, uint32_t list, uint32_t steps) {
  uint32_t link = list + 4 * uint32_t(int32_t(int16_t(schema::BuildSlotList(base, list).head())));
  while (steps != 0) {
    link = list + 4 * uint32_t(SignedHalf(base, link));
    steps -= 1;
  }
  return link;
}

// The build a list's link names: the slot of the node (+2).
uint32_t BuildAt(uint8_t* base, uint32_t list, uint32_t link) {
  return Load(base, schema::BuildSlotList(base, list).slots_address() +
                        4 * uint32_t(SignedHalf(base, link + 2)));
}

}  // namespace

void BuildListRemoveAt(uint8_t* base, uint32_t list, uint32_t position) {
  ITC_NATIVE_RUNNING("sub_8219C160 BuildList::RemoveAt");
  const schema::BuildSlotList slots(base, list);
  const uint32_t link = position == 0 ? slots.head_address() : Walk(base, list, position - 1);
  const uint16_t node = Half(base, link);
  const uint32_t at = list + 4 * uint32_t(int32_t(int16_t(node)));
  const uint16_t next = Half(base, at);
  guest::StoreU16(base, at, node);
  guest::StoreU16(base, link, next);
  const uint16_t moved = Half(base, at);
  const uint16_t free = slots.free();
  slots.set_free(moved);
  guest::StoreU16(base, at, free);
  slots.set_count(uint16_t(slots.count() - 1));
}

uint64_t BuildFill(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A0228 BuildScreen::Fill");
  const uint32_t frame = Frame(base, sp, 240);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  int32_t day = int32_t(Load(base, save + 6100));
  for (uint32_t k = 0; k < 5; k++) {
    Store(base, Screen(base).name_address() + 4 * k, Load(base, save + 8 + 4 * k));
  }
  Screen(base).set_money(Load(base, save + 6116));
  Screen(base).set_weekday(uint32_t(day - (day / 7) * 7));
  if (day > 999) {
    day = 999;
  }
  Screen(base).set_day(uint32_t(day));
  // The builds.
  const uint32_t record = frame + 96;
  for (uint32_t i = 0; i < 400; i++) {
    const uint32_t item = data_tables::Item(base, i);
    const uint32_t entry = save + kSaveBuilds + 12 * i;
    Store(base, record + 12, 0);
    Store(base, record + 44, 0);
    Store(base, record + 48, item + 160);
    const int32_t group = int8_t(guest::LoadU8(base, item + 34));
    Store(base, record + 24, 0);
    Store(base, record + 4, uint32_t(group));
    Store(base, record + 0, uint32_t(SignedHalf(base, item + 0)));
    Store(base, record + 20, uint32_t(int32_t(int8_t(guest::LoadU8(base, item + 36)))));
    Store(base, record + 28, uint32_t(SignedHalf(base, item + 42)));
    uint32_t kind = 0;
    switch (SignedHalf(base, item + 40)) {
      case 1: kind = 0; Store(base, record + 24, kind); break;
      case 2: kind = 1; Store(base, record + 24, kind); break;
      case 4: kind = 2; Store(base, record + 24, kind); break;
      case 7: kind = 3; Store(base, record + 24, kind); break;
      default: break;
    }
    const bool owned = SignedHalf(base, entry + 2) != 0;
    uint32_t flags;
    if (!owned && Half(base, entry + 4) == 0 && Half(base, entry + 0) == 0) {
      Store(base, record + 8, 0);
      flags = 1;
    } else if (!owned) {
      Store(base, record + 8, 1);
      flags = 3;
    } else {
      Store(base, record + 8, 2);
      flags = 7;
    }
    Store(base, record + 32, flags);
    const uint32_t seen = Load(base, save + kSaveSeen + 4 * (i >> 5)) & (1u << (i & 31));
    if (seen != 0) {
      Store(base, record + 40, 1);
      Store(base, record + 32, flags | 8);
    } else {
      Store(base, record + 40, 0);
    }
    if (group == 48 || group == 49) {
      Store(base, record + 16, group == 48 ? 2 : 1);
      const uint32_t first = group == 48 ? 48 : 52;
      const uint32_t v = kind == 0 ? first : kind == 1 ? first + 1 : first + 2 + (kind != 2 ? 1 : 0);
      Store(base, record + 4, v);
    } else {
      Store(base, record + 16, 0);
    }
    Store(base, record + 36, Load(base, entry + 8));
    calls.Call(kMemcpy, frame, 0x821A04B8,
               Screen(base).builds_address() + schema::BuildRecord::kSize * i, record, 52);
  }
  // The lists made empty.
  for (uint32_t off = 0; off < kListsEnd; off += kListBytes) {
    const schema::BuildSlotList list(
        base, schema::BuildGroup(base, Screen(base).groups_address() + off).list_address());
    for (uint32_t k = 0; k < 400; k++) {
      guest::StoreU16(base, list.links_address() + 4 * k + 2, uint16_t(k));
      guest::StoreU16(base, list.links_address() + 4 * k, uint16_t(k + 1));
    }
    guest::StoreU16(base, list.links_address() + 1596, 0xFFFF);  // the last node's next
    list.set_head(0xFFFF);
    list.set_unknown_1606(0xFFFF);
    list.set_free(0);
    list.set_unknown_1602(0xFFFF);
    list.set_count(0);
  }
  // Each build into its group's list.
  for (uint32_t off = 0; off < 52 * 400; off += 52) {
    const schema::BuildScreenData screen = Screen(base);
    const schema::BuildRecord build(base, screen.builds_address() + off);
    Store(base, frame + 80, build.address);
    const uint32_t list =
        schema::BuildGroup(base, screen.groups_address() + build.group() * kListBytes).list_address();
    calls.Call(kBuildListInsert, frame, 0x821A0560, list, frame + 80,
               uint64_t(int64_t(int16_t(schema::BuildSlotList(base, list).count()))));
  }
  // The state-2 count as a percentage.
  const schema::BuildScreenData screen = Screen(base);
  int32_t owned = 0;
  for (uint32_t i = 0; i < 400; i++) {
    if (schema::BuildRecord(base, screen.builds_address() + schema::BuildRecord::kSize * i).state() ==
        2) {
      owned++;
    }
  }
  Store(base, frame + 80, owned < 0 ? kNone : 0);
  Store(base, frame + 84, uint32_t(owned));
  {
    const float scaled = float(double(Single(owned)) * double(guest::LoadFloat(base, kPercent)));
    screen.set_number(uint32_t(TruncateWord(double(scaled))));
  }
  const float scale = guest::LoadFloat(base, kRatioScale);
  // The lists' summaries.
  for (uint32_t j = 0, off = 0; off < kListsEnd; j++, off += kListBytes) {
    const schema::BuildGroup summary(base, Screen(base).groups_address() + off);
    summary.set_index(j);
    const int32_t count = int16_t(summary.list_count());
    int32_t states2 = 0;
    uint32_t changed = 0;
    int32_t kind = -1;
    const uint32_t list = summary.list_address();
    for (int32_t k = 0; k < count; k++) {
      const schema::BuildRecord build(base, BuildAt(base, list, Walk(base, list, uint32_t(k))));
      if (build.state() == 2) {
        states2++;
      }
      if (build.flags() != build.saved_flags()) {
        changed = 1;
      }
      summary.set_category(build.category());
      const int32_t its = int32_t(build.value());
      if (kind == -1) {
        kind = its;
      } else if (kind != -2 && kind != its) {
        kind = -2;
      }
      summary.set_level(build.level());
    }
    summary.set_value(uint32_t(kind));
    if (count > 0) {
      summary.set_count(uint32_t(count));
      summary.set_owned(uint32_t(states2));
      Store(base, frame + 88, count < 0 ? kNone : 0);
      Store(base, frame + 92, uint32_t(count));
      Store(base, frame + 80, states2 < 0 ? kNone : 0);
      Store(base, frame + 84, uint32_t(states2));
      const float ratio = float(double(Single(states2)) / double(Single(count)));
      const float scaled = float(double(ratio) * double(scale));
      summary.set_percent(uint32_t(TruncateWord(double(scaled))));
      summary.set_all_owned(states2 == count ? 1 : 0);
      summary.set_mark(changed);
    } else {
      summary.set_percent(0);
      summary.set_count(0);
      summary.set_owned(0);
      summary.set_all_owned(0);
      summary.set_mark(0);
    }
    summary.set_kind(j - 48 <= 3 ? 2 : j - 52 <= 3 ? 1 : 0);
  }
  // The builds above the player's stage removed.
  uint64_t r3 = calls.Call(kStageOf, frame, 0x821A0774, save);
  const int32_t limit = int32_t(uint32_t(r3) + 1);
  schema::BuildScreenData screen_now = Screen(base);
  for (uint32_t off = 0; off < kListsEnd; off += kListBytes) {
    const schema::BuildGroup summary(base, screen_now.groups_address() + off);
    const int32_t count = int16_t(summary.list_count());
    if (count <= 0) {
      continue;
    }
    const uint32_t list = summary.list_address();
    uint32_t removed = 0;
    for (int32_t k = 0; k < count; k++) {
      const uint32_t position = uint32_t(k) - removed;
      const schema::BuildRecord build(base, BuildAt(base, list, Walk(base, list, position)));
      if (int32_t(build.level()) > limit) {
        BuildListRemoveAt(base, list, position);
        r3 = list;
        removed++;
      }
    }
    screen_now = Screen(base);
  }
  // Whether a kind-3 list has builds.
  uint32_t kinds3 = 0;
  for (uint32_t j = 0; j < 56; j++) {
    const schema::BuildGroup group(base, screen_now.groups_address() + kListBytes * j);
    if (group.category() == 3 && group.list_count() != 0) {
      kinds3++;
    }
  }
  screen_now.set_opened(kinds3 > 0 ? 1 : 0);
  return r3;
}

uint32_t BuildSetUpStatus(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219C438 BuildScreen::SetUpStatus");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = FindMovie(base, kBuildMovie, frame + 80);
  calls.Call(kResetPose, frame, 0x8219C470, movie);
  calls.Call(kSetChildNameText, frame, 0x8219C48C, movie, 0, 6, Screen(base).name_address());
  calls.Call(kSetNumberText, frame, 0x8219C4A4, movie, 0, 9, Screen(base).money());
  const uint64_t args[8] = {movie, 0, 13, 8, Screen(base).day(), 0, 0, 0};
  calls.CallArgs(kSetText, frame, 0x8219C4C0, args);
  const uint32_t node = ui_movies::FindNode(base, movie, 0, 11);
  Store(base, node + 108, Screen(base).weekday());
  Screen(base).set_car_shown(0);
  Screen(base).set_stand_model(0);
  Screen(base).set_set_up(1);
  return node;
}

uint64_t BuildEnter(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_821A0860 BuildScreen::Enter");
  const uint32_t frame = Frame(base, sp, 96);
  BuildFill(base, frame, calls);
  BuildSetUpStatus(base, frame, calls);
  PagesGo(base, Globals::build_pages(base).address, 0, 0, 1);
  return calls.Call(kFadeIn, frame, 0x821A08C4, Load(base, kFadeInLayer), 0, 0, 0);
}

}  // namespace type2_screens
