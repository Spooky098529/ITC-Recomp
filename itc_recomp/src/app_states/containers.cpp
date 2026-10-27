// The containers the app's update and its state stack use, natively
// (containers.h, docs/functions/app-states.md).

#include "app_states/containers.h"

#include "guest.h"

#include "base/log.h"
#include "memory/c_runtime_heap.h"

namespace app_states {

namespace {

uint32_t Load(uint8_t* base, uint32_t address) { return guest::LoadU32(base, address); }
void Store(uint8_t* base, uint32_t address, uint32_t value) {
  guest::StoreU32(base, address, value);
}

void Delete(uint32_t block, const char* what) {
  if (!memory::crt::OperatorDelete(block)) {
    ITC_LOG_ERROR("{}: operator delete refused {:08X}", what, block);
  }
}

}  // namespace

bool MessagesErase(uint8_t* base, uint32_t vector, uint32_t first_vector, uint32_t first,
                   uint32_t last_vector, uint32_t last) {
  if (first_vector == 0 || first_vector != last_vector) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82092E70 MessageVector::Erase");
  if (first != last) {
    const uint32_t end = Load(base, vector + kVectorLast);
    const uint32_t moved = uint32_t(int32_t(end - last) >> 2) << 2;
    const uint32_t new_last = first + moved;
    if (last != end) {
      const uint32_t delta = first - last;
      for (uint32_t from = last; from != end; from += 4) {
        Store(base, from + delta, Load(base, from));
      }
    }
    Store(base, vector + kVectorLast, new_last);
  }
  return true;
}

bool MessagesClear(uint8_t* base, uint32_t vector) {
  const uint32_t first = Load(base, vector + kVectorFirst);
  const uint32_t last = Load(base, vector + kVectorLast);
  if (first > last || vector == 0) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_82092F98 MessageVector::Clear");
  return MessagesErase(base, vector, vector, first, vector, last);
}

std::optional<uint32_t> MessagesAllocate(uint8_t* base, uint32_t count) {
  if (count != 0 && 0xFFFFFFFFu / count < 4) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_82092B78 MessageVector::Allocate");
  return memory::crt::OperatorNew(base, count * 4);
}

std::optional<uint32_t> OpAllocateNodes(uint8_t* base, uint32_t count) {
  if (count != 0 && 0xFFFFFFFFu / count < kOpNodeBytes) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ACF10 OpList::AllocateNodes");
  return memory::crt::OperatorNew(base, count * kOpNodeBytes);
}

bool OpIncSize(uint8_t* base, uint32_t list, uint32_t count) {
  const uint32_t size = Load(base, list + kListSize);
  if (kOpListMaxSize - size < count) {
    return false;
  }
  ITC_NATIVE_RUNNING("sub_820AD768 OpList::IncSize");
  Store(base, list + kListSize, size + count);
  return true;
}

uint32_t ListClear(uint8_t* base, uint32_t list) {
  ITC_NATIVE_RUNNING("sub_820ACF88 List::Clear");
  uint32_t head = Load(base, list + kListHead);
  uint32_t node = Load(base, head + kNodeNext);
  Store(base, head + kNodeNext, head);
  head = Load(base, list + kListHead);
  Store(base, head + kNodePrevious, head);
  head = Load(base, list + kListHead);
  Store(base, list + kListSize, 0);
  while (node != head) {
    const uint32_t next = Load(base, node + kNodeNext);
    Delete(node, "List::Clear");
    head = Load(base, list + kListHead);
    node = next;
  }
  return node;
}

std::optional<uint32_t> DelegateClone(uint8_t* base, uint32_t delegate) {
  const auto block = memory::crt::OperatorNew(base, kDelegateBytes);
  if (!block) {
    return std::nullopt;
  }
  ITC_NATIVE_RUNNING("sub_820ACA48 Delegate::Clone");
  if (*block == 0) {
    return 0;
  }
  Store(base, *block + 0, kDelegateVtable);
  Store(base, *block + 4, Load(base, delegate + 4));
  Store(base, *block + 8, Load(base, delegate + 8));
  return *block;
}

uint32_t LayerIsEmpty(uint8_t* base, uint32_t state) {
  ITC_NATIVE_RUNNING("sub_820ACE50 StateLayer::IsEmpty");
  return Load(base, state + 28) == 0 && Load(base, state + 16) == 0 ? 1 : 0;
}

}  // namespace app_states
