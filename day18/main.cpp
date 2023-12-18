#include "aoc.h"

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    unordered_map<Point, pair<Point,uint32_t> > outline;
    unordered_set<Point> fill;
    vector<Point> pending;
    int32_t max_c = 0;
    int32_t max_r = 0;

    static constexpr bool PART1 = !PART2;
    int64_t solve() {
        Input i;//("example.txt");

        std::unordered_map<char, Point> dirs;
        dirs['U'] = { 0, -1 };
        dirs['D'] = { 0, 1 };
        dirs['L'] = { -1, 0 };
        dirs['R'] = { 1, 0 };


        int64_t ans = 0u;

        int angle = 0;
        Point pos;
        Point current_dir = Point::U();
        for (auto line : i.lines()) {
            auto d = dirs[split(line).front()];
            auto n = parse<int32_t>(split(line));
            line = line.substr(2, line.size() - 3);
            uint32_t colour = parseHex<uint32_t>(line);

            if (outline.empty()) {
				pos = { 0, 0 };
				current_dir = d;
                outline.insert({ pos, {d, colour} });
			}
            else {
                if (d == current_dir.cw90()) {
                    ++angle;
                }
                else if (d == current_dir.ccw90()) {
                    --angle;
                }
            }
            for (int i = 0; i < n; ++i) {
				pos += d;
                max_r = max(max_r, pos.y);
                max_c = max(max_c, pos.x);
                outline.insert({ pos, {d, colour} });
			}
        }
        for (auto [p, v] : outline) {
            fill.insert(p);
            if (angle > 0) {
				pending.push_back(p + v.first.cw90());
			}
            else {
                pending.push_back(p + v.first.ccw90());
            }
		}
        while (!pending.empty()) {
            auto p = pending.back();
			pending.pop_back();

            if (fill.insert(p).second) {
				for (auto d : Point::Dirs()) {
					pending.push_back(p + d);
				}
			}
        }

        ans = fill.size();

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
        println("part1: ", r1); // 52231

        Solver<true> p2;
        sw.start();
        auto r2 = p2.solve();
        sw.stop_print();
        println("part2: ", r2);
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
