#include "movies/movie.h"

#include "guest.h"

#include <map>
#include <memory>
#include <mutex>
#include <string>

#include "base/log.h"
#include "game_data/game_data.h"
#include "movies/player.h"
#include "native/renderer.h"
#include "native/state.h"
#include "schema/device_shaders.h"
#include "schema/movies.h"

namespace movies::movie {

namespace {

// The players by movie object. Play and Release run on the main thread,
// RenderFrame on the render thread, which keeps its own reference while it
// draws (the original's Release waited for the frame on the device lock).
std::mutex g_mutex;
std::map<uint32_t, std::shared_ptr<Player>> g_players;

std::shared_ptr<Player> PlayerOf(uint32_t movie) {
  std::lock_guard<std::mutex> lock(g_mutex);
  auto it = g_players.find(movie);
  return it == g_players.end() ? nullptr : it->second;
}

bool CommandDone(uint32_t movie) {
  return schema::Movie(movie).command_status() == kCommandDone;
}

void SetState(uint32_t movie, uint32_t state) {
  schema::Movie(movie).set_state(state);
}

std::string GuestString(uint32_t address, uint32_t max) {
  std::string s;
  for (uint32_t i = 0; address && i < max; i++) {
    char c = char(guest::LoadU8(address + i));
    if (!c) {
      break;
    }
    s += c;
  }
  return s;
}

}  // namespace

void Play(uint32_t movie, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_820AEC10 Movie::Play");
  // strncpy(+48, path, 260): the path, then zeros to 260 bytes.
  std::string name = GuestString(path, kPathBytes);
  // The movie's in host memory (E5 step 4): the path's bytes, guest order.
  uint8_t* const to = schema::Movie(movie).path();
  for (uint32_t i = 0; i < kPathBytes; i++) {
    to[i] = i < name.size() ? uint8_t(name[i]) : 0;
  }
  std::shared_ptr<Player> player;
  std::string error = "isn't in the game's data";
  if (auto relative = game_data::RelativePath(name)) {
    if (auto file = game_data::Find(*relative)) {
      player = Player::Open(*file, &error);
    }
  }
  if (player) {
    ITC_LOG_INFO("movies: {} opened{}", name, player->HasSound() ? "" : " (no sound)");
  } else {
    ITC_LOG_ERROR("movies: {} {}; it ends at once", name, error);
  }
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    g_players[movie] = player;
  }
  schema::Movie(movie).set_command_status(kCommandDone);
  SetState(movie, kLoaded);
}

void Start(uint32_t movie) {
  ITC_NATIVE_RUNNING("sub_820AE4E0 Movie::Start");
  if (!CommandDone(movie) || GetState(movie) == kNone) {
    return;
  }
  SetState(movie, kPlaying);
  if (std::shared_ptr<Player> player = PlayerOf(movie)) {
    player->Start();
    SetSounding(player.get());
  }
}

void Stop(uint32_t movie) {
  ITC_NATIVE_RUNNING("sub_820AE828 Movie::Stop");
  uint32_t state = GetState(movie);
  if (!CommandDone(movie) || (state != kPlaying && state != kPaused)) {
    return;
  }
  SetState(movie, kLoaded);
  if (std::shared_ptr<Player> player = PlayerOf(movie)) {
    player->Pause();
    SetSounding(nullptr);
  }
}

void Pause(uint32_t movie) {
  ITC_NATIVE_RUNNING("sub_820AE898 Movie::Pause");
  if (!CommandDone(movie) || GetState(movie) != kPlaying) {
    return;
  }
  SetState(movie, kPaused);
  if (std::shared_ptr<Player> player = PlayerOf(movie)) {
    player->Pause();
  }
}

void Resume(uint32_t movie) {
  ITC_NATIVE_RUNNING("sub_820AE8F8 Movie::Resume");
  if (!CommandDone(movie) || GetState(movie) != kPaused) {
    return;
  }
  SetState(movie, kPlaying);
  if (std::shared_ptr<Player> player = PlayerOf(movie)) {
    player->Resume();
  }
}

uint32_t GetState(uint32_t movie) {
  return schema::Movie(movie).state();
}

void Release(uint32_t movie) {
  ITC_NATIVE_RUNNING("sub_820AE528 Movie::Release");
  if (!CommandDone(movie) || GetState(movie) == kPlaying) {
    return;
  }
  std::shared_ptr<Player> player;
  {
    std::lock_guard<std::mutex> lock(g_mutex);
    auto it = g_players.find(movie);
    if (it == g_players.end()) {
      return;
    }
    player = std::move(it->second);
    g_players.erase(it);
  }
  SetSounding(nullptr);
}

void RenderFrame(uint32_t movie) {
  ITC_NATIVE_RUNNING("sub_820AE958 Movie::RenderFrame");
  std::shared_ptr<Player> player = PlayerOf(movie);
  if (!player) {
    // Not opened (the file's missing): it ends at once.
    SetState(movie, kLoaded);
    return;
  }
  if (const Frame* frame = player->CurrentFrame()) {
    native::Renderer_DrawMovie(schema::DeviceShadersGlobals::replay_device(),
                               frame->width, frame->height, frame->y.data(), frame->u.data(),
                               frame->v.data());
  }
  if (player->Ended()) {
    SetState(movie, kLoaded);
  }
}

}  // namespace movies::movie
