#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    uint32_t part1 = 0u;
    uint32_t part2 = 0u;

    for (string_view line : i.lines()) {
        split(line, ' ');
    }

    println("part1: ", part1);
    println("part2: ", part2);
}

//-----------------------------------------------------------------------------
int main() {
    try {
        Solve();    
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
