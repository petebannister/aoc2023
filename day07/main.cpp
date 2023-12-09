#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;

    uint32_t part1 = 0u;

    vector<string_view> hands;
    vector<uint32_t> bids;
    unordered_map<char, uint8_t> values = {
        {'2', 0}, {'3', 1}, {'4', 2}, {'5', 3}, {'6', 4}, {'7', 5}, {'8', 6}, {'9', 7}, {'T', 8}, {'J', 9}, {'Q', 10}, {'K', 11}, {'A', 12}
    };
    vector<uint32_t> rank;
    vector<uint32_t> strength;
    array<uint8_t, 13> counts;

    for (string_view line : i.lines()) {
        hands.push_back(split(line, ' '));
        bids.push_back(parse<uint32_t>(line));
    }
    rank.resize(hands.size());
    strength.resize(hands.size());
    std::iota(rank.begin(), rank.end(), 0);
    for (auto i : rank) {
        std::fill(counts.begin(), counts.end(), 0u);
        for (auto c : hands[i]) {
            counts[values[c]]++;
        }
        if (ranges::count(counts, 5u)) { 
            // five of a kind
            strength[i] = 8;
        }
        else if (ranges::count(counts, 4u)) { 
            // four of a kind
            strength[i] = 7;
        }
        else if (ranges::count(counts, 3u) && ranges::count(counts, 2u)) {
            // full house
            strength[i] = 6;
        }
        else if (ranges::count(counts, 3u)) {
            // three of a kind
            strength[i] = 3;
        }
        else if (ranges::count(counts, 2u) == 2) {
            // two pairs
            strength[i] = 2;
        }
        else if (ranges::count(counts, 2u)) {
            // one pair
            strength[i] = 1;
        }
        else {
            // high card
            strength[i] = 0;
        }
    }
    // sort lowest to highest
    ranges::sort(rank, [&](auto a, auto b) {
        if (strength[a] == strength[b]) {
            auto va = ranges::transform_view(hands[a], [&](auto c) { return values[c]; });
            auto vb = ranges::transform_view(hands[b], [&](auto c) { return values[c]; });

            return ranges::lexicographical_compare(va, vb); // returns true if va < vb
        }
        return strength[a] < strength[b];
    });
    for (size_t i = 0; i < rank.size(); i++) {
        part1 += bids[rank[i]] * (i + 1);
    }


    println("part1: ", part1);
}

//-----------------------------------------------------------------------------
void SolvePart2() {
    Input i;
    uint32_t part2 = 0u;
    vector<string_view> hands;
    vector<uint32_t> bids;
    unordered_map<char, uint8_t> values = {
        {'J', 0}, {'2', 1}, {'3', 2}, {'4', 3}, {'5', 4}, {'6', 5}, {'7', 6}, {'8', 7}, {'9', 8}, {'T', 9}, {'Q', 10}, {'K', 11}, {'A', 12}
    };
    vector<uint32_t> rank;
    vector<uint32_t> strength;
    array<uint8_t, 13> counts;

    for (string_view line : i.lines()) {
        hands.push_back(split(line, ' '));
        bids.push_back(parse<uint32_t>(line));
    }
    rank.resize(hands.size());
    strength.resize(hands.size());
    iota(rank.begin(), rank.end(), 0);
    for (auto i : rank) {
        int highest_strength = 0;
        for (uint8_t wild = 1; wild < 13u; ++wild) {
            fill(counts.begin(), counts.end(), 0u);
            for (auto c : hands[i]) {
                counts[(c == 'J')?wild:values[c]]++;                
            }
            if (ranges::count(counts, 5u)) { // five of a kind
                highest_strength = 8;
            }
            else if (ranges::count(counts, 4u)) { // four of a kind
                highest_strength = max(highest_strength, 7);
            }
            else if (ranges::count(counts, 3u) && ranges::count(counts, 2u)) { // full house
                highest_strength = max(highest_strength, 6);
            }
            else if (ranges::count(counts, 3u)) { // three of a kind
                highest_strength = max(highest_strength, 3);
            }
            else if (ranges::count(counts, 2u) == 2) { // two pairs
                highest_strength = max(highest_strength, 2);
            }
            else if (ranges::count(counts, 2u)) { // one pair
                highest_strength = max(highest_strength, 1);
            }
            else { // high card
                highest_strength = max(highest_strength, 0);
            }
        }
        strength[i] = highest_strength;
    }
    // sort lowest to highest
    ranges::sort(rank, [&](auto a, auto b) {
        if (strength[a] == strength[b]) {
            auto va = ranges::transform_view(hands[a], [&](auto c) { return values[c]; });
            auto vb = ranges::transform_view(hands[b], [&](auto c) { return values[c]; });
            return ranges::lexicographical_compare(va, vb); // returns true if va < vb
        }
        return strength[a] < strength[b];
        });
    for (size_t i = 0; i < rank.size(); i++) {
        part2 += bids[rank[i]] * (i + 1);
    }
    println("part2: ", part2);
}
//-----------------------------------------------------------------------------
int main() {
    try {
        Solve();
        SolvePart2();
    }
    catch (const exception& e) {
        printf("Exception: %s\n", e.what());
        return -1;
    }
    return 0;
}
