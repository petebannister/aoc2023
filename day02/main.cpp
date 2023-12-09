#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    uint32_t part1 = 0u;
    uint32_t part2 = 0u;

    map<string_view, int> limits = {
        {"red", 12},
		{"green", 13},
		{"blue", 14}
    };
    string_view colours[] = {"red", "green", "blue"};
    for (string_view line : i.lines()) {
        split(line, ' '); // drop "Game "
        auto game = parse<int>(split(line, ':'));
        bool possible = true;
        map<string_view, int> max_counts;
        while (!line.empty()) {
            auto draw = trim(split(line, ';'));
            map<string_view, int> counts;
            while (!draw.empty()) {
                auto colour = trim(split(draw, ','));
                auto count = parse<int>(trim(split(colour, ' ')));
                counts[colour] += count;
                max_counts[colour] = max(max_counts[colour], count);
            }
            for (auto colour : colours) {
                if (counts[colour] > limits[colour]) {
                    possible = false;
                }
            }
        }
        if (possible) {
            part1 += game;
        }
        int power = 1;
        for (auto c : colours) {
			power *= max_counts[c];
		}
        part2 += power;
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
