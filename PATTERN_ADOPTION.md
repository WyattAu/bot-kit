# PATTERN_ADOPTION

bot-kit is competition bot scaffolding (C++ judge entries plus a Rust
Battlesnake crate), not a chat or delivery bot. Patterns with no surface in
this code (upload handling, consumer inboxes, teardown scopes, event
pipelines) are omitted under the format's rule 6. Checked with
`pattern-standards/scripts/adoption-check.py`.

| Pattern | Status | Evidence |
|---|---|---|
| T13 | adopted | crates/snake/src/lib.rs:87 (pure game core, no I/O, unit-tested), crates/snake/src/main.rs:1 (the axum binary only adapts HTTP to engine::choose) |
| P70 | adopted | crates/snake/src/lib.rs:18 (serde defaults on optional wire fields; unknown Battlesnake API fields are ignored, no deny_unknown_fields) |
| X6 | partial | cpp/ahc-template/ahc.cpp:94 (SA loop halts on a wall-clock budget), cpp/ahc-template/ahc.cpp:102 (best-so-far is committed at halt; no marginal-utility scheduler) |
| P23 | n/a | canon baseline line names this repo; code shows no implementation of P23: no abort, cancellation, shutdown, or teardown-scope surface in crates/ (header note already declares teardown scopes absent under format rule 6) |
| T50 | n/a | canon baseline line names this repo; code shows no implementation of T50: no agent write loop, capability gate, or sandbox exists; the snake binary only adapts HTTP to the pure engine (crates/snake/src/main.rs:1) |
