// The compiler's array helpers (probably `vector constructor iterator' and
// `vector destructor iterator'), natively: each calls a function on every
// element of an array. The elements' functions known to be native are called
// as plain C++, the empty destructor not at all; any other goes back to the
// guest through `call`. See docs/functions/c-runtime.md.

#pragma once

#include <cstdint>

namespace c_runtime {

// Calls the guest function `function` with r3 = `element` (the registers'
// full 64 bits, as the translated code passes them).
using ElementCall = void (*)(void* user, uint64_t function, uint64_t element);

// sub_820911A8: calls `constructor` on each of `count` elements (the low 32
// bits, signed: 0 or less calls nothing) of `size` bytes from `array`, first
// to last.
void VectorConstruct(uint8_t* base, uint64_t array, uint64_t size, uint64_t count,
                     uint64_t constructor, ElementCall call, void* user);

// sub_820900C0: calls `destructor` on each of `count` elements of `size`
// bytes from `array`, last to first (the end is array + size x count, a
// 32-bit signed multiply).
void VectorDestruct(uint8_t* base, uint64_t array, uint64_t size, uint64_t count,
                    uint64_t destructor, ElementCall call, void* user);

// sub_82106540: an empty function (`blr`), the do-nothing destructor many
// objects share (and their vtables' empty slots). Nothing to do.
void EmptyDestructor();

}  // namespace c_runtime
