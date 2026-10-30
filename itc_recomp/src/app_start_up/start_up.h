// The app's start-up natively (docs/functions/app-start-up.md): plain C++
// over the guest layout. The adapters are in src/hooks/app_start_up.cpp.

#pragma once

#include <cstdint>
#include <functional>
#include <optional>

namespace app_start_up {

// The globals the start-up keeps the app's interface (app + 16, vtable
// 0x82000E38) in.
constexpr uint32_t kReadErrorApp = 0x82600344;    // ReadError::SetApp (+4 of 0x82600340)
constexpr uint32_t kAppInterface84 = 0x8266D084;  // sub_8209AC30
constexpr uint32_t kRenderApp = 0x826BABA0;       // sub_8209F580
// Movies::Set's: the movie class (the app + 128).
constexpr uint32_t kMovieClass = 0x826C1CDC;
// sub_8225FB70's two words.
constexpr uint32_t kWords8110 = 0x82816110;
constexpr uint32_t kWords8114 = 0x82816114;

// The interface's slot +44 (sub_82379350: its +112, the app + 128).
constexpr uint32_t kInterfaceMovie = 0x82379350;

// sub_8209AC50 ReadError::SetApp: `interface` stored at 0x82600344.
void ReadErrorSetApp(uint8_t* base, uint32_t interface);
// sub_8209AC30: `interface` stored at 0x8266D084.
void SetAppInterface84(uint8_t* base, uint32_t interface);
// sub_8209F580: `interface` stored at 0x826BABA0.
void SetRenderApp(uint8_t* base, uint32_t interface);
// sub_8225FB70: 0x82816110 and 0x82816114 = 0.
void ClearWords8110(uint8_t* base);
// sub_820A4360 Movies::Set: the interface's slot +44 answer (the movie
// class) stored at 0x826C1CDC; returns it. nullopt, with nothing done, where
// the slot isn't sub_82379350.
std::optional<uint32_t> MoviesSet(uint8_t* base, uint32_t interface);

// memcpy (sub_825A7410) through the guest, where c_runtime::Memcpy refuses.
using MemcpyCall = std::function<void(uint32_t destination, uint32_t source, uint32_t size)>;

// sub_822A8980 CreateVertexDeclaration: the 12-byte elements at `elements`
// up to the one whose stream (halfword +0) is 255; XMemAlloc(56 + 12 x
// count, 0x24800000), its first 56 bytes cleared, +0 = 0x00100005, +4 = 1,
// +20 = 0xFFFF0000, +24 the count, +28 the highest stream, +32 to +47 a
// stream map (255 for each stream used, else 0), +48 = 0, the elements
// copied to +52. Returns the declaration, or 0 when the allocation fails.
// Only with --native_heap (memory::crt::XMemAlloc). nullopt, with nothing
// done, for a stream above 15 (the original's map store, a byte on its
// stack at +80 + the stream, reaches past the map into its frame or its
// caller's: never in play).
std::optional<uint32_t> CreateVertexDeclaration(uint8_t* base, uint32_t elements,
                                                const MemcpyCall& memcpy_call);

}  // namespace app_start_up
