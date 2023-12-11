#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    int64_t part1 = 0u;
    int64_t part2 = 0u;

    auto lines = i.lines_vec();
    unordered_map<Point, Point> galaxies;
    unordered_map<int64_t, int64_t> cols; // initial => expanded
    unordered_map<int64_t, int64_t> rows;
    unordered_map<int64_t, int64_t> cols_factor; // expansion factor
    unordered_map<int64_t, int64_t> rows_factor;
    set<pair<Point,Point>> galaxy_pairs;

    int64_t exp_y = 0;
    for (auto row : integers(lines.size())) {
        auto&& line = lines[row];
        bool used_row = false;
        for (auto col : integers(line.size())) {
            if (line.data()[col] == '#') {
                auto p = Point(col, row);
				galaxies[p] = p;
                cols[col] = col;
                rows[row] = row;
                used_row = true;
			}
        }
        if (!used_row) {
            ++exp_y;
        }
        rows_factor[row] = exp_y;
    }
    int64_t exp_x = 0;
    for (auto col : integers(lines[0].size())) {
        if (!cols.contains(col)) {
            ++exp_x;
        }
        cols_factor[col] = exp_x;
    }
    auto expand = [&] {
        for (auto& [p, e] : galaxies) {
			e.x += cols_factor[p.x];
			e.y += rows_factor[p.y];
		}
    };
    expand();
    // Find the unique pairs
    for (auto& [g, e] : galaxies) {
        for (auto& [g2, e2] : galaxies) {
            if (g != g2) {
                if (!galaxy_pairs.contains({ g2, g })) {
                    galaxy_pairs.insert({ g, g2 });
                    part1 += (e2 - e).manhattan();
                }
            }
		}
    }


    println("part1: ", part1); // 9445168
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
