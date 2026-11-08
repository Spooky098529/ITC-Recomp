// The build screen's group, build and stand pages with their cursor jumps
// (docs/functions/type2-screens.md): see type2_screens.h.

#include "type2_screens/type2_screens.h"

#include "base/log.h"
#include "race_screens/race_screens.h"
#include "type2_screens/internal.h"

namespace type2_screens {

namespace {

constexpr uint32_t kNone = 0xFFFFFFFFu;

// The screen data's lists (build_screen.h): the shown groups' pointers and
// their count, the build list shown (its 16-bit links, head +1604, slots
// +1612 on) and its count (a halfword).
constexpr uint32_t kShownGroups = 0x318C4;
constexpr uint32_t kShownCount = 0x319A4;
constexpr uint32_t kBuilds = 0x319A8;
constexpr uint32_t kBuildCount = 0x31FF0;

int32_t S(uint32_t value) { return int32_t(value); }

uint32_t Screen(uint8_t* base) { return Load(base, kBuildScreen); }
uint32_t List(uint8_t* base) { return Load(base, kBuildList); }

// A halfword link or slot, sign-extended (lha).
uint32_t Half(uint8_t* base, uint32_t address) {
  return uint32_t(int32_t(int16_t(guest::LoadU16(base, address))));
}

// The build at `position` of the screen's build list: the head's node
// walked `position` links (as a count down to 0), its slot's pointer.
uint32_t BuildAt(uint8_t* base, uint32_t position) {
  const uint32_t list = Screen(base) + kBuilds;
  uint32_t node = list + (Half(base, list + 1604) << 2);
  for (uint32_t n = position; n != 0; n--) {
    node = list + (Half(base, node) << 2);
  }
  return Load(base, list + ((Half(base, node + 2) + 403) << 2));
}

// The shown group at `index`.
uint32_t GroupAt(uint8_t* base, uint32_t index) {
  return Load(base, Screen(base) + kShownGroups + 4 * index);
}

// The player's action mapper entry (0x826BAB80 + 8 × [0x826BABA4]).
uint32_t Entry(uint8_t* base) {
  return race_screens::kMapperEntries + (Load(base, race_screens::kControllerEntries) << 3);
}

// The mapper's slot 12 (+48) on `action` through `calls`, as the original's
// bctrl from `frame` returning to `lr`: whether its low byte is set.
bool Fired(uint8_t* base, uint32_t entry, uint32_t action, uint32_t frame, uint32_t lr,
           const Calls& calls) {
  const uint32_t mapper = Load(base, entry + 0);
  const uint32_t function = Load(base, Load(base, mapper) + 48);
  return (calls.Call(function, frame, lr, mapper, action) & 0xFF) != 0;
}

// The jumps' return addresses: the back slot's, FinishNamed's,
// SetSelection's, Fit's and the forward slot's.
struct JumpLrs {
  uint32_t back, finish, select, fit, forward;
};

// The jumps' shared body over the list's items 0 to `count` - 1 (the
// cursor at the list's scroll + selection): the back action (`back`) fired
// moves to the previous item `hit` takes (from the cursor down to 0, then
// from the end down past the cursor), the forward one to the next (up to
// the end, then from 0 up to the cursor); on a jump the named animation
// `named` of movie 6 finished, the selection set to the item less the
// scroll and the list refitted (`MenuList::Fit(list, 0)`). 1 when it
// jumped, else 0. Its 160-byte frame (the movie id at +80).
template <typename Hit>
uint32_t Jump(uint8_t* base, uint32_t sp, uint32_t back, uint32_t forward, bool groups,
              uint64_t named_base, bool named_fixed, const JumpLrs& lrs, Hit hit,
              const Calls& calls) {
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t entry = Entry(base);
  const uint32_t movie = FindMovie(base, kBuildMovie, frame + 80);
  const uint32_t selection = race_screens::ListSelection(base, List(base));
  const uint32_t scroll = Load(base, List(base) + 4);
  const uint32_t position = scroll + selection;
  const uint32_t count = groups ? Load(base, Screen(base) + kShownCount)
                                : Half(base, Screen(base) + kBuildCount);
  auto found = [&](uint32_t index) -> uint32_t {
    const uint64_t named = named_fixed ? named_base : uint64_t(selection) + named_base;
    calls.Call(kFinishNamed, frame, lrs.finish, movie, named);
    calls.Call(kListSetSelection, frame, lrs.select, List(base), index - scroll);
    calls.Call(kListFit, frame, lrs.fit, List(base), 0);
    return 1;
  };
  if (S(Load(base, entry + 4)) != 1 && Fired(base, entry, back, frame, lrs.back, calls)) {
    for (uint32_t i = position - 1; S(i) >= 0; i--) {
      if (hit(i)) {
        return found(i);
      }
    }
    for (uint32_t i = count - 1; S(i) > S(position); i--) {
      if (hit(i)) {
        return found(i);
      }
    }
    return 0;
  }
  if (S(Load(base, entry + 4)) == 1 || !Fired(base, entry, forward, frame, lrs.forward, calls)) {
    return 0;
  }
  for (uint32_t i = position + 1; S(i) < S(count); i++) {
    if (hit(i)) {
      return found(i);
    }
  }
  for (uint32_t i = 0; S(i) < S(position); i++) {
    if (hit(i)) {
      return found(i);
    }
  }
  return 0;
}

// The hits: groups marked (+32 set) or whose +28 is 0; builds whose +32
// isn't their +36, or whose kind (+8) isn't 2.
auto MarkedGroup(uint8_t* base) {
  return [base](uint32_t i) { return Load(base, GroupAt(base, i) + 32) != 0; };
}
auto Group28Clear(uint8_t* base) {
  return [base](uint32_t i) { return Load(base, GroupAt(base, i) + 28) == 0; };
}
auto ChangedBuild(uint8_t* base) {
  return [base](uint32_t i) {
    const uint32_t mark = Load(base, BuildAt(base, i) + 32);
    return mark != Load(base, BuildAt(base, i) + 36);
  };
}
auto KindNot2(uint8_t* base) {
  return [base](uint32_t i) { return S(Load(base, BuildAt(base, i) + 8)) != 2; };
}

// The pages' shared moves: MenuSound::Play from the page's frame.
uint64_t Sound(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t sound) {
  return calls.Call(kMenuSoundPlay, frame, lr, sound);
}
uint64_t ListCall(const Calls& calls, uint32_t function, uint32_t list, uint32_t frame,
                  uint32_t lr) {
  return calls.Call(function, frame, lr, list);
}
// The list not moving: +24 0 and its scroll (+4) its scroll before (+8).
bool Still(uint8_t* base, uint32_t list) {
  return Load(base, list + 24) == 0 && Load(base, list + 4) == Load(base, list + 8);
}

// BuildsPage and BuildPage: level `level`, the jumps given, the
// transitions (the first list moving, the second moving, accepted then
// queued, back) and their return addresses.
struct LevelPage {
  uint32_t level;
  uint32_t moving, group_moving, accepted, queued, back;
  uint32_t update, group_update, accepted_lr, cancelled_lr, sound_lr;
};

template <typename JumpA, typename JumpB, typename OnAccept>
uint64_t LevelPageRun(uint8_t* base, uint32_t sp, const LevelPage& p, JumpA jump_a,
                      JumpB jump_b, OnAccept on_accept, const Calls& calls) {
  const uint32_t frame = Frame(base, sp, 112);
  ListCall(calls, kListUpdate, Load(base, kBuildList), frame, p.update);
  ListCall(calls, kListUpdate, Load(base, kBuildGroupList), frame, p.group_update);
  if (!Still(base, List(base)) || jump_a(frame) != 0 || jump_b(frame) != 0) {
    PagesGo(base, Load(base, kBuildPages), p.level, p.moving, kNone);
    return Sound(calls, frame, p.sound_lr, 4);
  }
  if (Load(base, Load(base, kBuildGroupList) + 24) != 0) {
    PagesGo(base, Load(base, kBuildPages), p.level, p.group_moving, kNone);
    return Sound(calls, frame, p.sound_lr, 4);
  }
  uint64_t r3 = ListCall(calls, kListAccepted, List(base), frame, p.accepted_lr);
  if (uint32_t(r3) != 0) {
    PagesGo(base, Load(base, kBuildPages), p.level + 1, p.accepted, p.queued);
    on_accept(frame);
    return Sound(calls, frame, p.sound_lr, 0);
  }
  r3 = ListCall(calls, kListCancelled, List(base), frame, p.cancelled_lr);
  if (uint32_t(r3) == 0) {
    return r3;
  }
  PagesGo(base, Load(base, kBuildPages), kNone, p.back, kNone);
  return Sound(calls, frame, p.sound_lr, 2);
}

}  // namespace

uint32_t BuildJumpGroupA(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219DBC8 BuildScreen::JumpGroupA");
  return Jump(base, sp, 161, 162, true, 29, false,
              {0x8219DC60, 0x8219DCEC, 0x8219DCF8, 0x8219DD04, 0x8219DD30}, MarkedGroup(base),
              calls);
}

uint32_t BuildJumpGroupB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219DDB8 BuildScreen::JumpGroupB");
  return Jump(base, sp, 163, 164, true, 29, false,
              {0x8219DE50, 0x8219DEDC, 0x8219DEE8, 0x8219DEF4, 0x8219DF20}, Group28Clear(base),
              calls);
}

uint32_t BuildJumpBuildA(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219DFA8 BuildScreen::JumpBuildA");
  return Jump(base, sp, 161, 162, false, 61, false,
              {0x8219E040, 0x8219E19C, 0x8219E1A8, 0x8219E1B4, 0x8219E1E0}, ChangedBuild(base),
              calls);
}

uint32_t BuildJumpBuildB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219E348 BuildScreen::JumpBuildB");
  return Jump(base, sp, 163, 164, false, 61, false,
              {0x8219E3E0, 0x8219E4C8, 0x8219E4D4, 0x8219E4E0, 0x8219E50C}, KindNot2(base), calls);
}

uint32_t BuildJumpStandA(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219E600 BuildScreen::JumpStandA");
  return Jump(base, sp, 161, 162, false, 93, true,
              {0x8219E694, 0x8219E7F0, 0x8219E7FC, 0x8219E808, 0x8219E834}, ChangedBuild(base),
              calls);
}

uint32_t BuildJumpStandB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219E9A0 BuildScreen::JumpStandB");
  return Jump(base, sp, 163, 164, false, 93, true,
              {0x8219EA34, 0x8219EB1C, 0x8219EB28, 0x8219EB34, 0x8219EB60}, KindNot2(base), calls);
}

uint64_t BuildGroupsPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219EEC0 BuildScreen::GroupsPage");
  const uint32_t frame = Frame(base, sp, 96);
  ListCall(calls, kListUpdate, List(base), frame, 0x8219EEDC);
  if (!Still(base, List(base)) || BuildJumpGroupA(base, frame, calls) != 0 ||
      BuildJumpGroupB(base, frame, calls) != 0) {
    PagesGo(base, Load(base, kBuildPages), 0, 3, kNone);
    return Sound(calls, frame, 0x8219F030, 4);
  }
  uint64_t r3 = ListCall(calls, kListAccepted, List(base), frame, 0x8219EF1C);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x8219EF2C, 0);
    PagesGo(base, Load(base, kBuildPages), 1, 6, 7);
    return r3;
  }
  r3 = ListCall(calls, kListCancelled, List(base), frame, 0x8219EF70);
  if (uint32_t(r3) != 0) {
    r3 = Sound(calls, frame, 0x8219EF80, 2);
    PagesGo(base, Load(base, kBuildPages), 4, 5, kNone);
    return r3;
  }
  if (Load(base, List(base) + 28) == 0) {
    return r3;
  }
  r3 = Sound(calls, frame, 0x8219EFD0, 4);
  PagesGo(base, Load(base, kBuildPages), 0, 4, kNone);
  return r3;
}

uint64_t BuildBuildsPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219F048 BuildScreen::BuildsPage");
  return LevelPageRun(
      base, sp,
      {1, 8, 9, 11, 12, 10, 0x8219F068, 0x8219F074, 0x8219F0E4, 0x8219F134, 0x8219F1AC},
      [&](uint32_t frame) { return BuildJumpBuildA(base, frame, calls); },
      [&](uint32_t frame) { return BuildJumpBuildB(base, frame, calls); }, [](uint32_t) {},
      calls);
}

uint64_t BuildBuildPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8219F1C8 BuildScreen::BuildPage");
  return LevelPageRun(
      base, sp,
      {2, 14, 15, 17, 18, 16, 0x8219F1E8, 0x8219F1F4, 0x8219F264, 0x8219F30C, 0x8219F384},
      [&](uint32_t frame) { return BuildJumpStandA(base, frame, calls); },
      [&](uint32_t frame) { return BuildJumpStandB(base, frame, calls); },
      [&](uint32_t) {
        // The selected build seen: its +36 = its +32.
        const uint32_t list = List(base);
        const uint32_t position = race_screens::ListSelection(base, list) + Load(base, list + 4);
        const uint32_t build = BuildAt(base, position);
        Store(base, build + 36, Load(base, build + 32));
      },
      calls);
}

}  // namespace type2_screens
