// The game's object attributes setup (sub_822970A8) natively: an adapter in
// front of helpers::NamedObjectAttributes (src/helpers/object_attributes.cpp).
// Its callers, the named event and timer creation (sub_82295E78, sub_825BA2B0),
// stay recompiled: threads and sync are the core (docs/rexglue-removal.md).

#include "guest.h"

#include "helpers/object_attributes.h"

#include <string>

// r3 = the attributes (12 bytes), r4 = their name (an 8-byte ANSI string),
// r5 = the name's text (0 for none); returns r3.
extern "C" REX_FUNC(sub_822970A8) {
  REX_FUNC_PROLOGUE();
  const uint32_t attributes_address = ctx.r3.u32;
  const uint32_t name_address = ctx.r4.u32;
  const uint32_t text_address = ctx.r5.u32;

  std::string text;
  if (text_address) {
    for (uint32_t p = text_address;; p++) {
      char c = char(guest::LoadU8(base, p));
      if (!c) {
        break;
      }
      text += c;
    }
  }
  const helpers::AnsiString name =
      helpers::AnsiString_Init(text_address ? text.c_str() : nullptr, text_address);
  guest::StoreU16(base, name_address + 0, name.length);
  guest::StoreU16(base, name_address + 2, name.maximum_length);
  guest::StoreU32(base, name_address + 4, name.buffer);

  const helpers::ObjectAttributes attributes = helpers::NamedObjectAttributes(name_address);
  guest::StoreU32(base, attributes_address + 0, attributes.root_directory);
  guest::StoreU32(base, attributes_address + 4, attributes.object_name);
  guest::StoreU32(base, attributes_address + 8, attributes.attributes);

  ctx.r3.u64 = attributes_address;
}
