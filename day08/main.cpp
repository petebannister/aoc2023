#include "aoc.h"

using Memo = pair<string_view, uint64_t>;
template <>
struct hash<Memo> {
    size_t operator()(Memo const& p) const {
        return hash<string_view>()(p.first) ^ hash<uint64_t>()(p.second);
    }
};

//-----------------------------------------------------------------------------
void Solve() {
    //Input i("example.txt");
    Input i;

    uint64_t part1 = 0u;

    auto directions = i.lines().front();
    unordered_map<string_view, pair<string_view, string_view>> routes;

    for (Scanner scan : views::drop(i.lines(), 2)) {
        auto key = scan.str();
        scan.expect('=');
        scan.expect('(');
        auto left = scan.str(',');
        auto right = scan.str(')');
        routes[key] = {left, right};
    }

    if (routes.end() != routes.find("AAA")) {
        // Don't run this for example data for part 2.
        string_view loc = "AAA";
        while (loc != "ZZZ") {
            auto [left, right] = routes[loc];
            for (auto dir : directions) {
                auto [left, right] = routes[loc];
                loc = (dir == 'L') ? left : right;
                ++part1;
                if (loc == "ZZZ") {
                    break;
                }
            }
        }
        println("part1: ", part1); // 18673
    }

    // Part 2
    std::vector<Memo> positions;
    std::vector<uint64_t> steps;
    for (auto& [k, v] : routes) {
        if (k.ends_with('A')) {
			positions.push_back(Memo(k, 0u));
		}
	}
    steps.resize(positions.size(), 0u);

    // Given a position and a start point in the directions list, 
    // memoize #steps it takes to get to the next possible end point
    unordered_map<Memo, Memo> memo;
    auto distance = [&](uint64_t start_dir_index, string_view start_loc) {
        uint64_t n = 0u;
        auto key = Memo(start_loc, start_dir_index);
        bool done = false;
        auto dir_index = start_dir_index;
        auto loc = start_loc;
        while (!done) {
            auto [left, right] = routes[loc];

            for (; dir_index < directions.size(); ++dir_index) {
                auto existing = memo.find(Memo(loc, dir_index));
                if (existing != memo.end()) {
					auto [new_loc, new_n] = existing->second;
					n += new_n;
					loc = new_loc;
                    done = true;
					break;
				}
                auto dir = directions[dir_index];
                auto [left, right] = routes[loc];
                loc = (dir == 'L') ? left : right;
                ++n;
                if (loc.ends_with('Z')) {
                    done = true;
                    break;
                }
            }
            dir_index = 0u;
        }
        auto r = Memo(loc, n);
        memo[key] = r;
        return r;
    };

    // It looks like the actual data is different to the example data
    // all the route lengths are exact multiples of the directions list size.
    // And the directions list length is prime..

    auto done = false;
    size_t max_steps = 1u;
    bool all_loop = false;
    while (!done) {
        done = true;
        all_loop = true;
        size_t index = 0u;
        for (auto& pos : positions) {
            while (steps[index] < max_steps) {
                auto [loc, n] = distance(pos.second, pos.first);
                bool loop = (loc == pos.first);
                if (!loop) {
                    all_loop = false;
                }
                pos = Memo(loc, (pos.second + n) % directions.size());
                steps[index] += n;
                if (steps[index] > max_steps) {
                    max_steps = steps[index];
                    done = false;
                }
                else if (steps[index] < max_steps) {
					done = false;
				}
            }
            ++index;
        }
        if (all_loop) {
            break;
        }
    }

    uint64_t part2 = max_steps;

    if (!done && all_loop) {
        // All the items are stuck in loops, so we can just calculate the
        // number of steps to get to the end based upon the fact they all 
        // repeat.
        // O == Offset
        // P == Period
        // N == Period count
        // R = On + (Pn * Nn)
        // 
        // Solve terms in pairs.
        // O1 + P1 * N1 = O2 + P2 * N2
        //
        while (positions.size() > 1u) {
            auto a = positions.back();
            positions.pop_back();
            auto b = positions.back();

            uint64_t P1 = distance(a.second, a.first).second;
            uint64_t P2 = distance(b.second, b.first).second;
            uint64_t O1 = steps[positions.size()];
            uint64_t O2 = steps[positions.size() - 1];
            uint64_t N1 = 0u;
            uint64_t N2 = 0u;
            while (O1 + (P1 * N1) != O2 + (P2 * N2)) {
                while (O1 + (P1 * N1) < O2 + (P2 * N2)) {
                    ++N1;
                }
                while (O1 + (P1 * N1) > O2 + (P2 * N2)) {
                    ++N2;
                }
            }
            auto P = lcm(P1, P2);
            memo[b] = Memo(b.first, P);
            auto count = O2 + (P2 * N2);
            steps[positions.size() - 1] = count;

        }
        part2 = steps[0];
    }

    // 4393814975397 too low.
    // 17972669116327
    
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
