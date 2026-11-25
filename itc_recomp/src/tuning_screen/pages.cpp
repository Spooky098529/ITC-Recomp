// The tuning screen's movie calls (docs/functions/tuning-screen.md): see
// tuning_screen.h. The UI movie calls (UiMovie::Play, Loop, Finish,
// FinishNamed, PlayNamed, LoopNamed, SetText, ScrollText) through their overrides from each original's frame with its
// return addresses; SetText's unset varargs passed as 0 (the user's call,
// 2026-10-30).

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "camera_modes/steps.h"
#include "game_modes/game_modes.h"
#include "guest.h"
#include "schema/game_modes.h"
#include "schema/ui_movies_game.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"

namespace tuning_screen {

namespace {

uint32_t Load(uint32_t address) { return guest::LoadU32(address); }
void Store(uint32_t address, uint32_t value) {
  guest::StoreU32(address, value);
}

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  Store(sp - bytes, sp);
  return sp - bytes;
}

namespace steps = camera_modes::steps;

constexpr uint32_t kAll = 0xFFFFFFFFu;  // sub-scene -1: the scene itself

// std: a doubleword, big-endian.
void Store64(uint32_t address, uint64_t value) {
  Store(address, uint32_t(value >> 32));
  Store(address + 4, uint32_t(value));
}

}  // namespace

// The dialogs' message: the count at ring + 600 (the dispatcher's +804)
// incremented, then the record (start +604 + count - 1) mod 50 of 12 bytes
// = (event, 0, 0), stored +8, +4, +0.
void PostDialogMessage(uint32_t dispatcher, uint32_t event) {
  const schema::TuningDispatcher d(dispatcher);
  const uint32_t count = d.message_count() + 1;
  const uint32_t index = d.message_first() + count - 1;
  d.set_message_count(count);
  const game_modes::RingSlot record{d.messages() + (index - (index / 50) * 50) * 12};  // E5 step 4
  record.set_arg1(0);
  record.set_arg0(0);
  record.set_id(event);
}

// The two dialogs' texts on movie 16: string `title` into (-1, 0), 2 into
// (1, 3), 3 into (1, 4), from the return addresses `lr`; then the movie's
// +40960 = 0. The last SetText's r3.
uint64_t DialogTexts(const GuestCall& call, uint32_t frame, uint32_t entry,
                     uint32_t title, const uint32_t (&lr)[3]) {
  const uint32_t movie = Load(entry);
  SetText(call, frame, lr[0], movie, kAll, 0, title, 0);
  SetText(call, frame, lr[1], movie, 1, 3, 2, 0);
  const uint64_t r3 = SetText(call, frame, lr[2], movie, 1, 4, 3, 0);
  Store(movie + 40960, 0);
  return r3;
}

namespace {

// Command22's and Command23's copy: by the set (+30724) 0 or 1, its six
// words 36 bytes apart (from +10580 or +11336) into the pointers of the
// movie's +28 table (+672 or +640 on; the table read again for each).
void CopySetWords(uint32_t screen, uint32_t movie, int32_t set) {
  auto copy = [&](uint32_t from, uint32_t to) {
    for (uint32_t k = 0; k < 6; k++) {
      const uint32_t textures = Load(schema::UiMovie(movie).textures_address());
      const uint32_t at = Load(textures + to + 4 * k);
      Store(at, Load(screen + from + 36 * k));
    }
  };
  if (set == 0) {
    copy(10580, 672);
  } else if (set == 1) {
    copy(11336, 640);
  }
}

}  // namespace

uint32_t MovieEntry(uint32_t frame, uint32_t id) {
  Store(frame + 80, id);
  return ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80);
}

uint64_t FinishNamed(const GuestCall& call, uint32_t frame, uint32_t lr, uint32_t movie,
                     uint64_t named) {
  return call(kFinishNamed, {movie, named}, frame, lr);
}

uint64_t PlayNamed(const GuestCall& call, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named, uint32_t chained) {
  return call(kPlayNamed, {movie, named, chained, 0, 0, 0}, frame, lr);
}

uint64_t LoopNamed(const GuestCall& call, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  return call(kLoopNamed, {movie, named}, frame, lr);
}

uint64_t SetText(const GuestCall& call, uint32_t frame, uint32_t lr, uint32_t movie,
                 uint32_t sub_scene, uint32_t child, uint32_t id, uint64_t argument) {
  return call(kSetText, {movie, sub_scene, child, id, argument, 0, 0, 0}, frame, lr);
}

void Advance(uint32_t screen) {
  const auto queue = TS(screen).queue();
  const uint32_t next = queue.next();
  if (int32_t(next) == -1) {
    const uint32_t first = queue.resume();
    queue.set_current(0xFFFFFFFFu);
    queue.set_page(first);
  } else {
    queue.set_step(0);
    queue.set_current(next);
    queue.set_next(0xFFFFFFFFu);
  }
  queue.set_started(0);
}

uint64_t UpdateListCounter(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EED00 TuningScreen::UpdateListCounter");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t movie = Load(MovieEntry(frame, kMovieId));
  SetText(call, frame, 0x821EED50, movie, 1, 23, 8, uint64_t(TS(screen).list_cursor()) + 1);
  SetText(call, frame, 0x821EED64, movie, 1, 24, 449, 0);
  return SetText(call, frame, 0x821EED80, movie, 1, 25, 8,
                 uint64_t(TS(screen).list_last()) + 1);
}

uint64_t SetPartName(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F2068 TuningScreen::SetPartName");
  const uint32_t frame = Frame(sp, 848);
  const uint32_t movie = Load(MovieEntry(frame, kMovieId));
  const uint32_t table = frame + 96;
  Store(table, 376);
  for (uint32_t i = 1; i < 180; i++) {
    Store(table + 4 * i, 999 + i);
  }
  const uint64_t number =
      WheelNumber(TS(screen).part_group(), TS(screen).list_cursor());
  const uint32_t id = Load(uint32_t(number << 2) + table);
  return SetText(call, frame, 0x821F20F4, movie, 1, 20, id, 0);
}

uint64_t PlayGroupAnimation(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821EE880 TuningScreen::PlayGroupAnimation");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = MovieEntry(frame, kMovieId);
  const uint32_t movie = Load(entry);
  uint64_t r3 = entry;
  const uint32_t group = TS(screen).part_group();
  if (group <= 16) {
    uint32_t named = 68;
    uint32_t lr = 0x821EE930;
    switch (group) {
      case 0: case 7: case 13: named = 65; lr = 0x821EE8F4; break;
      case 8: case 14: named = 66; lr = 0x821EE908; break;
      case 11: named = 67; lr = 0x821EE91C; break;
      default: break;
    }
    FinishNamed(call, frame, lr, movie, named);
    r3 = PlayNamed(call, frame, 0x821EE94C, movie, named, 1);
  }
  if (int32_t(TS(screen).unknown_25208()) == 11) {
    const int32_t g = int32_t(TS(screen).part_group());
    if (g == 10 || g == 12) {
      FinishNamed(call, frame, 0x821EE978, movie, 71);
      r3 = PlayNamed(call, frame, 0x821EE994, movie, 71, 1);
    }
  }
  return r3;
}

uint64_t Command31(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82202880 TuningScreen::Command31");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = MovieEntry(frame, 13);
  const int32_t state = int32_t(TS(screen).state());
  const uint32_t movie = Load(entry);
  TS(screen).set_command_end_after_setup(1);
  if (state == 23) {
    FinishNamed(call, frame, 0x822028D4, movie, 220);
    return PlayNamed(call, frame, 0x8220290C, movie, 220, 1);
  }
  if (state == 24) {
    FinishNamed(call, frame, 0x822028F0, movie, 222);
    return PlayNamed(call, frame, 0x8220290C, movie, 222, 1);
  }
  return entry;
}

uint64_t Command9(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E9050 TuningScreen::Command9");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = MovieEntry(frame, kMovieId);
  uint64_t r3 = entry;
  const uint32_t movie = Load(entry);
  if (TS(screen).command_first_run() != 0) {
    FinishNamed(call, frame, 0x821E9098, movie, 61);
    r3 = PlayNamed(call, frame, 0x821E90B4, movie, 61, 1);
    if (TS(screen).command_end_after_setup() != 0) {
      TS(screen).set_command_end_after_setup(0);
    }
    TS(screen).set_command_first_run(0);
  }
  if (TS(screen).command_end_now() != 0) {
    Advance(screen);
  }
  return r3;
}

uint64_t ShowDialog(const GuestCall& call, uint64_t r3, uint32_t dispatcher,
                    uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D6230 TuningScreen::ShowDialog");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(sp, 128);
  const int32_t state = int32_t(TS(screen).state());
  TS(screen).set_pages_enabled(0);
  uint32_t title = 0;
  switch (state) {
    case 9: title = 14; break;
    case 11: title = 15; break;
    case 12: title = 18; break;
    case 13: title = 19; break;
    case 14: title = 16; break;
    default: break;
  }
  if (title != 0) {
    const uint32_t entry = MovieEntry(frame, 16);
    r3 = DialogTexts(call, frame, entry, title, {0x821D6364, 0x821D6378, 0x821D638C});
    PostDialogMessage(dispatcher, 34);
  } else if (state == 10) {
    const uint32_t movie = Load(MovieEntry(frame, 19));
    r3 = SetText(call, frame, 0x821D62AC, movie, kAll, 1, 23, 0);
    PostDialogMessage(dispatcher, 37);
  }
  Advance(screen);
  return r3;
}

uint64_t Command30(const GuestCall& call, uint64_t r3, uint32_t dispatcher,
                   uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82202750 TuningScreen::Command30");
  const uint32_t screen = uint32_t(r3);
  const uint32_t frame = Frame(sp, 128);
  const int32_t state = int32_t(TS(screen).state());
  TS(screen).set_pages_enabled(0);
  uint32_t title = 0;
  switch (state) {
    case 35: title = 18; break;
    case 36: title = 19; break;
    case 37: title = 16; break;
    default: return r3;
  }
  const uint32_t entry = MovieEntry(frame, 16);
  r3 = DialogTexts(call, frame, entry, title, {0x822027F4, 0x82202808, 0x8220281C});
  PostDialogMessage(dispatcher, 34);
  return r3;
}

uint64_t Command7(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821E8B68 TuningScreen::Command7");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = MovieEntry(frame, kMovieId);
  const uint32_t movie = Load(entry);
  auto wheel = [&] { return uint64_t(TS(screen).wheel_part_cursor()); };
  if (TS(screen).command_first_run() != 0) {
    if (TS(screen).command_end_after_setup() != 0) {
      FinishNamed(call, frame, 0x821E8BC0, movie, wheel() + 415);
      PlayNamed(call, frame, 0x821E8BE0, movie, wheel() + 415, 1);
      FinishNamed(call, frame, 0x821E8BEC, movie, 1008);
      PlayNamed(call, frame, 0x821E8C08, movie, 1008, 1);
      PlayNamed(call, frame, 0x821E8C24, movie, 1012, 0);
      FinishNamed(call, frame, 0x821E8C30, movie, 1012);
      TS(screen).set_command_end_after_setup(0);
    } else {
      FinishNamed(call, frame, 0x821E8C40, movie, 964);
      PlayNamed(call, frame, 0x821E8C5C, movie, 964, 1);
    }
    TS(screen).set_command_first_run(0);
  }
  const uint32_t node = ui_movies::FindNode(movie, 27, 0);
  // mulli then extsw: the product's low word, signed; std, lfd, fcfid,
  // frsp, stfs.
  const int64_t x = int32_t(uint32_t(wheel() * 40));
  Store64(frame + 80, uint64_t(x));
  Store(UiNode(node).y_address(), steps::Bits(steps::Single(double(x))));
  if (TS(screen).command_end_now() != 0) {
    Advance(screen);
    TS(screen).set_command_end_now(0);
  }
  return node;
}

uint64_t Command23(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82202368 TuningScreen::Command23");
  const uint32_t frame = Frame(sp, 112);
  const uint32_t entry = MovieEntry(frame, 13);
  const int32_t set = int32_t(TS(screen).editor_page());
  const uint32_t movie = Load(entry);
  CopySetWords(screen, movie, set);
  uint64_t r3 = entry;
  const int32_t pick = int32_t(TS(screen).scroll_direction());
  if (pick == 2 || pick == 1) {
    const int32_t now = int32_t(TS(screen).editor_page());
    uint32_t named = 0, lr = 0;
    if (pick == 2 && now == 0) {
      named = 165; lr = 0x82202494;
    } else if (pick == 2 && now == 1) {
      named = 160; lr = 0x822024B0;
    } else if (pick == 1 && now == 0) {
      named = 164; lr = 0x822024D8;
    } else if (pick == 1 && now == 1) {
      named = 159; lr = 0x822024F4;
    }
    if (named != 0) {
      FinishNamed(call, frame, lr, movie, named);
      r3 = PlayNamed(call, frame, 0x82202510, movie, named, 1);
    }
  }
  TS(screen).set_scroll_direction(0);
  return r3;
}

uint64_t Command18(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82200600 TuningScreen::Command18");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = MovieEntry(frame, 13);
  const uint32_t movie = Load(entry);
  if (TS(screen).command_first_run() != 0) {
    if (TS(screen).command_end_after_setup() != 0) {
      Advance(screen);
      TS(screen).set_command_end_after_setup(0);
    } else {
      const uint32_t slot = uint32_t(TS(screen).history_depth());
      const bool is_28 = int32_t(Load(screen + 4 * (slot + 2438))) == 28;
      const uint32_t named = is_28 ? 225 : 239;
      FinishNamed(call, frame, is_28 ? 0x822006A8 : 0x822006B8, movie, named);
      PlayNamed(call, frame, 0x822006D4, movie, named, 1);
    }
    TS(screen).set_command_first_run(0);
  }
  const uint32_t node = ui_movies::Child(ui_movies::FindNode(movie, 11, 1), 0);
  // extsw, std, lfd, fcfid, frsp; fmuls by the float at 0x82001FB0 (40.0).
  const int64_t x = int32_t(TS(screen).edit_cursor());
  Store64(frame + 80, uint64_t(x));
  const double scaled = steps::Fmuls(steps::Single(double(x)), steps::Lfs(0x82001FB0));
  Store(UiNode(node).y_address(), steps::Bits(scaled));
  auto named = [&] { return uint64_t(TS(screen).edit_cursor()) + 130; };
  FinishNamed(call, frame, 0x82200728, movie, named());
  PlayNamed(call, frame, 0x82200748, movie, named(), 1);
  return LoopNamed(call, frame, 0x82200758, movie, named());
}

uint64_t Command25(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82204888 TuningScreen::Command25");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = MovieEntry(frame, 13);
  const uint32_t movie = Load(entry);
  if (TS(screen).command_first_run() != 0) {
    if (TS(screen).command_end_after_setup() != 0) {
      Advance(screen);
      TS(screen).set_command_end_after_setup(0);
    } else {
      FinishNamed(call, frame, 0x82204918, movie, 227);
      PlayNamed(call, frame, 0x82204934, movie, 227, 1);
    }
    TS(screen).set_command_first_run(0);
  }
  PlaceStateNodes(screen, frame);
  call(kResetTextScroll, {movie, 2, 0}, frame, 0x82204950);
  SetText(call, frame, 0x82204964, movie, 2, 0, 820, 0);
  return call(kScrollText, {movie, 2, 0, 1}, frame, 0x82204978);
}

uint64_t Command8(const GuestCall& call, uint64_t r3, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821F5F70 TuningScreen::Command8");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t screen = uint32_t(r3);
  const uint32_t movie = Load(MovieEntry(frame, kMovieId));
  if (TS(screen).command_first_run() != 0) {
    FinishNamed(call, frame, 0x821F5FB8, movie, 1007);
    PlayNamed(call, frame, 0x821F5FD4, movie, 1007, 1);
    FinishNamed(call, frame, 0x821F5FE0, movie, 1011);
    PlayNamed(call, frame, 0x821F5FFC, movie, 1011, 1);
    if (TS(screen).command_end_after_setup() != 0) {
      TS(screen).set_command_end_after_setup(0);
    }
    TS(screen).set_command_first_run(0);
  }
  FinishNamed(call, frame, 0x821F601C, movie, 70);
  PlayNamed(call, frame, 0x821F6038, movie, 70, 1);
  LoopNamed(call, frame, 0x821F6044, movie, 70);
  FinishNamed(call, frame, 0x821F6050, movie, 69);
  PlayNamed(call, frame, 0x821F606C, movie, 69, 1);
  LoopNamed(call, frame, 0x821F6078, movie, 69);
  PlayGroupAnimation(call, screen, frame);
  SetPartName(call, screen, frame);
  const uint64_t rows = FillPartRows(r3, frame);
  if (TS(screen).command_end_now() != 0) {
    Advance(screen);
    TS(screen).set_command_end_now(0);
  }
  return rows;
}

uint64_t PlayIntro(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D06B0 TuningScreen::PlayIntro");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = MovieEntry(frame, kMovieId);
  const uint32_t movie = Load(entry);
  if (TS(screen).command_first_run() == 0) {
    return entry;
  }
  call(kFinish, {movie, 9, 0}, frame, 0x821D06F8);
  call(kPlay, {movie, 9, 0, 1, 0, 0, 0}, frame, 0x821D0718);
  uint64_t r3 = call(kLoop, {movie, 9, 0}, frame, 0x821D0728);
  if (TS(screen).command_end_after_setup() != 0) {
    call(kPlay, {movie, kAllScenes, 0, 0, 0, 0, 0}, frame, 0x821D0758);
    r3 = call(kFinish, {movie, kAllScenes, 0}, frame, 0x821D0768);
    Advance(screen);
    TS(screen).set_command_end_after_setup(0);
  } else {
    call(kFinish, {movie, kAllScenes, 0}, frame, 0x821D07A8);
    r3 = call(kPlay, {movie, kAllScenes, 0, 0, 0, 0, 0}, frame, 0x821D07C8);
  }
  TS(screen).set_command_first_run(0);
  return r3;
}

namespace {

// OpenItems' +9716 handling for animation `index` of the scene: with +9720
// the animation snapped to its end, 1012 and `named` (when not 0) played
// and finished, the queue's step, +9720 = 0; else it restarted; +9716 = 0.
// The return addresses from `lr` in call order. The last call's r3.
uint64_t SnapOrRestart(const GuestCall& call, uint32_t screen, uint32_t frame,
                       uint32_t movie, uint32_t index, uint32_t named,
                       std::initializer_list<uint32_t> lr) {
  const uint32_t* at = lr.begin();
  uint64_t r3;
  if (TS(screen).command_end_after_setup() != 0) {
    call(kPlay, {movie, kAllScenes, index, 0, 0, 0, 0}, frame, at[0]);
    call(kFinish, {movie, kAllScenes, index}, frame, at[1]);
    PlayNamed(call, frame, at[2], movie, 1012, 0);
    r3 = FinishNamed(call, frame, at[3], movie, 1012);
    if (named != 0) {
      PlayNamed(call, frame, at[4], movie, named, 0);
      r3 = FinishNamed(call, frame, at[5], movie, named);
    }
    Advance(screen);
    TS(screen).set_command_end_after_setup(0);
  } else {
    const uint32_t* restart = at + (named != 0 ? 6 : 4);
    call(kFinish, {movie, kAllScenes, index}, frame, restart[0]);
    r3 = call(kPlay, {movie, kAllScenes, index, 1, 0, 0, 0}, frame, restart[1]);
  }
  TS(screen).set_command_first_run(0);
  return r3;
}

}  // namespace

uint64_t OpenItems(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821D0A48 TuningScreen::OpenItems");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = MovieEntry(frame, kMovieId);
  const uint32_t movie = Load(entry);
  if (int32_t(TS(screen).category()) == 4) {
    if (TS(screen).command_first_run() != 0) {
      SnapOrRestart(call, screen, frame, movie, 2, 0,
                    {0x821D0ABC, 0x821D0ACC, 0x821D0AE8, 0x821D0AF4, 0x821D0B34, 0x821D0B54});
    }
    FinishNamed(call, frame, 0x821D0B64, movie, 976);
    PlayNamed(call, frame, 0x821D0B80, movie, 976, 1);
    FinishNamed(call, frame, 0x821D0B8C, movie, 1013);
    PlayNamed(call, frame, 0x821D0BA8, movie, 1013, 1);
    return LoopNamed(call, frame, 0x821D0BB4, movie, 1013);
  }
  if (TS(screen).command_first_run() == 0) {
    return entry;
  }
  return SnapOrRestart(call, screen, frame, movie, 1, 1,
                       {0x821D0BF0, 0x821D0C00, 0x821D0C1C, 0x821D0C28, 0x821D0C44, 0x821D0C50,
                        0x821D0C90, 0x821D0CB0});
}

namespace {

// Command15's and Command22's text: (2, 0)'s scroll reset, string `id`,
// scrolled. ScrollText's r3.
uint64_t ScrolledText(const GuestCall& call, uint32_t frame, uint32_t movie, uint32_t id,
                      const uint32_t (&lr)[3]) {
  call(kResetTextScroll, {movie, 2, 0}, frame, lr[0]);
  SetText(call, frame, lr[1], movie, 2, 0, id, 0);
  return call(kScrollText, {movie, 2, 0, 1}, frame, lr[2]);
}

}  // namespace

uint64_t Command15(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_821FF1D0 TuningScreen::Command15");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = MovieEntry(frame, 13);
  const uint32_t movie = Load(entry);
  if (TS(screen).command_first_run() != 0) {
    if (TS(screen).command_end_after_setup() != 0) {
      Advance(screen);
      TS(screen).set_command_end_after_setup(0);
    } else {
      FinishNamed(call, frame, 0x821FF260, movie, 218);
      PlayNamed(call, frame, 0x821FF27C, movie, 218, 1);
      FinishNamed(call, frame, 0x821FF288, movie, 1);
      PlayNamed(call, frame, 0x821FF2A4, movie, 1, 1);
      LoopNamed(call, frame, 0x821FF2B0, movie, 1);
      FinishNamed(call, frame, 0x821FF2BC, movie, 153);
      PlayNamed(call, frame, 0x821FF2D8, movie, 153, 1);
      LoopNamed(call, frame, 0x821FF2E4, movie, 153);
    }
    TS(screen).set_command_first_run(0);
  }
  const uint32_t node = ui_movies::FindNode(movie, 3, 0);
  // extsw, std, lfd, fcfid, frsp; fmuls by the float at 0x82001FB0 (40.0).
  const int64_t x = int32_t(TS(screen).editor_page());
  Store64(frame + 80, uint64_t(x));
  const double scaled = steps::Fmuls(steps::Single(double(x)), steps::Lfs(0x82001FB0));
  Store(UiNode(node).y_address(), steps::Bits(scaled));
  auto named = [&] { return uint64_t(TS(screen).editor_page()) + 6; };
  FinishNamed(call, frame, 0x821FF330, movie, named());
  PlayNamed(call, frame, 0x821FF350, movie, named(), 1);
  LoopNamed(call, frame, 0x821FF360, movie, named());
  return ScrolledText(call, frame, movie, 804, {0x821FF370, 0x821FF384, 0x821FF398});
}

uint64_t Command22(const GuestCall& call, uint32_t screen, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82202090 TuningScreen::Command22");
  const uint32_t frame = Frame(sp, 128);
  const uint32_t entry = MovieEntry(frame, 13);
  const uint32_t movie = Load(entry);
  if (TS(screen).command_first_run() != 0) {
    if (TS(screen).command_end_after_setup() != 0) {
      Advance(screen);
      TS(screen).set_command_end_after_setup(0);
    } else {
      // The word at +31192 + 4 × (3 × set + +30732), the sum's low word.
      const uint32_t index =
          TS(screen).editor_page() * 3 + TS(screen).layer_cursor() + 7798;
      const bool none = int32_t(Load(screen + (index << 2))) == -1;
      const uint32_t named = none ? 233 : 223;
      FinishNamed(call, frame, none ? 0x82202144 : 0x82202154, movie, named);
      PlayNamed(call, frame, 0x82202170, movie, named, 1);
    }
    TS(screen).set_command_first_run(0);
  }
  const int32_t set = int32_t(TS(screen).editor_page());
  if (set == 0 || set == 1) {
    // 167 then 166, or 162 then 161, restarted and looped.
    const uint32_t first = set == 0 ? 167 : 162;
    const uint32_t b = set == 0 ? 166 : 161;
    FinishNamed(call, frame, set == 0 ? 0x8220218C : 0x822021F8, movie, first);
    PlayNamed(call, frame, set == 0 ? 0x822021A8 : 0x82202214, movie, first, 1);
    LoopNamed(call, frame, set == 0 ? 0x822021B4 : 0x82202220, movie, first);
    FinishNamed(call, frame, set == 0 ? 0x822021C0 : 0x8220222C, movie, b);
    PlayNamed(call, frame, set == 0 ? 0x822021DC : 0x82202248, movie, b, 1);
    LoopNamed(call, frame, 0x82202254, movie, b);
  }
  CopySetWords(screen, movie, int32_t(TS(screen).editor_page()));
  return ScrolledText(call, frame, movie, 811, {0x82202338, 0x8220234C, 0x82202360});
}

}  // namespace tuning_screen
