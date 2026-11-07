// The tuning screen's page set-ups for the category bar and the level, type
// and option lists (docs/functions/tuning-screen.md): see tuning_screen.h.
// The UI movie calls (UiMovie::Play, Loop, Finish, FinishNamed, PlayNamed,
// LoopNamed, SetText, ScrollText and sub_8211C5F8) through their overrides
// from each original's frame with its return addresses; movie 2's nodes
// through the UI movies' plain C++ (FindNode, Child); SetText's unset varargs
// passed as 0 (the user's call, 2026-10-30).

#include "tuning_screen/tuning_screen.h"

#include "action_mappers/action_mappers.h"
#include "base/log.h"
#include "camera_modes/steps.h"
#include "debug/pause_menu_log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
uint32_t Byte(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  Store(base, sp - bytes, sp);
  return sp - bytes;
}

namespace steps = camera_modes::steps;

// The ints made floats through the frame: the doubleword stored at frame +
// 80 (std), loaded (lfd), converted (fcfid, frsp) and multiplied by the
// 40.0 at 0x82001FB0 (fmuls); the float's bits for the stfs.
uint32_t Scaled(uint8_t* base, uint32_t frame, int64_t x) {
  Store(base, frame + 80, uint32_t(uint64_t(x) >> 32));
  Store(base, frame + 84, uint32_t(x));
  return steps::Bits(steps::Fmuls(steps::Single(double(x)), steps::Lfs(base, 0x82001FB0)));
}

// lwa: a word, signed. lbz: a byte.
int64_t Signed(uint8_t* base, uint32_t address) { return int32_t(Load(base, address)); }
int64_t Unsigned8(uint8_t* base, uint32_t address) { return Byte(base, address); }

// The animation's flags after a +9716 set-up: +9720 = 0 when set, +9716 = 0.
void ClearFlags(uint8_t* base, uint32_t screen) {
  if (Load(base, screen + 9720) != 0) {
    Store(base, screen + 9720, 0);
  }
  Store(base, screen + 9716, 0);
}

// The queue's step with +9724, then +9724 = 0.
void StepWith9724(uint8_t* base, uint32_t screen) {
  if (Load(base, screen + 9724) != 0) {
    Advance(base, screen);
    Store(base, screen + 9724, 0);
  }
}

// The player's action mapper entry (0x826BAB80 + 8 × [0x826BABA4]).
constexpr uint32_t kMapperEntries = 0x826BAB80;
constexpr uint32_t kControllerEntries = 0x826BABA4;
constexpr uint32_t kPressedSlot = 0x8210BF40;  // ActionMapper::Pressed

// The mapper's slot 10 (+40) on `action`, as the original's bctrl from
// `frame` returning to `lr`: ActionMapper::Pressed's plain C++ when the
// vtable holds it and `native_slots` (logged for --log_pause_menu as its
// adapter does), else the slot through `call`. Its r3.
uint64_t PressedSlot(uint8_t* base, const GuestCall& call, uint32_t mapper, uint64_t action,
                     uint32_t frame, uint32_t lr, bool native_slots) {
  const uint32_t function = Load(base, Load(base, mapper) + 40);
  if (native_slots && function == kPressedSlot) {
    const uint32_t id = uint32_t(action);
    const uint32_t r3 = action_mappers::Pressed(base, mapper, id);
    debug::PauseMenuLog_NoteSlotRead(10, mapper, id, lr, double(r3));
    return r3;
  }
  return call(function, {mapper, action}, frame, lr);
}

}  // namespace

uint64_t MoveCategory(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp,
                      bool native_slots) {
  ITC_NATIVE_RUNNING("sub_821D07D8 TuningScreen::MoveCategory");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t mappers = Load(base, kControllerEntries) * 8 + kMapperEntries;
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const uint32_t menu = Load(base, screen + 10040);
  const uint32_t movie = Load(base, entry);
  const int32_t step = int32_t(Load(base, menu + 8));
  auto category = [&] { return uint64_t(Load(base, screen + kCategory)); };
  auto finish = [&](uint64_t index, uint32_t lr) {
    return call(kFinish, {movie, 27, index}, frame, lr);
  };
  if (step == -1 || step == 1) {
    const bool wrapped = Load(base, menu + 40) != 0;
    uint64_t first;
    if (wrapped) {
      // The bar snapped to the category: node (27, 0)'s +16 = float(+9656)
      // × 40.0.
      finish(12, step == -1 ? 0x821D0848 : 0x821D08EC);
      finish(13, step == -1 ? 0x821D0858 : 0x821D08FC);
      const uint32_t node = ui_movies::FindNode(base, movie, 27, 0);
      Store(base, node + 16, Scaled(base, frame, Signed(base, screen + kCategory)));
      first = step == -1 ? 8 : 2;
    } else {
      // The bar slid one way: 12 (down) or 13 (up) restarted.
      const uint64_t slide = step == -1 ? 12 : 13;
      finish(slide, step == -1 ? 0x821D089C : 0x821D0928);
      call(kPlay, {movie, 27, slide, 1, 0, 0, 0}, frame, step == -1 ? 0x821D08BC : 0x821D0948);
      first = category() + (step == -1 ? 3 : 1);
    }
    finish(first, 0x821D095C);
    finish(category() + 2, 0x821D0970);
    call(kPlay, {movie, 27, category() + 2, 1, 0, 0, 0}, frame, 0x821D0994);
    return call(kLoop, {movie, 27, category() + 2}, frame, 0x821D09A8);
  }
  // No step: with action 159 pressed the category animations 2 to 13
  // finished; else action 158 asked (its answer not used). The entry's +4
  // and +0 read again on each try.
  uint64_t r3 = entry;
  if (int32_t(Load(base, mappers + 4)) != 1) {
    r3 = PressedSlot(base, call, Load(base, mappers), 159, frame, 0x821D09D0, native_slots);
    if ((r3 & 0xFF) != 0) {
      for (uint64_t k = 0; k < 10; k++) {
        finish(k + 2, 0x821D09EC);
      }
      finish(12, 0x821D0A08);
      return finish(13, 0x821D0A18);
    }
  }
  if (int32_t(Load(base, mappers + 4)) != 1) {
    r3 = PressedSlot(base, call, Load(base, mappers), 158, frame, 0x821D0A40, native_slots);
  }
  return r3;
}

uint64_t MarkPick(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D6420 TuningScreen::MarkPick");
  const uint32_t frame = Frame(base, sp, 144);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const int32_t state = int32_t(Load(base, screen + kState));
  const uint32_t movie = Load(base, entry);
  Store(base, screen + 9720, 1);
  uint64_t r3 = entry;
  // The row of `field`'s byte k on sub-scene n: Child(FindNode(n, 1), k + 5)
  // and FindNode(n, 0)'s children k + 15 and k + 26, their +16 = float(k) ×
  // 40.0 (the byte read again for each).
  auto row = [&](uint32_t n, uint32_t field) {
    const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, n, 1),
                                           Byte(base, screen + field) + 5);
    Store(base, mark + 16, Scaled(base, frame, Unsigned8(base, screen + field)));
    const uint32_t node = ui_movies::FindNode(base, movie, n, 0);
    const uint32_t a = ui_movies::Child(base, node, Byte(base, screen + field) + 15);
    Store(base, a + 16, Scaled(base, frame, Unsigned8(base, screen + field)));
    const uint32_t b = ui_movies::Child(base, node, Byte(base, screen + field) + 26);
    Store(base, b + 16, Scaled(base, frame, Unsigned8(base, screen + field)));
  };
  auto restart = [&](uint64_t named, uint32_t finish_lr, uint32_t play_lr, uint32_t chained) {
    FinishNamed(call, frame, finish_lr, movie, named);
    return PlayNamed(call, frame, play_lr, movie, named, chained);
  };
  bool marked = false;  // 486 restarted
  if (state == 4) {
    const int32_t category = int32_t(Load(base, screen + kCategory));
    auto item = [&] { return Byte(base, screen + kItem); };
    if ((category == 0 && item() == 2) || (category == 1 && item() == 3)) {
      row(28, kLevel);
      marked = true;
    } else if (category == 3) {
      const uint32_t node = ui_movies::FindNode(base, movie, 27, item() + 5);
      Store(base, node + 16, Scaled(base, frame, Unsigned8(base, screen + kItem)));
      row(30, kLevel);
      r3 = restart(629, 0x821D6754, 0x821D6958, 1);
    } else if (category == 4 && item() == 4) {
      row(28, kType);
      marked = true;
    } else if (category == 5 && item() == 4) {
      restart(981, 0x821D6834, 0x821D6850, 1);
      r3 = restart(1010, 0x821D685C, 0x821D6958, 0);
    } else if (category == 6 && item() == 0) {
      row(28, kType);
      marked = true;
    }
  } else if (state == 53) {
    // k = [+33548]; 3 puts the marks at 80.0 (0x82001E8C).
    const uint32_t k_at = screen + 33548;
    const uint32_t far = steps::Bits(steps::Lfs(base, 0x82001E8C));
    if (int32_t(Load(base, k_at)) == 3) {
      const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 1), 7);
      Store(base, mark + 16, far);
    } else {
      const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 1),
                                             Load(base, k_at) + 5);
      Store(base, mark + 16, Scaled(base, frame, Signed(base, k_at)));
    }
    const uint32_t node = ui_movies::FindNode(base, movie, 28, 0);
    if (int32_t(Load(base, k_at)) == 3) {
      Store(base, ui_movies::Child(base, node, 17) + 16, far);
      Store(base, ui_movies::Child(base, node, 28) + 16, far);
    } else {
      const uint32_t a = ui_movies::Child(base, node, Load(base, k_at) + 15);
      Store(base, a + 16, Scaled(base, frame, Signed(base, k_at)));
      const uint32_t b = ui_movies::Child(base, node, Load(base, k_at) + 26);
      Store(base, b + 16, Scaled(base, frame, Signed(base, k_at)));
    }
    marked = true;
  }
  if (marked) {
    r3 = restart(486, 0x821D693C, 0x821D6958, 1);
  }
  Advance(base, screen);
  return r3;
}

uint64_t Command16(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FF6F0 TuningScreen::Command16");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t entry = MovieEntry(base, frame, 13);
  const uint32_t movie = Load(base, entry);
  if (Load(base, screen + 9716) != 0) {
    if (Load(base, screen + 9720) != 0) {
      PlayNamed(call, frame, 0x821FF754, movie, 219, 1);
      FinishNamed(call, frame, 0x821FF760, movie, 219);
      Advance(base, screen);
      Store(base, screen + 9720, 0);
    } else {
      FinishNamed(call, frame, 0x821FF7A0, movie, 219);
      PlayNamed(call, frame, 0x821FF7BC, movie, 219, 1);
    }
    Store(base, screen + 9716, 0);
  }
  const uint32_t pick = Load(base, screen + 30732);
  if (pick <= 2) {
    // 195 + pick (169 + pick with the set +30724 not 0) restarted and
    // looped, 44 and 45 finished, 46 restarted.
    struct Lr {
      uint32_t finish, play;
    };
    static constexpr Lr kLr[3][2] = {
        {{0x821FFA4C, 0x821FFA68}, {0x821FFA78, 0x821FFA94}},
        {{0x821FF9E4, 0x821FFA00}, {0x821FFA10, 0x821FFA2C}},
        {{0x821FF97C, 0x821FF998}, {0x821FF9A8, 0x821FF9C4}},
    };
    const bool other = int32_t(Load(base, screen + 30724)) != 0;
    const uint64_t named = (other ? 169 : 195) + pick;
    const Lr& lr = kLr[pick][other ? 1 : 0];
    FinishNamed(call, frame, lr.finish, movie, named);
    PlayNamed(call, frame, lr.play, movie, named, 1);
    LoopNamed(call, frame, 0x821FFAA0, movie, named);
    FinishNamed(call, frame, 0x821FFAAC, movie, 44);
    FinishNamed(call, frame, 0x821FFAB8, movie, 45);
    FinishNamed(call, frame, 0x821FFAC4, movie, 46);
    PlayNamed(call, frame, 0x821FFAE0, movie, 46, 1);
  } else if (pick <= 4) {
    // 195 to 197 and 213 (169 to 171 and 189) finished, 213 (189) played,
    // 44 (pick 3) or 45 (4) restarted and looped.
    const bool other = int32_t(Load(base, screen + 30724)) != 0;
    static constexpr uint32_t kFinishLr[2][2][4] = {
        {{0x821FF8B8, 0x821FF8C4, 0x821FF8D0, 0x821FF8DC},
         {0x821FF8EC, 0x821FF8F8, 0x821FF904, 0x821FF910}},
        {{0x821FF7FC, 0x821FF808, 0x821FF814, 0x821FF820},
         {0x821FF830, 0x821FF83C, 0x821FF848, 0x821FF854}},
    };
    const uint32_t (&lr)[4] = kFinishLr[pick - 3][other ? 1 : 0];
    const uint64_t first = other ? 169 : 195, last = other ? 189 : 213;
    for (uint64_t k = 0; k < 3; k++) {
      FinishNamed(call, frame, lr[k], movie, first + k);
    }
    FinishNamed(call, frame, lr[3], movie, last);
    const uint64_t bar = pick == 3 ? 44 : 45;
    PlayNamed(call, frame, pick == 3 ? 0x821FF92C : 0x821FF870, movie, last, 1);
    FinishNamed(call, frame, pick == 3 ? 0x821FF938 : 0x821FF87C, movie, bar);
    PlayNamed(call, frame, pick == 3 ? 0x821FF954 : 0x821FF898, movie, bar, 1);
    LoopNamed(call, frame, 0x821FF960, movie, bar);
  }
  call(kResetTextScroll, {movie, 2, 0}, frame, 0x821FFAF0);
  SetText(call, frame, 0x821FFB04, movie, 2, 0, 805, 0);
  return call(kScrollText, {movie, 2, 0, 1}, frame, 0x821FFB18);
}

namespace {

// The tire's pictures on FindNode(n, 0) (ShowLevels' n 30 with [+35948] and
// [+36076], ShowOptions' 33 with [+35952] and [+36080]): item 0, with the
// front's and the rear's the same, child 2's +16 = float(front) × 40.0 (and
// with `clear` children 11, 10 and 1's +80 = 0); else child 2's the same,
// child 11 shown, the node found again, child 1's +16 = float(rear) × 40.0
// and child 5 shown; item 1 child 2's front, child 11 shown; item 2 child
// 2's rear, child 10 shown. `match` the item-0 test's two pairs.
void TirePictures(uint8_t* base, uint32_t screen, uint32_t frame, uint32_t movie, uint32_t n,
                  uint32_t front, uint32_t rear, bool match, bool clear) {
  const uint32_t item = Byte(base, screen + kItem);
  if (item == 0) {
    const uint32_t node = ui_movies::FindNode(base, movie, n, 0);
    const uint32_t picture = ui_movies::Child(base, node, 2);
    Store(base, picture + 16, Scaled(base, frame, Signed(base, screen + front)));
    if (match) {
      if (clear) {
        Store(base, ui_movies::Child(base, node, 11) + 80, 0);
        Store(base, ui_movies::Child(base, node, 10) + 80, 0);
        Store(base, ui_movies::Child(base, node, 1) + 80, 0);
      }
      return;
    }
    Store(base, ui_movies::Child(base, node, 11) + 80, 1);
    const uint32_t again = ui_movies::FindNode(base, movie, n, 0);
    const uint32_t back = ui_movies::Child(base, again, 1);
    Store(base, back + 16, Scaled(base, frame, Signed(base, screen + rear)));
    Store(base, ui_movies::Child(base, again, 5) + 80, 1);
  } else if (item == 1 || item == 2) {
    const uint32_t node = ui_movies::FindNode(base, movie, n, 0);
    const uint32_t picture = ui_movies::Child(base, node, 2);
    Store(base, picture + 16,
          Scaled(base, frame, Signed(base, screen + (item == 1 ? front : rear))));
    Store(base, ui_movies::Child(base, node, item == 1 ? 11 : 10) + 80, 1);
  }
}

// FinishNamed(named), PlayNamed(named, chained), LoopNamed(named), from the
// return addresses `lr`. LoopNamed's r3.
uint64_t Restarted(const GuestCall& call, uint32_t frame, uint32_t movie, uint64_t named,
                   uint32_t chained, const uint32_t (&lr)[3]) {
  FinishNamed(call, frame, lr[0], movie, named);
  PlayNamed(call, frame, lr[1], movie, named, chained);
  return LoopNamed(call, frame, lr[2], movie, named);
}

}  // namespace

uint64_t ShowOptions(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D5028 TuningScreen::ShowOptions");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const int32_t category = int32_t(Load(base, screen + kCategory));
  const uint32_t movie = Load(base, entry);
  uint64_t r3 = entry;
  auto option = [&] { return uint64_t(Byte(base, screen + kOption)); };
  auto scene = [&](uint64_t index, uint32_t finish_lr, uint32_t play_lr) {
    call(kFinish, {movie, kAllScenes, index}, frame, finish_lr);
    call(kPlay, {movie, kAllScenes, index, 0, 0, 0, 0}, frame, play_lr);
  };
  if (category == 3) {
    if (Load(base, screen + 9716) != 0) {
      scene(30, 0x821D508C, 0x821D50AC);
      bool match = false;
      if (Byte(base, screen + kItem) == 0) {
        match = int32_t(Load(base, screen + 35952)) == int32_t(Load(base, screen + 36080)) &&
                int32_t(Load(base, screen + 35948)) == int32_t(Load(base, screen + 36076));
      }
      TirePictures(base, screen, frame, movie, 33, 35952, 36080, match, true);
      ClearFlags(base, screen);
    }
    const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, 33, 1), 0);
    Store(base, mark + 16, Scaled(base, frame, Unsigned8(base, screen + kOption)));
    Restarted(call, frame, movie, option() + 844, 1, {0x821D5310, 0x821D5330, 0x821D5340});
    r3 = Restarted(call, frame, movie, option() + 783, 0, {0x821D5350, 0x821D5370, 0x821D55BC});
  } else if (category == 4 && Byte(base, screen + kItem) == 4) {
    if (Load(base, screen + 9716) != 0) {
      scene(34, 0x821D53AC, 0x821D53CC);
      ClearFlags(base, screen);
    }
    const uint32_t mark = ui_movies::FindNode(base, movie, 31, 0);
    Store(base, mark + 16, Scaled(base, frame, Unsigned8(base, screen + kOption)));
    r3 = Restarted(call, frame, movie, option() + 675, 1, {0x821D5428, 0x821D5448, 0x821D55BC});
  } else if (category == 5 && Byte(base, screen + kItem) == 1) {
    if (Load(base, screen + 9716) != 0) {
      scene(6, 0x821D548C, 0x821D54AC);
      const uint32_t picture = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 0), 2);
      Store(base, picture + 80, 1);
      Store(base, picture + 16, Scaled(base, frame, Signed(base, screen + 36612)));
      ClearFlags(base, screen);
    }
    const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 1), 0);
    Store(base, mark + 16, Scaled(base, frame, Unsigned8(base, screen + kOption)));
    Restarted(call, frame, movie, option() + 489, 1, {0x821D554C, 0x821D556C, 0x821D557C});
    r3 = Restarted(call, frame, movie, option() + 428, 0, {0x821D558C, 0x821D55AC, 0x821D55BC});
  }
  StepWith9724(base, screen);
  return r3;
}

uint64_t ShowLevels(uint8_t* base, const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D12C0 TuningScreen::ShowLevels");
  const uint32_t frame = Frame(base, sp, 160);
  const uint32_t entry = MovieEntry(base, frame, kMovieId);
  const int32_t category = int32_t(Load(base, screen + kCategory));
  const uint32_t movie = Load(base, entry);
  uint64_t r3 = entry;
  auto level = [&] { return uint64_t(Byte(base, screen + kLevel)); };
  auto scene = [&](uint64_t index, uint32_t finish_lr, uint32_t play_lr) {
    call(kFinish, {movie, kAllScenes, index}, frame, finish_lr);
    call(kPlay, {movie, kAllScenes, index, 0, 0, 0, 0}, frame, play_lr);
  };
  if (category >= 0 && category <= 2) {
    // Power (0), power train and body (1, 2). The extended levels: the
    // category 0 item 0 (1 item 4) with the byte +9677 above 8 (the +9656
    // and bytes read again for each test).
    const int32_t want_category = category == 0 ? 0 : 1;
    const uint32_t want_item = category == 0 ? 0 : 4;
    auto picked = [&] {
      return int32_t(Load(base, screen + kCategory)) == want_category &&
             Byte(base, screen + kItem) == want_item;
    };
    auto extended = [&] { return picked() && Byte(base, screen + 9677) > 8; };
    if (Load(base, screen + 9716) != 0) {
      if (category == 0) {
        scene(5, 0x821D1324, 0x821D1344);
      } else {
        scene(6, 0x821D1794, 0x821D17B4);
      }
      const uint32_t picture = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 0), 2);
      Store(base, picture + 80, 1);
      int64_t x;
      if (extended()) {
        const uint32_t choice = GetCurrentChoice(base, screen);
        const uint32_t pick = Byte(base, screen + kLevel);
        x = int32_t(Load(base, screen + 25100) - pick + choice);
      } else {
        x = int32_t(GetCurrentChoice(base, screen));
      }
      Store(base, picture + 16, Scaled(base, frame, x));
      ClearFlags(base, screen);
    }
    const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, 28, 1), 0);
    const int64_t x =
        extended() ? Signed(base, screen + 25100) : Unsigned8(base, screen + kLevel);
    Store(base, mark + 16, Scaled(base, frame, x));
    if (extended()) {
      auto word = [&] { return uint64_t(Load(base, screen + 25100)); };
      FinishNamed(call, frame, 0x821D1488, movie, word() + 428);
      PlayNamed(call, frame, 0x821D14A8, movie, word() + 428, 1);
      LoopNamed(call, frame, 0x821D14B8, movie, word() + 428);
      FinishNamed(call, frame, 0x821D14C8, movie, word() + 489);
      PlayNamed(call, frame, 0x821D14E8, movie, word() + 489, 0);
      r3 = LoopNamed(call, frame, 0x821D195C, movie, word() + 489);
    } else {
      FinishNamed(call, frame, 0x821D18EC, movie, level() + 428);
      PlayNamed(call, frame, 0x821D190C, movie, level() + 428, 1);
      LoopNamed(call, frame, 0x821D191C, movie, level() + 428);
      FinishNamed(call, frame, 0x821D192C, movie, level() + 489);
      PlayNamed(call, frame, 0x821D194C, movie, level() + 489, 0);
      r3 = LoopNamed(call, frame, 0x821D195C, movie, level() + 489);
    }
  } else if (category == 3) {
    if (Load(base, screen + 9716) != 0) {
      scene(7, 0x821D152C, 0x821D154C);
      bool match = false;
      if (Byte(base, screen + kItem) == 0) {
        match = int32_t(Load(base, screen + 35948)) == int32_t(Load(base, screen + 36076));
      }
      TirePictures(base, screen, frame, movie, 30, 35948, 36076, match, false);
      ClearFlags(base, screen);
    }
    const uint32_t mark = ui_movies::Child(base, ui_movies::FindNode(base, movie, 30, 1), 0);
    Store(base, mark + 16, Scaled(base, frame, Unsigned8(base, screen + kLevel)));
    FinishNamed(call, frame, 0x821D1744, movie, level() + 632);
    PlayNamed(call, frame, 0x821D1764, movie, level() + 632, 1);
    r3 = LoopNamed(call, frame, 0x821D195C, movie, level() + 632);
  }
  if (Byte(base, screen + 9677) > 8) {
    FinishNamed(call, frame, 0x821D1974, movie, 976);
    PlayNamed(call, frame, 0x821D1990, movie, 976, 1);
    FinishNamed(call, frame, 0x821D199C, movie, 1013);
    PlayNamed(call, frame, 0x821D19B8, movie, 1013, 1);
    r3 = LoopNamed(call, frame, 0x821D19C4, movie, 1013);
  }
  StepWith9724(base, screen);
  return r3;
}

}  // namespace tuning_screen
