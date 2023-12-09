#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    uint32_t part1 = 0u;
    uint32_t part2 = 0u;

    auto iline = i.lines().begin();
    auto split_line = [](auto line) -> vector<uint32_t> {
        vector<uint32_t> r;
		split(line, ' ');
        line = trim(line);
        while (!line.empty()) {
            r.push_back(parse<uint32_t>(split(line, ' ')));
            line = trim(line);
        }
		return r;
	};
    auto times = split_line(*iline++);
    auto distances = split_line(*iline++);

    size_t index = 0u;
    part1 = 1;
    for (auto time : times) {
		auto distance_record = distances[index++];
        uint32_t winning_options = 0;
        for (uint32_t t = 0u; t < time; ++t) {
            auto speed = t;
            auto remain = time - t;
            auto distance = speed * remain;
            if (distance > distance_record) {
                ++winning_options;
            }
		}
        part1 *= winning_options;
	}

    println("part1: ", part1);

    // part2

    iline = i.lines().begin();
    auto parse_line_part2 = [](auto line) -> uint64_t {
        split(line, ' ');
        line = trim(line);
        std::string numstr;
        while (!line.empty()) {
            numstr += split(line, ' ');
            line = trim(line);
        }
        return parse<uint64_t>(numstr.c_str());
    };
    auto time = parse_line_part2(*iline++);
    auto distance_record = parse_line_part2(*iline++);
    for (uint32_t t = 0u; t < time; ++t) {
        auto speed = t;
        auto remain = time - t;
        auto distance = speed * remain;
        if (distance > distance_record) {
            ++part2;
        }
    }

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
