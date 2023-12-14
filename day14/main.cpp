#include "aoc.h"

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;
    using G = vector<vector<char>>;

    size_t C, R;
    G grid;

    std::map<G, uint32_t> seen;

    int64_t solve() {
        Input i; // ("example.txt");

        grid = i.chars_vec();
        C = grid[0].size();
        R = grid.size();

        if (PART1) {
            rollN();
        }
        else {
            seen[grid] = 0u;
            auto L = findLoop();
            //println("loop: ", L.first, " ", L.second);
            // 1000000000uL
            auto offset = L.first;
            auto cycle_length = L.second;
            auto n = 1000000000uL - (offset + 1);
            auto stat_cycle = offset + n % cycle_length;
            //println("cycle: ", stat_cycle);
            for (auto&& g : seen) {
                if (g.second == stat_cycle) {
					grid = g.first;
                    //println("found");
					break;
				}
			}
        }

        int64_t ans = calcLoad();

        return ans;
    }

    std::pair<uint32_t, uint32_t> findLoop()
    {
        uint32_t cycle = 0;
        for (;;++cycle) {
            rollCycle();
			auto it = seen.find(grid);
			if (it != seen.end()) {
				return { it->second, cycle - it->second };
			}
			seen[grid] = cycle;
        }
    }

    void rollN() {
        for (auto c : integers(C)) {
            size_t mr = 0;
            for (auto r : integers(R)) {
                auto& ch = grid[r][c];
                if (ch == '#') {
                    mr = r + 1;
                }
                else if (ch == 'O') {
                    if (r != mr) {
                        grid[mr][c] = 'O';
                        ch = '.';
                    }
                    ++mr;
                }
            }
        }
    }
    void rollS() {
        for (size_t c = 0; c < C; ++c) {
            size_t mr = R - 1;
            auto r = R;
            while (r--) {
                auto& ch = grid[r][c];
                if (ch == '#') {
                    mr = r - 1;
                }
                else if (ch == 'O') {
                    if (r != mr) {
                        grid[mr][c] = 'O';
                        ch = '.';
                    }
                    --mr;
                }
            }
        }
    }
    void rollW() {
        for (size_t r = 0; r < R; ++r) {
            size_t mc = 0;
            for (size_t c = 0; c < C; ++c) {
                auto& ch = grid[r][c];
                if (ch == '#') {
                    mc = c + 1;
                }
                else if (ch == 'O') {
                    if (c != mc) {
                        grid[r][mc] = 'O';
                        ch = '.';
                    }
                    ++mc;
                }
            }
        }
    }
    void rollE() {
        for (size_t r = 0; r < R; ++r) {
            size_t mc = C - 1;
            size_t c = C;
            while (c--) {
                auto& ch = grid[r][c];
                if (ch == '#') {
                    mc = c - 1;
                }
                else if (ch == 'O') {
                    if (c != mc) {
                        grid[r][mc] = 'O';
                        ch = '.';
                    }
                    --mc;
                }
            }
        }
    }
    void rollCycle() {
        rollN();
        rollW();
        rollS();
        rollE();
    }

    int64_t calcLoad() {
        int64_t ans = 0;
        for (auto c : integers(C)) {
            for (auto r : integers(R)) {
                auto& ch = grid[r][c];
                if (ch == 'O') {
                    ans += R - r;
                }
            }
        }
        return ans;
    }

};

//-----------------------------------------------------------------------------
int main() {
    try {
        Solver<false> p1;
        Solver<true> p2;
        Stopwatch sw;

        sw.start();
        auto r1 = p1.solve();
        sw.stop_print();
        println("part1: ", r1);

        sw.start();
        auto r2 = p2.solve();
        sw.stop_print();
        println("part2: ", r2); // 104619
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
