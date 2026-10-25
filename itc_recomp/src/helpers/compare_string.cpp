// XAPI's string compares as plain C++; see compare_string.h.

#include "helpers/compare_string.h"

#include "base/log.h"
#include "guest.h"
#include "hooks/last_error.h"

namespace helpers {

uint64_t CompareStringA(uint8_t* base, uint32_t pcr, uint32_t flags, uint32_t s1, int32_t n1,
                        uint32_t s2, int32_t n2) {
  ITC_NATIVE_RUNNING("sub_82297100 CompareStringA");
  const bool fold = (flags & 1) != 0;
  const bool word_sort = (flags & 0x1000) == 0;
  const bool skip_symbols = (flags & 4) != 0;
  if (s1 == 0 || s2 == 0 || n1 < -1 || n2 < -1) {
    last_error::Set(base, pcr, 87);  // ERROR_INVALID_PARAMETER
    return 0;
  }
  // What was skipped: picks the answer for strings equal but for it.
  uint32_t skipped = 0;
  auto skipped_answer = [&]() -> uint64_t {
    return guest::LoadU32(base, kSkippedAnswers + 4 * skipped);
  };
  // One of them ran out of its length (as the original's subfic, subfe,
  // rlwinm and addi: 3 when the first did, else 0xFFFFFFFE + 3).
  auto ran_out = [&]() -> uint64_t { return n1 == 0 ? 3 : 0xFFFFFFFEull + 3; };
  while (n1 != 0) {
    if (n2 == 0) {
      return ran_out();
    }
    const uint8_t c1 = guest::LoadU8(base, s1);
    if (c1 == 0 && guest::LoadU8(base, s2) == 0) {
      return skipped_answer();
    }
    if (c1 == 0) {
      return 1;
    }
    const uint8_t c2 = guest::LoadU8(base, s2);
    if (c2 == 0) {
      return 3;
    }
    uint32_t w1 = guest::LoadU32(base, kCharacterTypes + 4 * uint32_t(c1));
    uint32_t w2 = guest::LoadU32(base, kCharacterTypes + 4 * uint32_t(c2));
    // How the pair moves on: both, the first only, the second only.
    enum class Step { kBoth, kFirst, kSecond } step = Step::kBoth;
    bool compare = true;
    if (word_sort && (w1 & 0x10000) != 0) {
      if ((w2 & 0x10000) != 0) {
        if (w1 < w2) {
          skipped = 7;
        } else if (w1 > w2) {
          skipped = 11;
        }
        compare = false;
      } else {
        if (skipped == 0) {
          skipped = 4;
        }
        skipped |= 1;
        step = Step::kFirst;
        compare = false;
      }
    } else if (word_sort && (w2 & 0x10000) != 0) {
      if (skipped == 0) {
        skipped = 8;
      }
      skipped |= 2;
      step = Step::kSecond;
      compare = false;
    } else if (skip_symbols && (w1 & 0x20000) != 0) {
      step = Step::kFirst;
      compare = false;
    } else if (skip_symbols && (w2 & 0x20000) != 0) {
      step = Step::kSecond;
      compare = false;
    }
    if (compare) {
      if (fold) {
        if ((w1 & 0x100000) != 0) {
          w1 = (uint32_t(c1) + 32) | 0x80000;
        }
        if ((w2 & 0x100000) != 0) {
          w2 = (uint32_t(c2) + 32) | 0x80000;
        }
      }
      if (w1 < w2) {
        return 1;
      }
      if (w1 > w2) {
        return 3;
      }
    }
    if (step != Step::kFirst) {
      n2 = int32_t(uint32_t(n2) - 1);
      s2++;
    }
    if (step != Step::kSecond) {
      n1 = int32_t(uint32_t(n1) - 1);
      s1++;
    }
  }
  return n2 == 0 ? skipped_answer() : ran_out();
}

uint64_t Lstrcmp(uint8_t* base, uint32_t pcr, uint32_t a, uint32_t b) {
  ITC_NATIVE_RUNNING("sub_82296130 lstrcmpA");
  const uint64_t compared = CompareStringA(base, pcr, 0, a, -1, b, -1);
  if (uint32_t(compared) != 0) {
    return compared - 2;
  }
  if (a == 0) {
    return b == 0 ? 0 : ~0ull;
  }
  if (b == 0) {
    return 1;
  }
  uint64_t difference;
  do {
    const uint8_t ca = guest::LoadU8(base, a++);
    difference = uint64_t(ca) - uint64_t(guest::LoadU8(base, b++));
    if (ca == 0) {
      break;
    }
  } while (uint32_t(difference) == 0);
  return difference;
}

}  // namespace helpers
