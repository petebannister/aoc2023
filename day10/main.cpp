#include "aoc.h"


constexpr Point const DIRS[] = {
	{ 0, -1 },
	{ 1, 0 },
	{ 0, 1 },
	{ -1, 0 },
};
constexpr uint8_t U = 0;
constexpr uint8_t R = 1;
constexpr uint8_t D = 2;
constexpr uint8_t L = 3;
constexpr Point INVALID = { 0, 0 };

//-----------------------------------------------------------------------------
constexpr int turn(Point dir, char c) {
    if (dir == DIRS[U]) {
        if (c == '|') {
            return 0;
        }
        return (c == 'F') ? 1 : -1;
    }
    if (dir == DIRS[R]) {
        if (c == '-') {
            return 0;
        }
        return (c == '7') ? 1 : -1;
    }
    if (dir == DIRS[D]) {
        if (c == '|') {
            return 0;
        }
        return (c == 'J') ? 1 : -1;
    }
    if (dir == DIRS[L]) {
        if (c == '-') {
            return 0;
        }
        return (c == 'L') ? 1 : -1;
    }
    return 0;
}
//-----------------------------------------------------------------------------
Point turn_dir(Point dir, int t) {
    if (t < 0) {
        return dir.ccw90();
    }
    if (t > 0) {
        return dir.cw90();
    }
    return dir;

}
//-----------------------------------------------------------------------------
Point go(Point dir, int c) {
    auto t = turn(dir, c);
    return turn_dir(dir, t);
}

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    auto lines = i.lines();
    vector<string_view> maze(lines.begin(), lines.end());
    unordered_set<Point> loop_points;

    // Get a symbol from the maze.
    auto get = [&](Point p) {
        if (p.y >= 0 || p.y < (int)maze.size()) {
		    auto& row = maze[p.y];
            if (p.x > 0 || p.x < (int)row.size()) {
                return row[p.x];
            }
		}
		return '.';
	};

    Point S;
    for (auto y : integers(maze.size())) {
        auto x = maze[y].find('S');
        if (x != string_view::npos) {
            S = { (int)x, (int)y };
			break;
		}    
    }
    auto initial_dir = INVALID;
    for (auto d : DIRS) {
        auto p = S + d;
        auto c = get(p);
        auto dd = go(d, c);
        if (dd != INVALID) {
            initial_dir = dd;
            break;
        }
    }
    assert(initial_dir != INVALID);

    auto d = initial_dir;
    auto p = S + initial_dir;
    uint32_t steps = 0u;
    int a = 0; // angle
    while (p != S) {
        auto c = get(p);
        auto t = turn(d, c);
        a += t;
        d = turn_dir(d, t);
        p += d;
        ++steps;
        loop_points.insert(p);
    }
    assert(steps % 2 != 0); // Should be odd for a unique solution.
    assert(a == 4 || a == -4); // Should be a closed loop in either CW or CCW direction.

    auto part1 = (steps / 2) + 1; // since steps is odd + 1.
    println("part1: ", part1); // 7086

    // Part 2

    // Get first set of points in the interior of the loop, to 
    // seed the fill algorithm.
    unordered_set<Point> interior;
    bool const ccw = a < 0; // Determine which side of the loop line to fill.
    d = initial_dir;
    p = S + initial_dir;
    while (p != S) {
        auto c = get(p);
        auto dd = (ccw) ? d.ccw90() : d.cw90();
        auto p_int = (p + dd);
        if (!loop_points.contains(p_int)) {
			interior.insert(p_int);
		}
        d = go(d, c);
        p += d;
    }
    // Fill the interior.  Put all known starting points into a queue.
    // For each point see if there is an adjacent point that we have not
    // identified as interior yet.  If so, add it to the interior provided
    // it is not part of the loop.  This point gets added to the queue so
    // that the whole interior gets filled.
    vector<Point> q(interior.begin(), interior.end());
    while (!q.empty()) {
		auto p = q.back();
		q.pop_back();
        for (auto d : DIRS) {
			auto p2 = p + d;
            if (!interior.contains(p2)) {
                if (!loop_points.contains(p2)) {
                    interior.insert(p2);
                    q.push_back(p2);
                }
			}
        }
    }
    auto part2 = interior.size();
    println("part2: ", part2); // 317
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
