// Tumbleweed — CodeCup 2027 entry skeleton (C++23, single file).
//
// CodeCup variant rules (codecup.nl/tumbleweed/rules.php — these DIFFER from
// the official game):
//   * Hex board, 6 per side, 91 cells. Columns A-K run NE->SW, rows 1-11 W->E.
//   * Start: neutral v2 stack at F6, red v1 and white v1 at judge-given cells.
//   * You output only the CELL; the placed value is auto-determined as the
//     number of friendly stacks the new stack sees (unobstructed straight line).
//   * Placing on an occupied cell (capture / friendly upgrade) requires
//     new value > existing value. Initial stacks must see >= 1 friendly.
//   * No suicide: your sightline count must be >= opponent's to that cell.
//   * Territory (judge-simulated) ends the game; score = territory cells
//     + 200/100 win/loss bonus. Invalid move or 30s TOTAL clock = default loss.
//
// Protocol (stdin/stdout, line-based, flush every move):
//   red:   read [red_init, white_init, "Start"]  -> print move
//   white: read [red_init, white_init, red_move] -> print move
//   then alternate: read opponent move, print move. "Quit" => exit.
//
// Engine ladder (docs/TUMBLEWEED.md):
//   v0 greedy material (this file) -> v1 sightline-difference eval ->
//   v2 judge-style territory simulation -> v3 alpha-beta on territory ->
//   v4 Caia self-play opening book.

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace tw {

constexpr int N = 11;  // board label space is 11x11; 91 cells are legal

enum Color : std::uint8_t { Empty = 0, Red = 1, White = 2, Neutral = 3 };

struct Stack {
    Color color;
    int value;
};

struct Board {
    std::array<std::array<Stack, N + 1>, N + 1> cell{};  // [c][r], 1-based

    static bool valid(int c, int r) {
        return c >= 1 && c <= N && r >= 1 && r <= N && (c - r >= -5) && (c - r <= 5);
    }

    // The six hex steps in (column, row) space for this labeling. Three
    // axes: rows (W->E), columns (NE->SW, constant letter), and NW->SE
    // diagonals (constant c-r). Validated against the official sample game:
    // red B2 -> E5 must be a sightline, which fixes the third axis to (+1,+1).
    static constexpr std::array<std::pair<int, int>, 6> kSteps{{
        {1, 0}, {-1, 0},   // rows (W->E)
        {0, 1}, {0, -1},   // columns (NE->SW)
        {1, 1}, {-1, -1},  // NW->SE diagonals (preserves c-r)
    }};

    // Number of stacks of `color` visible from (c, r). Rays start at the six
    // neighbors and stop at the first stack; the target cell's own occupant
    // (if any) is never counted — placement replaces it.
    int seen(Color color, int c, int r) const {
        int count = 0;
        for (auto [dc, dr] : kSteps) {
            int cc = c + dc, rr = r + dr;
            while (valid(cc, rr) && cell[cc][rr].color == Empty) {
                cc += dc;
                rr += dr;
            }
            if (valid(cc, rr) && cell[cc][rr].color == color) ++count;
        }
        return count;
    }

    int seen_opponent(Color me, int c, int r) const {
        return seen(me == Red ? White : Red, c, r);
    }

    bool legal(Color me, int c, int r) const {
        if (!valid(c, r)) return false;
        const Stack& s = cell[c][r];
        const int v = seen(me, c, r);
        if (v < 1) return false;                                    // must see a friend
        if (v < seen_opponent(me, c, r)) return false;              // no suicide
        if (s.color != Empty && v <= s.value) return false;         // capture needs strictly greater
        return true;
    }

    // Apply a placement for `color` at (c, r). Value is auto-determined.
    void apply(Color color, int c, int r) { cell[c][r] = {color, seen(color, c, r)}; }
};

struct Timer {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    [[nodiscard]] long long elapsed_ms() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start)
            .count();
    }
};

// ---------------------------------------------------------------------------
// Engine v0: greedy material. Budget: 30s TOTAL for the whole game — the
// skeleton spends microseconds per move; the search ladder can spend seconds.
// ---------------------------------------------------------------------------

struct Engine {
    Board board;
    Color me = Red;
    Color opp = White;
    Timer clock;

    struct Move {
        int c = 0, r = 0;
        long long score = -1;
    };

    std::vector<Move> legal_moves() const {
        std::vector<Move> moves;
        for (int c = 1; c <= N; ++c) {
            for (int r = 1; r <= N; ++r) {
                if (!board.legal(me, c, r)) continue;
                Move m{c, r, 0};
                const int v = board.seen(me, c, r);
                m.score = v;                                        // new material
                if (board.cell[c][r].color == opp) {
                    m.score += 3LL * board.cell[c][r].value;        // stripping enemy stacks
                } else if (board.cell[c][r].color == Neutral) {
                    m.score += 2LL * board.cell[c][r].value;        // claiming the big neutral
                }
                // TODO(v1): prefer placements where (seen(me) - seen(opp)) is
                // maximal, and penalize leaving cheap recaptures nearby.
                // TODO(v2): replicate the judge's territory simulation for the
                // true score. TODO(v3): alpha-beta on territory eval.
                moves.push_back(m);
            }
        }
        return moves;
    }

    Move best() {
        Move best;
        for (const Move& m : legal_moves()) {
            if (m.score > best.score) best = m;
        }
        return best;  // judge guarantees a legal move exists on your turn
    }
};

// ---------------------------------------------------------------------------
// Protocol
// ---------------------------------------------------------------------------

std::string cell_name(int c, int r) {
    std::string s;
    s += static_cast<char>('A' + c - 1);
    s += std::to_string(r);
    return s;
}

bool parse_cell(const std::string& s, int& c, int& r) {
    if (s.size() < 2) return false;
    c = s[0] - 'A' + 1;
    r = std::stoi(s.substr(1));
    return Board::valid(c, r);
}

std::string read_line() {
    std::string line;
    if (!std::getline(std::cin, line)) line = "Quit";
    while (!line.empty() && (line.back() == '\r' || line.back() == '\n')) line.pop_back();
    return line;
}

void print_move(const Engine::Move& m) {
    std::cout << cell_name(m.c, m.r) << '\n' << std::flush;
}

}  // namespace tw

int main() {
    using namespace tw;

    Engine engine;
    engine.me = Red;
    engine.opp = White;

    const std::string red_init = read_line();
    const std::string white_init = read_line();
    const std::string third = read_line();

    int rc = 0, rr = 0, wc = 0, wr = 0;
    if (!parse_cell(red_init, rc, rr) || !parse_cell(white_init, wc, wr)) return 1;

    // Setup: neutral v2 at F6; initial red/white stacks are judge-given, v1.
    engine.board.cell[6][6] = {Neutral, 2};
    engine.board.cell[rc][rr] = {Red, 1};
    engine.board.cell[wc][wr] = {White, 1};

    // Play our best move: output it AND apply it — the board must reflect
    // our own stacks or we re-play the same cell forever.
    auto play = [&engine] {
        const auto m = engine.best();
        print_move(m);
        engine.board.apply(engine.me, m.c, m.r);
    };

    if (third == "Start") {
        // We are red and move first.
        play();
    } else {
        // We are white; `third` is red's first move.
        engine.me = White;
        engine.opp = Red;
        int c = 0, r = 0;
        if (!parse_cell(third, c, r)) return 0;
        engine.board.apply(Red, c, r);
        play();
    }

    for (;;) {
        const std::string line = read_line();
        if (line == "Quit") return 0;
        int c = 0, r = 0;
        if (!parse_cell(line, c, r)) return 0;
        engine.board.apply(engine.opp, c, r);
        play();
    }
}
