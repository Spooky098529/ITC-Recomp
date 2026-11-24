// The program's start ("A4 brought forward" in docs/rexglue-removal.md):
// our own `main`, which sets up the flags, the paths, the log and SDL, maps
// guest memory (src/memory/mapping.h), writes the image's data into it
// (compiled in since E5's step 8: src/image/image.h), starts our audio output and the game's main
// thread (one of ours, in our start-up, src/app/start_up.cpp, in place of
// the game's xstart), and runs the main thread's event loop. Since E2 (step
// 5) the SDK's Runtime (its memory, kernel, loader and VFS) isn't made, and
// since A4 (E4's step 4) the flags, the paths and the log file are ours too.
// Before 2026-11-15 the SDK's main() and its ReXApp
// did all this (rexglue-sdk/src/ui/windowed_app_main_sdl.cpp and
// rex_app.cpp, no longer built into our executable); what they did that we
// don't need (their window, ImGui overlays, achievement toasts, the GPU
// plugin) is left out.

#include "app/app.h"
#include "threads/threads.h"
#include "memory/kernel_variables.h"
#include "image/image.h"
#include "memory/mapping.h"

#include "audio/native_audio.h"
#include "base/flags.h"
#include "base/log.h"
#include "debug/thread_cpu.h"
#include "game_data/game_data.h"
#include "guest_code.h"
#include "input/gamepads.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <atomic>
#include <cstring>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

// The start-up's flags (the SDK's until A4, same names and defaults; its
// user_data_root, update_data_root, cache_root and metadata_root, which
// nothing used, are gone: given, they're reported as unknown).
ITC_FLAG_STRING(game_data_root, "", "Runtime",
                "The game's files (DATA/); empty: the executable's folder. "
                "itc.toml may set it too");
ITC_FLAG_STRING(log_level, "info", "Log",
                "Log level: trace, debug, info, warn, error, critical, off (off: no log file)");
ITC_FLAG_STRING(log_file, "", "Log", "Log file path (empty: logs/itc_<n>.log, the next number)");
ITC_FLAG_BOOL(log_verbose, false, "Log", "Log at trace (debug) when log_level is info");
ITC_FLAG_STRING(video_driver, "", "UI/Window",
                "SDL video driver to use, such as \"wayland\" or \"x11\". Empty picks SDL's "
                "default for the session");

namespace app {
namespace {

constexpr const char* kName = "itc";

struct Paths {
  std::filesystem::path game_data;
  std::filesystem::path config;
};

std::atomic<bool> g_quitting{false};

std::mutex g_queue_mutex;
std::deque<std::function<void()>> g_queue;
uint32_t g_wake_event = 0;

// The executable's folder (itc.toml, logs/ and, by default, the game's
// files are there).
std::filesystem::path ExecutableFolder() {
  std::error_code error;
  const std::filesystem::path exe = std::filesystem::read_symlink("/proc/self/exe", error);
  return error ? std::filesystem::current_path() : exe.parent_path();
}

// The game's files: --game_data_root (the command line or itc.toml), else
// the executable's folder.
Paths ResolvePaths(const std::filesystem::path& exe_dir) {
  Paths paths;
  paths.config = exe_dir / (std::string(kName) + ".toml");
  const std::string& root = ITC_FLAG(game_data_root);
  paths.game_data = root.empty() ? exe_dir : std::filesystem::path(root);
  return paths;
}

// The log: our file (logs/itc_<n>.log next to the executable, or --log_file;
// none with log_level off; the shader translator's lines come into it
// through its shims, src/translator_shims/).
void SetUpLogging(const std::filesystem::path& exe_dir, const Paths& paths,
                  const std::vector<std::string>& others) {
  std::string level = ITC_FLAG(log_level);
  if (ITC_FLAG(log_verbose) && level == "info") {
    level = "trace";
  }
  if (level == "off") {
    base::StartLogFile("", level);
  } else {
    const std::string& file = ITC_FLAG(log_file);
    base::StartLogFile(file.empty() ? base::NextLogPath(exe_dir / "logs", kName).string() : file,
                       level);
  }
  ITC_LOG_INFO("{} starting", kName);
  ITC_LOG_INFO("  Game directory: {}", paths.game_data.string());
  base::flags::WarnAboutProblems();
  for (const std::string& other : others) {
    ITC_LOG_WARN("flags: not a flag on the command line: {}", other);
  }
  base::LogFlags();
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

// The game's files: the root our file layer reads from (src/game_data/).
bool CheckGameData(const Paths& paths) {
  if (!std::filesystem::is_directory(paths.game_data)) {
    Fail("--game_data_root does not exist: " + paths.game_data.string());
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
  // The flags: the command line, then itc.toml for what it didn't set (so
  // itc.toml may set game_data_root too).
  const std::vector<std::string> others = base::flags::ParseCommandLine(argc, argv);
  const std::filesystem::path exe_dir = app::ExecutableFolder();
  base::flags::LoadConfig(exe_dir / "itc.toml");
  const app::Paths paths = app::ResolvePaths(exe_dir);
  app::SetUpLogging(exe_dir, paths, others);
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
  // The image's data in guest memory (compiled in: src/image/image.h), the
  // kernel variables bound.
  uint8_t* base = memory::Base();
  image::CopyInto(base);
  memory::BindKernelVariables(base);
  native_audio::Start();
  // The game's main thread: one of our threads (E1, src/threads/) running
  // our start-up (src/app/start_up.cpp) with the image's stack size.
  if (!threads::Enabled()) {
    app::Fail("--native_threads=false is no longer supported");
    return EXIT_FAILURE;
  }
  std::thread main_thread = threads::StartMainThread(base, image::kStackSize, app::StartUp);
  std::thread waiter([thread = std::move(main_thread)]() mutable {
    thread.join();
    ITC_LOG_INFO("Execution complete");
    app::RequestQuit();
  });
  waiter.detach();
  app::RunLoop();
  return EXIT_SUCCESS;
}
