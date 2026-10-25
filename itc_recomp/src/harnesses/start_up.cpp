// The start-up checks (src/hooks/start_up.cpp) against the recompiled ones:
// the PAL-50 check's answer and the heap check's, run after boot, when the
// heap exists (so neither makes one; the boot's own call made it).

#include "harness.h"

namespace harness {

void StartUp(Context& h) {
  {
    Results results("CheckPal50 (sub_82296AB0)");
    Both(h, results, [] {}, [](PPCContext&) {}, __imp__sub_82296AB0, sub_82296AB0, h.data, 0,
         "the console");
    results.Report();
  }
  // The heap's handle and the answer.
  const uint32_t heap = 0x8297149C;
  {
    Results results("CreateHeap (sub_82297C08), made");
    Both(h, results, [] {}, [](PPCContext&) {}, __imp__sub_82297C08, sub_82297C08, heap, 4,
         "the heap made");
    results.Report();
  }
  {
    Results results("CheckHeap (sub_82297CE0), made");
    Both(h, results, [] {}, [](PPCContext&) {}, __imp__sub_82297CE0, sub_82297CE0, heap, 4,
         "the heap made");
    results.Report();
  }
}

}  // namespace harness
