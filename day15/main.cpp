#include "aoc.h"

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;

    uint8_t H(string_view s) {
        uint8_t r = 0;
        for (auto c : s) {
			r += c;
            r *= 17;
		}
        return r;
    }

    using B = list<pair<string_view,int>>;
    B T[256];

    int64_t solve() {
        Input i;//("example.txt");

        int64_t ans = 0u;

        for (auto line : i.lines()) {
            while (!line.empty()) {
                auto term = split(line, ',');
                if (PART1) {
                    ans += H(term);
                }
                else {
                    auto cpos = term.find('=');
                    if (cpos == term.npos) {
                        cpos = term.find('-');
                        if (cpos == term.npos) {
                            assert(false);
						}
                    }
                    auto label = term.substr(0, cpos);
                    auto cmd = term[cpos];

                    auto h = H(label);

                    if (cmd == '-') {
                        T[h].remove_if([=](auto&& v) { return v.first == label; });
                    }
                    else if (cmd == '=') {
                        auto v = parse<int>(term.substr(cpos + 1));
                        auto i = std::find_if(begin(T[h]), end(T[h]), [=](auto&& v) { return v.first == label; });
                        if (i != T[h].end()) {
							i->second = v;
						}
						else {
							T[h].push_back({ label, v });
						}
					}
                }
            }
        }

        if (PART2) {
            // focusing power
            for (auto i : integers(256)) {
                auto&& b = T[i];
                int slot = 1;
                for (auto&& x : b) {
                    ans += (i + 1) * slot * x.second;
                    ++slot;
                }
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
        println("part2: ", r2); // 270858 too low
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
