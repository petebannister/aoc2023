#include "aoc.h"


constexpr Point INVALID = { 0, 0 };
constexpr int INVALID_TURN = 666;

//-----------------------------------------------------------------------------
constexpr int turn(Point dir, char c) 
{
    if ((c == '|') || (c == '-')) {
        return 0;
    }
    if (dir == Point::U()) {
        return (c == 'F') ? 1 : (c == '7') ? -1 : INVALID_TURN;
    }
    if (dir == Point::R()) {
        return (c == '7') ? 1 : (c == 'J') ? -1 : INVALID_TURN;
    }
    if (dir == Point::D()) {
        return (c == 'J') ? 1 : (c == 'L') ? -1 : INVALID_TURN;
    }
    if (dir == Point::L()) {
        return (c == 'L') ? 1 : (c == 'F') ? -1 : INVALID_TURN;
    }
    return INVALID_TURN;
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
    Input i; // ("ex2.txt");

    auto lines = i.lines();
    vector<string_view> maze(lines.begin(), lines.end());
    unordered_set<Point> loop_points;

    // Get a symbol from the maze.
    auto get = [&](Point p) {
        if (p.y >= 0 && p.y < (int)maze.size()) {
		    auto& row = maze[p.y];
            if (p.x >= 0 && p.x < (int)row.size()) {
                return row[p.x];
            }
		}
		return '.';
	};

    Point S; // Find the starting point.
    for (auto y : integers(maze.size())) {
        auto x = maze[y].find('S');
        if (x != string_view::npos) {
            S = { (int)x, (int)y };
			break;
		}    
    }

    // Find the loop.  This will account for possibility
    // of a starting path that is a dead end also.
    int a = 0; // angle
    uint32_t steps = 0u;
    auto initial_dir = INVALID;
    for (auto d : Point::Dirs()) {
        initial_dir = d;
        auto p = S + initial_dir;
        a = 0; // angle
        loop_points = { p };
        steps = 0u;
        while (p != S) {
            auto c = get(p);
            auto t = turn(d, c);
            if (t == INVALID_TURN) {
				break;
			}
            a += t;
            d = turn_dir(d, t);
            p += d;
            ++steps;
            loop_points.insert(p);
        }
        if (p == S) {
            break;
		}
    }
    auto part1 = (steps / 2) + (steps % 2); // if steps is odd + 1.
    println("part1: ", part1); // 7086

    // Part 2

    // Get first set of points in the interior of the loop, to 
    // seed the fill algorithm.
    unordered_set<Point> interior;
    bool const ccw = a < 0; // Determine which side of the loop line to fill.
    auto d = initial_dir;
    auto p = S + initial_dir;
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
        for (auto d : Point::Dirs()) {
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
