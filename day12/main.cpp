#include "aoc.h"

//-----------------------------------------------------------------------------
struct State
{

    char ch = '?';
    uint8_t i = 0u;
    uint8_t wi = 0u;
    int8_t w = -1;

    auto value() const {
        return (uint32_t((uint8_t)w) << 24) | (uint32_t(wi) << 16) | (uint32_t(i) << 8) | (uint32_t(ch));
    }
    bool operator==(State const& rhs) const {
		return value() == rhs.value();
	}
    bool operator!=(State const& rhs) const {
		return value() != rhs.value();
	}
	bool operator<(State const& rhs) const {
		return value() < rhs.value();
	}
};
namespace std {
	template<> struct hash<State> {
		size_t operator()(State const& s) const {
			return s.value();
		}
	};
}

struct X {
    string_view s;
    vector<uint8_t> W;
    string dbg;
    unordered_map<State, uint64_t> S;

    uint64_t f() {
        S.clear();
		return f(0, 0, -1);
	}
    uint64_t fc(char ch, uint8_t i, uint8_t wi, int8_t w) {
        auto st = State{ ch, i, wi, w };
        auto it = S.find(st);
        if (it != S.end()) {
			return it->second;
		}
        auto n = fc_i(ch, i, wi, w);
        S[st] = n;
        return n;
    }
    uint64_t fc_i(char ch, uint8_t i, uint8_t wi, int8_t w) {
        dbg[i] = ch;
        //print_dbg(i);
        if (ch == '#') {
            // is # allowed here?
            if (w == 0) {
                // no - must be '.' or end of string
				return 0u;
			}
            if (w > 0) {
                // existing group
				return f(i + 1, wi, w - 1);
			}
            // new group
            if (wi == W.size()) {
                // no more widths
                return 0u;
            }
            return f(i + 1, wi + 1, W[wi] - 1);
        }
        else if (ch == '.') {
            // is . allowed here?
            if (w > 0) {
                // no
                return 0u;
            }
            return f(i + 1, wi, -1);
        }
        else {
            assert(false);
        }
        return 0;
    }
    uint64_t f(uint8_t i, uint8_t wi, int8_t w) {
        if (i == s.size()) {
            if (w < 1) {
                if (wi == W.size()) {
                    return 1u;
                }
            }
            return 0u;
		}
		uint64_t n = 0u;
        auto ch = s[i];
		if (ch == '?') {
            n += fc('#', i, wi, w);
            n += fc('.', i, wi, w);
		}
        else {
            n += fc(ch, i, wi, w);
        }
		return n;
    }
    void print_dbg(uint8_t i) {
        println(string_view(dbg.c_str(), i + 1));
    }
};

//-----------------------------------------------------------------------------
void Solve(bool part2) {
    Input i; // ("example.txt");

    uint64_t ans = 0u;
    
    vector<State> S;
    string s;

    X x;

    for (Scanner line : i.lines()) {
        auto s1 = line.str(' ');
        auto _widths = line.vec<uint8_t>(',');
                
        s.clear();
        vector<uint8_t> W;

        uint8_t copies = (part2) ? 5 : 1; // 5 for part 2

        for (auto i : integers(copies)) {
            if (s.empty()) {
                s.assign(s1.begin(), s1.end());
            }
            else {
                s += '?';
                s.append(s1.begin(), s1.end());
            }
            W.insert(W.end(), _widths.begin(), _widths.end());
        }
        println(s);
        
        x.s = s;
        x.dbg = s;
        x.W = W;
        auto n = x.f();
        ans += n;
        println(n);
    }

    // part1: 7007
    println((part2)?"part2: ": "part1: ", ans);
}
//-----------------------------------------------------------------------------
int main() {
    try {
        Solve(false); // 7007
        Solve(true); // 3476169006222
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
