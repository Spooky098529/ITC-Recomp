// The tuning screen's part tables (docs/functions/tuning-screen.md): see
// tuning_screen.h. Each finds a word index into the screen (computed in 32
// bits, as the originals' address arithmetic is) by the category (+9656,
// signed), the item (byte +9660, unsigned) and the picks.

#include "tuning_screen/tuning_screen.h"

#include "base/log.h"
#include "guest.h"

#include <optional>

namespace tuning_screen {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
uint32_t Byte(uint8_t* base, uint32_t address) { return guest::LoadU8(base, address); }
int32_t CategoryOf(uint8_t* base, uint32_t screen) {
  return int32_t(Load(base, screen + kCategory));
}
uint32_t ItemOf(uint8_t* base, uint32_t screen) { return Byte(base, screen + kItem); }
// The screen's word at word index `index`.
uint32_t Word(uint8_t* base, uint32_t screen, uint32_t index) {
  return Load(base, screen + index * 4);
}

}  // namespace

uint32_t CategoryItemCount(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CC6C0 TuningScreen::CategoryItemCount");
  static constexpr uint32_t kCounts[7] = {4, 5, 1, 3, 10, 8, 4};
  const uint32_t m = Load(base, screen + kCategory);
  return m < 7 ? kCounts[m] : 0;
}

uint32_t IsItemAvailable(uint8_t* base, uint32_t screen, uint64_t item) {
  ITC_NATIVE_RUNNING("sub_821CDB18 TuningScreen::IsItemAvailable");
  const uint32_t r4 = uint32_t(item);
  std::optional<uint32_t> index;
  const int32_t m = CategoryOf(base, screen);
  if (m == 0) {
    if (int32_t(Load(base, screen + kState)) != 5) {
      return 1;
    }
    const uint32_t p = ItemOf(base, screen);
    if (p == 0) {
      index = Load(base, screen + kPartSet) * 10 + r4 + 8526;
    } else if (p == 1) {
      index = (Load(base, screen + kPartSet) + 1238) * 7 + r4;
    } else if (p == 2) {
      index = r4 + 8748;
    } else if (p == 3) {
      index = r4 + 8771;
    }
  } else if (m == 1) {
    static constexpr uint32_t kRows[5] = {8800, 8822, 8844, 8866, 8892};
    const uint32_t p = ItemOf(base, screen);
    if (p < 5) {
      index = r4 + kRows[p];
    }
  } else if (m == 2) {
    index = r4 + 8920;
  } else if (m == 3) {
    index = r4 + 8950;
  } else if (m == 4) {
    if (ItemOf(base, screen) == 9) {
      index = r4 + 9144;
    }
  }
  return index ? Word(base, screen, *index) : 1;
}

uint64_t WheelNumber(uint64_t brand, uint64_t model) {
  ITC_NATIVE_RUNNING("sub_821CDED8 TuningScreen::WheelNumber");
  static constexpr uint32_t kFirst[17] = {0,   1,   39,  47,  72,  99,  116, 123, 124,
                                          126, 135, 144, 147, 162, 163, 165, 170};
  const uint32_t b = uint32_t(brand);
  if (b > 16) {
    return 0;
  }
  return model + kFirst[b];
}

uint32_t GetPrice(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CD160 TuningScreen::GetPrice");
  guest::StoreU32(base, screen + kPrice, 0);
  std::optional<uint32_t> index;
  const int32_t m = CategoryOf(base, screen);
  const uint32_t p = ItemOf(base, screen);
  auto level = [&] { return Byte(base, screen + kLevel); };
  auto type = [&] { return Byte(base, screen + kType); };
  if (m == 0) {
    if (p == 0) {
      index = level() + Load(base, screen + kPartSet) * 10 + 8476;
    } else if (p == 1) {
      index = (Load(base, screen + kPartSet) + 1233) * 7 + level();
    } else if (p == 2) {
      index = level() + 8741;
    } else if (p == 3) {
      index = level() + 8764;
    }
  } else if (m == 1) {
    static constexpr uint32_t kRows[5] = {8793, 8815, 8837, 8859, 8882};
    if (p < 5) {
      index = level() + kRows[p];
    }
  } else if (m == 2) {
    index = level() + 8913;
  } else if (m == 3) {
    static constexpr uint32_t kRows[3] = {8935, 8972, 9004};
    if (p < 3) {
      index = level() * 3 + Byte(base, screen + kOption) + kRows[p];
    }
  } else if (m == 4) {
    static constexpr uint32_t kRows[10] = {9036, 9047, 9058, 9069, 0,
                                           9105, 9114, 9123, 9132, 9141};
    if (p == 4) {
      index = (type() + 4541) * 2 + Byte(base, screen + kType2);
    } else if (p <= 9) {
      index = type() + kRows[p];
    }
  } else if (m == 5) {
    if (p == 0) {
      index = Load(base, screen + 25204) * 38 + Load(base, screen + 25220) + 6312;
    } else if (p == 1) {
      index = Byte(base, screen + kOption) + 9151;
    } else if (p == 2) {
      index = type() + 9156;
    } else if (p == 3) {
      index = type() + 9163;
    }
  } else if (m == 6) {
    if (p == 0) {
      index = type() + 9172;
    } else if (p == 1) {
      index = type() + 9184;
    } else if (p == 2) {
      index = type() + 9193;
    } else if (p == 3) {
      const int32_t kind = int32_t(Load(base, screen + 33540));
      if (kind == 0) {
        index = Load(base, screen + 33548) + 8396;
      } else if (kind == 1) {
        index = (Load(base, screen + 33564) + 1682) * 5 + Load(base, screen + 33572);
      }
    }
  }
  if (index) {
    guest::StoreU32(base, screen + kPrice, Word(base, screen, *index));
  }
  return Load(base, screen + kPrice);
}

uint32_t GetCurrentChoice(uint8_t* base, uint32_t screen) {
  ITC_NATIVE_RUNNING("sub_821CD868 TuningScreen::GetCurrentChoice");
  std::optional<uint32_t> offset;
  const int32_t m = CategoryOf(base, screen);
  const uint32_t p = ItemOf(base, screen);
  if (m == 0) {
    if (p == 0) {
      offset = (Load(base, screen + kPartSet) + 8576) * 4;
    } else if (p == 1) {
      offset = (Load(base, screen + kPartSet) + 8701) * 4;
    } else if (p == 2) {
      offset = 35020;
    } else if (p == 3) {
      offset = 35140;
    }
  } else if (m == 1) {
    static constexpr uint32_t kRows[5] = {35228, 35316, 35404, 35492, 35608};
    if (p < 5) {
      offset = kRows[p];
    }
  } else if (m == 2) {
    offset = 35708;
  } else if (m == 3) {
    static constexpr uint32_t kRows[3] = {35820, 35948, 36076};
    if (p < 3) {
      offset = kRows[p];
    }
  } else if (m == 4) {
    static constexpr uint32_t kRows[10] = {36164, 36208, 36252, 36300, 36372,
                                           36436, 36472, 36508, 36544, 36588};
    if (p <= 9) {
      offset = kRows[p];
    }
  } else if (m == 5) {
    if (p == 0) {
      offset = (Load(base, screen + 25196) + 6958) * 4;
    } else if (p == 1) {
      offset = 36612;
    } else if (p == 2) {
      offset = 36636;
    } else if (p == 3) {
      offset = 36668;
    }
  } else if (m == 6) {
    if (p == 0) {
      offset = 36708;
    } else if (p == 1) {
      offset = 36752;
    } else if (p == 2) {
      offset = 36796;
    } else if (p == 3 && int32_t(Load(base, screen + 33540)) == 0) {
      offset = 33616;
    }
  }
  return offset ? Load(base, screen + *offset) : 0;
}

uint32_t GetChoiceFlag(uint8_t* base, uint32_t screen, uint64_t choice) {
  ITC_NATIVE_RUNNING("sub_821CDC38 TuningScreen::GetChoiceFlag");
  const uint32_t r4 = uint32_t(choice);
  std::optional<uint32_t> index;
  const int32_t m = CategoryOf(base, screen);
  if (m == 0) {
    const uint32_t p = ItemOf(base, screen);
    if (p == 0) {
      index = Load(base, screen + kPartSet) * 10 + r4 + 8581;
    } else if (p == 1) {
      index = Load(base, screen + kPartSet) * 7 + r4 + 8706;
    } else if (p == 2) {
      index = r4 + 8757;
    } else if (p == 3) {
      index = r4 + 8786;
    }
  } else if (m == 1) {
    static constexpr uint32_t kRows[5] = {8808, 8830, 8852, 8875, 8903};
    const uint32_t p = ItemOf(base, screen);
    if (p < 5) {
      index = r4 + kRows[p];
    }
  } else if (m == 2) {
    index = r4 + 8928;
  } else if (m == 3) {
    const uint32_t p = ItemOf(base, screen);
    if (p == 0) {
      index = Byte(base, screen + kLevel) * 3 + r4 + 8957;
    } else if (p == 1) {
      index = Byte(base, screen + kLevel) * 3 + r4 + 8989;
    } else if (p == 2) {
      index = (Byte(base, screen + kLevel) + 3007) * 3 + r4;
    }
  } else if (m == 4) {
    static constexpr uint32_t kRows[10] = {9042, 9053, 9064, 9076, 0,
                                           9110, 9119, 9128, 9137, 9148};
    const uint32_t p = ItemOf(base, screen);
    if (p == 4) {
      index = Byte(base, screen + kType) * 2 + r4 + 9095;
    } else if (p <= 9) {
      index = r4 + kRows[p];
    }
  } else if (m == 5) {
    const uint32_t p = ItemOf(base, screen);
    if (p == 0) {
      index = Load(base, screen + 25204) * 38 + r4 + 6964;
    } else if (p == 1) {
      index = r4 + 9154;
    } else if (p == 2) {
      index = r4 + 9160;
    } else if (p == 3) {
      index = r4 + 9168;
    }
  } else if (m == 6) {
    const uint32_t p = ItemOf(base, screen);
    if (p == 0) {
      index = r4 + 9179;
    } else if (p == 1) {
      index = r4 + 9189;
    } else if (p == 2) {
      index = r4 + 9200;
    } else if (p == 3) {
      const int32_t kind = int32_t(Load(base, screen + 33540));
      if (kind == 0) {
        index = r4 + 8406;
      } else if (kind == 1) {
        index = (Load(base, screen + 33564) + 1691) * 5 + r4;
      }
    }
  }
  return index ? Word(base, screen, *index) : 0;
}

}  // namespace tuning_screen
