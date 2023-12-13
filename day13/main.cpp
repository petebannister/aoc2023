#include "aoc.h"

//-----------------------------------------------------------------------------
template <bool part2>
struct Solver
{
    using V = vector<vector<bool>>;
    V cols;
    V rows;
    int gridnum = 0;

    void solve() {
        Input i; // ("example.txt");

        uint64_t ans = 0u;

        uint32_t row = 0u;
        for (string_view line : i.lines()) {
            if (line.empty()) {
                ans += solve_i();
				cols.clear();
				rows.clear();
                row = 0u;
                ++gridnum;
				continue;
			}
            rows.resize(row + 1u);
            rows.back().resize(line.size());
            if (cols.empty()) {
                cols.resize(line.size());
            }
            uint32_t col = 0u;
            for (auto c : line) {
                auto v = (c == '#');
                rows[row][col] = v;
                cols[col].push_back(v);
                ++col;
			}
            ++row;
        }
        if (!rows.empty()) {
            ans += solve_i();
		}

        println((part2) ? "part2: " : "part1: ", ans);
    }
    uint32_t solve_i() {
        uint32_t x = 0u;
        auto r = solveV(rows);
        if (r > -1) {
			x = r * 100;
        }
        else {
            auto c = solveV(cols);
            if (c > -1) {
                x = c;
            }
        }
        //println(x);
        return x;
    }

    int32_t solveV(V& v) {
        auto r = reflectionPoint(v);
        if (!part2) {
			return r;
		}
        for (auto a : integers(v.size())) {
            for (auto b : integers(v[a].size())) {
                bool t = v[a][b]; // can't use auto here!!
                v[a][b] = !t;
                auto r2 = reflectionPoint(v, r);
                if (r2 > -1 && r2 != r) {
                    //print_v(v);
                    //println("smudge: ", a, ", ", b, ", ", r2);
					return r2;
				}
                v[a][b] = t;
			}
        }
        return -1;
    }

    int32_t reflectionPoint(V& v, int32_t except = -1) {
        for (uint32_t x = 0u; x < v.size() - 1; ++x) {
            auto n = min(x + 1u, ((uint32_t)v.size() - x) - 1u);
            bool ok = true;
            for (auto k = 0u; k < n; ++k) {
                if (v[x - k] != v[x + k + 1]) {
                    ok = false;
                    break;
				}
            }
            if (ok) {
                if ((x + 1u) != except) {
                    return x + 1u;
                }
            }
        }
        return -1;
    }
    int32_t print_v(V& v) {
        if (&v == &rows) {
            for (auto a : integers(v.size())) {
                for (auto b : integers(v[a].size())) {
                    print(v[a][b] ? '#' : '.');
                }
                println();
            }
		}
		else {
            for (auto a : integers(v[0].size())) {
                for (auto b : integers(v.size())) {
                    print(v[b][a] ? '#' : '.');
                }
                println();
            }
		}
        println();
        return 0;
    }
};

//-----------------------------------------------------------------------------
int main() {
    try {
        Solver<false> p1;
        Solver<true> p2;
        p1.solve(); // 33975
        p2.solve(); // 29083
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
