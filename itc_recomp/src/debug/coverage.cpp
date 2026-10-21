// The coverage build (itc_coverage, ITC_COVERAGE in CMakeLists.txt): which
// recompiled functions ran. Clang's -fsanitize-coverage=func,inline-bool-flag,
// pc-table gives each function of the recompiled code a flag, set the first
// time it runs, and a table of the functions' addresses in the same order.
// Every few seconds and at exit, the functions whose flag is set are written
// to coverage/seen_<pid>.txt next to the executable, as addresses relative to
// its load address (what nm prints); tools/coverage_seen.py turns them into
// guest addresses. Linux only, like the target.

#include <dlfcn.h>
#include <link.h>
#include <unistd.h>

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <string>
#include <thread>

namespace {

const bool* g_flags_start = nullptr;
const bool* g_flags_end = nullptr;
const uintptr_t* g_pcs = nullptr;  // pairs: address, flags (1: a function's entry)
uintptr_t g_load_address = 0;

int FirstModule(dl_phdr_info* info, size_t, void* out) {
  *static_cast<uintptr_t*>(out) = info->dlpi_addr;  // the executable comes first
  return 1;
}

void Write() {
  if (!g_flags_start || !g_pcs) {
    return;
  }
  std::error_code ec;
  std::filesystem::path dir = std::filesystem::read_symlink("/proc/self/exe", ec).parent_path() / "coverage";
  std::filesystem::create_directories(dir, ec);
  std::string path = (dir / ("seen_" + std::to_string(getpid()) + ".txt")).string();
  std::string temp = path + ".tmp";
  FILE* f = std::fopen(temp.c_str(), "w");
  if (!f) {
    return;
  }
  size_t count = g_flags_end - g_flags_start;
  for (size_t i = 0; i < count; i++) {
    if (g_flags_start[i]) {
      std::fprintf(f, "%zx\n", g_pcs[2 * i] - g_load_address);
    }
  }
  std::fclose(f);
  std::rename(temp.c_str(), path.c_str());
}

void Start() {
  if (!g_flags_start || !g_pcs) {
    return;  // both init calls come before main; start once they have
  }
  dl_iterate_phdr(FirstModule, &g_load_address);
  std::atexit(Write);
  std::thread([] {
    for (;;) {
      std::this_thread::sleep_for(std::chrono::seconds(5));
      Write();
    }
  }).detach();
  std::fprintf(stderr, "coverage: %zu functions, written to coverage/ next to the executable\n",
               static_cast<size_t>(g_flags_end - g_flags_start));
}

}  // namespace

// Called by the instrumented module's constructor, once each (one module).
extern "C" void __sanitizer_cov_bool_flag_init(bool* start, bool* end) {
  g_flags_start = start;
  g_flags_end = end;
  Start();
}

extern "C" void __sanitizer_cov_pcs_init(const uintptr_t* start, const uintptr_t*) {
  g_pcs = start;
  Start();
}
