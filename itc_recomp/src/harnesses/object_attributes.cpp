// The named objects' attributes (src/hooks/object_attributes.cpp) against
// the recompiled sub_822970A8: the 12-byte attributes, the 8-byte
// ANSI_STRING and r3, for a name, an empty one, none, a long one and one
// character.

#include "harness.h"

#include <cstring>

namespace harness {

void ObjectAttributes(Context& h) {
  uint8_t* base = h.base;
  const uint32_t data = h.data;  // attributes +0, name +16, text +32
  const std::string long_text(300, 'x');
  const char* texts[] = {"\\BaseNamedObjects\\ItcTest", "", nullptr, long_text.c_str(), "a"};
  Results results("InitNamedObjectAttributes (sub_822970A8)");
  for (const char* text : texts) {
    // The attributes and the string, as one 32-byte read (+12..+15 unused).
    Both(h, results,
         [&] {
           if (text) {
             Write(base, data + 32, text, uint32_t(std::strlen(text) + 1));
           }
         },
         [&](PPCContext& t) {
           t.r3.u64 = data;
           t.r4.u64 = data + 16;
           t.r5.u64 = text ? data + 32 : 0;
         },
         __imp__sub_822970A8, sub_822970A8, data, 24, text ? std::string(text).substr(0, 30) : "(none)");
  }
  results.Report();
}

}  // namespace harness
