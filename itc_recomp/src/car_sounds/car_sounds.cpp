// The car sounds natively (car_sounds.h, docs/functions/car-sounds.md).

#include "car_sounds/car_sounds.h"

#include "base/log.h"
#include "c_runtime/strings.h"
#include "guest.h"

#include <climits>
#include <cmath>

namespace car_sounds {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}
double LoadF(uint8_t* base, uint32_t address) { return double(guest::LoadFloat(base, address)); }
void StoreF(uint8_t* base, uint32_t address, double value) {
  guest::StoreFloat(base, address, float(value));
}

// The view's fields.
constexpr uint32_t kViewBankData = 12;  // the bank + its +8: +0 flags, +4 count, +84 the format
constexpr uint32_t kViewEntries = 16;   // 24 bytes a wave
constexpr uint32_t kViewWaveData = 20;
constexpr uint32_t kEntryBytes = 24;

bool PastCount(uint8_t* base, uint32_t view, uint32_t wave) {
  return wave >= Load(base, Load(base, view + kViewBankData) + 4);
}

// A voice holder's set-up as CarSound::CarSound and EngineSound's loops do
// it: the vtable, +20, then the voice words +4 to +16 and +24 to +36.
void ConstructHolder(uint8_t* base, uint32_t holder) {
  Store(base, holder + 0, kHolderVtable);
  Store(base, holder + 20, 0);
  for (uint32_t i = 0; i < 4; i++) {
    Store(base, holder + 4 + 4 * i, 0);
    Store(base, holder + 24 + 4 * i, 0);
  }
}

// fctiwz as the translated code does it: NaN gives 0x80000000, at or above
// INT_MAX gives INT_MAX, else the truncation (cvttsd: 0x80000000 below
// INT_MIN).
int32_t ConvertToInt(double value) {
  if (std::isnan(value)) {
    return INT32_MIN;
  }
  if (value >= double(INT_MAX)) {
    return INT_MAX;
  }
  if (value <= double(INT_MIN) - 1.0) {
    return INT32_MIN;
  }
  return int32_t(value);
}

}  // namespace

uint32_t WaveBankViewInit(uint8_t* base, uint32_t view, uint32_t bank) {
  ITC_NATIVE_RUNNING("sub_8210C9D8 WaveBankView::Init");
  Store(base, view + 0, bank);
  Store(base, view + 12, 0);
  Store(base, view + 16, 0);
  Store(base, view + 20, 0);
  Store(base, view + 4, 0);
  Store(base, view + 8, bank);
  Store(base, view + 12, Load(base, bank + 8) + bank);
  Store(base, view + 16, Load(base, bank + 16) + bank);
  Store(base, view + 20, Load(base, bank + 40) + bank);
  return 0;
}

uint32_t WaveBankFormat(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8210C850 WaveBankView::Format");
  const uint32_t data = Load(base, view + kViewBankData);
  if (wave >= Load(base, data + 4)) {
    return 1;
  }
  // The compact bank's one format, or the wave's entry's.
  const bool compact = (Load(base, data + 0) & 0x20000) != 0;
  const uint32_t format_at =
      compact ? data + 84 : Load(base, view + kViewEntries) + kEntryBytes * wave + 4;
  const uint32_t format = Load(base, format_at);
  if ((format & 3) != 0) {
    return 1;
  }
  guest::StoreU8(base, out + 0, (format >> 31) != 0 ? 1 : 2);
  // Read again after the store, as the original does.
  const uint32_t again =
      compact ? Load(base, Load(base, view + kViewBankData) + 84)
              : Load(base, Load(base, view + kViewEntries) + kEntryBytes * wave + 4);
  Store(base, out + 8, (again >> 5) & 0x3FFFF);
  const uint32_t last =
      compact ? Load(base, Load(base, view + kViewBankData) + 84)
              : Load(base, Load(base, view + kViewEntries) + kEntryBytes * wave + 4);
  guest::StoreU8(base, out + 4, uint8_t((last >> 2) & 7));
  return 0;
}

uint32_t WaveBankLength(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8210C920 WaveBankView::Length");
  if (PastCount(base, view, wave)) {
    return 1;
  }
  Store(base, out, Load(base, kEntryBytes * wave + Load(base, view + kViewEntries) + 12));
  return 0;
}

uint32_t WaveBankData(uint8_t* base, uint32_t view, uint32_t wave, uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8210C958 WaveBankView::Data");
  if (PastCount(base, view, wave)) {
    return 1;
  }
  const uint32_t entry = kEntryBytes * wave + Load(base, view + kViewEntries);
  const uint32_t start = Load(base, view + kViewWaveData);
  Store(base, out, Load(base, entry + 8) + start);
  return 0;
}

std::optional<uint32_t> WaveBankEntry(uint8_t* base, uint32_t view, uint32_t wave,
                                      uint32_t out) {
  ITC_NATIVE_RUNNING("sub_8210C998 WaveBankView::Entry");
  const uint32_t entry = kEntryBytes * wave + Load(base, view + kViewEntries);
  if (!c_runtime::XMemCpy(base, out, entry, kEntryBytes)) {
    return std::nullopt;
  }
  return 0;
}

void SetBanks(uint8_t* base, uint32_t sound, uint32_t i, uint32_t j) {
  ITC_NATIVE_RUNNING("sub_8210CB48 CarSounds::SetBanks");
  const uint32_t parameters = Load(base, Load(base, (i << 4) + kSoundParameters) + (j << 2));
  const uint32_t engine = parameters + 820;
  Store(base, sound + 496, parameters);
  Store(base, sound + 504, Load(base, parameters));
  Store(base, sound + 500, parameters);
  Store(base, sound + 508, Load(base, parameters));
  Store(base, sound + 412, engine);
  Store(base, sound + 420, Load(base, engine));
  Store(base, sound + 416, engine);
  Store(base, sound + 424, Load(base, engine));
  Store(base, sound + 1248, 1);
}

void TireSoundAmount(uint8_t* base, uint32_t part, double f1) {
  ITC_NATIVE_RUNNING("sub_8210CA28 TireSound::Amount");
  const double limit = LoadF(base, 0x82007FC8);
  double amount = LoadF(base, part + 72);
  if (amount < LoadF(base, 0x820067B0)) {
    amount = double(float(amount * LoadF(base, 0x82002A60)));
    StoreF(base, part + 80, amount);
    if (amount > limit) {
      amount = limit;
    }
  } else {
    amount = limit;
  }
  StoreF(base, part + 80, amount);
  const double one = LoadF(base, 0x82000A34);
  if (f1 == one) {
    StoreF(base, part + 80, double(float(LoadF(base, part + 80) * LoadF(base, 0x8200295C))));
  }
  // The target: 8192 - 14 x +70 for small +70, else f1 scaled, negated
  // between `one` and [0x82007FEC] and below `one`.
  const uint16_t step = guest::LoadU16(base, part + 70);
  int32_t target;
  if (step < 7) {
    target = int16_t(uint32_t(8192) - uint32_t(int32_t(int16_t(step)) * 14));
  } else {
    const double scaled = double(float(f1 * LoadF(base, 0x8200801C)));
    target = int16_t(uint32_t(ConvertToInt(scaled)) & 0xFFFF);
    bool negate;
    if (f1 > one) {
      negate = !(f1 > LoadF(base, 0x82007FEC));
    } else {
      negate = f1 < one;
    }
    if (negate) {
      target = int16_t(uint32_t(-target));
    }
  }
  target = int16_t(target);
  if (target < -1170) {
    target = -1170;
  } else if (target > 1170) {
    target = 1170;
  }
  const uint16_t current = guest::LoadU16(base, part + 76);
  const int32_t difference = int32_t(int16_t(target)) - int32_t(int16_t(current));
  guest::StoreU16(base, part + 76, uint16_t(uint32_t(difference / 3) + current));
}

void EngineSoundConstruct(uint8_t* base, uint32_t engine) {
  ITC_NATIVE_RUNNING("sub_8210CBA0 EngineSound::EngineSound");
  for (const uint32_t layer : {engine, engine + 84}) {
    Store(base, layer + 0, kLayerVtable);
    ConstructHolder(base, layer + 44);
  }
}

uint32_t CarSoundConstruct(uint8_t* base, uint32_t sound) {
  ITC_NATIVE_RUNNING("sub_8210CC18 CarSound::CarSound");
  EngineSoundConstruct(base, sound + 408);
  for (const uint32_t offset : {584u, 676u, 736u, 792u, 840u, 888u, 952u, 1016u, 1072u, 1136u}) {
    ConstructHolder(base, sound + offset);
  }
  return sound;
}

Destroyed CarSoundsDestroy(uint8_t* base, uint32_t sound, uint32_t flags) {
  ITC_NATIVE_RUNNING("sub_8210DCA8 CarSounds::Destroy");
  if ((flags & 2) != 0) {
    const uint32_t block = sound - 16;
    const uint32_t count = Load(base, block);
    // From the last element down; the loop's count is signed.
    uint32_t at = sound + count * kCarSoundBytes;
    for (int32_t left = int32_t(count - 1); left >= 0; left--) {
      at -= kCarSoundBytes;
      for (const uint32_t offset :
           {1136u, 1072u, 1016u, 952u, 888u, 840u, 792u, 736u, 676u, 584u}) {
        Store(base, at + offset, kHolderVtable);
      }
      Store(base, at + 492, kLayerVtable);
      Store(base, at + 536, kHolderVtable);
      Store(base, at + 408, kLayerVtable);
      Store(base, at + 452, kHolderVtable);
    }
    return {block, (flags & 1) != 0 ? block : 0, true};
  }
  for (const uint32_t offset : {1136u, 1072u, 1016u, 952u, 888u, 840u, 792u, 736u, 676u, 584u}) {
    Store(base, sound + offset, kHolderVtable);
  }
  Store(base, sound + 492, kLayerVtable);
  Store(base, sound + 536, kHolderVtable);
  Store(base, sound + 408, kLayerVtable);
  Store(base, sound + 452, kHolderVtable);
  return {sound, (flags & 1) != 0 ? sound : 0, false};
}

}  // namespace car_sounds
