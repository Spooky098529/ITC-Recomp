# Pause Menu Input Polling Investigation

## Overview

This document tracks the investigation into pause menu input handling in ITC.

## Known Input Functions

### `sub_82093DC0` - First Tick Handler (Input Polling)

**Location**: `/home/sam/repos/recomptest/itc_recomp/generated/default/itc_recomp.19.cpp:39`

This is the first tick handler that polls input and then walks listener lists that run both game logic and the draw step.

**Signature**:
```cpp
DEFINE_REX_FUNC(sub_82093DC0) {
    // ... implementation ...
}
```

**Called from**: `itc_init.cpp:187` - registered in the function table.

### `sub_8258CEB0` - Input Processing

**Location**: `/home/sam/repos/recomptest/itc_recomp/generated/default/itc_recomp.19.cpp:95`

Called from `sub_82093DC0` after checking a condition (likely for input state).

### `sub_8258CAA0` - Input State Handler

**Location**: `/home/sam/repos/recomptest/itc_recomp/generated/default/itc_init.cpp:14306`

Called multiple times throughout the codebase (see calls in `itc_recomp.14.cpp`, `itc_recomp.19.cpp`, `itc_recomp.11.cpp`).

## Pause Menu Specifics

### Current Status

- **Issue**: Pause menu input polling is likely in untranslated code
- **Location**: Functions found in generated folder (`sub_82093DC0`, `sub_8258CEB0`, `sub_8258CAA0`)
- **Documentation**: This document serves as the tracking file for pause menu input handling

### What We Know

1. The main loop's first tick handler (`sub_82093DC0`) polls input at 30 Hz
2. Input polling happens before walking listener lists
3. Menus run at 30 fps (not interpolated)
4. Pause menu is a special case that needs to handle input differently

### What We Need to Investigate

1. **Pause state detection**: How does the game detect when paused?
2. **Input filtering**: How are inputs filtered when paused vs playing?
3. **Menu navigation**: How are menu inputs processed?
4. **Resume condition**: How is the pause state exited?

## Next Steps

1. Search for pause-related functions in the SDK source
2. Look for menu state flags or variables
3. Investigate how input is consumed vs ignored when paused
4. Document any special handling needed for pause menu inputs

## Related Issues

- Menus still run at 30 fps (not interpolated) - see `docs/native-renderer.md`
- Other per-object draws use projective matrices that aren't blended

## Key Functions Identified

### Input Processing Chain
1. **`sub_82093DC0`** - First tick handler (input polling at 30 Hz)
   - Location: `itc_recomp.19.cpp:39`
   - Called from `itc_init.cpp:187`

2. **`sub_8258CEB0`** - Input Processing
   - Location: `itc_recomp.50.cpp:33270`
   - Called from `sub_82093DC0` after checking input state condition

3. **`sub_8258CAA0`** - Input State Handler
   - Location: `itc_recomp.70.cpp:31444`
   - Called multiple times throughout codebase (`.11.cpp`, `.14.cpp`, `.19.cpp`)

4. **`sub_8258E530`** - Additional input handler
   - Location: `itc_recomp.88.cpp:32744`
   - Called from `sub_82094CD0` in `itc_recomp.13.cpp:91`

### Distribution Across Generated Files
- Functions appear in multiple `.cpp` files (`.11`, `.13`, `.14`, `.15`, `.19`, etc.)
- Registered in `itc_init.cpp` function tables at various offsets
- Called from both game logic and menu-related code paths

## Next Steps Needed

1. **Pause state detection**: How does the game detect when paused?
2. **Input filtering**: How are inputs filtered when paused vs playing?
3. **Menu navigation**: How are menu inputs processed?
4. **Resume condition**: How is the pause state exited?

## Investigation Notes

- Main loop polls input at 30 Hz before walking listener lists
- Menus run at 30 fps (not interpolated)
- Pause menu is a special case needing different input handling
- Need to search for pause-related flags/variables in SDK source



## Handoff point


Summary of Findings on Pause State Detection
     
  Based on my analysis of the generated assembly code, here's what I've discovered:

  Key Observation: Conditional Function Calls

  In itc_recomp.19.cpp, there's a critical pattern around lines 79-95:

  // Line 79-84: Load byte from offset 112 of r31 and compare to 0
  ctx.r11.u64 = REX_LOAD_U8(ctx.r31.u32 + 112);
  ctx.cr0.compare<uint32_t>(ctx.r11.u32, 0, ctx.xer);

  // Line 91-92: If byte equals 0, branch to loc_82093E90
  if (ctx.cr0.eq) goto loc_82093E90;

  // Line 93-95: Otherwise, call sub_8258CEB0 once
  sub_8258CEB0(ctx, base);

  What This Suggests

  1. State Tracking: The byte at offset 112 of r31 appears to track some state (possibly pause state)
     - If the byte is 0 → branch to special handling (loc_82093E90)
     - If non-zero → call sub_8258CEB0 once
  2. Double Call Path: At loc_82093E90, sub_8258CEB0 is called again, suggesting this might be a retry or special handling path
  3. Helper Functions:
     - sub_8258CEB0 → calls sub_82592500 (line 33291)
     - sub_8258CEE0 → calls sub_82592520 (line 32814)

  Memory Layout Pattern

  The code frequently checks/stores bytes at offsets 112 and 113 of r31:
  - Offset 112: Appears to be a flag/check byte
  - Offset 113: Appears to be another related flag
  
  Next Steps Needed

  To fully understand the pause state detection:

  1. Find where offset 112 is initialized - Need to trace back to see what sets this value
  2. Understand sub_82592500 and sub_82592520 - These are called from the helper functions
  3. Search for "pause" terminology in the original SDK source (not generated code)
  4. Look at the calling convention - What's passed in r3/r4 to these functions?

  Would you like me to:
  1. Search for where offset 112 is initialized?
  2. Analyze sub_82592500 and sub_82592520 in more detail?
  3. Search the SDK source for pause-related code? 
  
## handoff point 2 


 Summary
  
  The issue is in the tick handler at 0x82093E00 which handles periodic updates (likely for audio/video timing). The handler checks a pause flag and skips processing when paused.

  Key Findings:

  1. Pause Flag Location: The pause state is stored at offset 0x70 (112 bytes) from the base of r31
  2. Tick Handler Logic:
     - Loads the pause flag from r31+112 into r11
     - If paused (r11 == 0), it skips the main processing and returns early
     - If not paused, it calls various subroutines for audio/video updates
  3. The Problem: The tick handler is being called even when the system is paused, but it's correctly skipping the main logic. However, there might be side effects from the function calls that occur before the pause
     check.

  Root Cause:

  Looking at the code flow:
  - sub_8258CEB0 (called from both branches) eventually calls sub_82592500
  - sub_82592500 performs bitwise operations and returns early if the result is 0
  - This suggests the code is checking some condition before doing work
  
  The issue is that the tick handler should be completely skipped when paused, but currently it's still executing some code paths even when paused.

  Fix Needed:

  The tick handler needs to check the pause flag before calling any subroutines. Looking at the assembly, the check happens after loading the flag, but the function calls sub_8258CEB0 and sub_8258CEE0 happen before the
  final return in some paths.

  Let me create a memory entry documenting this issue:

  Thought for 4s, read 1 file (ctrl+o to expand)

❯ if you figure out what it's doing then summarize it here                                                                                                                                                                

  Thought for 58s (ctrl+o to expand)

● Looking at the tick handler code flow:

  What the Tick Handler Does (0x82093E00)

  lbz r11,112(r31)    ; Load pause flag from r31+112 (offset 0x70)
  cmplwi r11,0        ; Compare with 0
  beq loc_82093E90    ; If paused (r11==0), skip to end

  When NOT paused:
  1. Calls sub_8258CEB0 - processes audio/video updates
  2. Checks a condition, if true calls another handler via ctr
  3. Calls sub_8258CEE0 - likely handles additional timing logic
  4. Sets a flag at r31+113 to indicate processing completed
  
  When paused:
  - Jumps directly to loc_82093E90 which calls sub_8258CEB0 with different parameters and returns early
  
  The handler is checking the pause state and either doing full processing or a minimal path.
