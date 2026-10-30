# Bot targets — competition → scaffold → entry path

Priority order from `WyattAu/competitions` registry (p1 = this week). Each row:
what's scaffolded in this repo, what remains, and the entry mechanics.

| # | Competition | Scaffold | Status | Entry path |
|---|------------|----------|--------|------------|
| 1 | **CodeCup 2027 — Tumbleweed** (`codecup`, p1) | `cpp/tumbleweed/` — full board model + legality + v0 greedy + protocol | ✅ compiles, needs v1/v2 eval | Register at codecup.nl → "My submissions" → upload source (C++ supported). Test round **Oct 31, 2026**; every ~3 weeks after. Test locally with Caia. 30s total clock |
| 2 | **Battlesnake** (`battlesnake`, p1) | `crates/snake/` — axum server, tail-aware safety + head-to-head + flood-fill | ✅ builds, unit-tested | Run (`PORT=8000 cargo run -p snake`), expose publicly (fly.io/render free tier), register at play.battlesnake.com → join arena + tournaments. 24/7 ladder |
| 3 | **AtCoder Heuristic Contest** (`ahc`, p1) | `cpp/ahc-template/` — timer + SA loop + fast IO | ✅ template | atcoder.jp account → contest page → single-file C++ submit. ~monthly; AHC074 was Oct 30, 2026 |
| 4 | **CodinGame bot programming** (`codingame`, p2) | none yet — single-file Rust pasted into their IDE | TODO | codingame.com account → bot programming league → Rust is supported. Continuous Wood→Legend ladder |
| 5 | **Battlecode** (`battlecode`, p2) | none — Java engine; decide Jan 2027 whether to port or skip | TODO | battlecode.org; scrimmage ladder open to handles |
| 6 | **Screeps: Arena** (`screeps-arena`, p3) | none — JS/WASM | TODO | Steam + arena.screeps.com |
| 7 | **Ludii GGP** (`ludii`, p3) | none — Java framework | TODO | ludii.games; academic + community benchmarks |

## Non-bot p1s tracked in the registry (not scaffolded here)

- **Crucible** (AI-security CTF, continuous) — crucible.dreadnode.io
- **ICFP Contest** — July 2027, team event
- **IMC Prosperity** — April 2027; dogfood `WyattAu/book-kit` (limit order book) as the trading engine core
- **AIcrowd ARC White-Box** ($150k, live now)
- Chess circuit: `WyattAu/latent-oracle` (value-max refactor in flight; two-profile plan: policy→Lichess bullet, value-max→CCRL/CEGT classical)

## Cadence

- CodeCup test rounds: ~every 3 weeks (watch competition list)
- AHC: ~monthly, 4h sprints + ~10d marathons alternate
- Battlesnake: arena always on; official tournaments seasonal
- CTFtime events: weekly, pick p1-relevant ones ad hoc
