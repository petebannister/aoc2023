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
Point INVALID = { 0, 0 };

Point go(Point dir, char c) {
    if (dir == DIRS[U]) {
        if (c == '|') {
            return dir;
        }
        return (c == 'F') ? DIRS[R] : (c == '7') ? DIRS[L] : INVALID;
    }
    if (dir == DIRS[R]) {
        if (c == '-') {
            return dir;
        }
        return (c == '7') ? DIRS[D] : (c == 'J') ? DIRS[U] : INVALID;
    }
    if (dir == DIRS[D]) {
        if (c == '|') {
			return dir;
		}
		return (c == 'J') ? DIRS[L] : (c == 'L') ? DIRS[R] : INVALID;
	}
    if (dir == DIRS[L]) {
        if (c == '-') {
			return dir;
		}
		return (c == 'L') ? DIRS[U] : (c == 'F') ? DIRS[D] : INVALID;
	}
    return INVALID;
}

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    uint32_t part1 = 0u;
    uint32_t part2 = 0u;

    auto lines = i.lines();
    vector<string_view> maze(lines.begin(), lines.end());

    auto get = [&](Point p) {
        if (p.y < 0 || p.y >= (int)maze.size()) {
			return '.';
		}
		auto& row = maze[p.y];
        if (p.x < 0 || p.x >= (int)row.size()) {
			return '.';
		}
		return row[p.x];
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
        println(c);
        auto dd = go(d, c);
        if (dd != INVALID) {
            initial_dir = dd;
            break;
        }
    }
    assert(initial_dir != INVALID);

    auto d = initial_dir;
    auto p = S + initial_dir;
    uint64_t steps = 0u;
    while (p != S) {
        auto c = get(p);
        d = go(d, c);
        p += d;
        ++steps;
    }
    assert(steps % 2 != 0);
    part1 = (steps / 2) + 1;
    


    println("part1: ", part1);
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
