// The type 8 panel screen's slots and handlers
// (docs/functions/type8-screens.md): see type8_screens.h. Read from the
// generated code.

#include "type8_screens/type8_screens.h"

#include "base/log.h"
#include "guest.h"
#include "mode_tick/mode_tick.h"
#include "schema/tuning_screen.h"
#include "schema/type67_screens.h"
#include "schema/type8_screens.h"
#include "ui_movies/game.h"

namespace type8_screens {

namespace {

using guest::LoadU32;
using guest::StoreU32;

namespace ts = type67_screens;
using G = schema::Type8ScreensGlobals;

constexpr uint32_t kPlayNamed = 0x8211DAC8;
constexpr uint32_t kDialogMovie = 16, kDialogCursor = 40960;

uint32_t Frame(uint32_t sp, uint32_t bytes) {
  StoreU32(sp - bytes, sp);
  return sp - bytes;
}

// PlayNamed(movie, named, 1, 0, 1, 0) from `frame` returning to `lr`.
uint64_t Play(const Calls& calls, uint32_t movie, uint32_t named, uint32_t frame, uint32_t lr) {
  const uint64_t args[6] = {movie, named, 1, 0, 1, 0};
  return calls.call_set(kPlayNamed, args, 6, frame, lr);
}

// ScreenFades::FadeOut([0x82001BF0], 0, 0, 0, 0): its plain C++, r4 left 0
// as the original sets it; FadeOut's r3 (the passes).
uint64_t FadeOut(const Calls& calls) {
  const uint32_t passes = G::fade_out_passes();
  mode_tick::ScreenFadesFadeOut(passes, 0, 0, 0, 0);
  calls.set_r4(0);
  return passes;
}

}  // namespace

// --- The leaves ---

uint64_t InitWords(uint64_t r3) {
  ITC_NATIVE_RUNNING("sub_82238420 Type8PanelScreen::InitWords");
  const uint32_t screen = uint32_t(r3);
  const schema::Type8PanelScreen s(screen);
  s.set_list0_rows_shown(3);
  s.set_list1_rows_shown(3);
  s.set_list0_colors_shown(4);
  s.set_list1_colors_shown(4);
  return r3;
}

uint64_t IsLoaded() {
  ITC_NATIVE_RUNNING("sub_82238418 Type8PanelScreen::IsLoaded");
  return ui_movies::IsLoaded(4);
}

uint64_t OpenMovies(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238440 Type8PanelScreen::OpenMovies");
  return calls.open(4, 22586, sp, lr);
}

uint64_t DeleteMovies(uint32_t sp, uint32_t lr, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238450 Type8PanelScreen::DeleteMovies");
  return calls.remove(4, sp, lr);
}

uint64_t SetWord(uint64_t r3, uint32_t function) {
  const schema::Type8PanelScreen s{uint32_t(r3)};
  switch (function) {
    case 0x82238458:
      ITC_NATIVE_RUNNING("sub_82238458 Type8PanelScreen::OnMessage2");
      s.set_message2_got(1);
      break;
    case 0x82238530:
      ITC_NATIVE_RUNNING("sub_82238530 Type8PanelScreen::OnSaveLoaded");
      s.set_load_asked(0);
      break;
    case 0x82238540:
      ITC_NATIVE_RUNNING("sub_82238540 Type8PanelScreen::OnAnswer0");
      s.set_dialog_answer(0);
      break;
    case 0x82238550:
      ITC_NATIVE_RUNNING("sub_82238550 Type8PanelScreen::OnAnswer1");
      s.set_dialog_answer(1);
      break;
    default:
      ITC_NATIVE_RUNNING("sub_82238560 Type8PanelScreen::OnAnswer2");
      s.set_dialog_answer(2);
      break;
  }
  return r3;
}

uint64_t Leave(uint64_t r3, uint64_t r4) {
  ITC_NATIVE_RUNNING("sub_82238B10 Type8PanelScreen::Leave");
  const uint32_t screen = uint32_t(r3);
  const schema::Type8PanelScreen s(screen);
  if ((schema::Type67ScreensGlobals::fade_out_count() |
       schema::Type67ScreensGlobals::fade_in_count()) != 0) {
    return r3;
  }
  uint32_t message = 4;
  if (s.loaded_this_visit() == 0) {
    s.set_state(9);
    message = 6;
  }
  type67_screens::Post(uint32_t(r4), message);
  s.set_draw_mode(0);
  return r3;
}

// --- The handlers ---

uint64_t ShowPlayerNodes(uint64_t r3, uint64_t first, uint64_t second,
                         uint32_t sp) {
  ITC_NATIVE_RUNNING("sub_82238570 Type8PanelScreen::ShowPlayerNodes");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 128);
  const schema::Type8PanelScreen s(screen);
  const uint32_t m = uint32_t(ts::PanelGroupMovie(s.group_address(), frame));
  // The node and its first child, read again for each call as the original
  // does.
  auto node = [&] { return s.player_node(); };
  auto child = [&](uint32_t k) { return s.player_child() + k; };
  uint64_t result = 0;
  auto shown = [&](uint32_t sub_scene, uint32_t c, uint32_t on) {
    const uint32_t found = ui_movies::FindNode(m, sub_scene, c);
    schema::UiMovieNode(found).set_visible(on);
    result = found;
  };
  auto texture = [&](uint32_t sub_scene, uint32_t c, uint32_t t) {
    result = ui_movies::SetNodeTexture(ui_movies::FindNode(m, sub_scene, c), t);
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

uint64_t FadeInGroup(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238468 Type8PanelScreen::FadeInGroup");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  const schema::Type8PanelScreen s(screen);
  if (s.message2_got() == 0 || s.opening_played() != 0) {
    return r3;
  }
  ts::PanelFadeIn(G::fade_in_passes(), frame, 0x822384A4, calls);
  const uint32_t m = uint32_t(ts::PanelGroupMovie(s.group_address(), frame));
  calls.call(kFinishNamed, frame, 0x822384B8, m, s.open_anim());
  Play(calls, m, s.open_anim(), frame, 0x822384D4);
  uint64_t result = ui_movies::IsNamedActive(m, s.loop_anim());
  if (uint32_t(result) == 0) {
    Play(calls, m, s.loop_anim(), frame, 0x82238504);
    result = calls.call(kLoopNamed, frame, 0x82238510, m, s.loop_anim());
  }
  s.set_opening_played(1);
  return result;
}

uint64_t UpdateOpening(uint64_t r3, uint64_t r4, uint32_t sp,
                       const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238A00 Type8PanelScreen::UpdateOpening");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  const schema::Type8PanelScreen s(screen);
  if (s.message2_got() == 0) {
    return r3;
  }
  ShowPlayerNodes(r3, 1, 0, frame);
  // The holder's slots 2 (WaitForFade) and 1 (Run), the original setting
  // only r3: r4 as ShowPlayerNodes leaves it (the node +11256, its last
  // HideNode's), then as the first slot leaves it.
  const uint64_t holder = r3 + kHolder;
  ts::Slot(holder, 8, uint64_t(s.player_node()), frame, 0x82238A44, calls);
  uint64_t result = ts::Slot(holder, 4, std::nullopt, frame, 0x82238A58, calls);
  if (s.load_asked() != 0) {
    return result;
  }
  const uint32_t m = uint32_t(ts::PanelGroupMovie(s.group_address(), frame));
  result = ui_movies::IsNamedActive(m, s.open_anim());
  if (uint32_t(result) != 0 || schema::Type67ScreensGlobals::fade_wait() != 0) {
    return result;
  }
  if (G::mode_before() == 1) {
    s.set_state(9);
    s.set_loaded_this_visit(0);
    return result;
  }
  ts::Post(uint32_t(r4), 29);
  s.set_load_asked(1);
  s.set_loaded_this_visit(1);
  return result;
}

uint64_t Show(uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238928 Type8PanelScreen::Show");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  const schema::Type8PanelScreen s(screen);
  ts::PanelScreenShow(r3, r4, frame, calls);
  if (s.loaded_this_visit() == 0) {
    // ScreenFades::FadeIn([0x82001BEC], 0, 0, 0, 0): its plain C++.
    mode_tick::ScreenFadesFadeIn(G::fade_in_passes(), 0, 0, 0, 0);
    calls.set_r4(0);
  }
  return ShowPlayerNodes(r3, 0, 0, frame);
}

uint64_t AskQuit(uint64_t r3, uint64_t r4, uint32_t player, uint32_t sp,
                 const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238BA0 Type8PanelScreen::AskQuit");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 128);
  const schema::Type8PanelScreen s(screen);
  StoreU32(frame + 80, kDialogMovie);
  const uint32_t movie =
      LoadU32(ui_movies::PairsFind(ui_movies::kMovieTable, frame + 80));
  calls.set_text(frame, 0x82238BE8, movie, 0xFFFFFFFFu, 0, 27);
  calls.set_text(frame, 0x82238BFC, movie, 1, 3, 2);
  calls.set_text(frame, 0x82238C10, movie, 1, 4, 3);
  StoreU32(movie + kDialogCursor, 1);
  G::set_dialog_player(player);
  ts::Post(uint32_t(r4), 7);
  s.set_draw_mode(1);
  s.set_dialog_answer(0xFFFFFFFFu);
  return ~uint64_t(0);
}

uint64_t DrawList(uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238DC8 Type8PanelScreen::DrawList");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 112);
  const schema::Type8PanelScreen s(screen);
  const uint64_t result = ts::PanelScreenDrawList(r3, r4, frame, calls);
  for (uint32_t player = 0; player < 2; player++) {
    if ((player == 0 ? s.list0_cancelled() : s.list1_cancelled()) == 0) {
      continue;
    }
    if (s.loaded_this_visit() != 0) {
      AskQuit(r3, r4, player, frame, calls);
      return ShowPlayerNodes(r3, 1, 1, frame);
    }
    const uint64_t passes = FadeOut(calls);
    s.set_draw_mode(2);
    return passes;
  }
  return result;
}

uint64_t Draw(uint64_t r3, uint64_t r4, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238E78 Type8PanelScreen::Draw");
  const int32_t quit = int32_t(schema::Type8PanelScreen(uint32_t(r3)).draw_mode());
  if (quit == 0) {
    return DrawList(r3, r4, sp, calls);
  }
  if (quit == 2) {
    return Leave(r3, r4);
  }
  return r3;
}

uint64_t OnDialogClosed(uint64_t r3, uint32_t sp, const Calls& calls) {
  ITC_NATIVE_RUNNING("sub_82238990 Type8PanelScreen::OnDialogClosed");
  const uint32_t screen = uint32_t(r3), frame = Frame(sp, 96);
  const schema::Type8PanelScreen s(screen);
  uint32_t dialog = 0;
  if (s.dialog_answer() == 0) {
    FadeOut(calls);
    dialog = 2;
  }
  s.set_draw_mode(dialog);
  return ShowPlayerNodes(r3, 0, 0, frame);
}

}  // namespace type8_screens
