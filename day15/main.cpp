#include "aoc.h"

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;

    uint8_t H(string_view s) {
        uint8_t r = 0;
        for (auto c : s) {
			r += c;
            r *= 17;
		}
        return r;
    }

    int64_t solve() {
        Input i;//("example.txt");

        int64_t ans = 0u;

        for (auto line : i.lines()) {
            while (!line.empty()) {
                auto term = split(line, ',');
                ans += H(term);
            }
        }

        return ans;
    }
};

//-----------------------------------------------------------------------------
int main() {
    try {
        Stopwatch sw;

        Solver<false> p1;
        sw.start();
        auto r1 = p1.solve();
        sw.stop_print();
        println("part1: ", r1);

        Solver<true> p2;
        sw.start();
        auto r2 = p2.solve();
        sw.stop_print();
        println("part2: ", r2);
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
