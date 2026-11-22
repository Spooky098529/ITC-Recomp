// The state digest (--state_digest, docs/e5-native.md "Checks"): one log
// line a race tick with a hash of the racers' state, so two runs of the pad
// replay can be compared tick by tick and the first tick that differs found.

#pragma once

namespace debug {

// Main thread, after a logic pass in a race.
void StateDigest_OnRaceTick();

// Main thread, when a race is entered (the tick count starts again).
void StateDigest_OnRaceEntered();

// Main thread, when the race starts (the race world loaded: the course
// set-up's message 4): the first one in a race starts the count again; ticks
// before it aren't logged.
void StateDigest_OnRaceStarted();

}  // namespace debug
