#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    uint32_t part1 = 0u;
    uint32_t part2 = 0u;

    std::bitset<100> winning_nums;
    std::vector<int> ncopies;
    for (string_view line : i.lines()) {
        winning_nums.reset();
        split(line, ' ');
        auto card_num = parse<int>(trim(split(line, ':')));
        if (ncopies.size() < card_num) {
            ncopies.resize(card_num);
        }
        auto card_index = card_num - 1;
        ncopies[card_index]++;

        auto winning_nums_str = split(line, '|');
        while (!winning_nums_str.empty()) {
            winning_nums_str = trim(winning_nums_str);
            winning_nums.set(parse<int>(trim(split(winning_nums_str, ' '))));
        }

        int score = 1;
        int n = 0;
        while (!line.empty()) {
            line = trim(line);
            auto num = parse<int>(trim(split(line, ' ')));
            //my_nums.push_back(num);
            if (winning_nums.test(num)) {
                score *= 2;
                ++n;
			}
        }
        auto copies = ncopies[card_index];
        if (ncopies.size() < card_num + n) {
            ncopies.resize(card_num + n);
        }
        for (int i = 0; i < n; ++i) {
            ncopies[card_index + i + 1] += copies;
        }
        score /= 2;
        part1 += score;
    }
    part2 = std::accumulate(ncopies.begin(), ncopies.end(), 0);
    // 3697057540 too high

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
