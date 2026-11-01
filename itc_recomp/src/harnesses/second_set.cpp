// The second set's objects (src/second_set/, docs/functions/second-set.md)
// against the recompiled functions. Each case fills a block of guest memory
// and the stack below the call's r1 with random bytes, writes its inputs,
// runs the original (with --native_second_set off, so its callees are the
// originals too) and the register steps (hooks/second_set.h) from the same
// inputs, and compares the block, the frames' words and r3. The point
// records' matrices are only copied (no float math), so they're compared
// exactly.

#include "harness.h"

#include "base/flags.h"
#include "base/log.h"
#include "hooks/second_set.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"
#include "model_animation/model_animation.h"
#include "second_set/finds.h"
#include "second_set/points.h"

#include <fmt/format.h>

#include <bit>
#include <functional>
#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_second_set);
ITC_FLAG_DECLARE(bool, native_scene_loaders);
ITC_FLAG_DECLARE(bool, native_material_passes);

namespace harness {

namespace {

namespace ss = second_set;

constexpr uint32_t kBlockBytes = 0x20000;
constexpr uint32_t kStackBytes = 1024;  // below the call's r1, compared
constexpr uint32_t kPointRecordBytes = ss::kPointRecordBytes;

void RandomBytes(uint8_t* base, uint32_t address, uint32_t bytes, Random& r) {
  for (uint32_t i = 0; i < bytes; i += 4) {
    guest::StoreU32(base, address + i, uint32_t(r.Next()));
  }
}

void Put(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void PutString(uint8_t* base, uint32_t address, const std::string& s) {
  Write(base, address, s.c_str(), uint32_t(s.size() + 1));
}

std::string RandomName(Random& r, uint32_t length) {
  static const char kChars[] = "abcdefgxyzABZ_0129*";
  std::string name;
  for (uint32_t i = 0; i < length; i++) {
    name += kChars[r.Below(sizeof(kChars) - 1)];
  }
  return name;
}

// `name` with some letters' case flipped, some characters made `*`, or one
// changed.
std::string Variant(Random& r, std::string name) {
  for (char& ch : name) {
    switch (r.Below(8)) {
      case 0:
        if (ch >= 'a' && ch <= 'z') {
          ch = char(ch - 32);
        } else if (ch >= 'A' && ch <= 'Z') {
          ch = char(ch + 32);
        }
        break;
      case 1: ch = '*'; break;
      default: break;
    }
  }
  if (!name.empty() && r.Below(4) == 0) {
    name[r.Below(uint32_t(name.size()))] = char('a' + r.Below(26));
  }
  return name;
}

// One side: --native_second_set off for the recompiled one.
void RunSide(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t,
             uint8_t* base) {
  ITC_FLAG(native_second_set) = v == 1;
  (v == 0 ? recompiled : native)(t, base);
  ITC_FLAG(native_second_set) = true;
}

// One case through both versions: `bytes` of the block and the stack below
// r1 filled at random, `setup(t)` writing the inputs and the registers (once:
// they're written back for the second run).
void Case(Context& h, Results& results, uint32_t block, uint32_t bytes, Random& r,
          const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
          GuestFunction native, const std::string& what) {
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  const uint32_t stack = start.r1.u32 - kStackBytes;
  RandomBytes(h.base, block, bytes, r);
  RandomBytes(h.base, stack, kStackBytes, r);
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, bytes);
  const std::vector<uint8_t> frames = Read(h.base, stack, kStackBytes);
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), bytes);
    Write(h.base, stack, frames.data(), kStackBytes);
    PPCContext t = start;
    RunSide(v, recompiled, native, t, h.base);
    out[v].Bytes(Read(h.base, block, bytes));
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

// The frames' words compared, the saved registers' and v127's slots left out
// (the plain C++ doesn't write them): a case through both versions with the
// words at r1 - `below` + [0, bytes) compared, for each window.
struct Window {
  uint32_t below;
  uint32_t bytes;
};

void FrameCase(Context& h, Results& results, uint32_t block, uint32_t bytes, Random& r,
               const std::function<void(PPCContext&)>& setup, GuestFunction recompiled,
               GuestFunction native, const std::vector<Window>& windows,
               const std::string& what) {
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  const uint32_t stack = start.r1.u32 - kStackBytes;
  RandomBytes(h.base, block, bytes, r);
  RandomBytes(h.base, stack, kStackBytes, r);
  setup(start);
  const std::vector<uint8_t> inputs = Read(h.base, block, bytes);
  const std::vector<uint8_t> frames = Read(h.base, stack, kStackBytes);
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    Write(h.base, block, inputs.data(), bytes);
    Write(h.base, stack, frames.data(), kStackBytes);
    PPCContext t = start;
    RunSide(v, recompiled, native, t, h.base);
    out[v].Bytes(Read(h.base, block, bytes));
    for (const Window& w : windows) {
      out[v].Bytes(Read(h.base, start.r1.u32 - w.below, w.bytes));
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

// Clear's frame words (+80 to +160) under a caller's frame of `outer` bytes.
Window ClearWindow(uint32_t outer) {
  return {outer + ss::kClearFrameBytes - 80, 80};
}

// --- The point records ---

void Points(Context& h, Random& r, uint32_t block) {
  for (int which = 0; which < 2; which++) {
    Results results(which == 0 ? "PointRecord::SetNode (sub_820CE8D8)"
                               : "PointRecord::SetShape (sub_820CE900)");
    for (int c = 0; c < 32; c++) {
      Case(
          h, results, block, 0x400, r,
          [&](PPCContext& t) {
            PutString(h.base, block + 0x300, RandomName(r, c == 0 ? 0 : r.Below(31)));
            t.r3.u64 = block;
            t.r4.u64 = block + 0x300;
          },
          which == 0 ? __imp__sub_820CE8D8 : __imp__sub_820CE900,
          which == 0 ? hooks::SecondSetPointSetNode : hooks::SecondSetPointSetShape,
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PointRecord::Clear (sub_820CEA08)");
    for (int c = 0; c < 16; c++) {
      FrameCase(
          h, results, block, 0x400, r,
          [&](PPCContext& t) {
            t.r3.u64 = block + 16 * r.Below(8);
          },
          __imp__sub_820CEA08, hooks::SecondSetPointClear, {ClearWindow(0)},
          fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PointRecord::PointRecord (sub_820D0270)");
    for (int c = 0; c < 16; c++) {
      FrameCase(
          h, results, block, 0x400, r,
          [&](PPCContext& t) {
            t.r3.u64 = block + 16 * r.Below(8);
          },
          __imp__sub_820D0270, hooks::SecondSetPointConstruct,
          {ClearWindow(ss::kConstructFrameBytes)}, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    // A part model at +0x400: +408 nodes (0 to 8, now and then negative)
    // of 192 bytes at [+36] (+0x800), each +40 a shape index (-1 now and
    // then) into 544-byte shapes at [+40] (+0x1400), each named at +0; the
    // record's +192 one of the names, a variant of one or another.
    Results results("ModelPoint::FindShape (sub_820CEB30)");
    static const char* kNames[] = {"fb00punch_shape", "fog**_shape", "base_shape",
                                   "target_00_shape", "hl**cover_shape", "x"};
    const uint32_t record = block, model = block + 0x400;
    const uint32_t nodes = block + 0x800, shapes = block + 0x1400;
    for (int c = 0; c < 64; c++) {
      FrameCase(
          h, results, block, 0x3000, r,
          [&](PPCContext& t) {
            const uint32_t count = r.Below(9);
            Put(h.base, model + 408, r.Below(16) == 0 ? uint32_t(-int32_t(r.Below(3))) : count);
            Put(h.base, model + 36, nodes);
            Put(h.base, model + 40, shapes);
            std::vector<std::string> names;
            for (uint32_t s = 0; s < 8; s++) {
              names.push_back(r.Below(4) == 0 ? RandomName(r, r.Below(12))
                                              : kNames[r.Below(std::size(kNames))]);
              PutString(h.base, shapes + 544 * s, names.back());
            }
            for (uint32_t n = 0; n < 8; n++) {
              Put(h.base, nodes + 192 * n + 40, r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(8));
            }
            std::string wanted = names[r.Below(8)];
            switch (r.Below(4)) {
              case 0: wanted = Variant(r, wanted); break;
              case 1: wanted = RandomName(r, r.Below(12)); break;
              default: break;
            }
            PutString(h.base, record + 192, wanted);
            t.r3.u64 = record;
            t.r4.u64 = model;
          },
          __imp__sub_820CEB30, hooks::SecondSetFindShape,
          {{ss::kFindShapeFrameBytes - 80, 64}}, fmt::format("case {}", c));
    }
    results.Report();
  }
}

// The point groups' resets and inits: the group (or record) at the block,
// its words random; the names from the image's tables. The block, the last
// Clear's frame words (and ResetHeadlights' identity) and r3 compared.
void Groups(Context& h, Random& r, uint32_t block) {
  struct Group {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t frame;  // the function's own frame
    uint32_t bytes;  // what it touches from r3
    bool init;       // a single record's init (Clear under PointRecord's too)
  };
  const Group groups[] = {
      {"PartsPoints::ResetFrontBumper (sub_820DE0E0)", __imp__sub_820DE0E0,
       hooks::SecondSetPartsResetFrontBumper, 112, 2656, false},
      {"PartsPoints::ResetRearBumper (sub_820DE2C8)", __imp__sub_820DE2C8,
       hooks::SecondSetPartsResetRearBumper, 112, 1696, false},
      {"PartsPoints::ResetRearSpoiler (sub_820DE498)", __imp__sub_820DE498,
       hooks::SecondSetPartsResetRearSpoiler, 112, 720, false},
      {"PartsPoints::ResetBonnet (sub_820DE548)", __imp__sub_820DE548,
       hooks::SecondSetPartsResetBonnet, 128, 480, false},
      {"PartsPoints::ResetLeftDoor (sub_820DE600)", __imp__sub_820DE600,
       hooks::SecondSetPartsResetLeftDoor, 128, 6096, false},
      {"PartsPoints::ResetRightDoor (sub_820DE7E8)", __imp__sub_820DE7E8,
       hooks::SecondSetPartsResetRightDoor, 128, 6096, false},
      {"PartsPoints::ResetDoors (sub_820DE9D0)", __imp__sub_820DE9D0,
       hooks::SecondSetPartsResetDoors, 112, 5520, false},
      {"PartsPoints::ResetHeadlights (sub_820DF740)", __imp__sub_820DF740,
       hooks::SecondSetPartsResetHeadlights, 192, 3200, false},
      {"PartsPoints::ResetTailLights (sub_820DEBF0)", __imp__sub_820DEBF0,
       hooks::SecondSetPartsResetTailLights, 128, 3600, false},
      {"PartsPoints::ResetFenders (sub_820DECD0)", __imp__sub_820DECD0,
       hooks::SecondSetPartsResetFenders, 112, 1680, false},
      {"PartsPoints::ResetCabin (sub_820DEE88)", __imp__sub_820DEE88,
       hooks::SecondSetPartsResetCabin, 112, 1200, false},
      {"PartsPoints::ResetEngine (sub_820DF020)", __imp__sub_820DF020,
       hooks::SecondSetPartsResetEngine, 112, 480, false},
      {"PartsPoints::ResetMufflers (sub_820DF0C0)", __imp__sub_820DF0C0,
       hooks::SecondSetPartsResetMufflers, 128, 1456, false},
      {"PartsPoints::ResetShadows (sub_820DF128)", __imp__sub_820DF128,
       hooks::SecondSetPartsResetShadows, 112, 976, false},
      {"PartsPoints::InitCooling (sub_820DF410)", __imp__sub_820DF410,
       hooks::SecondSetPartsInitCooling, 96, 240, true},
      {"PartsPoints::InitSideSkirts (sub_820DF570)", __imp__sub_820DF570,
       hooks::SecondSetPartsInitSideSkirts, 96, 240, true},
      {"PartsPoints::InitLeftMirror (sub_820DF668)", __imp__sub_820DF668,
       hooks::SecondSetPartsInitLeftMirror, 96, 240, true},
      {"PartsPoints::InitRightMirror (sub_820DF6B0)", __imp__sub_820DF6B0,
       hooks::SecondSetPartsInitRightMirror, 96, 240, true},
      {"PartsPoints::InitMirror (sub_820DF6F8)", __imp__sub_820DF6F8,
       hooks::SecondSetPartsInitMirror, 96, 240, true},
      {"CockpitPoints::InitBonnet (sub_820DC0A8)", __imp__sub_820DC0A8,
       hooks::SecondSetCockpitInitBonnet, 96, 240, true},
      {"CockpitPoints::InitMirror (sub_820DC0F0)", __imp__sub_820DC0F0,
       hooks::SecondSetCockpitInitMirror, 96, 240, true},
      {"CockpitPoints::ResetFenders (sub_820DB878)", __imp__sub_820DB878,
       hooks::SecondSetCockpitResetFenders, 112, 720, false},
      {"CockpitPoints::ResetCabin (sub_820DB930)", __imp__sub_820DB930,
       hooks::SecondSetCockpitResetCabin, 112, 480, false},
      {"CockpitPoints::ResetDoors (sub_820DB9E8)", __imp__sub_820DB9E8,
       hooks::SecondSetCockpitResetDoors, 112, 480, false},
      {"CockpitPoints::ResetHeadlights (sub_820DBAA0)", __imp__sub_820DBAA0,
       hooks::SecondSetCockpitResetHeadlights, 128, 1920, false},
  };
  for (const Group& g : groups) {
    Results results(g.name);
    std::vector<Window> windows = {ClearWindow(g.frame)};
    if (g.init) {
      windows.push_back(ClearWindow(g.frame + ss::kConstructFrameBytes));
    }
    if (g.frame == 192) {
      windows.push_back({192 - 80, 64});  // ResetHeadlights' identity
    }
    for (int c = 0; c < 4; c++) {
      FrameCase(
          h, results, block, 0x2000, r,
          [&](PPCContext& t) { t.r3.u64 = block + 16 * r.Below(8); }, g.recompiled, g.native,
          windows, fmt::format("case {} ({} bytes)", c, g.bytes));
    }
    results.Report();
  }
}

// --- The point set-ups ---
//
// A world of two part models, each with up to 8 nodes (192 bytes, +40 a
// shape index or -1) and 8 shapes (544 bytes, named at +0, +396 one of two
// material blocks of up to 3 materials), its skeleton (+112) bound to a node
// file of 1 to 8 nodes; the point block's records named from the shapes'
// and nodes' names (or variants, or others). Each side builds it from the
// same seed and runs from the same random stack, the recompiled one with
// --native_second_set, --native_scene_loaders and --native_material_passes
// off (ModelPoint::Find, NameMatches and ScaleBlended the originals). The
// block (the records' matrices as floats), the models (the skeleton's array
// words left out), the skeletons' arrays, the materials (their first four
// words as floats) and r3 compared; the frames aren't (the plain C++ doesn't
// write the saved registers' slots).

// A small float (eighths from -4 to 4). As scene_loaders' harness.
uint32_t SmallFloat(Random& r) {
  return std::bit_cast<uint32_t>(float(int32_t(r.Below(65)) - 32) / 8.0f);
}

// A node file (4 KB at `file`) of `names.size()` nodes, as scene_loaders'
// harness builds them (ModelPoint::Find's cases).
void MakeNodeFile(uint8_t* base, uint32_t file, const std::vector<std::string>& names,
                  Random& r) {
  const uint32_t nodes = uint32_t(names.size());
  const uint32_t bones = r.Below(nodes + 1);
  const uint32_t first_bone = r.Below(nodes - bones + 1);
  RandomBytes(base, file, 4096, r);
  Put(base, file + 4, nodes);
  Put(base, file + 8, bones);
  Put(base, file + 12, first_bone);
  const uint32_t kRecords = 256, kSources = 1536, kMatrices = 1920, kTail = 2432;
  Put(base, file + 16, kRecords);
  Put(base, file + 20, kSources);
  Put(base, file + 24, kSources + 128);
  Put(base, file + 28, kSources + 256);
  Put(base, file + 32, kTail + 64);
  Put(base, file + 36, kMatrices);
  Put(base, file + 40, kTail + 128);
  Put(base, file + 44, kTail);
  Put(base, file + 48, kTail + 192);
  for (uint32_t n = 0; n < nodes; n++) {
    const uint32_t record = file + kRecords + 160 * n;
    PutString(base, record, names[n]);
    Put(base, record + 92, n == 0 || r.Below(4) == 0 ? 0xFFFFFFFFu : r.Below(n));
    static const uint32_t kTypes[] = {0, 1, 2, 3, 4, 5, 0x80, 0x81, 32};
    Put(base, record + 132, kTypes[r.Below(std::size(kTypes))]);
  }
  for (uint32_t at = kSources; at < kTail; at += 4) Put(base, file + at, SmallFloat(r));
}

// A skeleton's arrays (offset, bytes, header bytes before the address) as
// Skeleton::Bind makes them.
struct SkeletonArray {
  uint32_t offset, bytes, header;
};
std::vector<SkeletonArray> SkeletonArrays(uint8_t* base, uint32_t file) {
  const uint32_t nodes = guest::LoadU32(base, file + 4);
  const uint32_t bones = guest::LoadU32(base, file + 8);
  return {{128, 20, 0},
          {136, 16 + 144 * nodes, 16},
          {132, 16 + 64 * nodes, 16},
          {144, 16 + 64 * bones, 16},
          {176, 16 * nodes, 0}};
}

// The skeleton's arrays' contents into `out` (past their headers), each
// then freed, and their words in the skeleton cleared.
void TakeSkeleton(uint8_t* base, uint32_t skeleton, uint32_t file, Outputs& out) {
  for (const SkeletonArray& a : SkeletonArrays(base, file)) {
    const uint32_t at = guest::LoadU32(base, skeleton + a.offset);
    out.Word(at != 0);
    if (at == 0) continue;
    if (a.offset != 128) {
      out.Floats(base, at, (a.bytes - a.header) / 4);
    }
    memory::crt::OperatorDeleteArray(at - a.header);
    Put(base, skeleton + a.offset, 0);
  }
}

// The world's layout from the block.
constexpr uint32_t kPoints = 0;            // the point block (37,096 bytes)
constexpr uint32_t kModels = 0xA000;       // two part models, 0x400 apart
constexpr uint32_t kNodes = 0xA800;        // their nodes, 0x600 apart
constexpr uint32_t kShapes = 0xB400;       // their shapes, 0x1200 apart
constexpr uint32_t kMaterials = 0xD800;    // two material blocks, 0x800 apart
constexpr uint32_t kFiles = 0xE800;        // two node files, 0x1000 apart
constexpr uint32_t kModelWords = 0x10800;  // the 17 (or 6) models' words
constexpr uint32_t kCarLoads = 0x10900;    // [+37084]: +24 = the next
constexpr uint32_t kCarObject = 0x10A00;   // +6700, +6708 read
constexpr uint32_t kScene = 0x12600;       // [+37088]: +4 the model set
constexpr uint32_t kWorldBytes = 0x12800;

const char* const kShapeNames[] = {"fb00punch_shape", "fog00_shape",   "window01_shape",
                                   "base_shape",      "hl00cover_shape", "rb00num_shape",
                                   "seat00_L_shape",  "x_shape"};
const char* const kNodeNames[] = {"dl00_rot_00", "hl_00", "tl00_00_ef", "mf00_00_ef",
                                  "bn_00",       "hl00ef_00", "x"};

// The point groups: (offset in the block, records). Parts' and cockpit's.
struct Span {
  uint32_t at, records;
};
const Span kPartsSpans[] = {{0, 11},     {2656, 1},  {2896, 7},   {4592, 1},  {4832, 3},
                            {5552, 2},   {6032, 25}, {12128, 25}, {18224, 23}, {23760, 1},
                            {24000, 1},  {24240, 1}, {24480, 13}, {27680, 15}, {31280, 7},
                            {32960, 5},  {34160, 2}, {34896, 5},  {36096, 4}};
const Span kCockpitSpans[] = {{0, 3}, {720, 2}, {1200, 1}, {1440, 2}, {1920, 1}, {2160, 8}};

std::string PickName(Random& r, const char* const* names, size_t count) {
  std::string name = names[r.Below(uint32_t(count))];
  switch (r.Below(6)) {
    case 0: return Variant(r, name);
    case 1: return RandomName(r, r.Below(12));
    default: return name;
  }
}

// The world from `seed`: random bytes, the two models, the point records'
// names (for `spans`), the models' words (`kinds` of them, 0 now and then),
// the car loads and the scene with model set `set`.
void BuildWorld(uint8_t* base, uint32_t block, uint64_t seed, const Span* spans, size_t span_count,
                uint32_t kinds, uint32_t set) {
  Random g(seed);
  RandomBytes(base, block, kWorldBytes, g);
  for (uint32_t m = 0; m < 2; m++) {
    const uint32_t model = block + kModels + 0x400 * m;
    const uint32_t nodes = block + kNodes + 0x600 * m;
    const uint32_t shapes = block + kShapes + 0x1200 * m;
    const uint32_t file = block + kFiles + 0x1000 * m;
    Put(base, model + 36, nodes);
    Put(base, model + 40, shapes);
    Put(base, model + 408, g.Below(8) == 0 ? 0 : 1 + g.Below(8));
    for (uint32_t n = 0; n < 8; n++) {
      Put(base, nodes + 192 * n + 40, g.Below(5) == 0 ? 0xFFFFFFFFu : g.Below(8));
    }
    for (uint32_t i = 0; i < 8; i++) {
      PutString(base, shapes + 544 * i, kShapeNames[g.Below(std::size(kShapeNames))]);
      Put(base, shapes + 544 * i + 396, block + kMaterials + 0x800 * g.Below(2));
    }
    std::vector<std::string> names(1 + g.Below(8));
    for (auto& n : names) n = kNodeNames[g.Below(std::size(kNodeNames))];
    MakeNodeFile(base, file, names, g);
    const uint32_t skeleton = model + 112;
    for (const SkeletonArray& a : SkeletonArrays(base, file)) Put(base, skeleton + a.offset, 0);
    if (!model_animation::SkeletonBind(base, skeleton, file, 0)) {
      ITC_LOG_WARN("harness: second_set: Skeleton::Bind declined");
    }
  }
  for (uint32_t b = 0; b < 2; b++) {
    const uint32_t materials = block + kMaterials + 0x800 * b;
    Put(base, materials + 8, g.Below(2));
    Put(base, materials + 12, g.Below(3));
    for (uint32_t i = 0; i < 3; i++) {
      for (uint32_t w = 0; w < 4; w++) {
        Put(base, materials + 16 + 512 * i + 4 * w, SmallFloat(g));
      }
    }
  }
  for (size_t s = 0; s < span_count; s++) {
    for (uint32_t i = 0; i < spans[s].records; i++) {
      const uint32_t record = block + kPoints + spans[s].at + kPointRecordBytes * i;
      PutString(base, record + 160, PickName(g, kNodeNames, std::size(kNodeNames)));
      PutString(base, record + 192, PickName(g, kShapeNames, std::size(kShapeNames)));
    }
  }
  for (uint32_t k = 0; k < kinds; k++) {
    // The parts' kinds may be missing (0); the cockpit's six are always
    // there (its Find reads each).
    const uint32_t which = g.Below(5);
    Put(base, block + kModelWords + 4 * k,
        which == 0 && kinds == 17 ? 0 : block + kModels + 0x400 * (which & 1));
  }
  Put(base, block + 37084, block + kCarLoads);
  Put(base, block + kCarLoads + 24, block + kCarObject);
  Put(base, block + 37088, block + kScene);
  Put(base, block + kScene + 4, set);
}

// The block's words as outputs: the point records' +16 to +160 (`spans`),
// the mufflers' matrices and the headlights' copied float (the parts') as
// floats, the rest exactly.
void TakeBlock(uint8_t* base, uint32_t block, uint32_t bytes, const Span* spans,
               size_t span_count, bool parts, Outputs& out) {
  std::vector<bool> floats(bytes / 4, false);
  auto mark = [&](uint32_t from, uint32_t count) {
    for (uint32_t w = 0; w < count && from / 4 + w < floats.size(); w++) floats[from / 4 + w] = true;
  };
  for (size_t s = 0; s < span_count; s++) {
    for (uint32_t i = 0; i < spans[s].records; i++) {
      mark(spans[s].at + kPointRecordBytes * i + 16, 36);
    }
  }
  if (parts) {
    mark(34640, 64);
    mark(24480 + 3184, 1);
  }
  uint32_t w = 0;
  while (w < floats.size()) {
    uint32_t end = w;
    while (end < floats.size() && floats[end] == floats[w]) end++;
    if (floats[w]) {
      out.Floats(base, block + 4 * w, end - w);
    } else {
      out.Bytes(Read(base, block + 4 * w, 4 * (end - w)));
    }
    w = end;
  }
}


void FindSide(int v, GuestFunction recompiled, GuestFunction native, PPCContext& t,
              uint8_t* base) {
  ITC_FLAG(native_second_set) = v == 1;
  ITC_FLAG(native_scene_loaders) = v == 1;
  ITC_FLAG(native_material_passes) = v == 1;
  (v == 0 ? recompiled : native)(t, base);
  ITC_FLAG(native_second_set) = true;
  ITC_FLAG(native_scene_loaders) = true;
  ITC_FLAG(native_material_passes) = true;
}

// One set-up case: the world from `seed`, r3 = the block + `at`, r4 = model
// `model` (0 or 1) or, for -1, the models' words.
void FindCase(Context& h, Results& results, uint32_t block, uint64_t seed, const Span* spans,
              size_t span_count, bool parts, uint32_t kinds, uint32_t set, uint32_t at,
              int model, GuestFunction recompiled, GuestFunction native, const std::string& what) {
  PPCContext start = CallContext(h);
  start.r1.u64 &= ~0xFull;
  const uint32_t stack = start.r1.u32 - kStackBytes;
  Random sr(seed ^ 0x5354);
  RandomBytes(h.base, stack, kStackBytes, sr);
  const std::vector<uint8_t> frames = Read(h.base, stack, kStackBytes);
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    BuildWorld(h.base, block, seed, spans, span_count, kinds, set);
    Write(h.base, stack, frames.data(), kStackBytes);
    PPCContext t = start;
    t.r3.u64 = block + at;
    t.r4.u64 = model < 0 ? block + kModelWords : block + kModels + 0x400 * uint32_t(model);
    FindSide(v, recompiled, native, t, h.base);
    TakeBlock(h.base, block, parts ? 37096 : 4096, spans, span_count, parts, out[v]);
    for (uint32_t m = 0; m < 2; m++) {
      const uint32_t object = block + kModels + 0x400 * m;
      const uint32_t file = block + kFiles + 0x1000 * m;
      std::vector<uint8_t> words = Read(h.base, object, 464);
      for (const SkeletonArray& a : SkeletonArrays(h.base, file)) {
        std::fill_n(words.begin() + 112 + a.offset, 4, 0);
      }
      out[v].Bytes(words);
      TakeSkeleton(h.base, object + 112, file, out[v]);
    }
    for (uint32_t b = 0; b < 2; b++) {
      const uint32_t materials = block + kMaterials + 0x800 * b;
      out[v].Bytes(Read(h.base, materials, 16));
      for (uint32_t i = 0; i < 3; i++) {
        out[v].Floats(h.base, materials + 16 + 512 * i, 4);
        out[v].Bytes(Read(h.base, materials + 32 + 512 * i, 96));
      }
    }
    out[v].Word(t.r3.u32);
    out[v].Word(uint32_t(t.r3.u64 >> 32));
  }
  results.Check(out[0], out[1], what);
}

void Finds(Context& h, Random& r, uint32_t block) {
  if (!hooks::SecondSetFindsNative()) {
    ITC_LOG_WARN("harness: second_set: the point set-ups skipped (memcpy, --native_models or "
                 "--native_material_passes not native)");
    return;
  }
  struct Find {
    const char* name;
    GuestFunction recompiled, native;
    uint32_t at;  // the group in the parts' block
  };
#define ITC_FIND_STEP(function) \
  [](PPCContext& c, uint8_t* b) { hooks::SecondSetFindStep(c, b, ss::function); }
  const Find finds[] = {
      {"PartsPoints::FindFrontBumper (sub_820DE140)", __imp__sub_820DE140,
       ITC_FIND_STEP(PartsFindFrontBumper), 0},
      {"PartsPoints::FindRearBumper (sub_820DE328)", __imp__sub_820DE328,
       ITC_FIND_STEP(PartsFindRearBumper), 2896},
      {"PartsPoints::FindRearSpoiler (sub_820DE4E8)", __imp__sub_820DE4E8,
       ITC_FIND_STEP(PartsFindRearSpoiler), 4832},
      {"PartsPoints::FindBonnet (sub_820DE5B0)", __imp__sub_820DE5B0,
       ITC_FIND_STEP(PartsFindBonnet), 5552},
      {"PartsPoints::FindLeftDoor (sub_820DE668)", __imp__sub_820DE668,
       ITC_FIND_STEP(PartsFindLeftDoor), 6032},
      {"PartsPoints::FindRightDoor (sub_820DE878)", __imp__sub_820DE878,
       ITC_FIND_STEP(PartsFindRightDoor), 12128},
      {"PartsPoints::FindDoors (sub_820DEA20)", __imp__sub_820DEA20,
       ITC_FIND_STEP(PartsFindDoors), 18224},
      {"PartsPoints::FindHeadlights (sub_820DF7F0)", __imp__sub_820DF7F0,
       ITC_FIND_STEP(PartsFindHeadlights), 24480},
      {"PartsPoints::FindTailLights (sub_820DEC58)", __imp__sub_820DEC58,
       ITC_FIND_STEP(PartsFindTailLights), 27680},
      {"PartsPoints::FindFenders (sub_820DED20)", __imp__sub_820DED20,
       ITC_FIND_STEP(PartsFindFenders), 31280},
      {"PartsPoints::FindCabin (sub_820DEED8)", __imp__sub_820DEED8,
       ITC_FIND_STEP(PartsFindCabin), 32960},
      {"PartsPoints::FindMufflers (sub_820E0080)", __imp__sub_820E0080,
       ITC_FIND_STEP(PartsFindMufflers), 34640},
  };
  for (const Find& f : finds) {
    Results results(f.name);
    for (int c = 0; c < 8; c++) {
      FindCase(h, results, block, r.Next(), kPartsSpans, std::size(kPartsSpans), true, 17,
               r.Below(2), f.at, int(r.Below(2)), f.recompiled, f.native,
               fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("PartsPoints::Find (sub_820E0258)");
    for (int c = 0; c < 16; c++) {
      const uint32_t set = c % 2 == 0 ? 0 : (c % 4 == 1 ? 1 : uint32_t(r.Next()));
      FindCase(h, results, block, r.Next(), kPartsSpans, std::size(kPartsSpans), true, 17, set,
               0, -1, __imp__sub_820E0258, hooks::SecondSetPartsFind,
               fmt::format("case {} (set {:#x})", c, set));
    }
    results.Report();
  }
  {
    Results results("CockpitPoints::FindHeadlights (sub_820DBB08)");
    for (int c = 0; c < 8; c++) {
      FindCase(h, results, block, r.Next(), kCockpitSpans, std::size(kCockpitSpans), false, 6,
               0, 2160, int(r.Below(2)), __imp__sub_820DBB08,
               ITC_FIND_STEP(CockpitFindHeadlights), fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("CockpitPoints::Find (sub_820DBC88)");
    for (int c = 0; c < 12; c++) {
      FindCase(h, results, block, r.Next(), kCockpitSpans, std::size(kCockpitSpans), false, 6,
               0, 0, -1, __imp__sub_820DBC88, hooks::SecondSetCockpitFind,
               fmt::format("case {}", c));
    }
    results.Report();
  }
#undef ITC_FIND_STEP
}

}  // namespace

void SecondSet(Context& h) {
  Random r(0x7365636F6E64ull);  // "second"
  const uint32_t allocation = memory::Alloc(kBlockBytes + 16, 0);
  const uint32_t block = (allocation + 15) & ~15u;
  Points(h, r, block);
  Groups(h, r, block);
  Finds(h, r, block);
  memory::Free(allocation);
}

}  // namespace harness
