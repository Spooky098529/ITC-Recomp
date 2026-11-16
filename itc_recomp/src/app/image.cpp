// The SDK's description of the image (docs/rexglue-removal.md, "Before E1:
// the rewiring", stage 5): its layout, and the functions the kernel calls
// by address ("The kernel's entries" there): the game's main thread's start
// (our start-up, at xstart's address) and the thread starts. In place of
// generated/'s itc_init.cpp, which registered every recompiled function.

#include "guest_code.h"

#include <rex/ppc/func.h>

REX_EXTERN(xstart);
REX_EXTERN(sub_825BA558);  // XapiThreadStartup
REX_EXTERN(sub_822B69A8);  // Device::WorkerThread
REX_EXTERN(sub_8245BE08);  // XAudio::MasterThread
REX_EXTERN(sub_8245BED0);  // XAudio::WorkerThread

namespace {

const PPCFuncMapping kKernelEntries[] = {
    {0x82296C80, xstart},
    {0x825BA558, sub_825BA558},
    {0x822B69A8, sub_822B69A8},
    {0x8245BE08, sub_8245BE08},
    {0x8245BED0, sub_8245BED0},
    {0, nullptr},
};

}  // namespace

const rex::PPCImageInfo PPCImageConfig = {
    .code_base = REX_CODE_BASE,
    .code_size = REX_CODE_SIZE,
    .image_base = REX_IMAGE_BASE,
    .image_size = REX_IMAGE_SIZE,
    .func_mappings = kKernelEntries,
    .rexcrt_heap = false,
};
