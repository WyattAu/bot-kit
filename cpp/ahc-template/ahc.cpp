// AtCoder Heuristic Contest — single-file C++23 template.
//
// Structure per AHC convention: interactive-less batch problem, relative
// score, long window. Everything below compiles clean under
// `g++ -std=c++23 -O2 -Wall -Wextra -Wpedantic`.
//
// Replace the `Problem` section per contest; keep the `Toolkit` intact.

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

// ---------------------------------------------------------------------------
// Toolkit
// ---------------------------------------------------------------------------

struct Timer {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    [[nodiscard]] long long elapsed_ms() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start)
            .count();
    }
    [[nodiscard]] bool over(long long budget_ms) const { return elapsed_ms() >= budget_ms; }
};

inline std::mt19937_64& rng() {
    static std::mt19937_64 g(std::random_device{}());
    return g;
}

[[nodiscard]] inline double rand01() {
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(rng());
}

// ---------------------------------------------------------------------------
// Problem — swap this section per contest
// ---------------------------------------------------------------------------

struct State {
    int n = 0;
    std::vector<int> a;         // example payload
    std::vector<int> order;     // current solution

    void read() {
        std::cin >> n;
        a.resize(n);
        for (auto& x : a) std::cin >> x;
        order.resize(n);
        for (int i = 0; i < n; ++i) order[i] = i;
    }

    [[nodiscard]] long long score() const {
        long long s = 0;
        for (int i = 1; i < n; ++i) s += std::abs(a[order[i]] - a[order[i - 1]]);
        return s;  // minimize; flip sign for maximize-style contests
    }

    bool neighbor() {
        const int i = static_cast<int>(rng()() % static_cast<std::uint64_t>(n));
        const int j = static_cast<int>(rng()() % static_cast<std::uint64_t>(n));
        if (i == j) return false;
        std::swap(order[i], order[j]);
        return true;
    }

    void output() const {
        for (int i = 0; i < n; ++i) std::cout << order[i] << " \n"[i + 1 == n];
    }
};

// ---------------------------------------------------------------------------
// Solver: simulated annealing with a wall-clock budget (AHC time limits are
// per-testcase wall time; start conservative, tune per problem).
// ---------------------------------------------------------------------------

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr long long kBudgetMs = 1500;
    Timer timer;

    State state;
    state.read();

    long long best_score = state.score();
    auto best_order = state.order;

    while (!timer.over(kBudgetMs)) {
        const long long before = state.score();
        if (!state.neighbor()) continue;
        const long long after = state.score();

        const double temp = 1.0 - static_cast<double>(timer.elapsed_ms()) / static_cast<double>(kBudgetMs);
        const bool accept = after <= before || rand01() < std::exp(static_cast<double>(before - after) / (temp + 1e-9));
        if (accept) {
            if (after < best_score) {
                best_score = after;
                best_order = state.order;
            }
        } else {
            state.neighbor();  // undo (same swap indices were consumed — re-swap)
        }
    }

    state.order = best_order;
    state.output();
    return 0;
}
