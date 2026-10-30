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
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"

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
      (v == 0 ? __imp__sub_820A4360 : sub_820A4360)(t, h.base);
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

}  // namespace

void AppStartUp(Context& h) {
  Setter(h, "ReadError::SetApp (sub_8209AC50)", __imp__sub_8209AC50, sub_8209AC50,
         {asu::kReadErrorApp});
  Setter(h, "SetAppInterface84 (sub_8209AC30)", __imp__sub_8209AC30, sub_8209AC30,
         {asu::kAppInterface84});
  Setter(h, "SetRenderApp (sub_8209F580)", __imp__sub_8209F580, sub_8209F580, {asu::kRenderApp});
  Setter(h, "ClearWords8110 (sub_8225FB70)", __imp__sub_8225FB70, sub_8225FB70,
         {asu::kWords8110, asu::kWords8114});
  MoviesSet(h);
  CreateVertexDeclaration(h);
  NetworkSetUp(h);
  SceneTargetCases(h);
}

}  // namespace harness
