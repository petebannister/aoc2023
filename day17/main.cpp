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

    uint8_t G(Point const& p) const {
		return grid[p.y * C + p.x];
	}
    bool isValidPos(Point const& p) const {
		return (p.x >= 0) && (p.y >= 0) && (p.x < C) && (p.y < R);
	}

    int64_t solve() {
        Input i;// ("example.txt");

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
        std::deque<pair<Move, uint32_t>> queue;


        costs.clear();

        // Start at the target as we will always know the cost to get there
        auto const start = Point(0, 0);
        auto const target = Point(C - 1, R - 1);
        queue.push_back({ Move{ start, Dir(1, 0), 0u }, 0u });
        costs[queue.back().first] = 0u;
        queue.push_back({ Move{ start, Dir(0, 1), 0u }, 0u });
        costs[queue.back().first] = 0u;

        Cost min_cost = numeric_limits<Cost>::max();

        while (!queue.empty()) {
            auto x = queue.back();
            queue.pop_back();
            auto m = x.first;
            m.pos += m.dir;
            if (isValidPos(m.pos)) {
                auto const prior_cost = x.second;
                auto cell = G(m.pos);
                auto cost = prior_cost + cell;
                if (min_cost > cost) {
                    if ((cost + (target - m.pos).manhattan()) < min_cost) {
                        auto ires = costs.insert({ m, cost });
                        if (ires.second || (cost < ires.first->second)) {
                            if (!ires.second) {
                                ires.first->second = cost;
                            }

                            bool end = (m.pos == target);
                            if constexpr (PART2) {
                                end = end && (m.n >= 4);
                            }

                            if (end) {
                                min_cost = cost;
                                if constexpr (PART2) {
                                    println(min_cost);
                                }
                            }
                            else {
                                ++m.n;
                                if constexpr (PART1) {
                                    if (m.n == 3u) {
                                        queue.push_back({ Move{ m.pos, m.dir.ccw90(), 0u }, cost });
                                        queue.push_back({ Move{ m.pos, m.dir.cw90(), 0u }, cost });
                                    }
                                    else {
                                        queue.push_back({ m, cost });
                                        queue.push_back({ Move{ m.pos, m.dir.ccw90(), 0u }, cost });
                                        queue.push_back({ Move{ m.pos, m.dir.cw90(), 0u }, cost });
                                    }
                                }
                                else { // PART2
                                    if (m.n < 4u) {
                                        queue.push_back({ m, cost });
                                    }
                                    else if (m.n == 10u) {
                                        queue.push_back({ Move{ m.pos, m.dir.ccw90(), 0u }, cost });
                                        queue.push_back({ Move{ m.pos, m.dir.cw90(), 0u }, cost });
                                    }
                                    else {
                                        queue.push_back({ m, cost });
                                        queue.push_back({ Move{ m.pos, m.dir.ccw90(), 0u }, cost });
                                        queue.push_back({ Move{ m.pos, m.dir.cw90(), 0u }, cost });
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        ans = min_cost;

        return ans;
    }
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
        println("part2: ", r2);

        println("part2: ", r2);
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
