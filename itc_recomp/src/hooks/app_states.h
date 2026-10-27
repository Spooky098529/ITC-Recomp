// The app's update and its state stack's register steps (src/app_states/,
// docs/functions/app-states.md), shared by the adapters
// (src/hooks/app_states.cpp, behind --native_app_states) and the harness
// (without the flag). Each leaves r3 as the original does and runs the
// original where the plain C++ refuses. The ones that allocate or free run
// natively only while --native_heap routes the game's heap to the
// allocation layer (else the original).

#pragma once

#include "guest.h"

namespace hooks {

// MessageVector::Erase sub_82092E70: r3 = the result, r4 = the vector, r5
// and r6 the checked iterators (the vector in the high word; stored at the
// caller's r1 + 32 and + 40); the first stored at r3, r3 left.
void AppMessagesErase(PPCContext& ctx, uint8_t* base);
// MessageVector::Clear sub_82092F98: r3 = the vector; r3 = Erase's result in
// its 96-byte frame (r1 - 16).
void AppMessagesClear(PPCContext& ctx, uint8_t* base);
// MessageVector::Allocate sub_82092B78 and OpList::AllocateNodes
// sub_820ACF10: r3 = the count; r3 the block.
void AppMessagesAllocate(PPCContext& ctx, uint8_t* base);
void AppOpAllocateNodes(PPCContext& ctx, uint8_t* base);
// OpList::IncSize sub_820AD768: r3 = the list, r4 = the count; r3 left.
void AppOpIncSize(PPCContext& ctx, uint8_t* base);
// List::Clear sub_820ACF88: r3 = the list; r3 = its head.
void AppListClear(PPCContext& ctx, uint8_t* base);
// PtrList::Erase sub_82098E80: r3 = the result, r4 = the list, r5 = the
// iterator (the list in the high word); the next node's iterator at r3 and
// the caller's r1 + 32; r3 left.
void AppPtrListErase(PPCContext& ctx, uint8_t* base);
// Delegate::Clone sub_820ACA48: r3 = the delegate; r3 the copy or 0.
void AppDelegateClone(PPCContext& ctx, uint8_t* base);
// StateLayer::IsEmpty sub_820ACE50: r3 = the layer + 4; r3 = 1 or 0.
void AppLayerIsEmpty(PPCContext& ctx, uint8_t* base);

}  // namespace hooks
