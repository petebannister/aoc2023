#include "aoc.h"

struct BitGrid
{
    static constexpr uint32_t R = 100u;
    static constexpr uint32_t C = 100u;
    static constexpr uint32_t Stride = 2u; // 2 QWords = 128 bits, allows fast indexing
    uint64_t bits[R * Stride];
    BitGrid() {
        memset(bits, 0, sizeof(bits));
    }
    __forceinline bool get(uint32_t r, uint32_t c) const {
        auto const x = c >> 6;
        auto const y = r << 1;
        auto q = bits[y + x];
        auto const b = c & 63;
        return (q >> b) & 1;
    }
    __forceinline void set(uint32_t r, uint32_t c, bool v) {
        auto const x = c >> 6;
        auto const y = r << 1;
        auto const b = c & 63;
        if (v) {
            bits[y + x] |= 1uLL << b;
        }
        else {
            bits[y + x] &= ~(1uLL << b);
        }
    }
    bool operator==(const BitGrid& rhs) const {
        for (auto i = 0u; i < R * Stride; ++i) {
            if (bits[i] != rhs.bits[i]) {
                return false;
            }
        }
        return true;
    }
    size_t hash() const {
        // Not a great hash but fast to compute
        size_t h = 0;
        for (auto b : bits) {
            h += b;
        }
        return h;
    }
};

namespace std
{
    template <>
    struct hash<BitGrid>
    {
        size_t operator()(const BitGrid& g) const {
            return g.hash();
        }
    };
}


//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;
    //uint32_t C = 0;
    //uint32_t R = 0;

    static constexpr uint32_t R = 100u;
    static constexpr uint32_t C = 100u;
    BitGrid fixed;
    BitGrid rollers;

    std::unordered_map<BitGrid, uint32_t> seen;

    int64_t solve() {
        Input i; // ("example.txt");
       // for (auto line : i.lines()) {
        //    ++R;
        //}

        //C = (*i.lines().begin()).size();
        //fixed.init(R, C);
        //rollers.init(R, C);

        uint32_t r = 0;
        for (auto line : i.lines()) {
            uint32_t c = 0;
            for (auto ch : line) {
                if (ch == '#') {
                    fixed.set(r, c, true);
                }
                else if (ch == 'O') {
                    rollers.set(r, c, true);
                }
                ++c;
            }
            ++r;
        }

        if (PART1) {
            rollN();
        }
        else {
            seen[rollers] = 0u;
            auto L = findLoop();
            //println("loop: ", L.first, " ", L.second);
            // 1000000000uL
            auto offset = L.first;
            auto cycle_length = L.second;
            auto n = 1000000000uL - (offset + 1);
            auto stat_cycle = offset + n % cycle_length;
            //println("cycle: ", stat_cycle);
            for (auto&& g : seen) {
                if (g.second == stat_cycle) {
                    rollers = g.first;
                    //println("found");
                    break;
                }
            }
        }

        int64_t ans = calcLoad();

        return ans;
    }

    std::pair<uint32_t, uint32_t> findLoop()
    {
        uint32_t cycle = 0;
        for (;;++cycle) {
            rollCycle();
            auto it = seen.find(rollers);
            if (it != seen.end()) {
                return { it->second, cycle - it->second };
            }
            seen[rollers] = cycle;
        }
    }

    void rollN() {
        for (uint32_t c = 0; c < C; ++c) {
            uint32_t mr = 0;
            for (uint32_t r = 0; r < R; ++r) {
                if (fixed.get(r, c)) { // '#'
                    mr = r + 1;
                }
                else if (rollers.get(r, c)) { // 'O'
                    if (r != mr) {
                        rollers.set(mr, c, true);
                        rollers.set(r, c, false);
                    }
                    ++mr;
                }
            }
        }
    }
    void rollS() {
        for (uint32_t c = 0; c < C; ++c) {
            uint32_t mr = R - 1;
            auto r = R;
            while (r--) {
                if (fixed.get(r, c)) { // '#'
                    mr = r - 1;
                }
                else if (rollers.get(r, c)) { // 'O'
                    if (r != mr) {
                        rollers.set(mr, c, true);
                        rollers.set(r, c, false);
                    }
                    --mr;
                }
            }
        }
    }
    void rollW() {
        for (uint32_t r = 0; r < R; ++r) {
            uint32_t mc = 0;
            for (uint32_t c = 0; c < C; ++c) {
                if (fixed.get(r, c)) { // '#'
                    mc = c + 1;
                }
                else if (rollers.get(r, c)) { // 'O'
                    if (c != mc) {
                        rollers.set(r, mc, true);
                        rollers.set(r, c, false);
                    }
                    ++mc;
                }
            }
        }
    }
    void rollE() {
        for (uint32_t r = 0; r < R; ++r) {
            uint32_t mc = C - 1;
            uint32_t c = C;
            while (c--) {
                if (fixed.get(r, c)) { // '#'
                    mc = c - 1;
                }
                else if (rollers.get(r, c)) { // 'O'
                    if (c != mc) {
                        rollers.set(r, mc, true);
                        rollers.set(r, c, false);
                    }
                    --mc;
                }
            }
        }
    }
    void rollCycle() {
        rollN();
        rollW();
        rollS();
        rollE();
    }

    int64_t calcLoad() {
        int64_t ans = 0;
        for (uint32_t c = 0; c < C; ++c) {
            for (uint32_t r = 0; r < R; ++r) {
                if (rollers.get(r, c)) {
                    ans += R - r;
                }
            }
        }
        return ans;
    }

};

//-----------------------------------------------------------------------------
int main() {
    try {
        Solver<false> p1;
        Solver<true> p2;
        Stopwatch sw;


        sw.start();
        auto r1 = p1.solve();
        sw.stop_print();
        println("part1: ", r1);

        sw.start();
        auto r2 = p2.solve();
        sw.stop_print();
        println("part2: ", r2); // 104619
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
