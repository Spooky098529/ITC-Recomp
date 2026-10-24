#include "harness.h"

#include "base/flags.h"
#include "base/hash.h"
#include "base/log.h"

#include <algorithm>
#include <sstream>
#include <string_view>

ITC_FLAG_STRING(run_harnesses, "", "ITC",
                "Run harnesses comparing native functions with the recompiled ones, once at "
                "the first input update: \"all\" or a comma-separated list "
                "(object_attributes, time, clock, strings, last_error, gamma, start_up, user, xmp, network, notify, profile, saves, file_class, movie_files, xma, sprites, billboards, command_list, c_runtime, math, ui_movies, ui_movies_game, race_overlays, vehicle_accessors, matrix_math, car_physics, collision, collision_world, heap, c_runtime_heap, sound_cues, xact, action_mappers, action_mappers_binding, data_tables, model_animation, car_specs, cameras, strings_and_font, std_string, track_route, ai_drivers, racer_controllers, track_zones, graphics_resources, video_output, device_state, course_scenery, post_effects, input)");

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
    ITC_LOG_INFO("harness: {} DIFFERS on {}: recompiled {} native {}", name_, what, Hex(recompiled),
                 Hex(native));
  }
}

void Outputs::Add(Kind kind, const uint8_t* data, uint32_t size) {
  entries_.push_back({kind, uint32_t(bytes_.size()), size});
  bytes_.insert(bytes_.end(), data, data + size);
}

void Outputs::Bytes(const std::vector<uint8_t>& bytes) {
  Add(Kind::kExact, bytes.data(), uint32_t(bytes.size()));
}

void Outputs::Word(uint32_t value) {
  const uint8_t b[4] = {uint8_t(value >> 24), uint8_t(value >> 16), uint8_t(value >> 8),
                        uint8_t(value)};
  Add(Kind::kExact, b, 4);
}

void Outputs::Float(uint32_t bits) {
  const uint8_t b[4] = {uint8_t(bits >> 24), uint8_t(bits >> 16), uint8_t(bits >> 8),
                        uint8_t(bits)};
  Add(Kind::kFloat, b, 4);
}

void Outputs::Floats(uint8_t* base, uint32_t address, uint32_t count) {
  for (uint32_t i = 0; i < count; i++) {
    Float(guest::LoadU32(base, address + 4 * i));
  }
}

void Outputs::Double(uint64_t bits) {
  uint8_t b[8];
  for (int i = 0; i < 8; i++) {
    b[i] = uint8_t(bits >> (56 - 8 * i));
  }
  Add(Kind::kDouble, b, 8);
}

namespace {

uint64_t BigEndian(const uint8_t* p, uint32_t size) {
  uint64_t v = 0;
  for (uint32_t i = 0; i < size; i++) {
    v = (v << 8) | p[i];
  }
  return v;
}

// All exponent bits set and a nonzero mantissa.
bool IsNaN(uint64_t bits, uint32_t size) {
  if (size == 4) {
    return (bits & 0x7F800000u) == 0x7F800000u && (bits & 0x007FFFFFu) != 0;
  }
  return (bits & 0x7FF0000000000000ull) == 0x7FF0000000000000ull &&
         (bits & 0x000FFFFFFFFFFFFFull) != 0;
}

}  // namespace

void Results::Check(const Outputs& recompiled, const Outputs& native, const std::string& what) {
  cases_++;
  typed_ = true;
  using Kind = Outputs::Kind;
  std::string differs;
  bool nan_only = false;
  if (recompiled.entries_.size() != native.entries_.size()) {
    differs = fmt::format("{} entries against {}", recompiled.entries_.size(),
                          native.entries_.size());
  }
  for (size_t i = 0; differs.empty() && i < recompiled.entries_.size(); i++) {
    const Outputs::Entry& a = recompiled.entries_[i];
    const Outputs::Entry& b = native.entries_[i];
    const uint8_t* pa = recompiled.bytes_.data() + a.offset;
    const uint8_t* pb = native.bytes_.data() + b.offset;
    if (a.kind != b.kind || a.size != b.size) {
      differs = fmt::format("entry {}: a different kind or size", i);
      break;
    }
    if (std::equal(pa, pa + a.size, pb)) {
      continue;
    }
    if (a.kind != Kind::kExact) {
      const uint64_t va = BigEndian(pa, a.size);
      const uint64_t vb = BigEndian(pb, b.size);
      if (IsNaN(va, a.size) && IsNaN(vb, b.size)) {
        nan_only = true;
        continue;
      }
    }
    const char* kind = a.kind == Kind::kExact ? "bytes" : a.kind == Kind::kFloat ? "float" : "double";
    differs = fmt::format("entry {} ({}, at byte {}): recompiled {} native {}", i, kind, a.offset,
                          Hex(std::vector<uint8_t>(pa, pa + a.size)),
                          Hex(std::vector<uint8_t>(pb, pb + b.size)));
  }
  if (!differs.empty()) {
    if (++differ_ <= 10) {
      ITC_LOG_INFO("harness: {} DIFFERS on {}: {}", name_, what, differs);
    }
  } else if (nan_only) {
    nan_only_++;
  }
}

void Results::Report() const {
  if (typed_) {
    ITC_LOG_INFO("harness: {}: {} cases, {} differ ({} NaN-only)", name_, cases_, differ_,
                 nan_only_);
  } else {
    ITC_LOG_INFO("harness: {}: {} cases, {} differ", name_, cases_, differ_);
  }
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
  const std::string& requested = ITC_FLAG(run_harnesses);
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
      {"sprites", Sprites},
      {"billboards", Billboards},
      {"command_list", CommandList},
      {"c_runtime", CRuntime},
      {"math", Math},
      {"ui_movies", UiMovies},
      {"ui_movies_game", UiMoviesGame},
      {"race_overlays", RaceOverlays},
      {"vehicle_accessors", VehicleAccessors},
      {"matrix_math", MatrixMath},
      {"car_physics", CarPhysics},
      {"collision", Collision},
      {"collision_world", CollisionWorld},
      {"heap", Heap},
      {"c_runtime_heap", CRuntimeHeap},
      {"sound_cues", SoundCues},
      {"xact", Xact},
      {"action_mappers", ActionMappers},
      {"action_mappers_binding", ActionMappersBinding},
      {"data_tables", DataTables},
      {"model_animation", ModelAnimation},
      {"car_specs", CarSpecs},
      {"cameras", Cameras},
      {"strings_and_font", StringsAndFont},
      {"std_string", StdString},
      {"track_route", TrackRoute},
      {"ai_drivers", AiDrivers},
      {"racer_controllers", RacerControllers},
      {"track_zones", TrackZones},
      {"graphics_resources", GraphicsResources},
      {"video_output", VideoOutput},
      {"device_state", DeviceState},
      {"course_scenery", CourseScenery},
      {"post_effects", PostEffects},
      {"input", Input},
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
      ITC_LOG_INFO("harness: running {}", e.name);
      const std::string_view name = e.name;
      h.random = Random(base::Hash64(name.data(), name.size()));
      e.run(h);
    }
  }
  ITC_LOG_INFO("harness: done");
}

}  // namespace harness
