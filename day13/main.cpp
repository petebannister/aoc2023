#include "aoc.h"

//-----------------------------------------------------------------------------
struct Solver
{
    using V = vector<vector<bool>>;
    V cols;
    V rows;

    void solve(bool part2) {
        Input i;// ("example.txt");

        uint64_t ans = 0u;

        uint32_t row = 0u;
        for (string_view line : i.lines()) {
            if (line.empty()) {
                ans += solve_i();
				cols.clear();
				rows.clear();
                row = 0u;
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
        auto r = reflectionPoint(rows);
        auto c = reflectionPoint(cols);
        uint32_t x = 0u;
        if (r > 0) {
			x += r * 100;
        }
        if (c > 0) {
            x += c;
        }
        println(x);
        return x;
    }

    int32_t reflectionPoint(V& v) {
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
                return x + 1u;
            }
        }
        return -1;
    }
};

//-----------------------------------------------------------------------------
int main() {
    try {
        Solver s;
        s.solve(false); // 28583 too low, 34037 too high, 33975
        //s.solve(true);
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
