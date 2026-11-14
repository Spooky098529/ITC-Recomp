// The type 8 panel screen's slots and handlers
// (docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code.

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "ui_movies/game.h"

namespace type8_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

namespace ts = type67_screens;

constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kDialogMovie = 16, kDialogCursor = 40960;

uint32_t Frame(uint8_t* base, uint32_t sp, uint32_t bytes) {
  StoreU32(base, sp - bytes, sp);
  return sp - bytes;
}

// PlayNamed(movie, named, 1, 0, 1, 0) from `frame` returning to `lr`.
uint64_t Play(const Calls& calls, uint32_t movie, uint32_t named, uint32_t frame, uint32_t lr) {
  const uint64_t args[6] = {movie, named, 1, 0, 1, 0};
  return calls.call_set(kPlayNamed, args, 6, frame, lr);
}

// ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0): its plain C++, r4 left 0
// as the original sets it; FadeOut's r3 (the passes).
uint64_t FadeOut(uint8_t* base, const Calls& calls) {
  const uint32_t passes = LoadU32(base, kFadeOutPasses);
  mode_tick::ScreenFadesFadeOut(base, passes, 0, 0, 0, 0);
  calls.set_r4(0);
  return passes;
}

}  // namespace

// --- The leaves ---

uint64_t InitWords(uint8_t* base, uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82238420 Type8PanelScreen::InitWords");
  const uint32_t screen = uint32_t(r3);
  StoreU32(base, screen + 13192, 3);
  StoreU32(base, screen + 15164, 3);
  StoreU32(base, screen + 13200, 4);
  StoreU32(base, screen + 15172, 4);
  return r3;
}

uint64_t IsLoaded(uint8_t* base) {
  ITC_NATIVE_RUNNING("sub_82238418 Type8PanelScreen::IsLoaded");
  return ui_movies::IsLoaded(base, 4);
}

uint64_t OpenMovies(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238440 Type8PanelScreen::OpenMovies");
  return calls.open(4, 22586, sp, lr);
}

uint64_t DeleteMovies(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238450 Type8PanelScreen::DeleteMovies");
  return calls.remove(4, sp, lr);
}

uint64_t SetWord(uint8_t* base, uint64_t r3, uint32_t function) {
  uint32_t at = 0, value = 0;
  switch (function) {
    case 0x82238458:
      ITC_NATIVE_RUNNING("sub_82238458 Type8PanelScreen::OnMessage2");
      at = 15228;
      value = 1;
      break;
    case 0x82238530:
      ITC_NATIVE_RUNNING("sub_82238530 Type8PanelScreen::OnSaveLoaded");
      at = 15232;
      value = 0;
      break;
    case 0x82238540:
      ITC_NATIVE_RUNNING("sub_82238540 Type8PanelScreen::OnAnswer0");
      at = 15244;
      value = 0;
      break;
    case 0x82238550:
      ITC_NATIVE_RUNNING("sub_82238550 Type8PanelScreen::OnAnswer1");
      at = 15244;
      value = 1;
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82238560 Type8PanelScreen::OnAnswer2");
      at = 15244;
      value = 2;
      break;
  }
  StoreU32(base, uint32_t(r3) + at, value);
  return r3;
}

uint64_t Leave(uint8_t* base, uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82238B10 Type8PanelScreen::Leave");
  const uint32_t screen = uint32_t(r3);
  if ((LoadU32(base, type67_screens::kFadeCountA) | LoadU32(base, type67_screens::kFadeCountB)) !=
      0) {
    return r3;
  }
  uint32_t message = 4;
  if (LoadU32(base, screen + 15240) == 0) {
    StoreU32(base, screen + 9608, 9);
    message = 6;
  }
  type67_screens::Post(base, uint32_t(r4), message);
  StoreU32(base, screen + 9656, 0);
  return r3;
}

// --- The handlers ---

uint64_t ShowPlayerNodes(uint8_t* base, uint64_t r3, uint64_t first, uint64_t second,
                         uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82238570 Type8PanelScreen::ShowPlayerNodes");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  const uint32_t m = uint32_t(ts::PanelGroupMovie(base, screen + kGroup, frame));
  // The node and its first child, read again for each call as the original
  // does.
  auto node = [&] { return LoadU32(base, screen + 11256); };
  auto child = [&](uint32_t k) { return LoadU32(base, screen + 11260) + k; };
  uint64_t result = 0;
  auto shown = [&](uint32_t sub_scene, uint32_t c, uint32_t on) {
    const uint32_t found = ui_movies::FindNode(base, m, sub_scene, c);
    StoreU32(base, found + 80, on);
    result = found;
  };
  auto texture = [&](uint32_t sub_scene, uint32_t c, uint32_t t) {
    result = ui_movies::SetNodeTexture(base, ui_movies::FindNode(base, m, sub_scene, c), t);
  };
  const bool a = uint32_t(first) != 0, b = uint32_t(second) != 0;
  shown(0xFFFFFFFFu, node(), a || b ? 1 : 0);
  if (a) {
    shown(node(), child(0), 1);
    shown(node(), child(1), 1);
    texture(node(), child(0), 0);
    texture(node(), child(1), 0);
  } else {
    shown(node(), child(0), 0);
    shown(node(), child(1), 0);
  }
  if (b) {
    shown(node(), child(2), 1);
    shown(node(), child(3), 1);
    texture(node(), child(2), 1);
    texture(node(), child(3), 1);
  } else {
    shown(node(), child(2), 0);
    shown(node(), child(3), 0);
  }
  shown(node(), child(4), 0);
  shown(node(), child(5), 0);
  return result;
}

uint64_t FadeInGroup(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238468 Type8PanelScreen::FadeInGroup");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  if (LoadU32(base, screen + 15228) == 0 || LoadU32(base, screen + 15236) != 0) {
    return r3;
  }
  ts::PanelFadeIn(base, LoadU32(base, kFadeInPasses), frame, 0x822384A4, calls);
  const uint32_t m = uint32_t(ts::PanelGroupMovie(base, screen + kGroup, frame));
  calls.call(kFinishNamed, frame, 0x822384B8, m, LoadU32(base, screen + 11264));
  Play(calls, m, LoadU32(base, screen + 11264), frame, 0x822384D4);
  uint64_t result = ui_movies::IsNamedActive(base, m, LoadU32(base, screen + 11268));
  if (uint32_t(result) == 0) {
    Play(calls, m, LoadU32(base, screen + 11268), frame, 0x82238504);
    result = calls.call(kLoopNamed, frame, 0x82238510, m, LoadU32(base, screen + 11268));
  }
  StoreU32(base, screen + 15236, 1);
  return result;
}

uint64_t UpdateOpening(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238A00 Type8PanelScreen::UpdateOpening");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  if (LoadU32(base, screen + 15228) == 0) {
    return r3;
  }
  ShowPlayerNodes(base, r3, 1, 0, frame);
  // The holder's slots 2 (WaitForFade) and 1 (Run), the original setting
  // only r3: r4 as ShowPlayerNodes leaves it (the node +11256, its last
  // HideNode's), then as the first slot leaves it.
  const uint64_t holder = r3 + kHolder;
  ts::Slot(base, holder, 8, uint64_t(LoadU32(base, screen + 11256)), frame, 0x82238A44, calls);
  uint64_t result = ts::Slot(base, holder, 4, std::nullopt, frame, 0x82238A58, calls);
  if (LoadU32(base, screen + 15232) != 0) {
    return result;
  }
  const uint32_t m = uint32_t(ts::PanelGroupMovie(base, screen + kGroup, frame));
  result = ui_movies::IsNamedActive(base, m, LoadU32(base, screen + 11264));
  if (uint32_t(result) != 0 || LoadU32(base, ts::kFadeWait) != 0) {
    return result;
  }
  if (LoadU32(base, kModeBefore) == 1) {
    StoreU32(base, screen + 9608, 9);
    StoreU32(base, screen + 15240, 0);
    return result;
  }
  ts::Post(base, uint32_t(r4), 29);
  StoreU32(base, screen + 15232, 1);
  StoreU32(base, screen + 15240, 1);
  return result;
}

uint64_t Show(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238928 Type8PanelScreen::Show");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  ts::PanelScreenShow(base, r3, r4, frame, calls);
  if (LoadU32(base, screen + 15240) == 0) {
    // ScreenFades::FadeIn([0x82001BEC], 0, 0, 0, 0): its plain C++.
    mode_tick::ScreenFadesFadeIn(base, LoadU32(base, kFadeInPasses), 0, 0, 0, 0);
    calls.set_r4(0);
  }
  return ShowPlayerNodes(base, r3, 0, 0, frame);
}

uint64_t AskQuit(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t player, uint32_t sp,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238BA0 Type8PanelScreen::AskQuit");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 128);
  StoreU32(base, frame + 80, kDialogMovie);
  const uint32_t movie =
      LoadU32(base, ui_movies::PairsFind(base, ui_movies::kMovieTable, frame + 80));
  calls.set_text(frame, 0x82238BE8, movie, 0xFFFFFFFFu, 0, 27);
  calls.set_text(frame, 0x82238BFC, movie, 1, 3, 2);
  calls.set_text(frame, 0x82238C10, movie, 1, 4, 3);
  StoreU32(base, movie + kDialogCursor, 1);
  StoreU32(base, kDialogPlayer, player);
  ts::Post(base, uint32_t(r4), 7);
  StoreU32(base, screen + 9656, 1);
  StoreU32(base, screen + 15244, 0xFFFFFFFFu);
  return ~uint64_t(0);
}

uint64_t DrawList(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238DC8 Type8PanelScreen::DrawList");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 112);
  const uint64_t result = ts::PanelScreenDrawList(base, r3, r4, frame, calls);
  for (uint32_t player = 0; player < 2; player++) {
    if (LoadU32(base, screen + 13220 + 1972 * player) == 0) {
      continue;
    }
    if (LoadU32(base, screen + 15240) != 0) {
      AskQuit(base, r3, r4, player, frame, calls);
      return ShowPlayerNodes(base, r3, 1, 1, frame);
    }
    const uint64_t passes = FadeOut(base, calls);
    StoreU32(base, screen + 9656, 2);
    return passes;
  }
  return result;
}

uint64_t Draw(uint8_t* base, uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238E78 Type8PanelScreen::Draw");
  const int32_t quit = int32_t(LoadU32(base, uint32_t(r3) + 9656));
  if (quit == 0) {
    return DrawList(base, r3, r4, sp, calls);
  }
  if (quit == 2) {
    return Leave(base, r3, r4);
  }
  return r3;
}

uint64_t OnDialogClosed(uint8_t* base, uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238990 Type8PanelScreen::OnDialogClosed");
  const uint32_t screen = uint32_t(r3), frame = Frame(base, sp, 96);
  uint32_t dialog = 0;
  if (LoadU32(base, screen + 15244) == 0) {
    FadeOut(base, calls);
    dialog = 2;
  }
  StoreU32(base, screen + 9656, dialog);
  return ShowPlayerNodes(base, r3, 0, 0, frame);
}

}  // namespace type8_screens
