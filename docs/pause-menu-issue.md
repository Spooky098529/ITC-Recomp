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
