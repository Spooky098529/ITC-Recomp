// Kind 4's pause menu (docs/functions/race-screens-unseen.md): see unseen.h.
// Read from the generated code; never seen in play.

#include "race_screens/unseen.h"

#include "base/log.h"
#include "guest.h"
#include "ui_movies/game.h"
#include "ui_movies/ui_movies.h"
#include "vehicle_accessors/vehicle_accessors.h"

#include <bit>

namespace race_screens {

namespace {

constexpr uint64_t kNone = ~uint64_t(0);

uint32_t Get(uint8_t* base, uint32_t global) { return guest::LoadU32(base, global); }

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  guest::StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

uint32_t Movie(uint8_t* base) { return Get(base, Get(base, kPause4Screen)); }
uint32_t Data(uint8_t* base) { return Get(base, kPause4Data); }
uint32_t List(uint8_t* base) { return Get(base, kPause4List); }
uint32_t Pages(uint8_t* base) { return Get(base, kPause4Pages); }
uint32_t Selection(uint8_t* base) { return ListSelection(base, List(base)); }
// MenuList::GetPreviousSelection (sub_8220BC28): the list's +20.
uint32_t Previous(uint8_t* base) { return guest::LoadU32(base, List(base) + kListSelectionBefore); }
uint32_t DataWord(uint8_t* base, uint32_t field) { return guest::LoadU32(base, Data(base) + field); }
void SetData(uint8_t* base, uint32_t field, uint32_t value) {
  guest::StoreU32(base, Data(base) + field, value);
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
  return int32_t(DataWord(base, 60)) == 1 ? 4 : 3;
}

// A word's low byte, sign-extended (the sound options' setters' argument).
uint64_t LowByte(uint32_t word) { return uint64_t(int64_t(int8_t(word & 0xFF))); }

}  // namespace

// --- The helpers ---

uint32_t Pause4StepVolume(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210488 PauseMenu4::StepVolume");
  Frame(base, sp, 96);
  const int32_t side = int32_t(guest::LoadU32(base, List(base) + kListSide));
  const uint32_t selection = Selection(base);
  if (side != -1 && side != 1) return 0;
  if (selection < 2) {
    const uint32_t field = selection == 0 ? 8 : 12;
    SetData(base, field, DataWord(base, field) + uint32_t(side));
    const int32_t value = int32_t(DataWord(base, field));
    if (side == -1 ? value < 0 : value > 15) {
      SetData(base, field, side == -1 ? 0 : 15);
      return 0;
    }
    return 1;
  }
  if (selection != 2) return 0;
  const int32_t toggle = int32_t(DataWord(base, 60));
  if (side == -1 ? toggle != 1 : toggle != 0) return 0;
  SetData(base, 60, side == -1 ? 0 : 1);
  return 1;
}

uint32_t Pause4LoadVolumes(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_822107B8 PauseMenu4::LoadVolumes");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t options = save + 0x1F85C;
  SetData(base, 0, uint32_t(int32_t(int8_t(guest::LoadU8(base, options + 28)))));
  SetData(base, 4, uint32_t(int32_t(int8_t(guest::LoadU8(base, options + 29)))));
  SetData(base, 16, 15);
  SetData(base, 20, 15);
  return save;
}

uint32_t Pause4SaveVolumes(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210820 PauseMenu4::SaveVolumes");
  Frame(base, sp, 96);
  const uint32_t save = vehicle_accessors::SaveDataOf(base, 0);
  const uint32_t options = save + 0x1F85C;
  guest::StoreU8(base, options + 28, uint8_t(DataWord(base, 0)));
  guest::StoreU8(base, options + 29, uint8_t(DataWord(base, 4)));
  return save;
}

uint64_t Pause4AnswerPage(uint8_t* base, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210910 PauseMenu4::AnswerPage");
  const uint32_t frame = Frame(base, sp, 96);
  Pause4SaveVolumes(base, frame);
  return DataWord(base, 56);
}

uint64_t Pause4ShowItems(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210360 PauseMenu4::ShowItems");
  const uint32_t frame = Frame(base, sp, 128);
  const uint32_t movie = Movie(base);
  const uint32_t node = ui_movies::FindNode(base, movie, 4, 0);
  uint64_t r3 = node;
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, node + 104)); i++) {
    const uint32_t kind = DataWord(base, 76);
    const uint32_t item = guest::LoadU32(base, kPause4Items + ((kind * 6 + uint32_t(i)) << 2));
    const uint64_t child = uint64_t(guest::LoadU32(base, node + 100) + uint32_t(i));
    if (int32_t(item) != -1 && i < 6) {
      const uint64_t args[8] = {movie, 4, child, uint64_t(item) + 1989, 0, 0, 0, 0};
      r3 = calls.CallArgs(kSetText, frame, 0x822103E8, args);
    } else {
      const uint32_t hidden = ui_movies::FindNode(base, movie, 4, uint32_t(child));
      guest::StoreU32(base, hidden + 80, 0);
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
  for (int32_t i = 0; i < int32_t(guest::LoadU32(base, node + 104)); i++) {
    const bool enabled = ListIsEnabled(base, List(base), uint32_t(i)) != 0;
    const uint32_t color[3] = {enabled ? 205u : 255u, enabled ? 235u : 255u, enabled ? 67u : 255u};
    for (uint32_t k = 0; k < 3; k++) {
      r3 = ui_movies::FindNode(base, movie, 4, guest::LoadU32(base, node + 100) + uint32_t(i));
      guest::StoreU32(base, r3 + 56 + 4 * k, color[k]);
    }
  }
  return r3;
}

uint64_t Pause4SetUpMain(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210C00 PauseMenu4::SetUpMain");
  const uint32_t frame = Frame(base, sp, 128);
  {
    const uint32_t data = Data(base);
    guest::StoreU32(base, data + 52, 6);
    const uint32_t row = kPause4Items + guest::LoadU32(base, Data(base) + 76) * 24;
    for (uint32_t i = 0; i < 6; i++) {
      if (int32_t(guest::LoadU32(base, row + 4 * i)) == -1) {
        guest::StoreU32(base, Data(base) + 52, i);
        break;
      }
    }
  }
  const uint64_t count = DataWord(base, 52);
  guest::StoreU32(base, frame + 84, 159);
  guest::StoreU32(base, frame + 92, 158);
  const uint64_t init[8] = {List(base), count, count, 1, 154, 153, 155, 156};
  calls.CallArgs(kListInit, frame, 0x82210CA0, init);
  ListSetSelection(base, List(base), DataWord(base, 64));
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
    guest::StoreU32(base, r3 + 80, i == int32_t(uint32_t(volume)) ? 1 : 0);
  }
  return r3;
}

uint64_t Pause4ShowVolumes(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210670 PauseMenu4::ShowVolumes");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base);
  Pause4ShowVolume(base, ui_movies::FindNode(base, movie, 6, 5), DataWord(base, 8), frame, calls);
  return Pause4ShowVolume(base, ui_movies::FindNode(base, movie, 6, 6), DataWord(base, 12), frame,
                          calls);
}

uint64_t Pause4ShowVolumesB(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210DE0 PauseMenu4::ShowVolumesB");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t movie = Movie(base);
  calls.Call(kShowVolumeBar, frame, 0x82210E20, ui_movies::FindNode(base, movie, 6, 5),
             DataWord(base, 8));
  return calls.Call(kShowVolumeBar, frame, 0x82210E3C, ui_movies::FindNode(base, movie, 6, 6),
                    DataWord(base, 12));
}

uint32_t Pause4SetArrow(uint8_t* base, uint64_t value, uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82210420 PauseMenu4::SetArrow");
  const uint32_t frame = Frame(base, sp, 112);
  const uint32_t node = ui_movies::FindNode(base, Movie(base), 6, 0);
  const int64_t word = int64_t(int32_t(uint32_t(value)));
  guest::StoreU32(base, frame + 80, uint32_t(uint64_t(word) >> 32));
  guest::StoreU32(base, frame + 84, uint32_t(word));
  const uint32_t child = ui_movies::Child(base, node, 0);
  guest::StoreU32(base, child + 16, std::bit_cast<uint32_t>(float(double(word))));
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
    const uint32_t data = Data(base);
    guest::StoreU32(base, data + 8, guest::LoadU32(base, data + 0));
    guest::StoreU32(base, data + 12, guest::LoadU32(base, data + 4));
  }
  SetData(base, 60, 0);
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
    SetData(base, 68, 0);
    guest::StoreU32(base, step, 1);
  }
  Run(calls, frame, 0x8220FFD4, movie);
  SetData(base, 68, DataWord(base, 68) + 1);
  return int32_t(DataWord(base, 68)) >= 10 ? 1 : 0;
}

uint64_t Pause4OpenMain(uint8_t* base, uint32_t step, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82211098 PauseMenu4::OpenMain");
  const uint32_t frame = Frame(base, sp, 112);
  const uint64_t run = Transition(base, step, frame, 0x82211114, calls, [&](uint32_t movie) {
    Play(calls, frame, 0x822110E8, movie, kNone, 1);
    SetData(base, 64, 0);
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
    SetData(base, 64, selection);
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
      SetData(base, 60, 0);
    }
    if (selection < 3) {
      Play(calls, frame, 0x82210A7C, movie, 6, VolumeRow(base, selection));
      calls.Call(kLoopLast, frame, 0x82210A84, movie);
    }
    calls.Call(kFinishNamed, frame, 0x82210A90, movie, 19);
    calls.Call(kFinishNamed, frame, 0x82210A9C, movie, 20);
    if (previous != 2 && selection != 2) {
      const int32_t move = int32_t(guest::LoadU32(base, List(base) + kListMove));
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
      const bool on = int32_t(DataWord(base, 60)) == 1;
      calls.Call(kFinish, frame, on ? 0x82210B68 : 0x82210B78, movie, 6, on ? 3 : 4);
      Play(calls, frame, 0x82210B98, movie, 6, on ? 4 : 3);
      calls.Call(kLoopLast, frame, 0x82210BA0, movie);
    } else {
      const uint32_t effects = ui_movies::FindNode(base, movie, 6, 5);
      const uint32_t music = ui_movies::FindNode(base, movie, 6, 6);
      Pause4ShowVolume(base, music, DataWord(base, 12), frame, calls);
      Pause4ShowVolume(base, effects, DataWord(base, 8), frame, calls);
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
    guest::StoreU32(base, Pages(base) + 24, 0);
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
    SetData(base, 56, 1);
    PagesGo(base, Pages(base), 3, 5, 0xFFFFFFFFu);
    sound(0x8220FF14, 2);
    return 0;
  }
  if (uint32_t(calls.Call(kListAccepted, frame, 0x8220FDD0, List(base))) == 0) {
    return 0;
  }
  sound(0x8220FDE0, 0);
  const uint32_t at = Selection(base);
  const uint32_t data = Data(base);
  const uint32_t kind = guest::LoadU32(base, data + 76);
  const uint32_t item = guest::LoadU32(base, kPause4Items + ((kind * 6 + at) << 2));
  // The items that end the race: the answer (+56), the confirm dialog in
  // `mode` for the player who paused (+72), page 2.
  auto confirm = [&](uint32_t answer, uint32_t mode) {
    guest::StoreU32(base, data + 56, answer);
    RaceDialogOpen(base, mode, DataWord(base, 72), frame, calls);
    PagesGo(base, Pages(base), 2, 0xFFFFFFFFu, 0xFFFFFFFFu);
    return 0;
  };
  switch (item) {
    case 0:  // sub_8220FE3C, the resume (merged)
      guest::StoreU32(base, data + 56, 1);
      PagesGo(base, Pages(base), 3, 5, 0xFFFFFFFFu);
      return 0;
    case 2:
      PagesGo(base, Pages(base), 1, 6, 7);
      return 0;
    case 4:
      return confirm(guest::LoadU32(base, data + 72) == 0 ? 4 : 5, 4);
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
      guest::StoreU32(base, data + 56, 0);
      return 0;
  }
}

uint64_t Pause4VolumePage(uint8_t* base, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82210E58 PauseMenu4::VolumePage");
  const uint32_t frame = Frame(base, sp, 112);
  // SoundOptions::SetEffectsVolume and SetMusicVolume with the data's words
  // at `effects` and `music` (their low bytes sign-extended).
  auto set_volumes = [&](uint32_t effects, uint32_t effects_lr, uint32_t music,
                         uint32_t music_lr) {
    calls.Call(kSetEffectsVolume, frame, effects_lr, LowByte(DataWord(base, effects)));
    calls.Call(kSetMusicVolume, frame, music_lr, LowByte(DataWord(base, music)));
  };
  auto copy_two = [&](uint32_t to, uint32_t from) {
    const uint32_t data = Data(base);
    guest::StoreU32(base, data + to, guest::LoadU32(base, data + from));
    guest::StoreU32(base, data + to + 4, guest::LoadU32(base, data + from + 4));
  };
  auto sound = [&](uint64_t cue) {
    calls.Call(kMenuSoundPlay, frame, 0x82211078, cue);
    return 0;
  };
  calls.Call(kListUpdate, frame, 0x82210E78, List(base));
  if (guest::LoadU32(base, List(base) + kListMove) != 0) {
    PagesGo(base, Pages(base), 1, 8, 0xFFFFFFFFu);
    return sound(4);
  }
  if (Pause4StepVolume(base, frame) != 0) {
    PagesGo(base, Pages(base), 1, 9, 0xFFFFFFFFu);
    set_volumes(12, 0x82210EE0, 8, 0x82210EF0);
    return sound(4);
  }
  if (uint32_t(calls.Call(kListAccepted, frame, 0x82210EFC, List(base))) != 0) {
    if (int32_t(Selection(base)) == 2 && DataWord(base, 60) == 0) {
      copy_two(0, 8);
      Pause4ShowVolumesB(base, frame, calls);
      PagesGo(base, Pages(base), 0xFFFFFFFFu, 0, 10);
      set_volumes(4, 0x82210F68, 0, 0x82210FD0);
    } else if (int32_t(Selection(base)) == 2 && int32_t(DataWord(base, 60)) == 1) {
      copy_two(8, 16);
      Pause4ShowVolumes(base, frame, calls);
      set_volumes(12, 0x82210FC0, 8, 0x82210FD0);
    } else {
      ListSetSelection(base, List(base), 2);
      SetData(base, 60, 0);
      PagesGo(base, Pages(base), 1, 8, 0xFFFFFFFFu);
    }
    return sound(0);
  }
  if (uint32_t(calls.Call(kListCancelled, frame, 0x82211010, List(base))) == 0) {
    return 0;
  }
  copy_two(8, 0);
  Pause4ShowVolumes(base, frame, calls);
  PagesGo(base, Pages(base), 0xFFFFFFFFu, 0, 10);
  set_volumes(4, 0x82211060, 0, 0x82211070);
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
  guest::StoreU32(base, screen, movie);
  calls.Call(kResetPose, frame, 0x822113CC, movie);
  const uint32_t three = ui_movies::FindNode(base, guest::LoadU32(base, screen), 0xFFFFFFFFu, 3);
  const uint32_t five = ui_movies::FindNode(base, guest::LoadU32(base, screen), 0xFFFFFFFFu, 5);
  guest::StoreU32(base, five + 108, uint32_t(player));
  guest::StoreU32(base, three + 108, uint32_t(player));
  const uint32_t other = int32_t(uint32_t(player)) != 0 ? 1 : 0;
  guest::StoreU32(base, List(base) + kListPlayer, other);
  SetData(base, 72, other);
  SetData(base, 76, uint32_t(kind));
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
  const int32_t page = int32_t(guest::LoadU32(base, Pages(base)));
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
  const int32_t transition = int32_t(guest::LoadU32(base, pages + 12));
  uint64_t r3;
  if (transition == -1) {
    r3 = Run(calls, frame, 0x82211680, guest::LoadU32(base, screen));
  } else if (transition < 0) {
    PagesFinish(base, pages, 1);
    r3 = pages;
  } else {
    const uint32_t step = pages + 24;
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
  if (int32_t(guest::LoadU32(base, Pages(base))) == 2) {
    r3 = RaceDialogDraw(base, frame, calls);
  }
  return r3;
}

}  // namespace race_screens
