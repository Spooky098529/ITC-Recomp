// The small pieces of the game's C runtime start-up (Microsoft's, for the
// Xbox 360), natively: each runs once at boot and stores into the C
// runtime's globals in guest memory (docs/known-data.md). See
// docs/functions/c-runtime.md.

#pragma once

#include <cstdint>

namespace c_runtime {

// _cfltcvt_init (sub_825A6DB0): fills the float conversion table at
// 0x8266BA5C with its ten function pointers (+24 _cfltcvt_l).
void InitFloatConversions(uint8_t* base);

// _init_pointers' setters: each stores its argument (the encoded handler
// pointer, 0 at boot) in one global.
void SetHandler82971170(uint8_t* base, uint32_t value);  // sub_825AD140
void SetHandler82971178(uint8_t* base, uint32_t value);  // sub_825AEE18
void SetInvalidParameterHandler(uint8_t* base, uint32_t value);  // sub_825AFCB8, 0x82978320
void SetCriticalSectionInitializer(uint8_t* base, uint32_t value);  // sub_825B3560, 0x8297132C
// sub_825B3548: stores 0x825B3498 at 0x82971328.
void SetDefaultCriticalSectionInitializer(uint8_t* base);

// __initmbctable (sub_825B6958): once (the flag at 0x82978338), sets
// 0x8297136C to -3 and 0x8297135C to 0 and the multibyte info at
// [0x8266C450] to the C locale's with setSBCS; returns 0.
int32_t InitMbcTable(uint8_t* base);

// setSBCS (sub_825B68C8): clears the multibyte info's 257-byte table (+28)
// and +4 to +27, then copies the C locale's 257 + 256 bytes (0x8266C028 +28
// and +285) into +28 and +285, byte by byte.
void SetSbcs(uint8_t* base, uint32_t info);

}  // namespace c_runtime
