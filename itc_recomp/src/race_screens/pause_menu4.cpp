// Kind 4's pause menu (docs/functions/race-screens-unseen.md): see unseen.h.
// Read from the generated code; never seen in play.

#include "race_screens/unseen.h"

#include "base/log.h"
#include "guest.h"
#include "schema/menu_helpers.h"
#include "schema/race_event.h"
#include "schema/race_screens.h"
#include "schema/race_screens_unseen.h"
#include "schema/tuning_screen.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"


namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

using Globals = schema::RaceScreensUnseenGlobals;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t Movie(uint8_t* base) { return Globals::pause4_screen(base).movie().address; }
// The data, read again at each use.
schema::PauseMenu4Data Data(uint8_t* base) { return Globals::pause4_data(base); }
schema::MenuList ListView(uint8_t* base) { return Globals::pause4_list(base); }
uint32_t List(uint8_t* base) { return ListView(base).address; }
uint32_t Pages(uint8_t* base) { return Globals::pause4_pages(base).address; }
uint32_t Selection(uint8_t* base) { return ListSelection(base, List(base)); }
// MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
uint32_t Previous(uint8_t* base) { return ListView(base).selection_before(); }
// The kind's item `index` (six words a kind).
uint32_t Item(uint8_t* base, uint32_t kind, uint32_t index) {
  return guest::LoadU32(base, Globals::pause4_items_address() + ((kind * 6 + index) << 2));
}

// UiMovie::Play(movie, sub_scene, index, chained, 0, 0, 0) through its
// override.
uint64_t Play(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie, uint64_t sub_scene,
              uint64_t index, uint64_t chained = 0) {
  const uint64_t args[8] = {movie, sub_scene, index, chained, 0, 0, 0, 0};
  return calls.CallArgs(kPlay, frame, lr, args);
}

// UiMovie::PlayNamed(movie, named, 0, 0, 0, 0) through its override.
uint64_t PlayNamed(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie,
                   uint64_t named) {
  const uint64_t args[8] = {movie, named, 0, 0, 0, 0, 0, 0};
  return calls.CallArgs(kPlayNamed, frame, lr, args);
}

uint64_t Run(const Calls& calls, uint32_t frame, uint32_t lr, uint32_t movie) {
  return calls.Call(kRun, frame, lr, movie, 0);
}

// A transition's first call (the step word 0): `work(movie)`, then the word
// 1; then the movie run once. Run's r3.
template <typename Work>
uint64_t Transition(uint8_t* base, uint32_t step, uint32_t frame, uint32_t lr, const Calls& calls,
                    Work&& work) {
  const uint32_t movie = Movie(base);
  if (guest::LoadU32(base, step) == 0) {
    work(movie);
    guest::StoreU32(base, step, 1);
  }
  return Run(calls, frame, lr, movie);
}

// The volume rows (type 6): items 0 and 1 rows 1 and 2, item 2 row 4 with
// the data's +60 1, else 3.
uint64_t VolumeRow(uint8_t* base, uint32_t item) {
  if (item < 2) return uint64_t(item) + 1;
  return int32_t(Data(base).volume_toggle()) == 1 ? 4 : 3;
}

// A word's low byte, sign-extended (the sound options' setters' argument).
uint64_t LowByte(uint32_t word) { return uint64_t(int64_t(int8_t(word & 0xFF))); }

}  // namespace

// --- The helpers ---

uint32_t Pause4StepVolume(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210488 PauseMenu4::StepVolume");
  Frame(base, sp, 96);
  const int32_t side = int32_t(ListView(base).side());
  const uint32_t selection = Selection(base);
  if (side != -1 && side != 1) return 0;
  if (selection < 2) {
    // The music's edit for item 0, the effects' for item 1.
    auto edit = [&] {
      const schema::PauseMenu4Data data = Data(base);
      return selection == 0 ? data.music_edit() : data.effects_edit();
    };
    auto set_edit = [&](uint32_t value) {
      const schema::PauseMenu4Data data = Data(base);
      selection == 0 ? data.set_music_edit(value) : data.set_effects_edit(value);
    };
    set_edit(edit() + uint32_t(side));
    const int32_t value = int32_t(edit());
    if (side == -1 ? value < 0 : value > 15) {
      set_edit(side == -1 ? 0 : 15);
      return 0;
    }
    return 1;
  }
  if (selection != 2) return 0;
  const int32_t toggle = int32_t(Data(base).volume_toggle());
  if (side == -1 ? toggle != 1 : toggle != 0) return 0;
  Data(base).set_volume_toggle(side == -1 ? 0 : 1);
  return 1;
}

uint32_t Pause4LoadVolumes(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822107B8 PauseMenu4::LoadVolumes");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  // The options' bytes +28 and +29 (race-event.toml's SaveData names +28
  // effects_volume and +29 music_volume; this screen reads them the other way).
  const uint32_t music = uint32_t(int32_t(int8_t(schema::SaveData(base, save).effects_volume())));
  Data(base).set_music_volume(music);
  const uint32_t effects = uint32_t(int32_t(int8_t(schema::SaveData(base, save).music_volume())));
  Data(base).set_effects_volume(effects);
  Data(base).set_music_default(15);
  Data(base).set_effects_default(15);
  return save;
}

uint32_t Pause4SaveVolumes(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210820 PauseMenu4::SaveVolumes");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  // The options' bytes +28 and +29, as LoadVolumes reads them.
  schema::SaveData(base, save).set_effects_volume(uint8_t(Data(base).music_volume()));
  schema::SaveData(base, save).set_music_volume(uint8_t(Data(base).effects_volume()));
  return save;
}

uint64_t Pause4AnswerPage(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210910 PauseMenu4::AnswerPage");
  const uint32_t frame = Frame(base, sp, 96);
  Pause4SaveVolumes(base, frame);
  return Data(base).answer();
}

uint64_t Pause4ShowItems(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210360 PauseMenu4::ShowItems");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Movie(base);
  const uint32_t node = ui_movies::FindNode(base, movie, 4, 0);
  uint64_t r3 = node;
  // The group's child count and first child index, read as words.
  const schema::UiMovieNode group(base, node);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, group.height_address())); i++) {
    const uint32_t kind = Data(base).kind();
    const uint32_t item = Item(base, kind, uint32_t(i));
    const uint64_t child = uint64_t(guest::LoadU32(base, group.width_address()) + uint32_t(i));
    if (int32_t(item) != -1 && i < 6) {
      const uint64_t args[8] = {movie, 4, child, uint64_t(item) + 1989, 0, 0, 0, 0};
      r3 = calls.CallArgs(kSetText, frame, 0x822103E8, args);
    } else {
      const uint32_t hidden = ui_movies::FindNode(base, movie, 4, uint32_t(child));
      schema::UiMovieNode(base, hidden).set_visible(0);
      r3 = hidden;
    }
  }
  return r3;
}

uint32_t Pause4ColorItems(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210CD0 PauseMenu4::ColorItems");
  Frame(base, sp, 128);
  const uint32_t movie = Movie(base);
  const uint32_t node = ui_movies::FindNode(base, movie, 4, 0);
  uint32_t r3 = node;
  const schema::UiMovieNode group(base, node);
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, group.height_address())); i++) {
    const bool enabled = ListIsEnabled(base, List(base), uint32_t(i)) != 0;
    const uint32_t color[3] = {enabled ? 205u : 255u, enabled ? 235u : 255u, enabled ? 67u : 255u};
    for (uint32_t k = 0; k < 3; k++) {
      r3 = ui_movies::FindNode(base, movie, 4, guest::LoadU32(base, group.width_address()) + uint32_t(i));
      // red, green, blue
      guest::StoreU32(base, schema::UiMovieNode(base, r3).red_address() + 4 * k, color[k]);
    }
  }
  return r3;
}

uint64_t Pause4SetUpMain(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210C00 PauseMenu4::SetUpMain");
  const uint32_t frame = Frame(base, sp, 128);
  {
    Data(base).set_item_count(6);
    const uint32_t kind = Data(base).kind();
    for (uint32_t i = 0; i < 6; i++) {
      if (int32_t(Item(base, kind, i)) == -1) {
        Data(base).set_item_count(i);
        break;
      }
    }
  }
  const uint64_t count = Data(base).item_count();
  guest::StoreU32(base, frame + 84, 159);
  guest::StoreU32(base, frame + 92, 158);
  const uint64_t init[8] = {List(base), count, count, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x82210CA0, init);
  ListSetSelection(base, List(base), Data(base).main_selection());
  return Pause4ShowItems(base, frame, calls);
}

uint64_t Pause4RestoreNodes(uint8_t* base, uint32_t group, uint32_t child, uint32_t sp,
                            const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822106E8 PauseMenu4::RestoreNodes");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Movie(base);
  auto node = [&](uint32_t scene) {
    const uint32_t root = guest::LoadU32(base, movie + scene);
    return ui_movies::Child(
        base, int32_t(group) == -1 ? root : ui_movies::Child(base, root, group), child);
  };
  const uint32_t file = node(ui_movies::kMovieFile);
  const uint32_t live = node(ui_movies::kMovieLive);
  const uint32_t saved = node(ui_movies::kMovieSaved);
  const uint64_t a[8] = {movie, live, file, 0, 0, 0, 0, 0};
  calls.CallArgs(kRestorePose, frame, 0x82210794, a);
  const uint64_t b[8] = {movie, saved, file, 0, 0, 0, 0, 0};
  return calls.CallArgs(kRestorePose, frame, 0x822107AC, b);
}

uint32_t Pause4ShowVolume(uint8_t* base, uint32_t node, uint64_t volume, uint32_t sp,
                          const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822105F8 PauseMenu4::ShowVolume");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base);
  const uint32_t text = ui_movies::Child(base, node, 2);
  const uint64_t args[8] = {movie, text, 8, volume, 0, 0, 0, 0};
  calls.CallArgs(kSetNodeText, frame, 0x82210634, args);
  uint32_t r3 = 0;
  for (int32_t i = 1; i <= 15; i++) {
    r3 = ui_movies::Child(base, node, uint32_t(i + 3));
    schema::UiMovieNode(base, r3).set_visible(i == int32_t(uint32_t(volume)) ? 1 : 0);
  }
  return r3;
}

uint64_t Pause4ShowVolumes(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210670 PauseMenu4::ShowVolumes");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base);
  Pause4ShowVolume(base, ui_movies::FindNode(base, movie, 6, 5), Data(base).music_edit(), frame,
                   calls);
  return Pause4ShowVolume(base, ui_movies::FindNode(base, movie, 6, 6), Data(base).effects_edit(),
                          frame, calls);
}

uint64_t Pause4ShowVolumesB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210DE0 PauseMenu4::ShowVolumesB");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base);
  calls.Call(kShowVolumeBar, frame, 0x82210E20, ui_movies::FindNode(base, movie, 6, 5),
             Data(base).music_edit());
  return calls.Call(kShowVolumeBar, frame, 0x82210E3C, ui_movies::FindNode(base, movie, 6, 6),
                    Data(base).effects_edit());
}

uint32_t Pause4SetArrow(uint8_t* base, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210420 PauseMenu4::SetArrow");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t node = ui_movies::FindNode(base, Movie(base), 6, 0);
  const int64_t word = int64_t(int32_t(uint32_t(value)));
  guest::StoreU32(base, frame + 80, uint32_t(uint64_t(word) >> 32));
  guest::StoreU32(base, frame + 84, uint32_t(word));
  const uint32_t child = ui_movies::Child(base, node, 0);
  schema::UiMovieNode(base, child).set_y(float(double(word)));
  return child;
}

uint64_t Pause4SetUpVolume(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211250 PauseMenu4::SetUpVolume");
  const uint32_t frame = Frame(base, sp, 128);
  guest::StoreU32(base, frame + 92, 158);
  const uint32_t movie = Movie(base);
  guest::StoreU32(base, frame + 84, 159);
  const uint64_t init[8] = {List(base), 3, 3, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x822112A8, init);
  {
    const schema::PauseMenu4Data data = Data(base);
    data.set_music_edit(data.music_volume());
    data.set_effects_edit(data.effects_volume());
  }
  Data(base).set_volume_toggle(0);
  Pause4RestoreNodes(base, 0xFFFFFFFFu, 6, frame, calls);
  auto text = [&](uint32_t lr, uint32_t node, uint64_t id) {
    calls.Call(kSetNodeText, frame, lr, movie, node, id);
  };
  const uint32_t labels = ui_movies::FindNode(base, movie, 6, 0);
  text(0x82211308, ui_movies::Child(base, labels, 5), 2004);
  text(0x82211324, ui_movies::Child(base, labels, 6), 2005);
  const uint32_t marks = ui_movies::FindNode(base, movie, 6, 1);
  text(0x82211350, ui_movies::Child(base, marks, 3), 4);
  text(0x8221136C, ui_movies::Child(base, marks, 4), 7);
  Pause4ShowVolumes(base, frame, calls);
  return Pause4ShowVolumesB(base, frame, calls);
}

// --- The transitions ---

uint64_t Pause4RunTen(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220FF88 PauseMenu4::RunTen");
  const uint32_t frame = Frame(base, sp, 96);
  const uint32_t movie = Movie(base);
  if (guest::LoadU32(base, step) == 0) {
    Data(base).set_run_count(0);
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8220FFD4, movie);
  const uint32_t count = Data(base).run_count() + 1;
  Data(base).set_run_count(count);
  return int32_t(Data(base).run_count()) >= 10 ? 1 : 0;
}

uint64_t Pause4OpenMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211098 PauseMenu4::OpenMain");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t run = Transition(base, step, frame, 0x82211114, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x822110E8, movie, kNone, 1);
    Data(base).set_main_selection(0);
    Pause4SetUpMain(base, frame, calls);
    Pause4ColorItems(base, frame);
  });
  return uint32_t(run) == 0 ? 1 : 0;
}

uint64_t Pause4ShowMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210010 PauseMenu4::ShowMain");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x822100C8, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x82210058, movie, kNone, 0);
    calls.Call(kLoop, frame, 0x82210068, movie, kNone, 0);
    Play(calls, frame, 0x82210098, movie, 4, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x822100B4, movie, 4, uint64_t(Selection(base)) + 1);
  });
  return 1;
}

uint64_t Pause4BackToMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822100D8 PauseMenu4::BackToMain");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x82210160, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x82210130, movie, 4, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x8221014C, movie, 4, uint64_t(Selection(base)) + 1);
  });
  return 1;
}

uint64_t Pause4MoveMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210228 PauseMenu4::MoveMain");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x822102CC, calls, [&](uint32_t movie) {
    calls.Call(kFinish, frame, 0x82210270, movie, 4, uint64_t(Previous(base)) + 1);
    Play(calls, frame, 0x8221029C, movie, 4, uint64_t(Selection(base)) + 1, 1);
    calls.Call(kLoop, frame, 0x822102B8, movie, 4, uint64_t(Selection(base)) + 1);
  });
  return 1;
}

uint64_t Pause4CloseMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210170 PauseMenu4::CloseMain");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t run = Transition(base, step, frame, 0x82210204, calls, [&](uint32_t movie) {
    calls.Call(kFinish, frame, 0x822101C0, movie, 4, uint64_t(Selection(base)) + 1);
    calls.Call(kFinish, frame, 0x822101D0, movie, kNone, 0);
    Play(calls, frame, 0x822101F0, movie, kNone, 2);
  });
  return uint32_t(run) == 0 ? 1 : 0;
}

uint64_t Pause4OpenVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822114D8 PauseMenu4::OpenVolume");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t run = Transition(base, step, frame, 0x8221156C, calls, [&](uint32_t movie) {
    const uint32_t selection = Selection(base);
    Data(base).set_main_selection(selection);
    calls.Call(kFinish, frame, 0x82211534, movie, 4, uint64_t(selection) + 1);
    Pause4SetUpVolume(base, frame, calls);
    Play(calls, frame, 0x82211558, movie, kNone, 3);
  });
  return uint32_t(run) == 0 ? 1 : 0;
}

uint64_t Pause4ShowVolumeRow(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_822102D8 PauseMenu4::ShowVolumeRow");
  const uint32_t frame = Frame(base, sp, 112);
  Transition(base, step, frame, 0x82210344, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x82210328, movie, 6, 1);
    calls.Call(kLoopLast, frame, 0x82210330, movie);
  });
  return 1;
}

uint64_t Pause4MoveVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210940 PauseMenu4::MoveVolume");
  const uint32_t frame = Frame(base, sp, 144);
  Transition(base, step, frame, 0x82210AFC, calls, [&](uint32_t movie) {
    const uint32_t previous = Previous(base);
    const uint32_t selection = Selection(base);
    if (previous < 2) {
      calls.Call(kFinish, frame, 0x82210A08, movie, 6, uint64_t(previous) + 1);
    } else if (previous == 2) {
      calls.Call(kFinish, frame, 0x822109C0, movie, 6, VolumeRow(base, 2));
      if (selection == 0) {
        Pause4SetArrow(base, 0, frame);
      } else if (selection == 1) {
        Pause4SetArrow(base, 40, frame);
      }
      Data(base).set_volume_toggle(0);
    }
    if (selection < 3) {
      Play(calls, frame, 0x82210A7C, movie, 6, VolumeRow(base, selection));
      calls.Call(kLoopLast, frame, 0x82210A84, movie);
    }
    calls.Call(kFinishNamed, frame, 0x82210A90, movie, 19);
    calls.Call(kFinishNamed, frame, 0x82210A9C, movie, 20);
    if (previous != 2 && selection != 2) {
      const int32_t move = int32_t(ListView(base).move());
      if (move == 1) {
        PlayNamed(calls, frame, 0x82210AE8, movie, 20);
      } else if (move == -1) {
        PlayNamed(calls, frame, 0x82210AE8, movie, 19);
      }
    }
  });
  return 1;
}

uint64_t Pause4SetVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210B08 PauseMenu4::SetVolume");
  const uint32_t frame = Frame(base, sp, 128);
  Transition(base, step, frame, 0x82210BF4, calls, [&](uint32_t movie) {
    if (int32_t(Selection(base)) == 2) {
      const bool on = int32_t(Data(base).volume_toggle()) == 1;
      calls.Call(kFinish, frame, on ? 0x82210B68 : 0x82210B78, movie, 6, on ? 3 : 4);
      Play(calls, frame, 0x82210B98, movie, 6, on ? 4 : 3);
      calls.Call(kLoopLast, frame, 0x82210BA0, movie);
    } else {
      const uint32_t node5 = ui_movies::FindNode(base, movie, 6, 5);
      const uint32_t node6 = ui_movies::FindNode(base, movie, 6, 6);
      Pause4ShowVolume(base, node6, Data(base).effects_edit(), frame, calls);
      Pause4ShowVolume(base, node5, Data(base).music_edit(), frame, calls);
    }
  });
  return 1;
}

uint64_t Pause4CloseVolume(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211138 PauseMenu4::CloseVolume");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t run = Transition(base, step, frame, 0x82211204, calls, [&](uint32_t movie) {
    const uint32_t selection = Selection(base);
    if (selection < 3) {
      calls.Call(kFinish, frame, 0x822111CC, movie, 6, VolumeRow(base, selection));
    }
    Play(calls, frame, 0x822111EC, movie, kNone, 4);
    Pause4SetUpMain(base, frame, calls);
  });
  if (uint32_t(run) == 0) {
    PagesGo(base, Pages(base), 0, 3, 0xFFFFFFFFu);
    Globals::pause4_pages(base).set_step_started(0);
  }
  return 0;
}

// --- The pages ---

uint64_t Pause4MainPage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220FD48 PauseMenu4::MainPage");
  const uint32_t frame = Frame(base, sp, 112);
  auto sound = [&](uint32_t lr, uint64_t cue) { calls.Call(kMenuSoundPlay, frame, lr, cue); };
  calls.Call(kListUpdate, frame, 0x8220FD68, List(base));
  const uint32_t selection = Selection(base);
  if (selection != Previous(base)) {
    PagesGo(base, Pages(base), 0, 4, 0xFFFFFFFFu);
    sound(0x8220FDA4, 4);
  } else if (uint32_t(calls.Call(kListCancelled, frame, 0x8220FDB0, List(base))) != 0 ||
             uint32_t(calls.Call(kListPressed152, frame, 0x8220FDC0, List(base))) != 0) {
    Data(base).set_answer(1);
    PagesGo(base, Pages(base), 3, 5, 0xFFFFFFFFu);
    sound(0x8220FF14, 2);
    return 0;
  }
  if (uint32_t(calls.Call(kListAccepted, frame, 0x8220FDD0, List(base))) == 0) {
    return 0;
  }
  sound(0x8220FDE0, 0);
  const uint32_t at = Selection(base);
  const schema::PauseMenu4Data data = Data(base);
  const uint32_t kind = data.kind();
  const uint32_t item = Item(base, kind, at);
  // The items that end the race: the answer (+56), the confirm dialog in
  // `mode` for the player who paused (+72), page 2.
  auto confirm = [&](uint32_t answer, uint32_t mode) {
    data.set_answer(answer);
    RaceDialogOpen(base, mode, Data(base).player(), frame, calls);
    PagesGo(base, Pages(base), 2, 0xFFFFFFFFu, 0xFFFFFFFFu);
    return 0;
  };
  switch (item) {
    case 0:  // sub_8220FE3C, the resume (merged)
      data.set_answer(1);
      PagesGo(base, Pages(base), 3, 5, 0xFFFFFFFFu);
      return 0;
    case 2:
      PagesGo(base, Pages(base), 1, 6, 7);
      return 0;
    case 4:
      return confirm(data.player() == 0 ? 4 : 5, 4);
    case 5:
      return confirm(2, 1);
    case 6:
      return confirm(3, 3);
    case 7:
      return confirm(6, 2);
    case 8:
      return confirm(7, 2);
    case 10:
      return confirm(4, 5);
    case 1:
    case 3:
      return 0;
    default:  // 9 and past 10
      data.set_answer(0);
      return 0;
  }
}

uint64_t Pause4VolumePage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210E58 PauseMenu4::VolumePage");
  const uint32_t frame = Frame(base, sp, 112);
  // SoundOptions::SetEffectsVolume and SetMusicVolume with the edits or the
  // volumes (their low bytes sign-extended).
  auto set_volumes = [&](bool edits, uint32_t effects_lr, uint32_t music_lr) {
    const uint32_t effects = edits ? Data(base).effects_edit() : Data(base).effects_volume();
    calls.Call(kSetEffectsVolume, frame, effects_lr, LowByte(effects));
    const uint32_t music = edits ? Data(base).music_edit() : Data(base).music_volume();
    calls.Call(kSetMusicVolume, frame, music_lr, LowByte(music));
  };
  // The edits kept as the volumes, or put back from the volumes or the
  // defaults (music first, then effects).
  auto keep_edits = [&] {
    const schema::PauseMenu4Data data = Data(base);
    data.set_music_volume(data.music_edit());
    data.set_effects_volume(data.effects_edit());
  };
  auto edits_from_defaults = [&] {
    const schema::PauseMenu4Data data = Data(base);
    data.set_music_edit(data.music_default());
    data.set_effects_edit(data.effects_default());
  };
  auto edits_from_volumes = [&] {
    const schema::PauseMenu4Data data = Data(base);
    data.set_music_edit(data.music_volume());
    data.set_effects_edit(data.effects_volume());
  };
  auto sound = [&](uint64_t cue) {
    calls.Call(kMenuSoundPlay, frame, 0x82211078, cue);
    return 0;
  };
  calls.Call(kListUpdate, frame, 0x82210E78, List(base));
  if (ListView(base).move() != 0) {
    PagesGo(base, Pages(base), 1, 8, 0xFFFFFFFFu);
    return sound(4);
  }
  if (Pause4StepVolume(base, frame) != 0) {
    PagesGo(base, Pages(base), 1, 9, 0xFFFFFFFFu);
    set_volumes(true, 0x82210EE0, 0x82210EF0);
    return sound(4);
  }
  if (uint32_t(calls.Call(kListAccepted, frame, 0x82210EFC, List(base))) != 0) {
    if (int32_t(Selection(base)) == 2 && Data(base).volume_toggle() == 0) {
      keep_edits();
      Pause4ShowVolumesB(base, frame, calls);
      PagesGo(base, Pages(base), 0xFFFFFFFFu, 0, 10);
      set_volumes(false, 0x82210F68, 0x82210FD0);
    } else if (int32_t(Selection(base)) == 2 && int32_t(Data(base).volume_toggle()) == 1) {
      edits_from_defaults();
      Pause4ShowVolumes(base, frame, calls);
      set_volumes(true, 0x82210FC0, 0x82210FD0);
    } else {
      ListSetSelection(base, List(base), 2);
      Data(base).set_volume_toggle(0);
      PagesGo(base, Pages(base), 1, 8, 0xFFFFFFFFu);
    }
    return sound(0);
  }
  if (uint32_t(calls.Call(kListCancelled, frame, 0x82211010, List(base))) == 0) {
    return 0;
  }
  edits_from_volumes();
  Pause4ShowVolumes(base, frame, calls);
  PagesGo(base, Pages(base), 0xFFFFFFFFu, 0, 10);
  set_volumes(false, 0x82211060, 0x82211070);
  return sound(2);
}

uint64_t Pause4ConfirmPage(uint8_t* base, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_8220FF30 PauseMenu4::ConfirmPage");
  const uint32_t frame = Frame(base, sp, 96);
  const int32_t answer = int32_t(uint32_t(RaceDialogUpdate(base, r4, frame, calls)));
  if (answer == 1) {
    PagesGo(base, Pages(base), 3, 5, 0xFFFFFFFFu);
  } else if (answer == 2) {
    PagesGo(base, Pages(base), 0, 0xFFFFFFFFu, 0xFFFFFFFFu);
  }
  return 0;
}

// --- Open, Update and Draw ---

uint64_t Pause4Open(uint8_t* base, uint32_t screen, uint64_t player, uint64_t kind, uint32_t sp,
                    const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211390 PauseMenu4::Open");
  const uint32_t frame = Frame(base, sp, 128);
  guest::StoreU32(base, frame + 80, 5);
  const uint32_t movie =
      guest::LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  const schema::PauseMenu4 view(base, screen);
  view.set_movie(movie);
  calls.Call(kResetPose, frame, 0x822113CC, movie);
  const uint32_t three = ui_movies::FindNode(base, view.movie().address, 0xFFFFFFFFu, 3);
  const uint32_t five = ui_movies::FindNode(base, view.movie().address, 0xFFFFFFFFu, 5);
  schema::UiMovieNode(base, five).set_texture_entry(uint32_t(player));
  schema::UiMovieNode(base, three).set_texture_entry(uint32_t(player));
  const uint32_t other = int32_t(uint32_t(player)) != 0 ? 1 : 0;
  ListView(base).set_player(other);
  Data(base).set_player(other);
  Data(base).set_kind(uint32_t(kind));
  PagesGo(base, Pages(base), 0, 1, 2);
  calls.Call(kMenuSoundPlay, frame, 0x8221144C, 6);
  return Pause4LoadVolumes(base, frame);
}

uint64_t Pause4Update(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211458 PauseMenu4::Update");
  const uint32_t frame = Frame(base, sp, 112);
  // MainPage, VolumePage, ConfirmPage, AnswerPage.
  constexpr uint32_t kHandlers[4] = {0x8220FD48, 0x82210E58, 0x8220FF30, 0x82210910};
  for (uint32_t k = 0; k < 4; k++) guest::StoreU32(base, frame + 80 + 4 * k, kHandlers[k]);
  const int32_t page = int32_t(Globals::pause4_pages(base).page());
  switch (page) {
    case -1:
      return 0;
    case 0:
      return Pause4MainPage(base, frame, calls);
    case 1:
      return Pause4VolumePage(base, frame, calls);
    case 2:
      return Pause4ConfirmPage(base, r4, frame, calls);
    case 3:
      return Pause4AnswerPage(base, frame);
    default: {
      // Past the table: the frame's word, as the original's bctrl.
      const uint32_t handler = guest::LoadU32(base, (uint32_t(page) << 2) + frame + 80);
      const uint64_t regs[8] = {r3, r4, 0, 0, uint64_t(int64_t(int32_t(kHandlers[0]))),
                                uint64_t(int64_t(int32_t(kHandlers[1]))),
                                uint64_t(int64_t(int32_t(kHandlers[2]))),
                                uint64_t(int64_t(int32_t(kHandlers[3])))};
      return calls.CallArgs(handler, frame, 0x822114BC, regs);
    }
  }
}

uint64_t Pause4Draw(uint8_t* base, uint32_t screen, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211590 PauseMenu4::Draw");
  const uint32_t frame = Frame(base, sp, 192);
  // RunTen, OpenMain, ShowMain, BackToMain, MoveMain, CloseMain, OpenVolume,
  // ShowVolumeRow, MoveVolume, SetVolume, CloseVolume.
  constexpr uint32_t kHandlers[11] = {0x8220FF88, 0x82211098, 0x82210010, 0x822100D8,
                                      0x82210228, 0x82210170, 0x822114D8, 0x822102D8,
                                      0x82210940, 0x82210B08, 0x82211138};
  for (uint32_t k = 0; k < 11; k++) guest::StoreU32(base, frame + 80 + 4 * k, kHandlers[k]);
  const uint32_t pages = Pages(base);
  const int32_t transition = int32_t(schema::PageObject(base, pages).step());
  uint64_t r3;
  if (transition == -1) {
    r3 = Run(calls, frame, 0x82211680, schema::PauseMenu4(base, screen).movie().address);
  } else if (transition < 0) {
    PagesFinish(base, pages, 1);
    r3 = pages;
  } else {
    const uint32_t step = schema::PageObject(base, pages).step_started_address();
    uint64_t result;
    switch (transition) {
      case 0: result = Pause4RunTen(base, step, frame, calls); break;
      case 1: result = Pause4OpenMain(base, step, frame, calls); break;
      case 2: result = Pause4ShowMain(base, step, frame, calls); break;
      case 3: result = Pause4BackToMain(base, step, frame, calls); break;
      case 4: result = Pause4MoveMain(base, step, frame, calls); break;
      case 5: result = Pause4CloseMain(base, step, frame, calls); break;
      case 6: result = Pause4OpenVolume(base, step, frame, calls); break;
      case 7: result = Pause4ShowVolumeRow(base, step, frame, calls); break;
      case 8: result = Pause4MoveVolume(base, step, frame, calls); break;
      case 9: result = Pause4SetVolume(base, step, frame, calls); break;
      case 10: result = Pause4CloseVolume(base, step, frame, calls); break;
      default: {
        // Past the table: the frame's word, as the original's bctrl (r4 to
        // r9 the table's words it had loaded, r10 the table).
        const uint32_t handler = guest::LoadU32(base, (uint32_t(transition) << 2) + frame + 80);
        auto reg = [](uint32_t a) { return uint64_t(int64_t(int32_t(a))); };
        const uint64_t regs[8] = {step,          reg(0x82210170), reg(0x822114D8),
                                  reg(0x822102D8), reg(0x82210940), reg(0x82210B08),
                                  reg(0x82211138), uint64_t(frame) + 80};
        result = calls.CallArgs(handler, frame, 0x82211658, regs);
      }
    }
    const uint32_t now = Pages(base);
    PagesFinish(base, now, uint32_t(result));
    r3 = now;
  }
  if (int32_t(Globals::pause4_pages(base).page()) == 2) {
    r3 = RaceDialogDraw(base, frame, calls);
  }
  return r3;
}

}  // namespace race_screens
