// The app's start-up (src/app_start_up/, docs/functions/app-start-up.md)
// against the recompiled functions, through the adapters (with
// --native_app_start_up on). The globals the setters write are the game's
// live ones: each is saved first and put back after the cases. The vertex
// declarations both versions make are compared by their bytes (not their
// addresses) and freed. Everything is compared exactly (nothing here is a
// float).

#include "harness.h"

#include "app_start_up/start_up.h"
#include "base/log.h"
#include "hooks/heap.h"
#include "memory/c_runtime_heap.h"

#include <fmt/format.h>

#include <string>

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
}

}  // namespace harness
