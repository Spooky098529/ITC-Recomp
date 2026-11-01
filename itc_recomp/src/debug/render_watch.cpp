// Step 0 of docs/conventional-renderer.md: the register shadow watch and the
// GPU data read watch (see render_watch.h). Built on the input read watch
// tried on 2026-10-19: the watched host pages are made PROT_NONE, a fault
// (SIGSEGV) notes the faulting host instruction (its module and offset, for
// addr2line), then the page is opened and the one instruction single-
// stepped (the trap flag, SIGTRAP), after which the page is protected
// again. Other threads touching a page during a step aren't seen. Linux on
// x86-64 only; elsewhere the flags do nothing.

#include "debug/render_watch.h"

#include "base/flags.h"
#include "base/log.h"
#include "frame_pacing/game_messages.h"
#include "guest.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <fstream>
#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

#if defined(__linux__) && defined(__x86_64__)
#include <csignal>
#include <dlfcn.h>
#include <sys/mman.h>
#include <ucontext.h>
#include <unistd.h>
#define ITC_RENDER_WATCH 1
#endif

ITC_FLAG_BOOL(watch_register_shadow, false, "ITC",
              "Log the code outside the render thread's replay touching the graphics device's "
              "register shadow (page protection; the main thread waits for each replay; slow, "
              "a diagnostic)");
ITC_FLAG_BOOL(watch_gpu_data_reads, false, "ITC",
              "Log the CPU's reads and writes of vertex, index and texture data draws have used "
              "(page protection; slow, a diagnostic)");

namespace debug {

#ifdef ITC_RENDER_WATCH
namespace {

constexpr uint32_t kPage = 4096;
constexpr uint32_t kShadowBytes = 12440;
constexpr uint32_t kMaxShadowPages = 5;
constexpr uint32_t kPhysicalPages = 0x20000000u / kPage;
// A page faulting this often in a frame is opened until the frame's end.
constexpr uint32_t kFaultsPerFrame = 64;

// Entry tags.
enum Tag : uint32_t {
  kShadow = 0,         // offset: in the device, in words
  kShadowNeighbor = 1, // on a shadow page, outside +0..+12440
  kVertex = 2,
  kIndex = 3,
  kTexture = 4,
};
const char* TagName(uint32_t tag) {
  switch (tag) {
    case kShadow: return "shadow";
    case kShadowNeighbor: return "shadow-page-other";
    case kVertex: return "gpu-vertex";
    case kIndex: return "gpu-index";
    case kTexture: return "gpu-texture";
  }
  return "?";
}

uint8_t* g_base = nullptr;
bool g_installed = false;
struct sigaction g_old_segv, g_old_trap;

// The shadow's pages (guest addresses), written by the render thread while
// they're open.
uint32_t g_device = 0;
std::atomic<uint32_t> g_replay_device{0};
uint32_t g_shadow_pages[kMaxShadowPages];
uint32_t g_shadow_page_count = 0;
std::atomic<bool> g_shadow_protected{false};

// The physical pages' states: the kinds of data seen there (GpuData bits)
// and these.
constexpr uint8_t kArmed = 0x10;       // protected in the virtual views
constexpr uint8_t kTransition = 0x20;  // being protected or opened
constexpr uint8_t kDropped = 0x40;     // written since a draw used it
std::atomic<uint8_t>* g_pages = nullptr;
std::atomic<uint16_t>* g_page_faults = nullptr;
// Pages opened this frame for faulting too often, armed again at its end.
constexpr uint32_t kOpenedMax = 1 << 16;
std::atomic<uint32_t> g_opened[kOpenedMax];
std::atomic<uint32_t> g_opened_count{0};

// (pc << 16) | write << 15 | tag << 12 | offset (words, shadow only).
struct Entry {
  std::atomic<uint64_t> key{0};
  std::atomic<uint32_t> count{0};
  std::atomic<uint32_t> tid{0};
  std::atomic<uint32_t> phases{0};
  uint32_t logged_count = 0;  // the logging thread's
};
constexpr uint32_t kEntries = 1 << 15;
Entry* g_entries = nullptr;
std::atomic<uint64_t> g_noted{0}, g_table_full{0}, g_opened_pages{0}, g_dropped_pages{0},
    g_host_writes{0}, g_armed{0}, g_protect_errors{0};

thread_local uint32_t t_open[4];  // host page offsets from g_base
thread_local int t_open_count = 0;
thread_local bool t_open_shadow[4];

constexpr uint32_t kPhaseRace = 1, kPhaseFrontEnd = 2, kPhaseRenderOnly = 4, kPhaseBoot = 8;
uint32_t CurrentPhases() {
  uint64_t pass;
  bool render_only, main_thread;
  frame_pacing::GameMessages_PassFromAnyThread(pass, render_only, main_thread);
  if (pass == 0) {
    return kPhaseBoot;
  }
  return (frame_pacing::GameMessages_InRace() ? kPhaseRace : kPhaseFrontEnd) |
         (render_only ? kPhaseRenderOnly : 0);
}

void Note(uint64_t pc, bool write, uint32_t tag, uint32_t offset) {
  g_noted.fetch_add(1, std::memory_order_relaxed);
  uint64_t key = (pc << 16) | (uint64_t(write) << 15) | (uint64_t(tag) << 12) | (offset & 0xFFF);
  uint32_t h = uint32_t((key * 0x9E3779B97F4A7C15ull) >> 49) & (kEntries - 1);
  for (uint32_t probe = 0; probe < 64; probe++) {
    Entry& e = g_entries[(h + probe) & (kEntries - 1)];
    uint64_t k = e.key.load(std::memory_order_acquire);
    if (k == 0) {
      uint64_t expected = 0;
      k = e.key.compare_exchange_strong(expected, key) ? key : expected;
    }
    if (k == key) {
      e.count.fetch_add(1, std::memory_order_relaxed);
      e.tid.store(uint32_t(gettid()), std::memory_order_relaxed);
      e.phases.fetch_or(CurrentPhases(), std::memory_order_relaxed);
      return;
    }
  }
  g_table_full.fetch_add(1, std::memory_order_relaxed);
}

void Protect(uint64_t offset, uint64_t size, bool watch) {
  if (mprotect(g_base + offset, size, watch ? PROT_NONE : PROT_READ | PROT_WRITE)) {
    g_protect_errors.fetch_add(1, std::memory_order_relaxed);
  }
}

// A physical page's three virtual views (the 0xE0000000 view is offset by
// a page: GpuPhysicalAddress).
void ProtectPhysical(uint32_t first, uint32_t count, bool watch) {
  uint64_t start = uint64_t(first) * kPage, size = uint64_t(count) * kPage;
  Protect(0xA0000000ull + start, size, watch);
  Protect(0xC0000000ull + start, size, watch);
  if (start >= kPage) {
    Protect(0xE0000000ull + start - kPage, size, watch);
  } else if (size > kPage) {
    Protect(0xE0000000ull, size - kPage, watch);
  }
}

// The physical page behind a host offset in a virtual view, or UINT32_MAX.
uint32_t PhysicalPageOf(uint64_t offset) {
  uint64_t physical;
  if (offset >= 0xA0000000ull && offset < 0xC0000000ull) {
    physical = offset - 0xA0000000ull;
  } else if (offset >= 0xC0000000ull && offset < 0xE0000000ull) {
    physical = offset - 0xC0000000ull;
  } else if (offset >= 0xE0000000ull && offset < 0x100000000ull) {
    physical = offset - 0xE0000000ull + kPage;
  } else {
    return UINT32_MAX;
  }
  return physical < 0x20000000ull ? uint32_t(physical / kPage) : UINT32_MAX;
}

uint32_t KindTag(uint8_t state) {
  return (state & uint8_t(GpuData::kVertex))  ? kVertex
         : (state & uint8_t(GpuData::kIndex)) ? kIndex
                                               : kTexture;
}

// Opens a GPU data page: a write drops it until a draw uses it again; too
// many faults open it for the rest of the frame.
void OpenPhysical(uint32_t page, bool drop) {
  uint8_t state = g_pages[page].fetch_or(kTransition, std::memory_order_acq_rel);
  if (!(state & kArmed) || (state & kTransition)) {
    return;  // another thread is at it
  }
  g_pages[page].fetch_and(uint8_t(~kArmed), std::memory_order_acq_rel);
  ProtectPhysical(page, 1, false);
  if (drop) {
    g_pages[page].fetch_or(kDropped, std::memory_order_acq_rel);
    g_dropped_pages.fetch_add(1, std::memory_order_relaxed);
  } else {
    uint32_t i = g_opened_count.fetch_add(1, std::memory_order_acq_rel);
    if (i < kOpenedMax) {
      g_opened[i].store(page, std::memory_order_release);
    }
    g_opened_pages.fetch_add(1, std::memory_order_relaxed);
  }
  g_pages[page].fetch_and(uint8_t(~kTransition), std::memory_order_acq_rel);
}

void Chain(struct sigaction& old, int sig, siginfo_t* info, void* context) {
  if (old.sa_flags & SA_SIGINFO) {
    if (old.sa_sigaction) {
      old.sa_sigaction(sig, info, context);
      return;
    }
  } else if (old.sa_handler != SIG_DFL && old.sa_handler != SIG_IGN) {
    old.sa_handler(sig);
    return;
  }
  signal(sig, SIG_DFL);  // the fault happens again, unhandled
}

void Step(ucontext_t* uc, uint32_t page_offset_lo, bool shadow) {
  t_open[t_open_count] = page_offset_lo;
  t_open_shadow[t_open_count] = shadow;
  t_open_count++;
  uc->uc_mcontext.gregs[REG_EFL] |= 0x100;  // trap after one instruction
}

void OnSegv(int sig, siginfo_t* info, void* context) {
  auto* uc = static_cast<ucontext_t*>(context);
  uintptr_t address = reinterpret_cast<uintptr_t>(info->si_addr);
  uintptr_t base = reinterpret_cast<uintptr_t>(g_base);
  if (!g_base || address - base >= 0x100000000ull || t_open_count >= 4) {
    Chain(g_old_segv, sig, info, context);
    return;
  }
  uint64_t offset = address - base;
  uint64_t pc = uint64_t(uc->uc_mcontext.gregs[REG_RIP]);
  bool write = (uc->uc_mcontext.gregs[REG_ERR] & 2) != 0;
  // The shadow's pages.
  uint32_t guest_page = uint32_t(offset) & ~(kPage - 1);
  for (uint32_t i = 0; i < g_shadow_page_count; i++) {
    if (g_shadow_pages[i] != guest_page) {
      continue;
    }
    uint32_t in_device = uint32_t(offset) - g_device;
    if (in_device < kShadowBytes) {
      Note(pc, write, kShadow, in_device / 4);
    } else {
      Note(pc, write, kShadowNeighbor, 0);
    }
    Protect(guest_page, kPage, false);
    Step(uc, guest_page, true);
    return;
  }
  // GPU data.
  uint32_t page = g_pages ? PhysicalPageOf(offset) : UINT32_MAX;
  if (page != UINT32_MAX) {
    uint8_t state = g_pages[page].load(std::memory_order_acquire);
    if (state & kTransition) {
      return;  // being opened or protected: try again
    }
    if (state & kArmed) {
      Note(pc, write, KindTag(state), 0);
      if (write) {
        OpenPhysical(page, true);
        return;
      }
      if (g_page_faults[page].fetch_add(1, std::memory_order_relaxed) + 1 >= kFaultsPerFrame) {
        OpenPhysical(page, false);
        return;
      }
      Protect(guest_page, kPage, false);
      Step(uc, guest_page, false);
      return;
    }
  }
  Chain(g_old_segv, sig, info, context);
}

void OnTrap(int sig, siginfo_t* info, void* context) {
  if (t_open_count == 0) {
    Chain(g_old_trap, sig, info, context);
    return;
  }
  auto* uc = static_cast<ucontext_t*>(context);
  uc->uc_mcontext.gregs[REG_EFL] &= ~0x100;
  for (int i = 0; i < t_open_count; i++) {
    bool watch;
    if (t_open_shadow[i]) {
      watch = g_shadow_protected.load(std::memory_order_acquire);
    } else {
      uint32_t page = PhysicalPageOf(t_open[i]);
      watch = page != UINT32_MAX &&
              (g_pages[page].load(std::memory_order_acquire) & (kArmed | kTransition)) == kArmed;
    }
    if (watch) {
      Protect(t_open[i], kPage, true);
    }
  }
  t_open_count = 0;
}

void Install(uint8_t* base) {
  if (g_installed) {
    return;
  }
  g_base = base;
  g_entries = new Entry[kEntries];
  struct sigaction sa;
  std::memset(&sa, 0, sizeof(sa));
  sa.sa_flags = SA_SIGINFO | SA_NODEFER;
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = OnSegv;
  sigaction(SIGSEGV, &sa, &g_old_segv);
  sa.sa_sigaction = OnTrap;
  sigaction(SIGTRAP, &sa, &g_old_trap);
  g_installed = true;
}

std::string ThreadName(uint32_t tid) {
  std::ifstream comm("/proc/self/task/" + std::to_string(tid) + "/comm");
  std::string name;
  std::getline(comm, name);
  return name.empty() ? std::to_string(tid) : name;
}

// Each entry new or seen again since it was last logged, then the totals.
void Dump() {
  for (uint32_t i = 0; i < kEntries; i++) {
    Entry& e = g_entries[i];
    uint64_t key = e.key.load(std::memory_order_acquire);
    uint32_t count = e.count.load(std::memory_order_relaxed);
    if (!key || count == e.logged_count) {
      continue;
    }
    bool first = e.logged_count == 0;
    e.logged_count = count;
    if (!first) {
      continue;  // counts are logged every 60 s (below)
    }
    uint64_t pc = key >> 16;
    Dl_info dl{};
    const char* module = "?";
    uint64_t module_offset = pc;
    if (dladdr(reinterpret_cast<void*>(pc), &dl) && dl.dli_fname) {
      module = std::strrchr(dl.dli_fname, '/') ? std::strrchr(dl.dli_fname, '/') + 1 : dl.dli_fname;
      module_offset = pc - reinterpret_cast<uint64_t>(dl.dli_fbase);
    }
    uint32_t tag = uint32_t(key >> 12) & 7;
    uint32_t phases = e.phases.load(std::memory_order_relaxed);
    ITC_LOG_INFO("render watch: {}+0x{:X} {} +{} {} count {} thread {} phases{}{}{}{}", module,
                 module_offset, TagName(tag), tag == kShadow ? 4 * uint32_t(key & 0xFFF) : 0,
                 ((key >> 15) & 1) ? "write" : "read", count,
                 ThreadName(e.tid.load(std::memory_order_relaxed)),
                 (phases & kPhaseRace) ? " race" : "", (phases & kPhaseFrontEnd) ? " front-end" : "",
                 (phases & kPhaseRenderOnly) ? " render-only" : "",
                 (phases & kPhaseBoot) ? " boot" : "");
  }
  ITC_LOG_INFO("render watch: {} faults noted, {} not (table full); GPU data pages: {} opened "
               "for faulting often, {} dropped by writes, {} file reads into watched pages; {} "
               "armed in all, {} protection calls failed",
               g_noted.load(), g_table_full.load(), g_opened_pages.load(), g_dropped_pages.load(),
               g_host_writes.load(), g_armed.load(), g_protect_errors.load());
}

// Every 60 s, each entry's count so far.
void DumpCounts() {
  for (uint32_t i = 0; i < kEntries; i++) {
    Entry& e = g_entries[i];
    if (uint64_t key = e.key.load(std::memory_order_acquire)) {
      Dl_info dl{};
      uint64_t pc = key >> 16, module_offset = pc;
      const char* module = "?";
      if (dladdr(reinterpret_cast<void*>(pc), &dl) && dl.dli_fname) {
        module = std::strrchr(dl.dli_fname, '/') ? std::strrchr(dl.dli_fname, '/') + 1 : dl.dli_fname;
        module_offset = pc - reinterpret_cast<uint64_t>(dl.dli_fbase);
      }
      uint32_t tag = uint32_t(key >> 12) & 7;
      ITC_LOG_INFO("render watch: count {}+0x{:X} {} +{} {} {}", module, module_offset,
                   TagName(tag), tag == kShadow ? 4 * uint32_t(key & 0xFFF) : 0,
                   ((key >> 15) & 1) ? "write" : "read", e.count.load());
    }
  }
}

// The hand-over's wait (--watch_register_shadow).
std::mutex g_frames_mutex;
std::condition_variable g_frames_done;
uint64_t g_started = 0, g_finished = 0;

// The render thread's: pages to arm at the frame's end, and the ranges
// seen this frame (to skip repeats).
std::vector<uint32_t> g_to_arm;
std::unordered_set<uint64_t> g_seen_ranges;

void ArmRuns(std::vector<uint32_t>& pages) {
  std::sort(pages.begin(), pages.end());
  pages.erase(std::unique(pages.begin(), pages.end()), pages.end());
  size_t i = 0;
  while (i < pages.size()) {
    size_t j = i;
    // Each page's state first, then the protection.
    while (j < pages.size() && pages[j] == pages[i] + (j - i)) {
      g_page_faults[pages[j]].store(0, std::memory_order_relaxed);
      g_pages[pages[j]].fetch_or(kArmed, std::memory_order_acq_rel);
      j++;
    }
    ProtectPhysical(pages[i], uint32_t(j - i), true);
    g_armed.fetch_add(j - i, std::memory_order_relaxed);
    i = j;
  }
  pages.clear();
}

}  // namespace

uint64_t RenderWatch_BeforeHandOver() {
  if (!ITC_FLAG(watch_register_shadow)) {
    return 0;
  }
  std::lock_guard lock(g_frames_mutex);
  return g_started;
}

void RenderWatch_AfterHandOver(uint64_t token) {
  if (!ITC_FLAG(watch_register_shadow)) {
    return;
  }
  // The frame just handed over, drawn (not forever: a pass that hands
  // nothing over goes on after 250 ms).
  std::unique_lock lock(g_frames_mutex);
  g_frames_done.wait_for(lock, std::chrono::milliseconds(250),
                         [&] { return g_finished > token && g_finished == g_started; });
}

void RenderWatch_OnDrawStart(uint8_t* base) {
  bool shadow = ITC_FLAG(watch_register_shadow);
  if (!shadow && !ITC_FLAG(watch_gpu_data_reads)) {
    return;
  }
  Install(base);
  if (shadow) {
    {
      std::lock_guard lock(g_frames_mutex);
      g_started++;
    }
    g_shadow_protected.store(false, std::memory_order_release);
    for (uint32_t i = 0; i < g_shadow_page_count; i++) {
      Protect(g_shadow_pages[i], kPage, false);
    }
  }
  if (ITC_FLAG(watch_gpu_data_reads) && !g_pages) {
    g_pages = new std::atomic<uint8_t>[kPhysicalPages]();
    g_page_faults = new std::atomic<uint16_t>[kPhysicalPages]();
  }
  g_seen_ranges.clear();
}

void RenderWatch_OnDrawEnd(uint8_t* base) {
  bool shadow = ITC_FLAG(watch_register_shadow);
  bool gpu = ITC_FLAG(watch_gpu_data_reads);
  if ((!shadow && !gpu) || !g_installed) {
    return;
  }
  (void)base;
  if (gpu && g_pages) {
    // This frame's new pages, and those opened for faulting often.
    uint32_t opened = std::min(g_opened_count.exchange(0, std::memory_order_acq_rel), kOpenedMax);
    for (uint32_t i = 0; i < opened; i++) {
      uint32_t page = g_opened[i].load(std::memory_order_acquire);
      if (!(g_pages[page].load(std::memory_order_acquire) & (kArmed | kDropped))) {
        g_to_arm.push_back(page);
      }
    }
    uint32_t test_page = g_to_arm.empty() ? 0 : g_to_arm.front();
    ArmRuns(g_to_arm);
    // Once: a read through each view of an armed page has to be noted, or
    // a watch that sees nothing means nothing.
    static bool tested = false;
    if (!tested && test_page > 1) {
      tested = true;
      std::string result;
      for (uint64_t view : {0xA0000000ull, 0xC0000000ull, 0xE0000000ull - kPage}) {
        uint64_t before = g_noted.load();
        volatile uint8_t value = *reinterpret_cast<volatile uint8_t*>(
            g_base + view + uint64_t(test_page) * kPage);
        (void)value;
        result += fmt::format(" {:08X}: {}", view == 0xE0000000ull - kPage ? 0xE0000000u : view,
                              g_noted.load() > before ? "noted" : "NOT noted");
      }
      ITC_LOG_INFO("render watch: self-test on physical page {:08X}:{}", test_page * kPage,
                   result);
    }
  }
  if (shadow) {
    uint32_t device = g_replay_device.load(std::memory_order_relaxed);
    if (device != g_device) {
      g_device = device;
      g_shadow_page_count = 0;
      for (uint32_t p = device & ~(kPage - 1);
           device && p < device + kShadowBytes && g_shadow_page_count < kMaxShadowPages;
           p += kPage) {
        g_shadow_pages[g_shadow_page_count++] = p;
      }
      ITC_LOG_INFO("render watch: the device at {:08X}: {} pages watched", device,
                   g_shadow_page_count);
    }
    for (uint32_t i = 0; i < g_shadow_page_count; i++) {
      Protect(g_shadow_pages[i], kPage, true);
    }
    g_shadow_protected.store(true, std::memory_order_release);
    {
      std::lock_guard lock(g_frames_mutex);
      g_finished = g_started;
    }
    g_frames_done.notify_all();
  }
  static auto last = std::chrono::steady_clock::now();
  static int dumps = 0;
  auto now = std::chrono::steady_clock::now();
  if (now - last >= std::chrono::seconds(5)) {
    last = now;
    Dump();
    if (++dumps % 12 == 0) {
      DumpCounts();
    }
  }
}

void RenderWatch_OnReplay(uint32_t device) {
  g_replay_device.store(device, std::memory_order_relaxed);
}

bool RenderWatch_WatchingGpuData() {
  return ITC_FLAG(watch_gpu_data_reads) && g_pages;
}

void RenderWatch_OnGpuData(uint32_t physical, uint32_t size, GpuData kind) {
  if (!size || physical >= 0x20000000u || !g_seen_ranges.insert(uint64_t(physical) << 32 | size).second) {
    return;
  }
  uint32_t end = uint32_t((std::min<uint64_t>(uint64_t(physical) + size, 0x20000000u) - 1) / kPage);
  for (uint32_t page = physical / kPage; page <= end && page < kPhysicalPages; page++) {
    uint8_t state = g_pages[page].fetch_or(uint8_t(kind), std::memory_order_acq_rel);
    if (state & (kArmed | kTransition)) {
      continue;
    }
    g_pages[page].fetch_and(uint8_t(~kDropped), std::memory_order_acq_rel);
    g_to_arm.push_back(page);
  }
}

void RenderWatch_BeforeHostWrite(const uint8_t* host, uint64_t size) {
  if (!g_pages || !size || host < g_base) {
    return;
  }
  uint64_t offset = uint64_t(host - g_base);
  bool counted = false;
  for (uint64_t at = offset & ~uint64_t(kPage - 1); at < offset + size; at += kPage) {
    uint32_t page = PhysicalPageOf(at);
    if (page == UINT32_MAX) {
      continue;
    }
    if (g_pages[page].load(std::memory_order_acquire) & kArmed) {
      if (!counted) {
        g_host_writes.fetch_add(1, std::memory_order_relaxed);
        counted = true;
      }
      OpenPhysical(page, true);
    }
  }
}

#else

uint64_t RenderWatch_BeforeHandOver() { return 0; }
void RenderWatch_AfterHandOver(uint64_t) {}
void RenderWatch_OnDrawStart(uint8_t*) {}
void RenderWatch_OnReplay(uint32_t) {}
void RenderWatch_OnDrawEnd(uint8_t*) {}
bool RenderWatch_WatchingGpuData() { return false; }
void RenderWatch_OnGpuData(uint32_t, uint32_t, GpuData) {}
void RenderWatch_BeforeHostWrite(const uint8_t*, uint64_t) {}

#endif

}  // namespace debug
