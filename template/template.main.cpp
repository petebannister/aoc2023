#include "aoc.h"

//-----------------------------------------------------------------------------
struct Solver
{
    void solve(bool part2) {
        Input i;

        uint32_t ans = 0u;

        for (string_view line : i.lines()) {
            split(line, ' ');
        }

        println((part2) ? "part2: " : "part1: ", ans);
    }
};

//-----------------------------------------------------------------------------
int main() {
    try {
        Solver s;
        s.solve(false);
        s.solve(true);
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
