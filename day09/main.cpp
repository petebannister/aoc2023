#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;
    //Input i("example.txt");

    int64_t part1 = 0;
    int64_t part2 = 0;

    using T = int64_t;

    T linenum = 1;
    for (Scanner s : i.lines()) {
        auto nums = s.vec<T>();
        vector<T> diffs;
        auto last = nums.front();
        for (auto num : views::drop(nums, 1)) {
            auto d = num - last;
            T dd = d;
            last = num;
            for (auto& last_diff : diffs) {
                dd = d - last_diff;
                last_diff = d;
                d = dd;
            }
            diffs.push_back(dd);
        }

        // extrapolate
        T dd = 0;
        for (auto& d : views::reverse(diffs)) {
            d += dd;
            dd = d;
        }
        auto r = nums.back() + dd;
        println(linenum, " ", r);
        part1 += r;

        ++linenum;
    }

    // 966664654 too low
    // 1834094367 too low
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
