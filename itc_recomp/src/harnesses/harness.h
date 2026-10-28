// Harnesses: run a native function and the recompiled one it replaces
// (`__imp__sub_XXXXXXXX`) on the same inputs, compare what they leave in
// guest memory and r3 byte for byte, and log the differences. Off unless
// `--run_harnesses` names them ("all", or a comma-separated list such as
// "time,strings"); they run once, from the first Input::Update, and the
// game carries on. See "Checking with a harness" in
// docs/rexglue-removal.md.
//
// Inputs and outputs go in a scratch area of guest memory below the stack
// pointer (kScratchBytes at `data`), and each call runs with its stack below
// that, so nothing the game holds is touched.

#pragma once

#include "guest.h"

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace harness {

constexpr uint32_t kScratchBytes = 4096;

// A xorshift generator: the same cases on every run. Each harness starts its
// own, seeded from its name, so adding cases to one harness doesn't change
// another's inputs.
class Random {
 public:
  Random() = default;
  explicit Random(uint64_t seed) : state_(seed != 0 ? seed : kDefault) {}
  uint64_t Next();
  uint32_t Below(uint32_t n) { return uint32_t(Next() % n); }

 private:
  static constexpr uint64_t kDefault = 0x9E3779B97F4A7C15ull;
  uint64_t state_ = kDefault;
};

struct Context {
  PPCContext& ctx;
  uint8_t* base;
  uint32_t data;  // the scratch area
  Random random;
};

// A case's outputs with what each part is, for Results::Check: bytes and
// words compare exactly; floats and doubles also match when both are NaNs
// (any sign or payload), counted as NaN-only. Infinities and every other
// value must match exactly. Mark a value as a float only when it's shown to
// be one (f1, a vector's lanes, a field the code loads and stores as a
// float); when unsure, compare it exactly. See "Comparing floats" in
// docs/loops/b7-loop.md.
class Outputs {
 public:
  void Bytes(const std::vector<uint8_t>& bytes);
  void Word(uint32_t value);
  void Float(uint32_t bits);
  void Floats(uint8_t* base, uint32_t address, uint32_t count);  // guest words
  void Double(uint64_t bits);

 private:
  friend class Results;
  enum class Kind : uint8_t { kExact, kFloat, kDouble };
  struct Entry {
    Kind kind;
    uint32_t offset;
    uint32_t size;
  };
  void Add(Kind kind, const uint8_t* data, uint32_t size);
  std::vector<uint8_t> bytes_;
  std::vector<Entry> entries_;
};

// Counts a harness's cases and differences; logs the first ten.
class Results {
 public:
  explicit Results(std::string name) : name_(std::move(name)) {}
  void Check(const std::vector<uint8_t>& recompiled, const std::vector<uint8_t>& native,
             const std::string& what);
  void Check(const Outputs& recompiled, const Outputs& native, const std::string& what);
  void Report() const;

 private:
  std::string name_;
  int cases_ = 0;
  int differ_ = 0;
  int nan_only_ = 0;
  bool typed_ = false;  // a case went through the Outputs check
};

using GuestFunction = void (*)(PPCContext&, uint8_t*);

// Guest memory helpers.
void Fill(uint8_t* base, uint32_t address, uint32_t bytes, uint8_t value = 0xCD);
std::vector<uint8_t> Read(uint8_t* base, uint32_t address, uint32_t bytes);
void Write(uint8_t* base, uint32_t address, const void* data, uint32_t bytes);
void StoreU64(uint8_t* base, uint32_t address, uint64_t value);
std::string Hex(const std::vector<uint8_t>& bytes);

// A context for a call: the harness's, with the stack below the scratch area.
PPCContext CallContext(const Context& h);

// Runs one case through both versions: for each, the scratch area is filled
// with 0xCD, `setup()` writes the inputs, `args(t)` sets the registers, the
// function runs, and `out_bytes` at `out` plus r3 are kept; then compared.
template <typename Setup, typename Args>
void Both(Context& h, Results& results, Setup setup, Args args, GuestFunction recompiled,
          GuestFunction native, uint32_t out, uint32_t out_bytes, const std::string& what) {
  std::vector<uint8_t> result[2];
  for (int v = 0; v < 2; v++) {
    Fill(h.base, h.data, kScratchBytes);
    setup();
    PPCContext t = CallContext(h);
    args(t);
    (v == 0 ? recompiled : native)(t, h.base);
    result[v] = Read(h.base, out, out_bytes);
    for (int i = 0; i < 4; i++) {
      result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
    }
  }
  results.Check(result[0], result[1], what);
}

// Runs the harnesses `--run_harnesses` asks for, the first time it's called.
void RunRequested(PPCContext& ctx, uint8_t* base);

// The harnesses, one per area (src/harnesses/<area>.cpp).
void ObjectAttributes(Context& h);
void Time(Context& h);
void Clock(Context& h);
void Strings(Context& h);
void LastError(Context& h);
void Gamma(Context& h);
void StartUp(Context& h);
void User(Context& h);
void Xmp(Context& h);
void Network(Context& h);
void Notify(Context& h);
void Profile(Context& h);
void Saves(Context& h);
void FileClass(Context& h);
void DataFile(Context& h);
void MovieFiles(Context& h);
void Xma(Context& h);
void Sprites(Context& h);
void Billboards(Context& h);
void CommandList(Context& h);
void CRuntime(Context& h);
void Math(Context& h);
void UiMovies(Context& h);
void UiMoviesGame(Context& h);
void RaceOverlays(Context& h);
void VehicleAccessors(Context& h);
void MatrixMath(Context& h);
void CarPhysics(Context& h);
void Collision(Context& h);
void CollisionWorld(Context& h);
void Heap(Context& h);
void CRuntimeHeap(Context& h);
void SoundCues(Context& h);
void Xact(Context& h);
void ActionMappers(Context& h);
void DataTables(Context& h);
void ModelAnimation(Context& h);
void CarSpecs(Context& h);
void Cameras(Context& h);
void StringsAndFont(Context& h);
void StdString(Context& h);
void TrackRoute(Context& h);
void AiDrivers(Context& h);
void RacerControllers(Context& h);
void TrackZones(Context& h);
void GraphicsResources(Context& h);
void VideoOutput(Context& h);
void DeviceState(Context& h);
void CourseScenery(Context& h);
void PostEffects(Context& h);
void Input(Context& h);
void ActionMappersBinding(Context& h);
void ScreenObjects(Context& h);
void HardcodedShaders(Context& h);
void Framework(Context& h);
void GameModes(Context& h);
void ModeTick(Context& h);
void Minimap(Context& h);
void CRuntimeExit(Context& h);
void Models(Context& h);
void SceneModels(Context& h);
void RaceHud(Context& h);
void UiMovieObjects(Context& h);
void SceneryLists(Context& h);
void ShaderObjects(Context& h);
void RaceSetup(Context& h);
void Vehicle(Context& h);
void AppStates(Context& h);
void SceneLoaders(Context& h);
void CarModels(Context& h);
void SaveData(Context& h);

}  // namespace harness
