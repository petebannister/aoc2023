#include "aoc.h"
#include <immintrin.h>

struct Grid
{
    char v[100][128];

    Grid() {
		memset(v, '.', sizeof(v));
	}

    bool operator==(const Grid& rhs) const {
		return 0 == memcmp(v, rhs.v, sizeof(v));
	}
    bool operator<(const Grid& rhs) const {
        return 0 < memcmp(v, rhs.v, sizeof(v));
    }
    size_t hash() const {
        auto* e = (char*)v + sizeof(v);
        return std::accumulate((char*)v, e, 0u); 
		//return std::hash<std::string_view>{}(std::string_view(&v[0][0], sizeof(v)));
	}
    char* operator[](size_t r) {
		return v[r];
	}
};
namespace std {
	template <>
	struct hash<Grid>
	{
		size_t operator()(const Grid& g) const {
			return g.hash();
		}
	};
}

//-----------------------------------------------------------------------------
template <bool PART2>
struct Solver
{
    static constexpr bool PART1 = !PART2;

    static constexpr uint8_t C = 100u;
    static constexpr uint8_t R = 100u;

    Grid grid;

    std::unordered_map<Grid, uint32_t> seen;

    int64_t solve() {
        Input i; // ("example.txt");

        auto y = 0u;
        for (auto line : i.lines()) {
            memcpy(grid[y], line.data(), line.size());
            ++y;
        }

        //grid = i.chars_vec();
        //C = grid[0].size();
        //R = grid.size();

        //for (auto&& r : grid) {
            // Extend grid rows to next multiple 64 bytes since we are doing SIMD
        //    r.resize((r.size() + 63) & ~63, '.');
        //}

        if (PART1) {
            rollN_AVX();
        }
        else {
            seen[grid] = 0u;
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
                    grid = g.first;
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
            auto it = seen.find(grid);
            if (it != seen.end()) {
                return { it->second, cycle - it->second };
            }
            seen[grid] = cycle;
        }
    }

    template <uint8_t c>
    void rollN_AVX_C() 
    {
        __m512i mr = _mm512_setzero_si512();
        for (uint8_t r = 0; r < R; ++r) {
            auto* pr = &grid[r][c];
            auto ch = _mm512_loadu_si512(pr);
            auto rv = _mm512_set1_epi8(r);
            auto fixed = _mm512_cmpeq_epu8_mask(ch, _mm512_set1_epi8('#'));
            mr = _mm512_mask_add_epi8(mr, fixed, rv, _mm512_set1_epi8(1));
            auto movable = _mm512_cmpeq_epu8_mask(ch, _mm512_set1_epi8('O'));
            auto need_move = _mm512_cmpneq_epu8_mask(rv, mr);
            need_move &= ~fixed;
            need_move &= movable;
            // if anything set in need_move
            if (need_move) {
                auto new_ch = _mm512_mask_mov_epi8(ch, need_move, _mm512_set1_epi8('.'));
                _mm512_storeu_si512(pr, new_ch);
                uint8_t mrb[64];
                _mm512_storeu_si512(mrb, mr);
                for (uint8_t i = 0; i < 64; ++i) {
                    if (0 != (need_move & 1)) {
                        grid[mrb[i]][c + i] = 'O';
                    }
                    need_move >>= 1;
                }
            }
            mr = _mm512_mask_add_epi8(mr, movable, mr, _mm512_set1_epi8(1));
        }
    }
    void rollN_AVX() {
        rollN_AVX_C<0u>();
        rollN_AVX_C<64u>();
    }
    template <uint8_t c>
    void rollS_AVX_C()
    {
        __m512i mr = _mm512_set1_epi8(R - 1);
        auto r = R;
        while (r--) {
            auto* pr = &grid[r][c];
            auto ch = _mm512_loadu_si512(pr);
            auto rv = _mm512_set1_epi8(r);
            auto fixed = _mm512_cmpeq_epu8_mask(ch, _mm512_set1_epi8('#'));
            mr = _mm512_mask_add_epi8(mr, fixed, rv, _mm512_set1_epi8(-1));
            auto movable = _mm512_cmpeq_epu8_mask(ch, _mm512_set1_epi8('O'));
            auto need_move = _mm512_cmpneq_epu8_mask(rv, mr);
            need_move &= ~fixed;
            need_move &= movable;
            // if anything set in need_move
            if (need_move) {
                auto new_ch = _mm512_mask_mov_epi8(ch, need_move, _mm512_set1_epi8('.'));
                _mm512_storeu_si512(pr, new_ch);
                uint8_t mrb[64];
                _mm512_storeu_si512(mrb, mr);
                for (uint8_t i = 0; i < 64; ++i) {
                    if (0 != (need_move & 1)) {
                        grid[mrb[i]][c + i] = 'O';
                    }
                    need_move >>= 1;
                }
            }
            mr = _mm512_mask_add_epi8(mr, movable, mr, _mm512_set1_epi8(-1));
        }
    }
    void rollS() {
        for (size_t c = 0; c < C; ++c) {
            size_t mr = R - 1;
            auto r = R;
            while (r--) {
                auto& ch = grid[r][c];
                if (ch == '#') {
                    mr = r - 1;
                }
                else if (ch == 'O') {
                    if (r != mr) {
                        grid[mr][c] = 'O';
                        ch = '.';
                    }
                    --mr;
                }
            }
        }
    }
    void rollS_AVX() {
        rollS_AVX_C<0u>();
        rollS_AVX_C<64u>();
    }
    void printGrid() {
        for (auto&& r : grid) {
            for (auto&& c : r) {
				print(c);
			}
            println();
		}
        println();
    }
    void rollN() {
        for (auto c : integers(C)) {
            size_t mr = 0;
            for (auto r : integers(R)) {
                auto& ch = grid[r][c];
                if (ch == '#') {
                    mr = r + 1;
                }
                else if (ch == 'O') {
                    if (r != mr) {
                        grid[mr][c] = 'O';
                        ch = '.';
                    }
                    ++mr;
                }
            }
        }
    }
#if 1
    void rollW() {
        for (size_t r = 0; r < R; ++r) {
            size_t mc = 0;
            for (size_t c = 0; c < C; ++c) {
                auto& ch = grid[r][c];
                if (ch == '#') {
                    mc = c + 1;
                }
                else if (ch == 'O') {
                    if (c != mc) {
                        grid[r][mc] = 'O';
                        ch = '.';
                    }
                    ++mc;
                }
            }
        }
    }
    void rollE() {
        for (size_t r = 0; r < R; ++r) {
            size_t mc = C - 1;
            size_t c = C;
            while (c--) {
                auto& ch = grid[r][c];
                if (ch == '#') {
                    mc = c - 1;
                }
                else if (ch == 'O') {
                    if (c != mc) {
                        grid[r][mc] = 'O';
                        ch = '.';
                    }
                    --mc;
                }
            }
        }
    }
#endif

    void rotate() {
        auto tmp = grid;
        for (auto r : integers(R)) {
            for (auto c : integers(C)) {
                tmp[r][c] = grid[C - 1 - c][r];
            }
        }
        grid = std::move(tmp);
    }
    void rollCycle() {
#if 0
        rollN_AVX();
        rollW();
        rollS_AVX();
        rollE();
#else
        // Slightly faster.  If rollW / rollE could be vectorised then maybe that would be quicker.
        rollN_AVX();
        rotate();
        rollN_AVX();
        rotate();
        rollN_AVX();
        rotate();
        rollN_AVX();
        rotate();
#endif
    }

    int64_t calcLoad() {
        int64_t ans = 0;
        for (auto c : integers(C)) {
            for (auto r : integers(R)) {
                auto& ch = grid[r][c];
                if (ch == 'O') {
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
        println("part1: ", r1); // 112046

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
