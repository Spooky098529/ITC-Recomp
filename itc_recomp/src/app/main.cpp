// The program's start ("A4 brought forward" in docs/rexglue-removal.md):
// our own `main`, which sets up the flags, the paths, the log and SDL, maps
// guest memory (src/memory/mapping.h), loads default.xex into it
// (src/app/image_load.h), starts our audio output and the game's main
// thread (one of ours, in our start-up, src/app/start_up.cpp, in place of
// the game's xstart), and runs the main thread's event loop. Since E2 (step
// 5) the SDK's Runtime (its memory, kernel, loader and VFS) isn't made; the
// SDK is used only as a library (the flags, the log, paths). Before 2026-11-15 the SDK's main() and its ReXApp
// did all this (rexglue-sdk/src/ui/windowed_app_main_sdl.cpp and
// rex_app.cpp, no longer built into our executable); what they did that we
// don't need (their window, ImGui overlays, achievement toasts, the GPU
// plugin) is left out.

#include "app/app.h"
#include "threads/threads.h"
#include "memory/kernel_variables.h"
#include "app/image_load.h"
#include "memory/mapping.h"

#include "audio/native_audio.h"
#include "base/flags.h"
#include "base/log.h"
#include "base/log_sdk.h"
#include "debug/thread_cpu.h"
#include "game_data/game_data.h"
#include "guest_code.h"
#include "input/gamepads.h"

#include <SDL3/SDL.h>
#include <fmt/format.h>
#include <SDL3/SDL_main.h>
#include <rex/cvar.h>
#include <rex/filesystem.h>
#include <rex/logging.h>

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

// The SDK's flags this start-up reads (defined in its library).
ITC_FLAG_DECLARE(std::string, game_data_root);
ITC_FLAG_DECLARE(std::string, user_data_root);
ITC_FLAG_DECLARE(std::string, update_data_root);
ITC_FLAG_DECLARE(std::string, cache_root);
ITC_FLAG_DECLARE(std::string, metadata_root);
ITC_FLAG_DECLARE(std::string, log_file);
ITC_FLAG_DECLARE(std::string, log_level);
ITC_FLAG_DECLARE(bool, log_verbose);
ITC_FLAG_DECLARE(std::string, video_driver);

namespace app {
namespace {

constexpr const char* kName = "itc";

struct Paths {
  std::filesystem::path game_data;
  std::filesystem::path user_data;
  std::filesystem::path update_data;
  std::filesystem::path cache;
  std::filesystem::path metadata;
  std::filesystem::path config;
};

std::atomic<bool> g_quitting{false};

std::mutex g_queue_mutex;
std::deque<std::function<void()>> g_queue;
uint32_t g_wake_event = 0;

std::filesystem::path FlagPath(const std::string& value) {
  return value.empty() ? std::filesystem::path{} : std::filesystem::path(value);
}

// The paths, as ReXApp::SetupEnvironment worked them out (with our
// OnConfigurePaths: without --game_data_root, the executable's folder).
Paths ResolvePaths() {
  const std::filesystem::path exe_dir = rex::filesystem::GetExecutableFolder();
  Paths paths;
  paths.game_data = FlagPath(ITC_FLAG(game_data_root));
  if (paths.game_data.empty()) {
    paths.game_data = exe_dir;
  }
  paths.user_data = FlagPath(ITC_FLAG(user_data_root));
  if (paths.user_data.empty()) {
    paths.user_data = rex::filesystem::GetUserFolder() / kName;
  }
  paths.update_data = FlagPath(ITC_FLAG(update_data_root));
  paths.cache = FlagPath(ITC_FLAG(cache_root));
  if (paths.cache.empty()) {
    paths.cache = paths.user_data / "cache";
  }
  paths.metadata = FlagPath(ITC_FLAG(metadata_root));
  paths.config = exe_dir / (std::string(kName) + ".toml");
  return paths;
}

// itc.toml, then the log (ours taking over the file the SDK's logger
// opens: src/base/log_sdk.cpp).
void SetUpLogging(const Paths& paths) {
  if (std::filesystem::exists(paths.config)) {
    rex::cvar::LoadConfig(paths.config);
  }
  std::string level = ITC_FLAG(log_level);
  if (ITC_FLAG(log_verbose) && level == "info") {
    level = "trace";
  }
  const std::string& file = ITC_FLAG(log_file);
  auto config = rex::BuildLogConfig(file.empty() ? nullptr : file.c_str(), level,
                                    rex::ParseCategoryLevelsFromConfig(paths.config));
  if (file.empty()) {
    config.app_name = kName;
    config.log_dir = (rex::filesystem::GetExecutableFolder() / "logs").string();
  }
  rex::InitLogging(config);
  rex::RegisterLogLevelCallback();
  base::TakeOverSdkLogFile();
  ITC_LOG_INFO("{} starting", kName);
  ITC_LOG_INFO("  Game directory: {}", paths.game_data.string());
  ITC_LOG_INFO("  User data:      {}", paths.user_data.string());
}

bool SetUpSdl() {
  if (const std::string& driver = ITC_FLAG(video_driver); !driver.empty()) {
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, driver.c_str());
  }
  if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
    ITC_LOG_ERROR("SDL_InitSubSystem(SDL_INIT_VIDEO) failed: {}", SDL_GetError());
    return false;
  }
  g_wake_event = SDL_RegisterEvents(1);
  if (g_wake_event == 0) {
    ITC_LOG_ERROR("SDL_RegisterEvents failed: {}", SDL_GetError());
    return false;
  }
  return true;
}

void Fail(const std::string& message) {
  ITC_LOG_ERROR("{}", message);
  SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, kName, message.c_str(), nullptr);
}

// The game's files: default.xex where they are, and the root our file
// layer reads from (src/game_data/).
bool CheckGameData(const Paths& paths) {
  if (!std::filesystem::is_directory(paths.game_data)) {
    Fail("--game_data_root does not exist: " + paths.game_data.string());
    return false;
  }
  if (!std::filesystem::is_regular_file(paths.game_data / "default.xex")) {
    Fail("Entrypoint XEX not found: " + (paths.game_data / "default.xex").string());
    return false;
  }
  game_data::SetRoot(paths.game_data);
  return true;
}

void Wake() {
  SDL_Event event{};
  event.type = g_wake_event;
  SDL_PushEvent(&event);
}

void RunQueued() {
  std::deque<std::function<void()>> functions;
  {
    std::lock_guard lock(g_queue_mutex);
    functions.swap(g_queue);
  }
  for (auto& function : functions) {
    function();
  }
}

// Closing: as ReXApp::OnClosing did, less the SDK's title termination (the
// game's threads are ours and end with the process): the log flushed, the
// process exited at once.
[[noreturn]] void Close() {
  ITC_LOG_INFO("Window closing, shutting down...");
  ITC_LOG_INFO("Hard-exiting process.");
  rex::FlushLogging();
  std::_Exit(0);
}

// The main thread's loop: SDL's events (SDL_RunOnMainThread's callbacks run
// in SDL_WaitEvent), our queue, and a close.
void RunLoop() {
  while (!g_quitting.load()) {
    SDL_Event event;
    if (!SDL_WaitEvent(&event)) {
      ITC_LOG_ERROR("SDL_WaitEvent failed: {}", SDL_GetError());
      break;
    }
    if (event.type == g_wake_event) {
      RunQueued();
    } else if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
      Close();
    }
  }
  Close();
}

}  // namespace

void RunOnMainThread(std::function<void()> function) {
  {
    std::lock_guard lock(g_queue_mutex);
    g_queue.push_back(std::move(function));
  }
  Wake();
}

void RequestQuit() {
  g_quitting = true;
  Wake();
}

}  // namespace app

int main(int argc, char* argv[]) {
  rex::cvar::Init(argc, argv);
  rex::cvar::ApplyEnvironment();
  rex::InitLoggingEarly();
  const app::Paths paths = app::ResolvePaths();
  app::SetUpLogging(paths);
  // Guest memory (ours: src/memory/mapping.h), before anything that
  // allocates in it.
  if (!memory::MapGuest()) {
    app::Fail("Failed to map guest memory");
    return EXIT_FAILURE;
  }
  if (!app::SetUpSdl() || !app::CheckGameData(paths)) {
    return EXIT_FAILURE;
  }
  debug::ThreadCpu_Start();
  input::Gamepads_Start([](std::function<void()> function) {
    app::RunOnMainThread(std::move(function));
  });
  // The image in guest memory (our own loader: src/app/image_load.cpp), the
  // kernel variables bound.
  uint8_t* base = memory::Base();
  app::Image image;
  std::string error;
  if (!app::LoadImage(paths.game_data / "default.xex", image, error)) {
    app::Fail("Failed to load default.xex: " + error);
    return EXIT_FAILURE;
  }
  std::memcpy(base + image.load_address, image.bytes.data(), image.bytes.size());
  memory::BindKernelVariables(base);
  native_audio::Start();
  // The game's main thread: one of our threads (E1, src/threads/) running
  // our start-up (src/app/start_up.cpp) with the image's stack size.
  if (!threads::Enabled()) {
    app::Fail("--native_threads=false is no longer supported");
    return EXIT_FAILURE;
  }
  std::thread main_thread = threads::StartMainThread(base, image.stack_size, app::StartUp);
  std::thread waiter([thread = std::move(main_thread)]() mutable {
    thread.join();
    ITC_LOG_INFO("Execution complete");
    app::RequestQuit();
  });
  waiter.detach();
  app::RunLoop();
  return EXIT_SUCCESS;
}
