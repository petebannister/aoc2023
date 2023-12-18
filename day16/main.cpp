#include "aoc.h"

using Dir = Point;
using V = pair<Point, Dir>;
namespace std {
	template <>
	struct hash<V> {
		size_t operator()(const V& v) const {
			return hash<Point>()(v.first) ^ hash<Dir>()(v.second);
		}
	};
}

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;

    size_t R, C;
    vector<string_view> grid;

    unordered_set<V> visited;
	unordered_set<Point> energised;
	vector<V> queue;

	void step(Point p, Dir d) {
        if (p.x < 0 || p.x >= C || p.y < 0 || p.y >= R) {
            return;
        }
        if (visited.contains(V{ p,d })) {
			return;
		}
        auto c = grid[p.y][p.x];
        visited.insert(V{ p,d });
		energised.insert(p);

        if (c == '\\') {
            if (d.x == -1) {
				d = Point{ 0,-1 };
			}
			else if (d.x == 1) {
				d = Point{ 0,1 };
			}
			else if (d.y == -1) {
				d = Point{ -1,0 };
			}
			else if (d.y == 1) {
				d = Point{ 1,0 };
			}
			queue.push_back({ p + d,d });
        }
        else if (c == '/') {
            if (d.x == -1) {
                d = Point{ 0,1 };
            }
            else if (d.x == 1) {
				d = Point{ 0,-1 };
			}
			else if (d.y == -1) {
				d = Point{ 1,0 };
			}
			else if (d.y == 1) {
				d = Point{ -1,0 };
			}
			queue.push_back({ p + d,d });
        }
        else if (c == '|') {
            if (d.x == 0) {
				queue.push_back({ p + d,d });
            }
            else {
				auto a = Point{ 0,1 };
				auto b = Point{ 0,-1 };
				queue.push_back({ p + a, a });
				queue.push_back({ p + b, b });
            }

        }
        else if (c == '-') {
            if (d.y == 0) {
				queue.push_back({ p + d,d });
            }
            else {
				auto a = Point{ 1,0 };
				auto b = Point{ -1,0 };
				queue.push_back({ p + a, a });
				queue.push_back({ p + b, b });
			}
		}
        else {
			queue.push_back({ p + d,d });
        }
	}
	void beam(Point p, Dir d) {
		queue.push_back({ p,d });
		while (!queue.empty()) {
			auto v = queue.back();
			queue.pop_back();
			step(v.first, v.second);
		}
	}
	void reset() {
		energised.clear();
		visited.clear();
	}
    int64_t solve() {
        Input i;//("example.txt");

        int64_t ans = 0u;
        grid = i.lines_vec();
        C = grid[0].size();
        R = grid.size();

		if (PART1) {
			auto p = Point{ 0,0 };
			Dir d = Point{ 1,0 };
			beam(p, d);
			ans = energised.size();
		}
		else {
			for (auto c : integers(C)) {
				auto p = Point{ (int)c,0 };
				Dir d = Point{ 0,1 };
				beam(p, d);
				int64_t n = energised.size();
				ans = max(ans, n);
				reset();

				p = Point{ (int)c,(int)R-1 };
				d = Point{ 0,-1 };
				beam(p, d);
				n = energised.size();
				ans = max(ans, n);
				reset();
			}
			for (auto r : integers(R)) {
				auto p = Point{ 0, (int)r };
				Dir d = Point{ 1,0 };
				beam(p, d);
				int64_t n = energised.size();
				ans = max(ans, n);
				reset();

				p = Point{ (int)C-1, (int)r };
				d = Point{ -1,0 };
				beam(p, d);
				n = energised.size();
				ans = max(ans, n);
				reset();
			}
		}

        // printGrid2();
        return ans;
    }

    void printGrid() {
        for (auto r : integers(R)) {
			for (auto c : integers(C)) {
				auto p = Point{ (int)c,(int)r };
				auto ch = grid[r][c];
				if (visited.contains(V{ p,Point{1,0} })) {
					printf(">");
				}
				else if (visited.contains(V{ p,Point{-1,0} })) {
					printf("<");
				}
				else if (visited.contains(V{ p,Point{0,1} })) {
					printf("v");
				}
				else if (visited.contains(V{ p,Point{0,-1} })) {
					printf("^");
				}
				else {
					printf("%c", ch);
				}
			}
			printf("\n");
		}
	}
	void printGrid2() {
		for (auto r : integers(R)) {
			for (auto c : integers(C)) {
				auto p = Point{ (int)c,(int)r };
				auto ch = grid[r][c];
				bool b = false;
				if (visited.contains(V{ p,Point{1,0} })) {
					b = true;
				}
				else if (visited.contains(V{ p,Point{-1,0} })) {
					b = true;
				}
				else if (visited.contains(V{ p,Point{0,1} })) {
					b = true;
				}
				else if (visited.contains(V{ p,Point{0,-1} })) {
					b = true;
				}
				if (b) {
					printf("#");
				}
				else {
					printf("%c", ch);
				}
			}
			printf("\n");
		}
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
        println("part1: ", r1); // 9353 too high, 6883

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
