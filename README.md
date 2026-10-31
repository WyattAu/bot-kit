# bot-kit

Competition bot scaffolds for the WyattAu estate — one repo per discipline is
overkill; one registry (`WyattAu/competitions`) plus one buildable kit per game
is right. Every scaffold here **compiles, runs, and has a protocol-correct I/O
loop** on day one; strength improvements are commits, not setup work.

## Layout

```
crates/snake/        Battlesnake harness + pure core (Rust) — entry repo: WyattAu/wyatt-viper (planned)
cpp/botkit/          botkit.hpp — shared header-only C++ (labeled hex grids, timers, line protocol)
cpp/ahc-template/    AtCoder Heuristic Contest — SA template with clock budget (C++23)
docs/TARGETS.md      competition → scaffold → entry-path map (dated)
```

## Quickstart

```sh
# Battlesnake (test locally, deploy anywhere Rust runs)
cargo run -p snake                 # PORT=8000 default
cargo test -p snake

# AHC template
g++ -std=c++23 -O2 -o /tmp/ahc cpp/ahc-template/ahc.cpp
```

Entries consuming `cpp/botkit/botkit.hpp` (e.g. [`WyattAu/tumbleweed`](https://github.com/WyattAu/tumbleweed)) vendor + amalgamate it — see that repo's `scripts/`.

## Conventions

- **C++**: single-file entries where the judge expects them (CodeCup, AHC),
  C++23, no external deps, wall-clock budget always explicit. Shared utilities
  live in `cpp/botkit/botkit.hpp`; entry repos vendor it at a pinned commit and
  amalgamate into the judge's single-file format (see `WyattAu/tumbleweed`).
- **Rust**: axum/tokio for HTTP bots; pure logic separated from I/O so the
  interesting part is unit-testable.
- **CI**: estate `rust-kit.yml` (tier c) for the workspace; g++ compile gate for
  every cpp scaffold — a scaffold that doesn't compile is a lie.
- Deterministic self-play / local match running is `warploom`'s job
  (separate repo) once any game here needs a real training loop.

## Targets

See `docs/TARGETS.md`. Immediate: Tumbleweed test round **Oct 31 2026**,
Battlesnake arena (always on), AHC (~monthly).

## Related

- `WyattAu/competitions` — the registry this kit serves (135 entries)
- `WyattAu/latent-oracle` — the chess engine (its own repo, its own journey)
- `WyattAu/engineering-standards` — shared CI this repo consumes
- `WyattAu/warploom` — deterministic sim engine for future self-play work
- `WyattAu/book-kit` — LOB core to reuse for IMC Prosperity (April)
