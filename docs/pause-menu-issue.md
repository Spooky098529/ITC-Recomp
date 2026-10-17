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
  
