#include "thread_cpu.h"

#include <rex/cvar.h>
#include <rex/logging.h>

REXCVAR_DEFINE_BOOL(log_thread_cpu, false, "ITC",
                    "Log each busy thread's CPU use by name every 2 s (Linux only)");

#ifdef _WIN32

namespace debug {

void ThreadCpu_Start() {
  if (REXCVAR_GET(log_thread_cpu)) {
    REXLOG_WARN("thread cpu: --log_thread_cpu isn't supported on Windows");
  }
}

void ThreadCpu_Stop() {}

}  // namespace debug

#else

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <dirent.h>
#include <fstream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>

namespace {

constexpr auto kInterval = std::chrono::seconds(2);
// Threads using less of a core than this aren't listed.
constexpr double kListedShare = 0.05;

struct ThreadSample {
  std::string name;
  uint64_t user = 0, system = 0;  // clock ticks
  uint64_t voluntary = 0, involuntary = 0;  // context switches
  int cpu = -1;  // the CPU it last ran on
};

// /proc/<...>/stat: "tid (name) state ...". The name may hold spaces or
// parentheses, so fields are counted from the last ')'.
bool ReadStat(const std::string& path, ThreadSample& s) {
  std::ifstream f(path);
  std::string line;
  if (!std::getline(f, line)) {
    return false;
  }
  size_t open = line.find('('), close = line.rfind(')');
  if (open == std::string::npos || close == std::string::npos || close < open) {
    return false;
  }
  s.name = line.substr(open + 1, close - open - 1);
  // After ')': field 3 (state) onwards; utime is 14, stime 15, processor 39.
  std::vector<std::string> fields;
  size_t pos = close + 2;
  while (pos < line.size()) {
    size_t end = line.find(' ', pos);
    if (end == std::string::npos) {
      end = line.size();
    }
    fields.push_back(line.substr(pos, end - pos));
    pos = end + 1;
  }
  if (fields.size() < 37) {
    return false;
  }
  s.user = std::stoull(fields[14 - 3]);
  s.system = std::stoull(fields[15 - 3]);
  s.cpu = std::stoi(fields[39 - 3]);
  return true;
}

void ReadSwitches(const std::string& path, ThreadSample& s) {
  std::ifstream f(path);
  std::string line;
  while (std::getline(f, line)) {
    if (line.rfind("voluntary_ctxt_switches:", 0) == 0) {
      s.voluntary = std::stoull(line.substr(24));
    } else if (line.rfind("nonvoluntary_ctxt_switches:", 0) == 0) {
      s.involuntary = std::stoull(line.substr(27));
    }
  }
}

std::map<int, ThreadSample> SampleThreads() {
  std::map<int, ThreadSample> threads;
  DIR* dir = opendir("/proc/self/task");
  if (!dir) {
    return threads;
  }
  while (dirent* entry = readdir(dir)) {
    int tid = atoi(entry->d_name);
    if (tid <= 0) {
      continue;
    }
    std::string base = std::string("/proc/self/task/") + entry->d_name;
    ThreadSample s;
    if (ReadStat(base + "/stat", s)) {
      ReadSwitches(base + "/status", s);
      threads[tid] = s;
    }
  }
  closedir(dir);
  return threads;
}

// The whole machine's busy and total time (/proc/stat's "cpu" line), to see
// whether it's out of CPU as a whole.
bool SampleMachine(uint64_t& busy, uint64_t& total) {
  std::ifstream f("/proc/stat");
  std::string label;
  uint64_t v[8] = {};
  if (!(f >> label >> v[0] >> v[1] >> v[2] >> v[3] >> v[4] >> v[5] >> v[6] >> v[7]) ||
      label != "cpu") {
    return false;
  }
  total = 0;
  for (uint64_t x : v) {
    total += x;
  }
  busy = total - v[3] - v[4];  // less idle and iowait
  return true;
}

class ThreadCpu {
 public:
  void Start() {
    thread_ = std::thread([this] { Run(); });
  }

  void Stop() {
    {
      std::lock_guard lock(mutex_);
      stop_ = true;
    }
    wake_.notify_all();
    if (thread_.joinable()) {
      thread_.join();
    }
  }

 private:
  void Run() {
    const double ticks_per_s = double(sysconf(_SC_CLK_TCK));
    const long cpus = sysconf(_SC_NPROCESSORS_ONLN);
    REXLOG_INFO("thread cpu: sampling every {} s, {} CPUs", kInterval.count(), cpus);
    auto previous = SampleThreads();
    uint64_t machine_busy = 0, machine_total = 0;
    bool have_machine = SampleMachine(machine_busy, machine_total);
    auto last = std::chrono::steady_clock::now();
    while (true) {
      {
        std::unique_lock lock(mutex_);
        if (wake_.wait_for(lock, kInterval, [this] { return stop_; })) {
          return;
        }
      }
      auto now = std::chrono::steady_clock::now();
      double seconds = std::chrono::duration<double>(now - last).count();
      last = now;
      auto current = SampleThreads();

      struct Row {
        int tid;
        const ThreadSample* s;
        double user, system;  // share of a core
        uint64_t voluntary, involuntary;
      };
      std::vector<Row> rows;
      double process = 0;
      for (const auto& [tid, s] : current) {
        auto it = previous.find(tid);
        const ThreadSample* p = it != previous.end() ? &it->second : nullptr;
        Row r{tid, &s,
              (s.user - (p ? p->user : 0)) / ticks_per_s / seconds,
              (s.system - (p ? p->system : 0)) / ticks_per_s / seconds,
              s.voluntary - (p ? p->voluntary : 0), s.involuntary - (p ? p->involuntary : 0)};
        process += r.user + r.system;
        if (r.user + r.system >= kListedShare) {
          rows.push_back(r);
        }
      }
      std::sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) {
        return a.user + a.system > b.user + b.system;
      });

      std::string machine;
      uint64_t busy, total;
      if (have_machine && SampleMachine(busy, total) && total > machine_total) {
        machine = fmt::format(", machine {:.0f}% busy",
                              100.0 * (busy - machine_busy) / (total - machine_total));
        machine_busy = busy;
        machine_total = total;
      }
      std::string line;
      for (const Row& r : rows) {
        line += fmt::format(" | {} ({}) {:.0f}% (sys {:.0f}%) cpu{} sw {}/{}", r.s->name, r.tid,
                            100 * (r.user + r.system), 100 * r.system, r.s->cpu,
                            r.voluntary, r.involuntary);
      }
      REXLOG_INFO("thread cpu: process {:.0f}% of a core, {} threads{}{}", 100 * process,
                  current.size(), machine, line);
      previous = std::move(current);
    }
  }

  std::thread thread_;
  std::mutex mutex_;
  std::condition_variable wake_;
  bool stop_ = false;
};

ThreadCpu* g_thread_cpu = nullptr;

}  // namespace

namespace debug {

void ThreadCpu_Start() {
  if (!REXCVAR_GET(log_thread_cpu) || g_thread_cpu) {
    return;
  }
  g_thread_cpu = new ThreadCpu();
  g_thread_cpu->Start();
}

void ThreadCpu_Stop() {
  if (g_thread_cpu) {
    g_thread_cpu->Stop();
    delete g_thread_cpu;
    g_thread_cpu = nullptr;
  }
}

}  // namespace debug

#endif
