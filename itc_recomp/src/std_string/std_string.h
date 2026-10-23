// The C++ library's narrow string (Dinkumware's std::basic_string<char>, as
// linked into the game) as plain C++ over its guest layout
// (docs/functions/std-string.md). The game's inlined code reads the fields
// directly, so these work on the guest object, with the originals' loads and
// stores in their order; they go at the end of the port with the game's
// strings. Adapters: src/hooks/std_string.cpp.
//
// The layout (28 bytes): +0 the allocator (empty), +4 the text in place (16
// bytes) while the capacity is under 16, else a pointer to a heap block of
// capacity + 1 bytes; +20 the length; +24 the capacity (15 in place). The
// text ends with a 0 byte.
//
// Blocks come from operator new and go back with operator delete (the C
// runtime heap's plain C++, memory/c_runtime_heap.h), so the functions that
// allocate or free run natively only while --native_heap routes the game's
// heap to the allocation layer (the adapters' condition). Text is copied as
// memcpy_s copies it (c_runtime::Memcpy), outside the physical views (where
// memcpy's override records copies for the renderer's mirrors).
//
// The paths that never ran in play stay on the recompiled code: a function
// that would take one (memcpy_s's and free's errors, operator new's new
// handler and std::bad_alloc, _Xlen's length_error, _Xran's out_of_range,
// erase on a self-assign, append from the string's own text, replace)
// returns false (or nullopt) with nothing changed, and its adapter runs the
// game's own function from the start.

#pragma once

#include <cstdint>
#include <optional>

namespace std_string {

constexpr uint32_t kText = 4;
constexpr uint32_t kLength = 20;
constexpr uint32_t kCapacity = 24;
constexpr uint32_t kInPlace = 16;  // capacities under this keep the text in place

// The string's text: +4 itself while the capacity is under 16, else the
// pointer there (c_str).
uint32_t Text(uint8_t* base, uint32_t string);

// _Tidy (sub_82091328): when `built` and the capacity is 16 or more, the
// heap block's first `length` bytes copied into the string's own +4
// (memcpy_s(+4, 16, block, length), skipped for 0) and the block freed
// (operator delete); then +20 = `length`, +24 = 15 and the byte at
// +4 + `length` = 0. False, with nothing done, where the original must run
// (memcpy_s's errors: a length over 16 or a null block; a block the layer
// didn't give; a copy into the physical views or sharing memory).
bool Tidy(uint8_t* base, uint32_t string, bool built, uint32_t length);

// std::allocator<char>::allocate (sub_820912B0): operator new(`count`).
// nullopt where the original must run (a count malloc refuses: the new
// handler and std::bad_alloc; the allocator's own bad_alloc is for counts
// over 0xFFFFFFFF, never).
std::optional<uint32_t> Allocate(uint8_t* base, uint32_t count);

// _Copy (sub_82091458): a new block for `new_length` (its capacity
// `new_length` with the low four bits set, or 1.5 times the old capacity
// when that's more and doesn't overflow; capacity + 1 bytes from
// Allocate), the first `keep` bytes of the old text copied into it
// (memcpy_s), the old block freed (_Tidy(string, true, 0)), then +24 the
// capacity, +4 the block, +20 `keep` and the byte at the text + `keep` 0.
// False, with nothing done, where the original must run (a size malloc
// refuses, memcpy_s's errors, an old block the layer didn't give).
bool Copy(uint8_t* base, uint32_t string, uint32_t new_length, uint32_t keep);

// _Grow (sub_82091540): room for `new_length`: _Copy (keeping the text) when
// the capacity is less; else with `trim` and a length under 16, _Tidy back
// in place (keeping up to `new_length`); else for 0, the string emptied.
// Returns whether `new_length` isn't 0; nullopt, with nothing done, where
// the original must run (a length over 0xFFFFFFFE: _Xlen; its callees').
std::optional<bool> Grow(uint8_t* base, uint32_t string, uint32_t new_length, bool trim);

// assign(const string& source, size_t offset, size_t count)
// (sub_82091720): the source's text from `offset`, at most `count` (-1 for
// all) characters, into the string (_Grow, then memcpy_s), its length set
// and the text ended. False, with nothing done, where the original must run:
// an offset past the source's length (_Xran), the string assigned from
// itself (erase), or its callees' paths.
bool AssignFrom(uint8_t* base, uint32_t string, uint32_t source, uint32_t offset,
                uint32_t count);

// assign(const char* text, size_t count) (sub_82091808): `count` characters
// from `text` into the string. False, with nothing done, where the original
// must run: text inside the string's own (its assign from itself), or its
// callees' paths.
bool AssignText(uint8_t* base, uint32_t string, uint32_t text, uint32_t count);

// append(const char* text, size_t count) (sub_820A6F60): `count` characters
// from `text` after the string's. False, with nothing done, where the
// original must run: text inside the string's own (sub_820A6E60), a length
// that would pass 0xFFFFFFFF (_Xlen), or its callees' paths.
bool Append(uint8_t* base, uint32_t string, uint32_t text, uint32_t count);

// string() (sub_825929E8): _Tidy(string, false, 0), an empty string in place.
void Construct(uint8_t* base, uint32_t string);

// string(const char* text) (sub_825931D0): _Tidy(string, false, 0), then
// AssignText of the text's strlen characters. False where the original must
// run (AssignText's paths): only the empty start is written then, which the
// original writes again the same way before anything reads it.
bool ConstructFromText(uint8_t* base, uint32_t string, uint32_t text);

// string(const char* text, const allocator&) (sub_82593248):
// ConstructFromText.
bool ConstructFromTextWithAllocator(uint8_t* base, uint32_t string, uint32_t text);

// string(const string& source, size_t offset, size_t count) (sub_82593040):
// _Tidy(string, false, 0), then AssignFrom. False as ConstructFromText.
bool ConstructFrom(uint8_t* base, uint32_t string, uint32_t source, uint32_t offset,
                   uint32_t count);

// The library's wildcard search (sub_82592740, probably): the first match of
// `pattern` in the C string `text`, trying each start: '?' matches one
// character, '*' any run (the '*'s and '?'s after it skipped, then the next
// character looked for one ahead), the byte 2 anchors the match to the
// text's start and 3 to its end; other characters match themselves. Returns
// the match's start, its offset stored at `offset_out` and its length at
// `length_out` (each when not 0), or 0. The original's steps, quirks and all.
uint32_t WildcardSearch(uint8_t* base, uint32_t text, uint32_t pattern, uint32_t offset_out,
                        uint32_t length_out);

// The string's wildcard find (sub_82592970, probably): for a string that
// isn't empty, WildcardSearch(its text + `from`, ...), and on a match `from`
// added to the offset at `offset_out` (when not 0). Returns the match or 0.
uint32_t Find(uint8_t* base, uint32_t string, uint32_t pattern, uint32_t offset_out,
              uint32_t length_out, uint32_t from);

// The string's wildcard replace (sub_82593088, probably): `frame` is the
// original's frame (its r1 after the stwu: the offset and length at +80 and
// +84, cleared first). With no match from the start, the string's text
// (c_str); nullopt, with only the frame's two words written, when there is
// one: replace (sub_82592E18) never ran in play, so the original runs.
std::optional<uint32_t> ReplaceAll(uint8_t* base, uint32_t string, uint32_t pattern,
                                   uint32_t frame);

}  // namespace std_string
