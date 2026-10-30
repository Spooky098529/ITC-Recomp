// The app's start-up (src/app_start_up/, docs/functions/app-start-up.md)
// against the recompiled functions, through the adapters (with
// --native_app_start_up on). The globals the setters write are the game's
// live ones: each is saved first and put back after the cases. The vertex
// declarations both versions make are compared by their bytes (not their
// addresses) and freed. Everything is compared exactly (nothing here is a
// float).

#include "harness.h"

#include "app_start_up/start_up.h"
#include "base/flags.h"
#include "base/log.h"
#include "helpers/clock.h"
#include "hooks/app_start_up.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"
#include "memory/heap.h"

#include <fmt/format.h>

#include <string>
#include <vector>

ITC_FLAG_DECLARE(bool, native_app_start_up);

namespace harness {

namespace {

namespace asu = app_start_up;

constexpr uint32_t kAppInterfaceVtable = 0x82000E38;

// A setter of one or two globals: random r3 (or none), the globals read
// after each version, then put back.
void Setter(Context& h, const char* name, GuestFunction recompiled, GuestFunction native,
            std::initializer_list<uint32_t> globals) {
  std::vector<uint32_t> saved;
  for (const uint32_t g : globals) {
    saved.push_back(guest::LoadU32(h.base, g));
  }
  Results results(name);
  for (int c = 0; c < 8; c++) {
    const uint64_t r3 = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      for (const uint32_t g : globals) {
        guest::StoreU32(h.base, g, 0xA5A5A5A5u + c);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = r3;
      (v == 0 ? recompiled : native)(t, h.base);
      for (const uint32_t g : globals) {
        out[v].Word(guest::LoadU32(h.base, g));
      }
    }
    results.Check(out[0], out[1], fmt::format("r3 {:016X}", r3));
  }
  results.Report();
  size_t i = 0;
  for (const uint32_t g : globals) {
    guest::StoreU32(h.base, g, saved[i++]);
  }
}

// The removed adapters' register steps (the setters, sub_8225FB70 and
// Movies::Set: only App::StartUp's plain C++ calls them in play).
void NativeReadErrorSetApp(PPCContext& t, uint8_t* base) { asu::ReadErrorSetApp(base, t.r3.u32); }
void NativeSetAppInterface84(PPCContext& t, uint8_t* base) {
  asu::SetAppInterface84(base, t.r3.u32);
}
void NativeSetRenderApp(PPCContext& t, uint8_t* base) { asu::SetRenderApp(base, t.r3.u32); }
void NativeClearWords8110(PPCContext&, uint8_t* base) { asu::ClearWords8110(base); }
void NativeMoviesSet(PPCContext& t, uint8_t* base) {
  if (const auto movie = asu::MoviesSet(base, t.r3.u32)) {
    t.r3.u64 = *movie;
    return;
  }
  __imp__sub_820A4360(t, base);
}

void MoviesSet(Context& h) {
  const uint32_t saved = guest::LoadU32(h.base, asu::kMovieClass);
  Results results("Movies::Set (sub_820A4360)");
  // The interface at the scratch area's start; a fake vtable at +256 (a
  // copy of the app interface's, slot +44 the other getter, sub_82379338,
  // for the cases that take the original's path).
  const uint32_t interface = h.data;
  const uint32_t fake = h.data + 256;
  for (int c = 0; c < 8; c++) {
    const bool real = c < 6;
    const uint32_t word112 = uint32_t(h.random.Next());
    const uint32_t word108 = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t k = 0; k < 64; k += 4) {
        guest::StoreU32(h.base, fake + k, guest::LoadU32(h.base, kAppInterfaceVtable + k));
      }
      guest::StoreU32(h.base, fake + 44, 0x82379338);
      guest::StoreU32(h.base, interface, real ? kAppInterfaceVtable : fake);
      guest::StoreU32(h.base, interface + 108, word108);
      guest::StoreU32(h.base, interface + 112, word112);
      guest::StoreU32(h.base, asu::kMovieClass, 0x5A5A5A5A);
      PPCContext t = CallContext(h);
      t.r3.u64 = interface;
      (v == 0 ? __imp__sub_820A4360 : NativeMoviesSet)(t, h.base);
      out[v].Word(guest::LoadU32(h.base, asu::kMovieClass));
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, h.data, 512));
    }
    results.Check(out[0], out[1], real ? "the app's interface" : "another slot +44");
  }
  results.Report();
  guest::StoreU32(h.base, asu::kMovieClass, saved);
}

void CreateVertexDeclaration(Context& h) {
  Results results("CreateVertexDeclaration (sub_822A8980)");
  if (!hooks::NativeHeapActive()) {
    ITC_LOG_WARN("harness: app_start_up: CreateVertexDeclaration needs --native_heap");
    return;
  }
  const uint32_t elements = h.data;
  for (int c = 0; c < 24; c++) {
    // 0 to 12 elements; streams 0 to 15 (one case with 16 to 20, the
    // original's path).
    const uint32_t count = c == 0 ? 0 : 1 + h.random.Below(12);
    const bool wide = c == 23;
    std::vector<uint32_t> words(count * 3);
    std::vector<uint16_t> streams(count);
    for (uint32_t i = 0; i < count; i++) {
      streams[i] = uint16_t(wide && i == count / 2 ? 16 + h.random.Below(5)
                                                    : (c % 3 == 0 ? 0 : h.random.Below(16)));
      for (int k = 0; k < 3; k++) {
        words[i * 3 + k] = uint32_t(h.random.Next());
      }
    }
    const uint32_t end_word1 = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t i = 0; i < count; i++) {
        for (int k = 0; k < 3; k++) {
          guest::StoreU32(h.base, elements + 12 * i + 4 * k, words[i * 3 + k]);
        }
        guest::StoreU16(h.base, elements + 12 * i, streams[i]);
      }
      guest::StoreU32(h.base, elements + 12 * count, 0x00FF0000u | (end_word1 & 0xFFFF));
      guest::StoreU32(h.base, elements + 12 * count + 4, end_word1);
      PPCContext t = CallContext(h);
      t.r3.u64 = elements;
      (v == 0 ? __imp__sub_822A8980 : sub_822A8980)(t, h.base);
      const uint32_t declaration = t.r3.u32;
      out[v].Word(declaration != 0);
      if (declaration != 0) {
        out[v].Bytes(Read(h.base, declaration, 56 + 12 * count));
        memory::crt::XMemFree(declaration, 0x24800000);
      }
      out[v].Bytes(Read(h.base, h.data, 256));
    }
    results.Check(out[0], out[1], fmt::format("{} elements{}", count, wide ? ", a wide stream" : ""));
  }
  results.Report();
}

// --- The network manager's set-up ---

// The globals the set-up writes: the network's (0x826C6000 to 0x826C8000,
// with the manager at 0x826C74A0 and the user table), the sign-in bounds
// and XNet's block.
struct Region {
  uint32_t address;
  uint32_t bytes;
};
constexpr Region kNetworkRegions[] = {
    {0x826C6000, 0x2000}, {0x82618B78, 16}, {asu::kXNetConfig, 76}};
// The words the set-up writes, filled with random bytes in some cases.
constexpr Region kNetworkWritten[] = {
    {0x826C6650, 60},  {0x826C6080, 28},  {0x826C6AA0, 32},  {0x826C6AD4, 4},
    {0x826C72FC, 8},   {0x826C7670, 384}, {0x826C7938, 104}, {0x826C74A0 + 444, 16},
    {0x82618B7C, 8},   {asu::kXNetConfig, 76}};

class NetworkState {
 public:
  explicit NetworkState(uint8_t* base) : base_(base) {
    for (const Region& r : kNetworkRegions) {
      saved_.push_back(Read(base, r.address, r.bytes));
    }
  }
  ~NetworkState() { Restore(); }
  void Restore() {
    for (size_t i = 0; i < saved_.size(); i++) {
      Write(base_, kNetworkRegions[i].address, saved_[i].data(), uint32_t(saved_[i].size()));
    }
  }
  void Put(const std::vector<std::vector<uint8_t>>& state) {
    for (size_t i = 0; i < state.size(); i++) {
      Write(base_, kNetworkRegions[i].address, state[i].data(), uint32_t(state[i].size()));
    }
  }
  std::vector<std::vector<uint8_t>> Random(Random& random) {
    Restore();
    for (const Region& r : kNetworkWritten) {
      for (uint32_t i = 0; i < r.bytes; i++) {
        guest::StoreU8(base_, r.address + i, uint8_t(random.Next()));
      }
    }
    std::vector<std::vector<uint8_t>> state;
    for (const Region& r : kNetworkRegions) {
      state.push_back(Read(base_, r.address, r.bytes));
    }
    return state;
  }
  std::vector<std::vector<uint8_t>> Live() {
    Restore();
    return saved_;
  }
  void Out(Outputs& out) {
    for (const Region& r : kNetworkRegions) {
      out.Bytes(Read(base_, r.address, r.bytes));
    }
  }

 private:
  uint8_t* base_;
  std::vector<std::vector<uint8_t>> saved_;
};

// A network case: `prepare` the state (the random generator for its own
// words), registers from `args`, both versions from the same state, the
// regions, r3 and `extra(t)` compared.
template <typename Args, typename Extra>
void NetworkCase(Context& h, Results& results, NetworkState& state,
                 const std::vector<std::vector<uint8_t>>& before, GuestFunction recompiled,
                 GuestFunction native, Args args, Extra extra, const std::string& what) {
  Outputs out[2];
  for (int v = 0; v < 2; v++) {
    state.Put(before);
    Fill(h.base, h.data, kScratchBytes);
    PPCContext t = CallContext(h);
    args(t);
    (v == 0 ? recompiled : native)(t, h.base);
    state.Out(out[v]);
    out[v].Word(t.r3.u32);
    extra(t, out[v]);
  }
  results.Check(out[0], out[1], what);
}

// The register steps of the functions with no adapter (only
// Network::SetUp's plain C++ calls them): memcpy through the guest where
// c_runtime::Memcpy refuses.
asu::MemcpyCall GuestMemcpy(PPCContext& t, uint8_t* base) {
  return [&t, base](uint32_t to, uint32_t from, uint32_t size) {
    t.r3.u64 = to;
    t.r4.u64 = from;
    t.r5.u64 = size;
    sub_825A7410(t, base);
  };
}
void NativeSignInSetUp(PPCContext& t, uint8_t* base) {
  t.r3.u64 = asu::SignInSetUp(base, t.r3.u32, t.r4.u32, t.r5.u32, t.r6.u32, t.r7.u32);
}
void NativeXNetConfigure(PPCContext& t, uint8_t* base) {
  const uint32_t config = t.r3.u32;
  const uint32_t source = t.r4.u32;
  t.r3.u64 = asu::XNetConfigure(base, config, source, GuestMemcpy(t, base));
}
void NativeXNetStartup(PPCContext& t, uint8_t* base) {
  t.r3.u64 = asu::XNetStartup(base, GuestMemcpy(t, base));
}
void NativeNetworkStartUp(PPCContext& t, uint8_t* base) {
  t.r3.u64 = asu::NetworkStartUp(base, t.r3.u32, GuestMemcpy(t, base));
}

void NetworkSetUp(Context& h) {
  NetworkState state(h.base);
  helpers::SetClocksForHarness(std::nullopt, 0x12345678);
  const auto none = [](PPCContext&, Outputs&) {};
  {
    Results results("XNet::Configure (sub_82577560)");
    for (int c = 0; c < 16; c++) {
      // The header at the scratch area's +512, the block at +0.
      std::vector<uint32_t> header(11);
      for (auto& w : header) {
        w = uint32_t(h.random.Next());
      }
      if (c % 4 == 0) {
        header[2] |= 1;
      } else {
        header[2] &= ~1u;
      }
      if (c % 4 == 1) {
        header[9] &= 0xFFFF0000;  // +38 = 0
      }
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        for (uint32_t i = 0; i < 11; i++) {
          guest::StoreU32(h.base, h.data + 512 + 4 * i, header[i]);
        }
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data;
        t.r4.u64 = h.data + 512;
        (v == 0 ? __imp__sub_82577560 : NativeXNetConfigure)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, 1024));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("header {}", c));
    }
    results.Report();
  }
  {
    Results results("SignIn::SetUp (sub_8214F1C0)");
    for (int c = 0; c < 8; c++) {
      auto before = c == 0 ? state.Live() : state.Random(h.random);
      if (c % 2 == 1) {
        // No listener yet.
        for (int b = 0; b < 4; b++) {
          before[0][0x194C + b] = 0;
        }
      }
      const uint64_t r[5] = {h.random.Next(), h.random.Next(), h.random.Next(), h.random.Next(),
                             c % 4 == 3 ? 0 : h.random.Next()};
      NetworkCase(h, results, state, before, __imp__sub_8214F1C0, NativeSignInSetUp,
                  [&](PPCContext& t) {
                    t.r3.u64 = r[0];
                    t.r4.u64 = r[1];
                    t.r5.u64 = r[2];
                    t.r6.u64 = r[3];
                    t.r7.u64 = r[4];
                  },
                  none, fmt::format("case {}", c));
    }
    results.Report();
  }
  const uint64_t app = 0x40000000u | uint32_t(h.random.Next());
  {
    Results results("XNetStartup (sub_82575B08)");
    for (int c = 0; c < 2; c++) {
      auto before = c == 0 ? state.Live() : state.Random(h.random);
      NetworkCase(h, results, state, before, __imp__sub_82575B08, NativeXNetStartup,
                  [](PPCContext&) {}, none, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Network::StartUp (sub_82148868)");
    for (int c = 0; c < 3; c++) {
      auto before = c == 0 ? state.Live() : state.Random(h.random);
      NetworkCase(h, results, state, before, __imp__sub_82148868, NativeNetworkStartUp,
                  [&](PPCContext& t) { t.r3.u64 = app; }, none, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("Network::SetUp (sub_8214E190)");
    for (int c = 0; c < 3; c++) {
      auto before = c == 0 ? state.Live() : state.Random(h.random);
      if (c == 2) {
        for (int b = 0; b < 4; b++) {
          before[0][0x194C + b] = 0;
        }
      }
      NetworkCase(h, results, state, before, __imp__sub_8214E190, sub_8214E190,
                  [&](PPCContext& t) { t.r3.u64 = app; },
                  [&](PPCContext& t, Outputs& out) {
                    // The XNADDR in the original's 144-byte frame.
                    out.Bytes(Read(h.base, t.r1.u32 - 144 + 80, 36));
                  },
                  fmt::format("case {}", c));
    }
    results.Report();
  }
  helpers::SetClocksForHarness(std::nullopt, std::nullopt);
}

// --- The scene target ---

// SceneTarget::Create with the display's tiling block a fake in the scratch
// area (its multisample type 0 to 3, random tiles; the game's pointer put
// back after), the two globals saved and put back, everything made freed.
// The recompiled run has --native_app_start_up off (so its SceneTarget is
// the original too). Compared: r3's being 0, the target's words (+16, +20,
// +28 exactly; each resource's header with its address's low 12 bits, the
// texture's data addresses' top 20 bits cleared, as the cameras' cases do),
// the scratch area's tiling block.
void SceneTargetCases(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Results results("SceneTarget::Create (sub_8209B378) with SceneTarget::SceneTarget and "
                  "Tiling::LargestTile");
  const uint32_t saved_app = guest::LoadU32(h.base, asu::kSceneTargetApp);
  const uint32_t saved_target = guest::LoadU32(h.base, asu::kSceneTarget);
  const uint32_t saved_tiling = guest::LoadU32(h.base, asu::kDisplayTiling);
  const uint32_t tiling = h.data;
  for (int c = 0; c < 12; c++) {
    const uint32_t type = c < 4 ? uint32_t(c) : h.random.Below(4);
    const uint32_t tiles = c == 4 ? 0 : 1 + h.random.Below(4);
    std::vector<uint32_t> words(4 * tiles);
    for (uint32_t i = 0; i < tiles; i++) {
      const uint32_t left = h.random.Below(1280), top = h.random.Below(720);
      words[4 * i + 0] = left;
      words[4 * i + 1] = top;
      words[4 * i + 2] = left + 1 + h.random.Below(1280);
      words[4 * i + 3] = top + 1 + h.random.Below(720);
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, 512);
      guest::StoreU32(h.base, tiling + 0, 1280);
      guest::StoreU32(h.base, tiling + 4, 720);
      guest::StoreU32(h.base, tiling + 8, type);
      guest::StoreU32(h.base, tiling + 12, tiles);
      for (uint32_t i = 0; i < 4 * tiles; i++) {
        guest::StoreU32(h.base, tiling + 16 + 4 * i, words[i]);
      }
      guest::StoreU32(h.base, asu::kDisplayTiling, tiling);
      guest::StoreU32(h.base, asu::kSceneTarget, 0x5A5A5A5A);
      PPCContext t = CallContext(h);
      // The app's interface: the game's, so the video output's slots are.
      t.r3.u64 = saved_app;
      if (v == 0) {
        const bool flag = ITC_FLAG(native_app_start_up);
        ITC_FLAG(native_app_start_up) = false;
        __imp__sub_8209B378(t, h.base);
        ITC_FLAG(native_app_start_up) = flag;
      } else {
        sub_8209B378(t, h.base);
      }
      const uint32_t target = t.r3.u32;
      out[v].Word(target != 0);
      out[v].Word(guest::LoadU32(h.base, asu::kSceneTargetApp));
      out[v].Word(guest::LoadU32(h.base, asu::kSceneTarget) == target);
      out[v].Bytes(Read(h.base, tiling, 16 + 16 * tiles));
      if (target == 0) {
        continue;
      }
      out[v].Word(guest::LoadU32(h.base, target + 16));
      out[v].Word(guest::LoadU32(h.base, target + 20));
      out[v].Word(guest::LoadU32(h.base, target + 28));
      for (const uint32_t offset : {0u, 4u, 8u, 12u}) {
        const uint32_t surface = guest::LoadU32(h.base, target + offset);
        out[v].Word(surface != 0);
        if (surface == 0) {
          continue;
        }
        out[v].Word(surface & 0xFFF);
        out[v].Bytes(Read(h.base, surface, 48));
        memory::crt::XMemFree(surface, 0x24800000);
      }
      const uint32_t texture = guest::LoadU32(h.base, target + 24);
      out[v].Word(texture != 0);
      if (texture != 0) {
        out[v].Word(texture & 0xFFF);
        std::vector<uint8_t> header = Read(h.base, texture, 52);
        const uint32_t data = guest::LoadU32(h.base, texture + 32) & 0xFFFFF000;
        const uint32_t mips = guest::LoadU32(h.base, texture + 48) & 0xFFFFF000;
        out[v].Word(data != 0);
        out[v].Word(mips != 0);
        for (int k = 0; k < 3; k++) {
          header[32 + k] = 0;
          header[48 + k] = 0;
        }
        header[35] &= 0x0F;
        header[51] &= 0x0F;
        out[v].Bytes(header);
        if (mips) {
          memory::crt::XMemFree(mips, 0xBC800000);
        }
        memory::crt::XMemFree(data, 0xBC800000);
        memory::crt::XMemFree(texture, 0x24800000);
      }
      memory::crt::OperatorDelete(target);
    }
    results.Check(out[0], out[1], fmt::format("case {}: type {}, {} tiles", c, type, tiles));
  }
  guest::StoreU32(h.base, asu::kSceneTargetApp, saved_app);
  guest::StoreU32(h.base, asu::kSceneTarget, saved_target);
  guest::StoreU32(h.base, asu::kDisplayTiling, saved_tiling);
  results.Report();
}

// --- The light billboards' shaders ---

// Billboards::Init on the game's globals (saved and put back), both
// versions with the stored shaders; the recompiled run with
// --native_app_start_up off (its CreateVertexDeclaration the original too).
// Compared: the ring's two words, the glow word, the declaration's bytes
// (freed), and each shader object's being made and its first word (the
// objects hold their microcode's addresses, which differ by allocation;
// their bytes are the shader objects' harness's: left made, a few KiB).
void BillboardsInitCases(Context& h) {
  if (!hooks::NativeHeapActive()) {
    return;
  }
  Results results("Billboards::Init (sub_8209BE88)");
  constexpr uint32_t kGlobals = 0x826A5098, kWords = 5, kGlow = 0x8266D090;
  uint32_t saved[kWords];
  for (uint32_t i = 0; i < kWords; i++) {
    saved[i] = guest::LoadU32(h.base, kGlobals + 4 * i);
  }
  const uint32_t saved_glow = guest::LoadU32(h.base, kGlow);
  for (int c = 0; c < 4; c++) {
    const uint32_t ring = c % 2 == 0 ? 0 : 1 + h.random.Below(3);
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32(h.base, kGlobals + 0, ring);
      guest::StoreU32(h.base, kGlobals + 4, 0x1234 + c);
      guest::StoreU32(h.base, kGlow, 0xA5A5A5A5);
      PPCContext t = CallContext(h);
      if (v == 0) {
        const bool flag = ITC_FLAG(native_app_start_up);
        ITC_FLAG(native_app_start_up) = false;
        __imp__sub_8209BE88(t, h.base);
        ITC_FLAG(native_app_start_up) = flag;
      } else {
        sub_8209BE88(t, h.base);
      }
      out[v].Word(t.r3.u32);
      out[v].Word(guest::LoadU32(h.base, kGlobals + 0));
      out[v].Word(guest::LoadU32(h.base, kGlobals + 4));
      out[v].Word(guest::LoadU32(h.base, kGlow));
      for (const uint32_t at : {kGlobals + 8, kGlobals + 12}) {
        const uint32_t object = guest::LoadU32(h.base, at);
        out[v].Word(object != 0);
        out[v].Word(object != 0 ? guest::LoadU32(h.base, object) : 0);
      }
      const uint32_t declaration = guest::LoadU32(h.base, kGlobals + 16);
      out[v].Word(declaration != 0);
      if (declaration != 0) {
        out[v].Bytes(Read(h.base, declaration, 56 + 12 * guest::LoadU32(h.base, declaration + 24)));
        memory::crt::XMemFree(declaration, 0x24800000);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}: ring {}", c, ring));
  }
  for (uint32_t i = 0; i < kWords; i++) {
    guest::StoreU32(h.base, kGlobals + 4 * i, saved[i]);
  }
  guest::StoreU32(h.base, kGlow, saved_glow);
  results.Report();
}

// --- The text: the font's set-up ---

constexpr uint32_t kFontPointer = 0x826C5C00;
constexpr uint32_t kStringsPointer = 0x826C5C34;
constexpr uint32_t kStringsBytes = 4304;

// A font's outputs: its bytes with the declaration and the five shader
// objects' words (+16560 to +16583) left out, the declaration's bytes
// (freed), each shader object's being made and its first word (the objects
// hold their microcode's addresses, which differ by allocation; left made,
// as Billboards::Init's).
void FontOut(Context& h, Outputs& out, uint32_t font) {
  std::vector<uint8_t> bytes = Read(h.base, font, asu::kFontBytes);
  std::fill(bytes.begin() + 16560, bytes.begin() + 16584, 0);
  out.Bytes(bytes);
  const uint32_t declaration = guest::LoadU32(h.base, font + 16560);
  out.Word(declaration != 0);
  if (declaration != 0) {
    out.Bytes(Read(h.base, declaration, 56 + 12 * guest::LoadU32(h.base, declaration + 24)));
    memory::crt::XMemFree(declaration, 0x24800000);
  }
  for (uint32_t at = 16564; at < 16584; at += 4) {
    const uint32_t object = guest::LoadU32(h.base, font + at);
    out.Word(object != 0);
    out.Word(object != 0 ? guest::LoadU32(h.base, object) : 0);
  }
}

// Font::Font on a block of 16,592 bytes (0xCD or random), the recompiled
// one with --native_app_start_up off (its CreateVertexDeclaration the
// original too); also its frame's words +80 to +91 (the five color bytes
// it keeps there and the device lock's word).
void FontConstructCases(Context& h) {
  Results results("Font::Font (sub_82109228) with Font::CreateShaders (sub_821088B8)");
  const uint32_t font = memory::Alloc(asu::kFontBytes, 0);
  for (int c = 0; c < 4; c++) {
    std::vector<uint8_t> fill(asu::kFontBytes, 0xCD);
    if (c % 2 == 1) {
      for (auto& b : fill) {
        b = uint8_t(h.random.Next());
      }
    }
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Write(h.base, font, fill.data(), asu::kFontBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = font;
      Fill(h.base, t.r1.u32 - 512, 512);
      if (v == 0) {
        const bool flag = ITC_FLAG(native_app_start_up);
        ITC_FLAG(native_app_start_up) = false;
        __imp__sub_82109228(t, h.base);
        ITC_FLAG(native_app_start_up) = flag;
      } else {
        hooks::FontConstructSteps(t, h.base);
      }
      out[v].Word(t.r3.u32);
      out[v].Bytes(Read(h.base, t.r1.u32 - 128 + 80, 12));
      FontOut(h, out[v], font);
    }
    results.Check(out[0], out[1], fmt::format("case {}: {}", c, c % 2 ? "random" : "0xCD"));
  }
  memory::Free(font);
  results.Report();
}

// Text::Init on the game's globals (the font's and the string table's
// pointers saved and put back): the string table there (left) or 0 (made
// and compared, then freed); the font made each time compared and freed.
void TextInitCases(Context& h) {
  Results results("Text::Init (sub_8210ADD0) with Font::Init (sub_82109A00)");
  const uint32_t saved_font = guest::LoadU32(h.base, kFontPointer);
  const uint32_t saved_strings = guest::LoadU32(h.base, kStringsPointer);
  for (int c = 0; c < 4; c++) {
    const bool strings = c % 2 == 0;
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      guest::StoreU32(h.base, kFontPointer, 0x5A5A5A5A);
      guest::StoreU32(h.base, kStringsPointer, strings ? saved_strings : 0);
      PPCContext t = CallContext(h);
      t.r3.u64 = h.random.Next();
      if (v == 0) {
        const bool flag = ITC_FLAG(native_app_start_up);
        ITC_FLAG(native_app_start_up) = false;
        __imp__sub_8210ADD0(t, h.base);
        ITC_FLAG(native_app_start_up) = flag;
      } else {
        hooks::TextInitSteps(t, h.base);
      }
      const uint32_t font = guest::LoadU32(h.base, kFontPointer);
      const uint32_t table = guest::LoadU32(h.base, kStringsPointer);
      out[v].Word(font != 0 && memory::IsPages(font));
      out[v].Word(font & 0xFFF);
      if (strings) {
        out[v].Word(table == saved_strings);
        out[v].Word(t.r3.u32);
      } else {
        out[v].Word(table != 0 && memory::IsPages(table));
        out[v].Word(t.r3.u32 == table);
        if (table != 0) {
          out[v].Bytes(Read(h.base, table, kStringsBytes));
          memory::FreePages(table);
        }
      }
      if (font != 0 && memory::IsPages(font)) {
        FontOut(h, out[v], font);
        memory::FreePages(font);
      }
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: string table {}", c, strings ? "there" : "made"));
  }
  guest::StoreU32(h.base, kFontPointer, saved_font);
  guest::StoreU32(h.base, kStringsPointer, saved_strings);
  results.Report();
}

// --- The app's start-up ---

// App::StartUp's callees seen through hooks::g_start_up_recorder: each
// call's address, r3, r4, link register, stack pointer and r31 logged; r3
// answered with a number by position.
std::vector<uint32_t> g_start_up_log;
bool RecordStartUpCall(PPCContext& ctx, uint32_t address) {
  g_start_up_log.insert(g_start_up_log.end(), {address, ctx.r3.u32, ctx.r4.u32, uint32_t(ctx.lr),
                                               ctx.r1.u32, ctx.r31.u32});
  ctx.r3.u64 = 0x7A000000u + uint32_t(g_start_up_log.size());
  return true;
}

// App::StartUp on a fake interface in the scratch area (its vtable at +256,
// a copy of the app interface's with slot +16 the getter of +112,
// sub_82379350, slot +48 that of +108, sub_82379338, and slot +44 either
// the app's, sub_82379350, or the other getter, for Movies::Set's original
// path), the callees recorded (above); the recompiled one with
// --native_app_start_up off. Compared: the recorded calls, the six globals
// its leaves write (saved and put back), r3 (against the stack pointer) and
// r31.
void AppStartUpCases(Context& h) {
  Results results("App::StartUp (sub_82106548)");
  const std::initializer_list<uint32_t> globals = {asu::kReadErrorApp, asu::kAppInterface84,
                                                   asu::kRenderApp,    asu::kMovieClass,
                                                   asu::kWords8110,    asu::kWords8114};
  std::vector<uint32_t> saved;
  for (const uint32_t g : globals) {
    saved.push_back(guest::LoadU32(h.base, g));
  }
  const uint32_t interface = h.data;
  const uint32_t fake = h.data + 256;
  for (int c = 0; c < 8; c++) {
    const bool real = c < 6;
    const uint32_t word108 = uint32_t(h.random.Next());
    const uint32_t word112 = uint32_t(h.random.Next());
    const uint64_t r4 = h.random.Next(), r31 = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      for (uint32_t k = 0; k < 64; k += 4) {
        guest::StoreU32(h.base, fake + k, guest::LoadU32(h.base, kAppInterfaceVtable + k));
      }
      guest::StoreU32(h.base, fake + 16, 0x82379350);
      guest::StoreU32(h.base, fake + 44, real ? 0x82379350 : 0x82379338);
      guest::StoreU32(h.base, fake + 48, 0x82379338);
      guest::StoreU32(h.base, interface, fake);
      guest::StoreU32(h.base, interface + 108, word108);
      guest::StoreU32(h.base, interface + 112, word112);
      for (const uint32_t g : globals) {
        guest::StoreU32(h.base, g, 0xA5A5A5A5u + c);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = interface;
      t.r4.u64 = r4;
      t.r31.u64 = r31;
      const uint32_t stack = t.r1.u32;
      g_start_up_log.clear();
      hooks::g_start_up_recorder = RecordStartUpCall;
      if (v == 0) {
        const bool flag = ITC_FLAG(native_app_start_up);
        ITC_FLAG(native_app_start_up) = false;
        __imp__sub_82106548(t, h.base);
        ITC_FLAG(native_app_start_up) = flag;
      } else {
        hooks::AppStartUpSteps(t, h.base);
      }
      hooks::g_start_up_recorder = nullptr;
      if (c == 0 && v == 1) {
        ITC_LOG_INFO("harness: app_start_up: App::StartUp's cases record {} calls a run",
                     g_start_up_log.size() / 6);
      }
      for (const uint32_t word : g_start_up_log) {
        out[v].Word(word);
      }
      for (const uint32_t g : globals) {
        out[v].Word(guest::LoadU32(h.base, g));
      }
      out[v].Word(t.r3.u32 - stack);
      out[v].Word(t.r1.u32 - stack);
      out[v].Bytes(Read(h.base, stack - 16, 16));
      out[v].Word(uint32_t(t.r31.u64 >> 32));
      out[v].Word(t.r31.u32);
    }
    results.Check(out[0], out[1],
                  fmt::format("case {}: {}", c, real ? "the app's slot +44" : "another slot +44"));
  }
  size_t i = 0;
  for (const uint32_t g : globals) {
    guest::StoreU32(h.base, g, saved[i++]);
  }
  results.Report();
}

// --- The movie class's construction ---

// Frees a heap block a case made (the wrapper's, the wrapper).
void FreeBlock(uint32_t block) {
  if (block != 0 && memory::IsHeapBlock(block)) {
    memory::crt::OperatorDelete(block);
  }
}

// Runs `f` with --native_app_start_up off (v == 0) or on.
template <typename F>
void StartUpSide(int v, F f) {
  const bool flag = ITC_FLAG(native_app_start_up);
  ITC_FLAG(native_app_start_up) = v == 1;
  f();
  ITC_FLAG(native_app_start_up) = flag;
}

// The wrapper's lock: ScopedLock on a critical section the recompiled
// InitializeCriticalSection set up in a heap block (v == 0, the originals)
// against the host mutex's plain C++ on the same lock word; compared: the
// scratch area (the lock word) and r3 after each. Then whether the live
// wrapper's section (the movie class at 0x826C1CDC, +8, +4) is the one
// routed to the mutex, when the native constructor made it.
void NativeWrapperLockEnter(PPCContext& t, uint8_t* base) {
  asu::MovieWrapperLockEnter(base, t.r3.u32, t.r4.u32);
}
void NativeWrapperLockLeave(PPCContext& t, uint8_t* base) {
  t.r3.u64 = asu::MovieWrapperLockLeave(base, t.r3.u32);
}

void MovieWrapperLockCases(Context& h) {
  {
    Results results("ScopedLock (sub_8258E580, sub_8258E5B8, the movie wrapper's critical section)");
    const uint32_t section = *memory::crt::OperatorNew(h.base, asu::kMovieWrapperBlockBytes);
    for (int c = 0; c < 16; c++) {
      const uint32_t lock = h.data + 4 * h.random.Below(64);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = CallContext(h);
        if (v == 0) {
          t.r3.u64 = section;
          __imp__sub_8258E548(t, h.base);  // InitializeCriticalSection
        }
        t.r3.u64 = lock;
        t.r4.s64 = int32_t(section);
        (v == 0 ? __imp__sub_8258E580 : NativeWrapperLockEnter)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
        out[v].Word(uint32_t(t.r3.u64 >> 32));
        t.r3.u64 = lock;
        (v == 0 ? __imp__sub_8258E5B8 : NativeWrapperLockLeave)(t, h.base);
        out[v].Bytes(Read(h.base, h.data, kScratchBytes));
        out[v].Word(t.r3.u32);
      }
      results.Check(out[0], out[1], fmt::format("case {} (lock word at +{})", c, lock - h.data));
    }
    FreeBlock(section);
    const uint32_t movie = guest::LoadU32(h.base, asu::kMovieClass);
    const uint32_t wrapper = movie != 0 ? guest::LoadU32(h.base, movie + 8) : 0;
    const uint32_t live = wrapper != 0 ? guest::LoadU32(h.base, wrapper + 4) : 0;
    if (live != 0 && asu::MovieWrapperLock() != 0) {
      Outputs out[2];
      out[0].Word(1);
      out[1].Word(asu::IsMovieWrapperLock(live) ? 1 : 0);
      results.Check(out[0], out[1], "the live wrapper's section routed");
    }
    results.Report();
  }
}

// MovieWrapper::MovieWrapper on 8 bytes in the scratch area (the recompiled
// one with --native_app_start_up off, so its InitializeCriticalSection
// runs): compared the vtable, whether +4 is set, its block's +28 (the
// player), r3 and the prologue's saves above the stack pointer. The
// section's 28 bytes aren't compared (the host mutex in its place: left as
// operator new leaves them, as Edram::Alloc's). The live lock's address put
// back; the blocks freed.
void MovieWrapperCases(Context& h) {
  Results results("MovieWrapper::MovieWrapper (sub_82593DC0)");
  const uint32_t saved = asu::MovieWrapperLock();
  const uint32_t wrapper = h.data + 64;
  for (int c = 0; c < 8; c++) {
    const uint64_t r30 = h.random.Next(), r31 = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      PPCContext t = CallContext(h);
      t.r3.u64 = wrapper;
      t.r30.u64 = r30;
      t.r31.u64 = r31;
      t.lr = 0x820AEB1C;
      const uint32_t stack = t.r1.u32;
      StartUpSide(v, [&] {
        (v == 0 ? __imp__sub_82593DC0 : hooks::MovieWrapperSteps)(t, h.base);
      });
      const uint32_t block = guest::LoadU32(h.base, wrapper + 4);
      out[v].Word(guest::LoadU32(h.base, wrapper));
      out[v].Word(block != 0 ? 1 : 0);
      out[v].Word(block != 0 ? guest::LoadU32(h.base, block + 28) : 0xFFFFFFFF);
      out[v].Word(t.r3.u32);
      out[v].Word(t.r1.u32 - stack);
      out[v].Bytes(Read(h.base, stack - 24, 24));
      if (v == 1) {
        out[v].Word(asu::MovieWrapperLock() == block ? 1 : 0);
      } else {
        out[v].Word(1);
      }
      FreeBlock(block);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  asu::SetMovieWrapperLock(saved);
  results.Report();
}

// Movie::Movie on 312 bytes in the scratch area, the set-up block's run loop
// a fake object whose vtable's slot 2 (AddTickHandler) adds one to its +4
// (sub_8245F860) and slot 1 (AddListener) stores r4 at its +4
// (sub_820A70A8); the recompiled one with --native_app_start_up off (its
// wrapper the original's too). Compared: the movie (its +8, the wrapper, as
// whether it's set), the wrapper's vtable and its block's +28, the run
// loop's words, r3, the frame's +80 to +119 (the keys and delegates) and
// the prologue's saves. The blocks freed; the live lock's address put back.
void MovieConstructCases(Context& h) {
  constexpr uint32_t kCount4 = 0x8245F860;
  constexpr uint32_t kStoreR4At4 = 0x820A70A8;
  Results results("Movie::Movie (sub_820AEA88) with MovieWrapper::MovieWrapper (sub_82593DC0)");
  const uint32_t saved = asu::MovieWrapperLock();
  const uint32_t movie = h.data + 512;
  const uint32_t block = h.data + 128;
  const uint32_t loop = h.data + 192;
  const uint32_t vtable = h.data + 256;
  for (int c = 0; c < 8; c++) {
    const uint32_t word4 = uint32_t(h.random.Next());
    const uint64_t r28 = h.random.Next(), r29 = h.random.Next(), r30 = h.random.Next(),
                   r31 = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      guest::StoreU32(h.base, block + 0, loop);
      guest::StoreU32(h.base, block + 4, word4);
      guest::StoreU32(h.base, loop, vtable);
      guest::StoreU32(h.base, loop + 4, 0x100 * c);
      guest::StoreU32(h.base, vtable + 4, kStoreR4At4);
      guest::StoreU32(h.base, vtable + 8, kCount4);
      PPCContext t = CallContext(h);
      for (uint32_t k = 4; k <= 0x200; k += 4) {
        guest::StoreU32(h.base, t.r1.u32 - k, 0xC0DE0000u + k);
      }
      t.r3.u64 = movie;
      t.r4.u64 = block;
      t.r28.u64 = r28;
      t.r29.u64 = r29;
      t.r30.u64 = r30;
      t.r31.u64 = r31;
      t.lr = 0x820947A0;
      const uint32_t stack = t.r1.u32;
      StartUpSide(v, [&] {
        (v == 0 ? __imp__sub_820AEA88 : hooks::MovieConstructSteps)(t, h.base);
      });
      const uint32_t wrapper = guest::LoadU32(h.base, movie + 8);
      const uint32_t wrapper_block = wrapper != 0 ? guest::LoadU32(h.base, wrapper + 4) : 0;
      out[v].Bytes(Read(h.base, movie, 8));
      out[v].Word(wrapper != 0 ? 1 : 0);
      out[v].Bytes(Read(h.base, movie + 12, 312 - 12));
      out[v].Word(wrapper != 0 ? guest::LoadU32(h.base, wrapper) : 0);
      out[v].Word(wrapper_block != 0 ? 1 : 0);
      out[v].Word(wrapper_block != 0 ? guest::LoadU32(h.base, wrapper_block + 28) : 0);
      out[v].Bytes(Read(h.base, loop, 8));
      out[v].Word(t.r3.u32);
      out[v].Word(t.r1.u32 - stack);
      out[v].Bytes(Read(h.base, stack - 160 + 80, 40));
      out[v].Bytes(Read(h.base, stack - 40, 40));
      FreeBlock(wrapper_block);
      FreeBlock(wrapper);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  asu::SetMovieWrapperLock(saved);
  results.Report();
}

// --- The sound manager's construction ---

constexpr uint32_t kEngineAddRef = 0x8245F860;  // the engine's slot +4: +4 up by one
constexpr uint32_t kEnginePointer = 0x82954F08;

// The live XAudio engine (StartXAudio's XAudioInitialize takes its AddRef
// path): its count (+4) read before a version and put back after, the
// change kept. 0 when there's none, or its AddRef isn't sub_8245F860.
struct LiveEngine {
  uint32_t engine = 0;
  uint32_t count = 0;
  explicit LiveEngine(uint8_t* base) {
    const uint32_t live = guest::LoadU32(base, kEnginePointer);
    if (live != 0 && guest::LoadU32(base, guest::LoadU32(base, live) + 4) == kEngineAddRef) {
      engine = live;
    }
  }
  void Before(uint8_t* base) { count = guest::LoadU32(base, engine + 4); }
  uint32_t After(uint8_t* base) const {
    const uint32_t change = guest::LoadU32(base, engine + 4) - count;
    guest::StoreU32(base, engine + 4, count);
    return change;
  }
};

// StartXAudio alone: the engine (r3, not read) a scratch word; compared:
// its frame's +80 to +115 (the parameters), the prologue's saves, r3 in
// full and the engine's count change.
void StartXAudioCases(Context& h, LiveEngine& live) {
  Results results("AudioEngine::StartXAudio (sub_8259CD50)");
  for (int c = 0; c < 8; c++) {
    const uint64_t r30 = h.random.Next(), r31 = h.random.Next();
    const uint32_t fill = uint32_t(h.random.Next());
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      PPCContext t = CallContext(h);
      for (uint32_t k = 4; k <= 0x400; k += 4) {
        guest::StoreU32(h.base, t.r1.u32 - k, fill + k);
      }
      t.r3.u64 = h.data;
      t.r30.u64 = r30;
      t.r31.u64 = r31;
      t.lr = 0x821198F0;
      const uint32_t stack = t.r1.u32;
      live.Before(h.base);
      (v == 0 ? __imp__sub_8259CD50 : hooks::StartXAudioSteps)(t, h.base);
      out[v].Word(live.After(h.base));
      out[v].Bytes(Read(h.base, stack - 144 + 80, 36));
      out[v].Bytes(Read(h.base, stack - 24, 24));
      out[v].Word(t.r3.u32);
      out[v].Word(uint32_t(t.r3.u64 >> 32));
      out[v].Word(t.r1.u32 - stack);
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

// A manager's words with pointers into it as offsets (0xEE000000 + the
// offset) and the blocks it holds numbered (0xEF000000 + their index),
// then the blocks' words the same way: the bank vector's (+1516 to +1524),
// the lists' head nodes (+1532, +1544) and the name's buffer when it's on
// the heap (its capacity, +1580, above 15). The blocks freed after.
void ManagerOut(uint8_t* base, Outputs& out, uint32_t manager) {
  const uint32_t first = guest::LoadU32(base, manager + 1516);
  const uint32_t end = guest::LoadU32(base, manager + 1524);
  std::vector<std::pair<uint32_t, uint32_t>> blocks;  // address, bytes
  if (first != 0) {
    blocks.emplace_back(first, end - first < 0x1000 ? end - first : 0);
  }
  for (const uint32_t list : {manager + 1528, manager + 1540}) {
    const uint32_t node = guest::LoadU32(base, list + 4);
    if (node != 0) {
      blocks.emplace_back(node, 12);
    }
  }
  if (guest::LoadU32(base, manager + 1580) > 15) {
    const uint32_t text = guest::LoadU32(base, manager + 1560);
    blocks.emplace_back(text, 0);
  }
  const auto word = [&](uint32_t value) {
    if (value >= manager && value < manager + asu::kAudioManagerBytes) {
      return 0xEE000000u + (value - manager);
    }
    for (uint32_t i = 0; i < blocks.size(); i++) {
      if (value != 0 && value >= blocks[i].first &&
          value <= blocks[i].first + blocks[i].second) {
        return 0xEF000000u + (i << 16) + (value - blocks[i].first);
      }
    }
    return value;
  };
  for (uint32_t at = 0; at < asu::kAudioManagerBytes; at += 4) {
    out.Word(word(guest::LoadU32(base, manager + at)));
  }
  out.Word(uint32_t(blocks.size()));
  for (const auto& [address, bytes] : blocks) {
    for (uint32_t at = 0; at < bytes; at += 4) {
      out.Word(word(guest::LoadU32(base, address + at)));
    }
  }
  for (const auto& [address, bytes] : blocks) {
    FreeBlock(address);
  }
  FreeBlock(manager);
}

// AudioManager::Create on a fake owner in the scratch area (+0; its vtable
// at +64, slots 1 and 2 the getter of +60, sub_82090770, answering the
// object at +128, whose vtable at +192 has slot 1 the counter of its +4,
// sub_8245F860, and slots 2 to 4 the recorder of r4 at its +4,
// sub_820A70A8), the name "txr_xact" at +256 and a bank count (0 to 5) at
// +300; the recompiled one with --native_app_start_up off. The stack below
// filled the same for both (the word the constructor copies from its
// frame's +180 among them). Compared: the manager (ManagerOut), the fake
// object, r3 (as whether it's the manager), the frames' words (the
// constructor's +80 to +159 and +180, StartXAudio's +80 to +115), the
// prologues' saves and the live engine's count change.
void AudioManagerCases(Context& h, LiveEngine& live) {
  constexpr uint32_t kGetter60 = 0x82090770;
  constexpr uint32_t kCount4 = 0x8245F860;
  constexpr uint32_t kStoreR4At4 = 0x820A70A8;
  Results results("AudioManager::Create (sub_82119B38) with AudioManager::AudioManager "
                  "(sub_821197E0) and AudioEngine::StartXAudio (sub_8259CD50)");
  const uint32_t owner = h.data, owner_vtable = h.data + 64, object = h.data + 128,
                 object_vtable = h.data + 192, name = h.data + 256, count = h.data + 300;
  for (int c = 0; c < 12; c++) {
    const uint32_t banks = c < 4 ? 3 : h.random.Below(6);
    const uint32_t fill = uint32_t(h.random.Next());
    const uint32_t word4 = uint32_t(h.random.Next());
    const uint64_t r26 = h.random.Next(), r27 = h.random.Next(), r28 = h.random.Next(),
                   r29 = h.random.Next(), r30 = h.random.Next(), r31 = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      guest::StoreU32(h.base, owner, owner_vtable);
      guest::StoreU32(h.base, owner + 60, object);
      guest::StoreU32(h.base, owner_vtable + 4, kGetter60);
      guest::StoreU32(h.base, owner_vtable + 8, kGetter60);
      guest::StoreU32(h.base, object, object_vtable);
      guest::StoreU32(h.base, object + 4, word4);
      guest::StoreU32(h.base, object_vtable + 4, kCount4);
      for (uint32_t k = 8; k <= 16; k += 4) {
        guest::StoreU32(h.base, object_vtable + k, kStoreR4At4);
      }
      Write(h.base, name, "txr_xact", 9);
      guest::StoreU32(h.base, count, banks);
      PPCContext t = CallContext(h);
      for (uint32_t k = 4; k <= 0x800; k += 4) {
        guest::StoreU32(h.base, t.r1.u32 - k, fill + k);
      }
      t.r3.u64 = owner;
      t.r4.u64 = name;
      t.r5.u64 = count;
      t.r26.u64 = r26;
      t.r27.u64 = r27;
      t.r28.u64 = r28;
      t.r29.u64 = r29;
      t.r30.u64 = r30;
      t.r31.u64 = r31;
      t.lr = 0x820947C4;
      const uint32_t stack = t.r1.u32;
      const uint32_t frame = stack - 112 - 256;
      live.Before(h.base);
      StartUpSide(v, [&] {
        (v == 0 ? __imp__sub_82119B38 : hooks::AudioManagerCreateSteps)(t, h.base);
      });
      out[v].Word(live.After(h.base));
      const uint32_t manager = t.r3.u32;
      out[v].Word(manager != 0 ? 1 : 0);
      out[v].Bytes(Read(h.base, object, 8));
      out[v].Word(t.r1.u32 - stack);
      out[v].Bytes(Read(h.base, frame + 80, 80));
      out[v].Word(guest::LoadU32(h.base, frame + 180));
      out[v].Bytes(Read(h.base, frame - 144 + 80, 36));
      out[v].Bytes(Read(h.base, stack - 32, 32));
      out[v].Bytes(Read(h.base, stack - 112 - 56, 56));
      for (uint32_t at = frame - 24; at < frame; at += 4) {
        const uint32_t w = guest::LoadU32(h.base, at);
        out[v].Word(w >= manager && w < manager + asu::kAudioManagerBytes ? w - manager : w);
      }
      out[v].Bytes(Read(h.base, frame, 4));
      if (manager != 0) {
        ManagerOut(h.base, out[v], manager);
      }
    }
    results.Check(out[0], out[1], fmt::format("case {}: {} banks", c, banks));
  }
  results.Report();
}

void AudioManagerGroup(Context& h) {
  LiveEngine live(h.base);
  if (live.engine == 0) {
    ITC_LOG_WARN("harness: app_start_up: no live XAudio engine with sub_8245F860 as its AddRef: "
                 "the sound manager's cases skipped");
    return;
  }
  StartXAudioCases(h, live);
  AudioManagerCases(h, live);
}

// --- The app's second camera class ---

// Numbers the heap blocks a case's objects hold, for comparing them by
// structure: a word pointing into a known block becomes 0xEF000000 + its
// index << 16 + the offset.
struct Blocks {
  std::vector<std::pair<uint32_t, uint32_t>> list;  // address, bytes
  void Add(uint32_t address, uint32_t bytes) {
    if (address != 0) {
      list.emplace_back(address, bytes);
    }
  }
  uint32_t Of(uint32_t value) const {
    for (uint32_t i = 0; i < list.size(); i++) {
      if (value != 0 && value >= list[i].first && value < list[i].first + list[i].second) {
        return 0xEF000000u + (i << 16) + (value - list[i].first);
      }
    }
    return value;
  }
  void Out(uint8_t* base, Outputs& out) const {
    out.Word(uint32_t(list.size()));
    for (const auto& [address, bytes] : list) {
      for (uint32_t at = 0; at < bytes; at += 4) {
        out.Word(Of(guest::LoadU32(base, address + at)));
      }
    }
  }
  void Free() const {
    for (const auto& [address, bytes] : list) {
      FreeBlock(address);
    }
  }
};

void NativeAppCameraEntry(PPCContext& t, uint8_t* base) {
  t.r3.u64 = asu::AppCameraEntryConstruct(base, t.r3.u32);
}

// The second camera class's constructors: the entry (16 bytes) and the list
// (8 bytes, its vector's block numbered) alone, the camera (832 bytes, over
// Camera::Camera) alone, then the class (368 bytes) on a fake block in the
// scratch area: +0 an interface whose slots 4 (sub_820A70A8, r4 to its +4),
// 3 and 1 (sub_8245F860, its +4 up by one) take the handlers; +4 one whose
// slot 1 answers (sub_82090770, its +60) an object whose slot 3 answers
// the same way a screen with random words at +296 and +300. The recompiled
// ones with --native_app_start_up off. Compared: the objects and the blocks
// they hold numbered, the fakes' words, r3, the frames' words (the class's
// +28 and +80 to +159) and the prologues' saves. The blocks freed (their
// unwritten bytes match as each version gets the same blocks back from the
// allocation layer).
void AppCameraCases(Context& h) {
  constexpr uint32_t kGetter60 = 0x82090770;
  constexpr uint32_t kCount4 = 0x8245F860;
  constexpr uint32_t kStoreR4At4 = 0x820A70A8;
  {
    Results results("AppCameraEntry::AppCameraEntry (sub_820A75E8)");
    for (int c = 0; c < 4; c++) {
      Both(
          h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = h.data + 16 * c; },
          __imp__sub_820A75E8, NativeAppCameraEntry, h.data, 128, fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("AppCameraList::AppCameraList (sub_820A98C0) with sub_820A9198 and "
                     "sub_820A8F80");
    for (int c = 0; c < 4; c++) {
      const uint64_t r31 = h.random.Next();
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, h.data, kScratchBytes);
        PPCContext t = CallContext(h);
        t.r3.u64 = h.data + 8 * c;
        t.r31.u64 = r31;
        t.lr = 0x820A8C64;
        const uint32_t stack = t.r1.u32;
        (v == 0 ? __imp__sub_820A98C0 : hooks::AppCameraListConstructSteps)(t, h.base);
        Blocks blocks;
        const uint32_t vector = guest::LoadU32(h.base, h.data + 8 * c + 4);
        blocks.Add(vector, 16);
        out[v].Word(t.r3.u32);
        out[v].Word(guest::LoadU32(h.base, h.data + 8 * c));
        out[v].Word(blocks.Of(vector));
        if (vector != 0) {
          // The block's +0 isn't written (operator new's leftover).
          out[v].Bytes(Read(h.base, vector + 4, 12));
        }
        out[v].Bytes(Read(h.base, stack - 16, 16));
        blocks.Free();
      }
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  {
    Results results("AppCamera::Camera::Camera (sub_820A9E28)");
    for (int c = 0; c < 4; c++) {
      const uint64_t r31 = h.random.Next();
      const uint32_t fill = uint32_t(h.random.Next());
      const uint32_t camera = *memory::crt::OperatorNew(h.base, asu::kAppCameraCameraBytes);
      Outputs out[2];
      for (int v = 0; v < 2; v++) {
        Fill(h.base, camera, asu::kAppCameraCameraBytes, uint8_t(fill));
        PPCContext t = CallContext(h);
        for (uint32_t k = 4; k <= 0x400; k += 4) {
          guest::StoreU32(h.base, t.r1.u32 - k, fill + k);
        }
        t.r3.u64 = camera;
        t.r31.u64 = r31;
        t.lr = 0x820A8C40;
        const uint32_t stack = t.r1.u32;
        (v == 0 ? __imp__sub_820A9E28 : hooks::AppCameraCameraConstructSteps)(t, h.base);
        out[v].Word(t.r3.u32);
        out[v].Bytes(Read(h.base, camera, asu::kAppCameraCameraBytes));
        out[v].Bytes(Read(h.base, stack - 144 + 80, 48));
        out[v].Bytes(Read(h.base, stack - 16, 16));
      }
      FreeBlock(camera);
      results.Check(out[0], out[1], fmt::format("case {}", c));
    }
    results.Report();
  }
  Results results("AppCamera::AppCamera (sub_820A8B88) with AppCamera::Camera::Camera, "
                  "AppCameraList::AppCameraList and AppCameraEntry::AppCameraEntry");
  const uint32_t object = h.data + 1024;
  const uint32_t block = h.data, i0 = h.data + 16, i0_vtable = h.data + 32, i4 = h.data + 64,
                 i4_vtable = h.data + 128, service = h.data + 192, service_vtable = h.data + 256,
                 screen = h.data + 320;
  for (int c = 0; c < 8; c++) {
    const uint32_t width = uint32_t(h.random.Next()), height = uint32_t(h.random.Next());
    const uint32_t word16 = uint32_t(h.random.Next()), fill = uint32_t(h.random.Next());
    const uint64_t r27 = h.random.Next(), r28 = h.random.Next(), r29 = h.random.Next(),
                   r30 = h.random.Next(), r31 = h.random.Next();
    Outputs out[2];
    for (int v = 0; v < 2; v++) {
      Fill(h.base, h.data, kScratchBytes);
      guest::StoreU32(h.base, block + 0, i0);
      guest::StoreU32(h.base, block + 4, i4);
      guest::StoreU32(h.base, i0, i0_vtable);
      guest::StoreU32(h.base, i0 + 4, word16);
      guest::StoreU32(h.base, i0_vtable + 4, kCount4);
      guest::StoreU32(h.base, i0_vtable + 12, kCount4);
      guest::StoreU32(h.base, i0_vtable + 16, kStoreR4At4);
      guest::StoreU32(h.base, i4, i4_vtable);
      guest::StoreU32(h.base, i4 + 60, service);
      guest::StoreU32(h.base, i4_vtable + 4, kGetter60);
      guest::StoreU32(h.base, service, service_vtable);
      guest::StoreU32(h.base, service + 60, screen);
      guest::StoreU32(h.base, service_vtable + 12, kGetter60);
      guest::StoreU32(h.base, screen + 296, width);
      guest::StoreU32(h.base, screen + 300, height);
      PPCContext t = CallContext(h);
      for (uint32_t k = 4; k <= 0x600; k += 4) {
        guest::StoreU32(h.base, t.r1.u32 - k, fill + k);
      }
      t.r3.u64 = object;
      t.r4.u64 = block;
      t.r27.u64 = r27;
      t.r28.u64 = r28;
      t.r29.u64 = r29;
      t.r30.u64 = r30;
      t.r31.u64 = r31;
      t.f1.f64 = 3.0;
      t.f31.f64 = 7.0;
      t.lr = 0x82094770;
      const uint32_t stack = t.r1.u32;
      const uint32_t frame = stack - 224;
      StartUpSide(v, [&] {
        (v == 0 ? __imp__sub_820A8B88 : hooks::AppCameraSteps)(t, h.base);
      });
      Blocks blocks;
      const uint32_t camera = guest::LoadU32(h.base, object + 28);
      const uint32_t list = guest::LoadU32(h.base, object + 32);
      blocks.Add(camera, asu::kAppCameraCameraBytes);
      blocks.Add(list, 8);
      const uint32_t vector = list != 0 ? guest::LoadU32(h.base, list + 4) : 0;
      blocks.Add(vector, 16);
      uint32_t entry = 0;
      if (vector != 0) {
        const uint32_t first = guest::LoadU32(h.base, vector + 4);
        const uint32_t end = guest::LoadU32(h.base, vector + 12);
        blocks.Add(first, end - first < 0x100 ? end - first : 0);
        if (first != 0) {
          entry = guest::LoadU32(h.base, first);
        }
      }
      blocks.Add(entry, 16);
      out[v].Word(t.r3.u32 == object ? 1 : 0);
      out[v].Word(t.r1.u32 - stack);
      for (uint32_t at = 0; at < asu::kAppCameraBytes; at += 4) {
        out[v].Word(blocks.Of(guest::LoadU32(h.base, object + at)));
      }
      if (vector != 0) {
        // The vector's block's +0 isn't written (operator new's leftover).
        guest::StoreU32(h.base, vector, 0);
      }
      blocks.Out(h.base, out[v]);
      out[v].Bytes(Read(h.base, i0, 8));
      out[v].Word(blocks.Of(guest::LoadU32(h.base, frame + 28)));
      for (uint32_t at = frame + 80; at < frame + 160; at += 4) {
        const uint32_t w = guest::LoadU32(h.base, at);
        out[v].Word(w == object ? 0xEE000000u : blocks.Of(w));
      }
      out[v].Bytes(Read(h.base, stack - 56, 56));
      out[v].Double(t.f31.u64);
      blocks.Free();
    }
    results.Check(out[0], out[1], fmt::format("case {}", c));
  }
  results.Report();
}

}  // namespace

void AppStartUp(Context& h) {
  Setter(h, "ReadError::SetApp (sub_8209AC50)", __imp__sub_8209AC50, NativeReadErrorSetApp,
         {asu::kReadErrorApp});
  Setter(h, "SetAppInterface84 (sub_8209AC30)", __imp__sub_8209AC30, NativeSetAppInterface84,
         {asu::kAppInterface84});
  Setter(h, "SetRenderApp (sub_8209F580)", __imp__sub_8209F580, NativeSetRenderApp, {asu::kRenderApp});
  Setter(h, "ClearWords8110 (sub_8225FB70)", __imp__sub_8225FB70, NativeClearWords8110,
         {asu::kWords8110, asu::kWords8114});
  MoviesSet(h);
  CreateVertexDeclaration(h);
  NetworkSetUp(h);
  SceneTargetCases(h);
  BillboardsInitCases(h);
  if (hooks::TextInitNative()) {
    FontConstructCases(h);
    TextInitCases(h);
  }
  AppStartUpCases(h);
  if (hooks::NativeHeapActive()) {
    MovieWrapperLockCases(h);
    MovieWrapperCases(h);
    MovieConstructCases(h);
    AudioManagerGroup(h);
    AppCameraCases(h);
  }
}

}  // namespace harness
