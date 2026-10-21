// The network start-up's wrappers (src/hooks/network.cpp) against the
// recompiled ones, which ask the SDK: WSAStartup's answer (its WSADATA is
// never read, so not compared), the XNADDR and answer of XNetGetTitleXnAddr,
// and XNetXnAddrToMachineId's answer with its output left alone. The system
// link socket's drop isn't compared (the recompiled one makes a socket).

#include "harness.h"

namespace harness {

void Network(Context& h) {
  const uint32_t data = h.data;
  {
    Results results("WSAStartup (sub_82459B68)");
    Both(h, results, [] {},
         [&](PPCContext& t) {
           t.r3.u64 = 1;
           t.r4.u64 = 2;
           t.r5.u64 = data;
         },
         __imp__sub_82459B68, sub_82459B68, data, 0, "version 2");
    results.Report();
  }
  {
    Results results("XNetGetTitleXnAddr (sub_82459D50)");
    Both(h, results, [] {}, [&](PPCContext& t) { t.r3.u64 = data; }, __imp__sub_82459D50,
         sub_82459D50, data, 40, "the address");
    results.Report();
  }
  {
    Results results("XNetXnAddrToMachineId (sub_82459D40)");
    Both(h, results, [] {},
         [&](PPCContext& t) {
           t.r3.u64 = data;
           t.r4.u64 = data + 64;
         },
         __imp__sub_82459D40, sub_82459D40, data + 64, 8, "the address");
    results.Report();
  }
}

}  // namespace harness
