#include "aoc.h"

struct Part {
    int32_t x, m, a, s;

    int32_t rating() const {
		return x + m + a + s;
	}

    friend ostream& operator<<(ostream& os, const Part& p) {
		return os << p.x << ',' << p.m << ',' << p.a << ',' << p.s;
	}
};

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;

    unordered_map<string_view, string_view> cmds;
    vector<Part> parts;

    static int32_t field(Part const& p, char ch) {
		switch (ch) {
		case 'x': return p.x;
		case 'm': return p.m;
		case 'a': return p.a;
		case 's': return p.s;
        default:
            assert(false);
            break;
		}
		return 0;
	}

    string_view interpret(Part const& p, string_view cmd) {
        auto term = split(cmd, ',');
        auto op = term.find_first_of("<>");
        if (op != term.npos) {
            auto opch = term[op];
            auto lhs = term.substr(0, op);
            term = term.substr(op + 1);
            auto rhs = split(term, ':');
            auto v = parse<int32_t>(rhs);
            assert(lhs.size() == 1);
            auto lhsch = lhs[0];
            auto const f = field(p, lhsch);
            if (opch == '<') {
				return f < v ? term : interpret(p, cmd);
			}
			else if (opch == '>') {
				return f > v ? term : interpret(p, cmd);
			}
			else {
				assert(false);
			}
        }
        else {
            // Could either be a termination ('A' or 'R') or a command label.
            return term;
        }
    }
    string_view process(Part const& p) {
		auto cmd = cmds["in"];
		while (true) {
			auto term = interpret(p, cmd);
			if (term == "A") {
				return term;
			}
			else if (term == "R") {
				return term;
			}
			else {
				cmd = cmds[term];
			}
		}
	}

    int64_t solve() {
        Input i;//("example.txt");

        bool is_cmds = true;
        for (auto line : i.lines()) {
            if (line.empty()) {
				is_cmds = false;
				continue;
			}
			if (is_cmds) {
				auto key = split(line, '{');
                cmds[key] = split(line, '}');
			}
            else {
                Part p;
                split(line, '{');
                for (auto* f : { &p.x, &p.m, &p.a, &p.s }) {
                    split(line, '=');
                    *f = parse<int32_t>(split(line, ','));
                }
                parts.push_back(p);
			}
        }
        int64_t ans = 0u;
        for (auto const& part : parts) {
            auto r = process(part);
            if (r[0] == 'A') {
                ans += part.rating();
            }
        }

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
        println("part1: ", r1);

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
