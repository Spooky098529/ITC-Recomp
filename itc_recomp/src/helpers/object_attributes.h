// The Xbox kernel's counted ANSI string and object attributes, filled as the
// game's helper sub_822970A8 fills them (the string by RtlInitAnsiString's
// rule). Plain C++: guest addresses are kept as plain values, and
// src/hooks/object_attributes.cpp stores the results in guest memory, where
// the kernel calls taking them read them. See B6 in docs/rexglue-removal.md.

#pragma once

#include <cstdint>

namespace helpers {

// ANSI_STRING (8 bytes in guest memory: length +0, maximum length +2,
// buffer +4).
struct AnsiString {
  uint16_t length;
  uint16_t maximum_length;
  uint32_t buffer;
};

// OBJECT_ATTRIBUTES (12 bytes: root directory +0, object name +4,
// attributes +8).
struct ObjectAttributes {
  uint32_t root_directory;
  uint32_t object_name;
  uint32_t attributes;
};

// RtlInitAnsiString: `text` is the string (nullptr for none), `address` its
// guest address. The length is cut to 16 bits, and the maximum length is one
// more (wrapping, as the SDK's does); no string gives 0, 0, 0.
AnsiString AnsiString_Init(const char* text, uint32_t address);

// The attributes sub_822970A8 gives a named event or timer (sub_82295E78,
// sub_825BA2B0): the named objects' root (-4), `name_address` (its
// AnsiString) and OBJ_OPENIF (0x80, probably: the Windows value; an existing
// object of that name is opened).
ObjectAttributes NamedObjectAttributes(uint32_t name_address);

}  // namespace helpers
