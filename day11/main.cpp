#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;
    //Input i("example.txt");


    auto lines = i.lines_vec();
    unordered_set<Point64> galaxies;
    unordered_map<int64_t, int64_t> cols; // initial => expanded
    unordered_map<int64_t, int64_t> rows;
    unordered_map<int64_t, int64_t> cols_factor; // expansion factor
    unordered_map<int64_t, int64_t> rows_factor;
    set<pair<Point64,Point64>> galaxy_pairs;

    int64_t exp_y = 0;
    for (auto row : integers(lines.size())) {
        auto&& line = lines[row];
        bool used_row = false;
        for (auto col : integers(line.size())) {
            if (line.data()[col] == '#') {
                auto p = Point64(col, row);
				galaxies.insert(p);
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

    // Find the unique pairs
    for (auto const& g : galaxies) {
        for (auto const& g2 : galaxies) {
            if (g != g2) {
                if (!galaxy_pairs.contains({ g2, g })) {
                    galaxy_pairs.insert({ g, g2 });
                }
            }
		}
    }
    auto expanded = [&](Point64 p, int64_t factor) {
		return Point64(p.x + cols_factor[p.x] * factor, p.y + rows_factor[p.y] * factor);
	};

    

    int64_t part1 = 0u;
    int64_t part2 = 0u;
    for (auto& [g, g2] : galaxy_pairs) {
        auto e = expanded(g, 1);
        auto e2 = expanded(g2, 1);
        part1 += (e2 - e).manhattan();
        e = expanded(g, 999999); // expand by 1 million.  It is already expanded by 1 so we only need 999999 more.
        e2 = expanded(g2, 999999);
        //e = expanded(g, 99);
        //e2 = expanded(g2, 99);
        part2 += (e2 - e).manhattan();
    }


    println("part1: ", part1); // 9445168
    println("part2: ", part2); // 742306702870 too high.  742305960572
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
