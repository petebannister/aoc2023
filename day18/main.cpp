#include "aoc.h"

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    using Edge = pair<Point, Point>;
    vector<Edge> outline;
    vector<int32_t> scanlines;
    vector<Edge> active_edges; // Active edges
    size_t outline_index = 0;

    int32_t min_x = 0;
    int32_t min_y = 0;
    int32_t max_x = 0;
    int32_t max_y = 0;

    static constexpr bool PART1 = !PART2;
    int64_t solve() {
        Input i; // ("example.txt");

        std::unordered_map<char, Point> dirs;
        dirs['U'] = { 0, -1 };
        dirs['D'] = { 0, 1 };
        dirs['L'] = { -1, 0 };
        dirs['R'] = { 1, 0 };

        Point pos;
        Point current_dir = Point::U();
        scanlines.push_back(-1);
        scanlines.push_back(0);
        scanlines.push_back(1);

        for (auto line : i.lines()) {
            auto d = dirs[split(line).front()];
            auto n = parse<int32_t>(split(line));
            line = line.substr(2, line.size() - 3);
            uint32_t colour = parseHex<uint32_t>(line);
            if (PART2) {
                switch (colour & 0xF) {
                case 0: d = Point::R(); break;
                case 1: d = Point::D(); break;
                case 2: d = Point::L(); break;
                case 3: d = Point::U(); break;
                default:
                    throw runtime_error("invalid direction code");
                }
                n = (colour >> 4);
            }

            auto a = pos;
            auto b = pos + Point(d.x * n, d.y * n);
            pos = b;
            outline.push_back({ a, b });

            // We are interested in processing scanlines where there are changes in the outline.
            scanlines.push_back(pos.y);
            scanlines.push_back(pos.y+1);

            min_x = min(min_x, pos.x);
            min_y = min(min_y, pos.y);
            max_x = max(max_x, pos.x);
            max_y = max(max_y, pos.y);        
        }

        // Sort the points in the outline for feeding geometry
        // into the scanline algorithm.
        std::sort(outline.begin(), outline.end(), [](auto&& a, auto&& b) {
            auto ay = min(a.first.y, a.second.y);
            auto by = min(b.first.y, b.second.y);
            // X doesn't matter for sorting at this stage.
            return ay < by;
		});

        sort(scanlines.begin(), scanlines.end());
        scanlines.erase(unique(scanlines.begin(), scanlines.end()), scanlines.end());

        int64_t ans = 0u;
        for (auto iy : integers(scanlines.size() - 1)) {
            bool fill = false;
            auto y1 = scanlines[iy];
            auto y2 = scanlines[iy + 1];
            int64_t h = (y2 - y1);
            int64_t row_area = 0;

            updateActiveEdges(y1);

            int32_t x1 = 0;
            int32_t x2 = 0;
            bool up = false;
            bool horz = false;
            for (auto& edge : active_edges) {
				auto [a, b] = edge;
				if (isHorizontal(edge)) {
                    row_area += abs(b.x - a.x) - 1;
                    x1 = max(a.x, b.x);
                    horz = true;
				}
                else {
                    row_area += h;
                    auto this_up = (a.y < b.y);
                    if (horz) {
                        // just processed a horizontal edge
                        if (up != this_up) {
							fill = !fill;
						}
                    }
                    else {
                        fill = !fill;
                    }
                    up = this_up;
                    horz = false;
                    if (fill) {
                        x1 = a.x;
                    }
                    else {
                        x2 = a.x;
                        int64_t w = abs(x2 - x1) - 1;
                        if (w > 0) {
                            row_area += h * w;
                        }
                    }
                }
            }
            ans += row_area;
		}

        return ans;
    }

    void updateActiveEdges(int32_t y) {
        // Remove edges that are no longer needed.
        active_edges.erase(std::remove_if(active_edges.begin(), active_edges.end(), [y](auto&& e) {
            return ((e.first.y < y) && (e.second.y < y));
        }), active_edges.end());
        // Add any new edges that start/end at this scanline.
        // Any egdes in the outline before outline_index have been added already.
        for (; outline_index < outline.size(); ++outline_index) {
            auto [a, b] = outline[outline_index];
            if ((a.y == y) || (b.y == y)) {
                active_edges.push_back({ a, b });
            }
            else {
                break;
            }
        }
        // Sort the active edges by x
        std::sort(active_edges.begin(), active_edges.end(), [](auto&& a, auto&& b) {
            auto ax = min(a.first.x, a.second.x);
            auto bx = min(b.first.x, b.second.x);

            if (ax == bx) {
                return (isHorizontal(b));
            }
            return ax < bx;
        });
    }
    static bool isHorizontal(Edge const& e) {
        return (e.first.y == e.second.y);
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
