#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i; // ("example.txt");

    uint32_t part1 = 0u;
    uint32_t part2 = 0u;

    auto printbits = [](uint32_t v, uint8_t nbits) {
		while (nbits--) {
			print((v & 1) ? "#" : ".");
			v >>= 1;
		}
        println();
	};

    auto valid = [](uint32_t _v, auto const& widths, uint32_t set_bits, uint32_t unset_bits) {
        auto v = _v;
        if ((v & set_bits) != set_bits) {
            return false;
        }
        if ((v & unset_bits) != 0) {
			return false;
		}
        for (auto w : widths) {
            while ((v & 1) == 0) {
                v >>= 1;
                if (v == 0) {
					return false;
				}
            }
            while (w--) {
                if ((v & 1) == 0) {
                    return false;
                }
                v >>= 1;
            }
            if ((v & 1) != 0) {
                return false;
			}
        }
        if (v != 0) {
            return false;
        }
        return true;
    };

    for (Scanner line : i.lines()) {
        println("---------");
        println(line.line());


        auto s = line.str(' ');
        auto widths = line.vec<uint8_t>(',');
        std::reverse(widths.begin(), widths.end()); // LSB first
        uint32_t set_bits = 0u;
        uint32_t unset_bits = 0u;
        uint32_t unk = 0u; // unknown bits
        uint8_t nbits = s.size();
        assert(s.size() <= (sizeof(set_bits) * 8));
        for (auto c : s) {
            set_bits <<= 1;
            unset_bits <<= 1;
			unk <<= 1;

            if (c == '?') {
                unk |= 1u;
            }
            else if (c == '#') {
                set_bits |= 1u;
			}
            else if (c == '.') {
                unset_bits |= 1u;
            }
        }

        uint32_t n = 0u;
        for (auto v : integers(1 << nbits)) {
            if (valid(v, widths, set_bits, unset_bits)) {
				n++;
			}
		}
        println(n, " ", s);
        part1 += n;

#if 0
        vector<uint8_t> shifts;
        uint8_t shift = 0u;
        for (auto w : widths) {
            shifts.push_back(shift);
            shift += w + 1;
        }
        auto min_bits = std::accumulate(widths.begin(), widths.end(), 0u) + (widths.size() - 1);
        auto spare_bits = (nbits - min_bits); // bits per shift
        auto combinations = (nbits - min_bits) * widths.size();
        for (auto sn : integers(shifts.size())) {
            for (auto b : integers(spare_bits)) {

            }
        }
        println(spare_bits, ", ", widths.size(), " ", s);
#endif
    }

    // 489370 too high, 103361 too high..
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
