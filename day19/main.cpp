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
using Range = pair<int32_t, int32_t>;
struct PartRange
{
    Range v[4];

    int64_t combinations() const {
        int64_t n = 1;
        for (auto const& r : v) {
			n *= r.second - r.first;
		}
        return n;
	}
    auto fields() const {
		return tie(v[0], v[1], v[2], v[3]);
	}
    bool operator<(PartRange const& rhs) const {
		return fields() < rhs.fields();
	}
    bool operator==(PartRange const& rhs) const {
        return fields() == rhs.fields();
    }
    static bool overlaps(Range const& lhs, Range const& rhs) {
        return lhs.first < rhs.second && rhs.first < lhs.second;
    }
    static bool empty(Range const& r) {
        return r.first >= r.second;
    }
    bool overlaps(PartRange const& rhs) const {
        auto [x, m, a, s] = fields();
        auto [rx, rm, ra, rs] = rhs.fields();
        return overlaps(x, rx) && overlaps(m, rm) && overlaps(a, ra) && overlaps(s, rs);
	}
    bool empty() const {
        for (auto& r : v) {
            if (empty(r)) {
                return true;
            }
        }
        return false;
	}
};

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;

    unordered_map<string_view, string_view> cmds;
    vector<Part> parts;
    vector<PartRange> unique_ranges;
    deque<PartRange> insert_queue;

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
    static Range& field(PartRange& p, char ch) {
        switch (ch) {
        case 'x': return p.v[0];
        case 'm': return p.v[1];
        case 'a': return p.v[2];
        case 's': return p.v[3];
        default:
            throw runtime_error("invalid field");
        }
    }

    struct Cmd {
        int32_t v = 0;
        char op = 0;
        char field = 0;
        string_view label;
    };
    Cmd parse_term(string_view& cmd) {
        Cmd c;
		auto term = split(cmd, ',');
		auto op = term.find_first_of("<>");
		if (op != term.npos) {
			c.op = term[op];
			auto lhs = term.substr(0, op);
			term = term.substr(op + 1);
			auto rhs = split(term, ':');
            c.label = term;
			c.v = parse<int32_t>(rhs);
			assert(lhs.size() == 1);
			c.field = lhs[0];
		}
		else {
			// Could either be a termination ('A' or 'R') or a command label.
			c.label = term;
		}
		return c;
    }
    string_view interpret(Part const& p, string_view cmd) {
        auto c = parse_term(cmd);
        if (c.op) {
            auto const f = field(p, c.field);
            if (c.op == '<') {
                return f < c.v ? c.label : interpret(p, cmd);
            }
            else if (c.op == '>') {
                return f > c.v ? c.label : interpret(p, cmd);
            }
            else {
                throw runtime_error("invalid op");
            }
        }
        return c.label;
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
    void process(PartRange const& p, string_view cmd) {
        auto c = parse_term(cmd);
        if (c.op) {
            PartRange p1 = p, p2 = p;
            auto& f1 = field(p1, c.field);
            auto& f2 = field(p2, c.field);

            if (c.op == '<') {
                f1.second = min(f1.second, c.v);
                f2.first = max(f2.first, c.v);

            }
            else if (c.op == '>') {
                f1.first = min(f1.first, c.v);
                f2.second = max(f2.second, c.v);
            }
            else {
                throw runtime_error("invalid op");
            }
            if (f1.second > f1.first) {
                process(p1, c.label);
            }
            if (f2.second > f2.first) {
				process(p2, cmd);
			}
        }
        if (c.label == "A") {
            insert_range(p);
        }
        else if (c.label != "R") {
			process(p, cmds[c.label]);
		}
        // else rejected.
    }

    void insert_range(PartRange const& _p) 
    {
        insert_queue.clear();
        insert_queue.push_back(_p);

        while (!insert_queue.empty()) {
            auto p = insert_queue.front();
            insert_queue.pop_front();
            auto n = unique_ranges.size();
            bool ok = true;
            for (auto i = 0; i < n; ++i) {
                auto& r = unique_ranges[i];
                if (r == p) {
					ok = false;
					break;
				}
                if (r.overlaps(p)) {
                    ok = false;
                    for (auto iv = 0; iv < 4; ++iv) {
                        auto& rf = r.v[iv];
                        auto& pf = p.v[iv];
                        if ((rf != pf) && r.overlaps(rf, pf)) {
                            Range before(min(rf.first, pf.first), max(rf.first, pf.first));
                            Range after(min(rf.second, pf.second), max(rf.second, pf.second));
                            Range middle(max(rf.first, pf.first), min(rf.second, pf.second));
                            auto enqueue = [&](Range const& x) {
								if (x.first < x.second) {
                                    if (PartRange::overlaps(rf, x)) {
                                        auto a = r;
                                        a.v[iv] = x;
                                        insert_queue.push_back(a);
                                    }
                                    if (PartRange::overlaps(pf, x)) {
                                        auto a = p;
                                        a.v[iv] = x;
                                        insert_queue.push_back(a);
                                    }
								}
							};
                            enqueue(before);
                            enqueue(middle);
                            enqueue(after);
                            break;
                        }
                    }
                    unique_ranges.erase(unique_ranges.begin() + i);
                    break;
                }
            }
            if (ok) {
                unique_ranges.push_back(p);
            }
        }
	}

    int64_t solve() {
        Input i("example.txt");

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
        if (PART1) {
            for (auto const& part : parts) {
                auto r = process(part);
                if (r[0] == 'A') {
                    ans += part.rating();
                }
            }
        }
        else {
            auto r = Range{ 1, 4001 };
            PartRange p{ r, r, r, r };
            process(p, cmds["in"]);

            // distinct combinations in accepted_ranges
            for (auto&& c : unique_ranges) {
                ans += c.combinations();
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
        println("part1: ", r1); // 325952

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
