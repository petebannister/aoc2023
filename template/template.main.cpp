#include "aoc.h"

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    int64_t solve(bool part2) {
        Input i;

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
        Solver<false> p1;
        Solver<false> p2;
        println("part1: ", p1.solve(false));
        println("part2: ", p2.solve(false));
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
