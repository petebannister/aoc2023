#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    uint32_t part1 = 0u;
    uint32_t part2 = 0u;

    unordered_map<Point, char> symbols;
    unordered_map<Point, size_t> part_number_indexes;
    std::vector<size_t> part_number_list;
    int y = 0u;
    for (string_view line : i.lines()) {
        int x = 0;
        int part_number_x = 0;
        int part_number = 0;
        uint8_t part_number_len = 0u;
        auto put_partnumber = [&]() {
            if (part_number_len == 0u) {
				return;
			}
            auto index = part_number_list.size();
            part_number_list.push_back(part_number);
            while (part_number_len--) {
                part_number_indexes[Point(part_number_x, y)] = index;
                ++part_number_x;
			}
            part_number_len = 0u;
		    part_number = 0u;
		};
        for (auto c : line) {
            if (isdigit(c)) {
                if (part_number_len == 0u) {
					part_number_x = x;
				}
                part_number *= 10;
                part_number += c - '0';
                ++part_number_len;
			}
            else {
                put_partnumber();
                if (c != '.') {
                    symbols[Point(x, y)] = c;
                }
            }
            ++x;
		}
        put_partnumber();
        ++y;
    }
    for (auto [symbol_pt, symbol] : symbols) {
        Point dirs[] = { {-1,-1}, {0,-1}, {1,-1}, {-1,0}, {1,0}, {-1,1}, {0,1}, {1,1} };
        std::unordered_set<size_t> visited_indices;
        for (auto dir : dirs) {
            auto p = symbol_pt + dir;
            auto pit = part_number_indexes.find(p);
            if (pit != part_number_indexes.end()) {
                auto index = pit->second;
                if (!visited_indices.contains(index)) {
                    visited_indices.insert(index);
                    part1 += part_number_list[index];
                }
            }
        }

        if (symbol == '*') {
            if (visited_indices.size() == 2u) {
                int ratio = 1;
                for (auto index : visited_indices) {
					ratio *= part_number_list[index];
				}
                part2 += ratio;
            }
        }
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
