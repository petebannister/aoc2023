#include "aoc.h"

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;
    int64_t solve() {
        Input i;//("example.txt");

        int64_t ans = 0u;
        auto grid = i.lines_vec();
        auto C = grid[0].size();
        auto R = grid.size();

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
