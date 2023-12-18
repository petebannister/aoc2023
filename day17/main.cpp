#include "aoc.h"

#include <queue>

using Dir = Point;
struct Move {
    Point pos;
    Dir dir;
    uint8_t n = 0u;
    bool operator==(const Move& o) const {
		return pos == o.pos && dir == o.dir && n == o.n;
	}
    bool operator!=(const Move& o) const {
        return !(*this == o);
    }
};
namespace std {
    template <>
	struct hash<Move> {
		size_t operator()(const Move& d) const {
			return (d.n | ((d.dir.x + 1) << 6) | ((d.dir.y + 1) << 12)) ^ d.pos.hash();
		}
	};
}

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;
    using Cost = uint32_t;
    vector<uint8_t> grid;
    int32_t R, C;
    unordered_map<Move, Cost> costs;
    std::deque<Move> queue;
    Cost min_cost;

    uint8_t G(Point const& p) const {
		return grid[p.y * C + p.x];
	}
    bool isValidPos(Point const& p) const {
		return (p.x >= 0) && (p.y >= 0) && (p.x < C) && (p.y < R);
	}

    int64_t solve() {
        Input i; // ("example.txt");

        int64_t ans = 0u;
        grid.resize(i.size());

        R = 0u;
        for (auto line : i.lines()) {
            C = (int32_t) line.size();
            auto offset = R * C;
            size_t c = 0u;
            for (auto ch : line) {
                grid[offset + c] = (uint8_t)(ch - '0');
                ++c;
			}
            ++R;
        }


        costs.clear();
        min_cost = numeric_limits<Cost>::max();

        auto const start = Point(0, 0);
        auto const target = Point(C - 1, R - 1);
        queue.push_back(Move{ start, Dir(1, 0), 1u });
        costs[queue.back()] = 0u;
        queue.push_back(Move{ start, Dir(0, 1), 1u });
        costs[queue.back()] = 0u;

        while (!queue.empty()) {
            auto m = queue.front();
            queue.pop_front();
            iterate(m);
        }
        ans = min_cost;

        return ans;
    }

    Move forward(Move const& m) const {
		return Move{ m.pos + m.dir, m.dir, m.n + 1u };
	}
    Move left(Move const& m) const {
        auto d = m.dir.ccw90();
        return Move{ m.pos + d, d, 1u };
    }
    Move right(Move const& m) const {
        auto d = m.dir.cw90();
        return Move{ m.pos + d, d, 1u };
    }

    void iterate(Move const& m) {
        auto cost = costs[m];
        if constexpr (PART1) {
            if (m.n == 3u) {
                update(left(m), cost);
                update(right(m), cost);
            }
            else {
                update(forward(m), cost);
                update(left(m), cost);
                update(right(m), cost);
            }
        }
        else { // PART2
            if (m.n < 4u) {
                update(forward(m), cost);
            }
            else if (m.n < 10u) {
                update(forward(m), cost);
                update(left(m), cost);
                update(right(m), cost);
            }
            else {
                update(left(m), cost);
                update(right(m), cost);
            }
        }
    }

    void update(Move const& m, Cost entry_cost) {
        if (!isValidPos(m.pos)) {
            return;
		}
        auto const target = Point(C - 1, R - 1);

		auto cell = G(m.pos);
		auto new_cost = entry_cost + cell;
        if ((new_cost + (target - m.pos).manhattan()) > min_cost) {
            // This path is already too expensive
            return;
        }
        auto [existing, inserted] = costs.insert({ m, new_cost });
        if (!inserted) {
            if (existing->second <= new_cost) {
                return;
            }
            existing->second = new_cost;
        }
        bool end = (m.pos == target);
        if constexpr (PART2) {
            end = end && (m.n >= 4);
        }

		if (end) {
			min_cost = min(min_cost, new_cost);
            if constexpr (PART2) {
                println(min_cost);
                //printRoute(m);
                if (min_cost == 827) {
                    printRoute(m);
                }
            }
		}
        else {
            if (m.pos == target) {
                return;
            }
        
            // Need to re-process this node
            queue.push_back(m);
		}
	}
#if 1
    void printRoute(Move m) {
        std::vector<char> grid(R * C, '.');
        std::vector<Move> queue;
        std::unordered_set<Move> visited;
        queue.push_back(m);
        uint32_t verify = 0u;
        while (!queue.empty()) {
            if (m.pos == Point(0, 0)) {
                break;
            }
            m = queue.back();
            queue.pop_back();
            auto cost = costs[m];
            auto grid_val = G(m.pos);
            verify += grid_val;
            auto entry_cost = cost - grid_val;
            grid[m.pos.y * C + m.pos.x] = '0' + grid_val;
            auto a = m.pos - m.dir;
            if (m.n > 1) {
                if (isValidPos(a)) {
                    grid[a.y * C + a.x] = '#';
                    auto m2 = Move{ a, m.dir, m.n - 1u };
                    if (visited.insert(m2).second) {
                        queue.push_back(m2);
                    }
                }
            }
            else {
                auto b = m.dir.ccw90();
                auto c = m.dir.cw90();
                for (auto d : { b, c }) {
                    if (isValidPos(m.pos - m.dir)) {
                        for (uint8_t n = 4u; n < 11u; ++n) {
                            auto m2 = Move{ m.pos - m.dir, d, n };
                            auto i = costs.find(m2);
                            if (i != costs.end()) {
                                auto cost2 = i->second;
                                if (cost2 == entry_cost) {
                                    auto p = m2.pos;
                                    grid[p.y * C + p.x] = '#';
                                    if (visited.insert(m2).second) {
                                        queue.push_back(m2);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        for (auto y = 0u; y < R; ++y) {
			for (auto x = 0u; x < C; ++x) {
				print(grid[y * C + x]);
			}
			println();
		}        
		println(verify);
	}
#endif
};


//-----------------------------------------------------------------------------
int main() {
    try {
        Stopwatch sw;

        Solver<false> p1;
        sw.start();
        auto r1 = p1.solve();
        sw.stop_print();
        println("part1: ", r1); // 694

        Solver<true> p2;
        sw.start();
        auto r2 = p2.solve();
        sw.stop_print();
        println("part2: ", r2); // 827 too low, 897 too high. 828 wrong.
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
