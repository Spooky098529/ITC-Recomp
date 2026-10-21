#include "harness.h"

#include <rex/cvar.h>
#include <rex/logging.h>

#include <sstream>

REXCVAR_DEFINE_STRING(run_harnesses, "", "ITC",
                      "Run harnesses comparing native functions with the recompiled ones, once at "
                      "the first input update: \"all\" or a comma-separated list "
                      "(object_attributes, time, clock, strings, last_error, gamma, start_up, user, xmp, network, notify, profile, saves, file_class, movie_files, xma)");

namespace harness {

uint64_t Random::Next() {
  state_ ^= state_ << 13;
  state_ ^= state_ >> 7;
  state_ ^= state_ << 17;
  return state_;
}

void Results::Check(const std::vector<uint8_t>& recompiled, const std::vector<uint8_t>& native,
                    const std::string& what) {
  cases_++;
  if (recompiled != native && ++differ_ <= 10) {
    REXLOG_INFO("harness: {} DIFFERS on {}: recompiled {} native {}", name_, what, Hex(recompiled),
                Hex(native));
  }
}

void Results::Report() const {
  REXLOG_INFO("harness: {}: {} cases, {} differ", name_, cases_, differ_);
}

void Fill(uint8_t* base, uint32_t address, uint32_t bytes, uint8_t value) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, value);
  }
}

std::vector<uint8_t> Read(uint8_t* base, uint32_t address, uint32_t bytes) {
  std::vector<uint8_t> v(bytes);
  for (uint32_t i = 0; i < bytes; i++) {
    v[i] = guest::LoadU8(base, address + i);
  }
  return v;
}

void Write(uint8_t* base, uint32_t address, const void* data, uint32_t bytes) {
  for (uint32_t i = 0; i < bytes; i++) {
    guest::StoreU8(base, address + i, static_cast<const uint8_t*>(data)[i]);
  }
}

void StoreU64(uint8_t* base, uint32_t address, uint64_t value) {
  guest::StoreU32(base, address, uint32_t(value >> 32));
  guest::StoreU32(base, address + 4, uint32_t(value));
}

std::string Hex(const std::vector<uint8_t>& bytes) {
  std::string s;
  for (uint8_t b : bytes) {
    s += fmt::format("{:02X}", b);
  }
  return s;
}

PPCContext CallContext(const Context& h) {
  PPCContext t = h.ctx;
  t.r1.u64 = h.data - 1024;
  return t;
}

void RunRequested(PPCContext& ctx, uint8_t* base) {
  static bool run = false;
  const std::string& requested = REXCVAR_GET(run_harnesses);
  if (run || requested.empty()) {
    return;
  }
  run = true;
  struct Entry {
    const char* name;
    void (*run)(Context&);
  };
  static const Entry kHarnesses[] = {
      {"object_attributes", ObjectAttributes},
      {"time", Time},
      {"clock", Clock},
      {"strings", Strings},
      {"last_error", LastError},
      {"gamma", Gamma},
      {"start_up", StartUp},
      {"user", User},
      {"xmp", Xmp},
      {"network", Network},
      {"notify", Notify},
      {"profile", Profile},
      {"saves", Saves},
      {"file_class", FileClass},
      {"movie_files", MovieFiles},
      {"xma", Xma},
  };
  auto wanted = [&](const char* name) {
    if (requested == "all") {
      return true;
    }
    std::stringstream list(requested);
    std::string item;
    while (std::getline(list, item, ',')) {
      if (item == name) {
        return true;
      }
    }
    return false;
  };
  Context h{ctx, base, ctx.r1.u32 - kScratchBytes - 512, {}};
  for (const Entry& e : kHarnesses) {
    if (wanted(e.name)) {
      REXLOG_INFO("harness: running {}", e.name);
      e.run(h);
    }
  }
  REXLOG_INFO("harness: done");
}

}  // namespace harness
