#include "aoc.h"

using T = int64_t;
using Numbers = vector<T>;

//#define RECURSIVE_VERSION
#ifdef RECURSIVE_VERSION
// recursive solution, less efficient than my original method but wanted to 
// try implementing it as potentially simpler to read
//-----------------------------------------------------------------------------
auto diffs(Numbers const& seq) {
	Numbers d;
	for (auto i : integers(seq.size() - 1)) {
		d.push_back(seq[i + 1] - seq[i]);
	}
	return d;
}
//-----------------------------------------------------------------------------
int64_t seq_next(Numbers const& seq) {
    auto d = diffs(seq);
    if (all_of_v(d, 0)) {
        return seq.back();
    }
    return seq.back() + seq_next(d);
}
//-----------------------------------------------------------------------------
int64_t seq_prev(Numbers const& seq) {
    auto d = diffs(seq);
    if (all_of_v(d, 0)) {
        return seq.front();
    }
    return seq.front() - seq_prev(d);
}
#endif

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    int64_t part1 = 0;
    int64_t part2 = 0;

    for (Scanner s : i.lines()) {
        auto nums = s.vec<T>();

#ifdef RECURSIVE_VERSION
        part1 += seq_next(nums);
        part2 += seq_prev(nums);
#else
        vector<T> last_diffs;
        vector<T> first_diffs;
        auto last = nums.front();
        for (auto num : views::drop(nums, 1)) {
            auto d = num - last;
            T dd = d;
            last = num;
            for (auto& last_diff : last_diffs) {
                dd = d - last_diff;
                last_diff = d;
                d = dd;
            }
            last_diffs.push_back(dd);
            if (first_diffs.size() < last_diffs.size()) {
				first_diffs.push_back(dd);
			}
        }

        // extrapolate next value
        T dd = 0;
        for (auto& d : views::reverse(last_diffs)) {
            d += dd;
            dd = d;
        }
        auto n = nums.back() + dd;
        part1 += n;

        // extrapolate previous value
        dd = 0;
        for (auto& d : views::reverse(first_diffs)) {
            d -= dd;
            dd = d;
        }
        auto p = nums.front() - dd;
        part2 += p;
#endif
    }
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
