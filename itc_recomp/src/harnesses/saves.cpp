// The save slot steps whose recompiled versions only read from the SDK
// (src/hooks/saves.cpp): SaveSlot::CheckDevice and WaitDeviceSelector (the
// SDK's device data, read-only) and the enumeration's wait (its enumerator
// handle 0, so the recompiled CloseHandle fails harmlessly). Compared: the
// slot's fields they write (+36, +40, +92, +960, +964, +984) and r3. The
// other steps would make the SDK create, open or close content in its own
// folder, or finish later on its threads: an attract race and the user's
// play check them.

#include "harness.h"

namespace harness {

void Saves(Context& h) {
  uint8_t* base = h.base;
  // A slot (1,000 bytes) doesn't fit the scratch area: below it, past the
  // calls' stack (which starts 1 KB under it).
  const uint32_t slot = h.data - 16384;
  // The slot's bytes the steps write: +8 to +100 and +960 to +988.
  auto read_slot = [&] {
    std::vector<uint8_t> out = Read(base, slot + 8, 92);
    for (uint8_t b : Read(base, slot + 960, 28)) {
      out.push_back(b);
    }
    return out;
  };
  auto run = [&](const char* name, GuestFunction recompiled, GuestFunction native,
                 Results& results, uint32_t device, uint32_t status, uint32_t information,
                 const char* what) {
    std::vector<uint8_t> result[2];
    for (int v = 0; v < 2; v++) {
      Fill(base, slot, 1000);
      guest::StoreU32(base, slot + 36, device);
      guest::StoreU32(base, slot + 76, 0);  // no enumerator handle
      for (uint32_t overlapped : {slot + 8, slot + 48, slot + 100, slot + 136}) {
        Fill(base, overlapped, 28, 0);
        guest::StoreU32(base, overlapped, status);
        guest::StoreU32(base, overlapped + 4, information);
      }
      PPCContext t = CallContext(h);
      t.r3.u64 = slot;
      t.r4.u64 = slot;
      (v == 0 ? recompiled : native)(t, base);
      result[v] = read_slot();
      for (int i = 0; i < 4; i++) {
        result[v].push_back(uint8_t(t.r3.u32 >> (24 - 8 * i)));
      }
    }
    results.Check(result[0], result[1], std::string(name) + ": " + what);
  };

  Results results("save slot steps");
  run("SaveSlot::CheckDevice (sub_820A45C0)", __imp__sub_820A45C0, sub_820A45C0, results, 1, 0,
      0, "device 1");
  run("SaveSlot::CheckDevice (sub_820A45C0)", __imp__sub_820A45C0, sub_820A45C0, results, 0, 0,
      0, "no device");
  const struct {
    uint32_t device, status;
    const char* what;
  } selector_cases[] = {
      {1, 0, "chosen"},          {0, 0, "done, no device"}, {1, 997, "pending"},
      {1, 996, "incomplete"},    {1, 1223, "cancelled"},    {0, 1223, "cancelled, no device"},
  };
  for (const auto& c : selector_cases) {
    run("SaveSlot::WaitDeviceSelector (sub_820A48B8)", __imp__sub_820A48B8, sub_820A48B8,
        results, c.device, c.status, 0, c.what);
  }
  const struct {
    uint32_t status, information;
    const char* what;
  } enumerate_cases[] = {
      {0, 1, "one save"}, {0, 0, "none"},     {1627, 0, "no more files"},
      {997, 0, "pending"}, {5, 0, "an error"},
  };
  for (const auto& c : enumerate_cases) {
    run("the enumeration's wait (sub_820A4A88)", __imp__sub_820A4A88, sub_820A4A88, results, 1,
        c.status, c.information, c.what);
  }
  results.Report();
}

}  // namespace harness
