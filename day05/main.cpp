#include "aoc.h"

//-----------------------------------------------------------------------------
void Solve() {
    Input i;
    //Input i("example.txt");

    uint32_t part1 = 0u;
    int64_t part2 = 0u;

    std::vector<int64_t> seeds;
    string_view source;
    string_view dest;
    using Range = std::pair<int64_t, int64_t>;
    using Ranges = std::vector<Range>;
    using RangeQueue = std::deque<Range>;
    std::unordered_map<string_view, string_view> graph;
    std::unordered_map<string_view, Ranges> source_range_map;
    std::unordered_map<string_view, Ranges> dest_range_map;

    string_view seed_str = "seed";
    string_view location_str = "location";

    for (string_view line : i.lines()) {
        if (line.empty()) {
            continue;
        }
        if (line.starts_with("seeds:")) {
            split(line, ':');
            line = trim(line);
            while (!line.empty()) {
				seeds.push_back(parse<uint32_t>(split(line, ' ')));
			}
        }
        else {
            if (line.find(':') != line.npos) {
                source = split(line, '-');
                split(line, '-');
                dest = split(line, ' ');
                graph[source] = dest;
            }
            else {
                auto dst_start = parse<int64_t>(split(line, ' '));
                auto src_start = parse<int64_t>(split(line, ' '));
                auto num_range = parse<int64_t>(line);
                source_range_map[source].push_back({src_start, src_start + num_range});
                dest_range_map[dest].push_back({dst_start, dst_start + num_range});
            }
        }
    }

    std::vector<int64_t> locations;

    string_view current = seed_str;
    for (auto seed : seeds) {
        auto value = seed;
        while (current != location_str) {
			auto next = graph[current];
            auto& sources = source_range_map[current];
            auto& dests = dest_range_map[next];
            size_t index = 0;
            for (auto [start, end] : sources) {
                if (value >= start && value <= end) {
					auto [dst_start, dst_end] = dests[index];
					value = dst_start + (value - start);
					break;
				}
				++index;
			}
            if (next == location_str) {
                locations.push_back(value);
				break;
			}
			current = next;
		}
	}
    part1 = *std::min_element(locations.begin(), locations.end());

    RangeQueue ranges;
    RangeQueue new_ranges;
    RangeQueue intersections;
    RangeQueue split_ranges;
    for (size_t i = 0; i < seeds.size()-1; i+=2) {
        auto start = seeds[i];
        auto end = start + seeds[i+1];
		ranges.push_back({start, end});
	}    

    // Test
    //ranges.clear();
    //ranges.push_back({82, 83});

    current = seed_str;
    // Seems like this could be simplified..
    while (current != location_str) {
        auto next = graph[current];
        auto& sources = source_range_map[current];
        auto& dests = dest_range_map[next];

        // Note: Any source numbers that aren't mapped correspond 
        // to the same destination number.
        intersections.clear();
        for (auto const& range : ranges) {
            size_t index = 0;
            for (auto source_range : sources) {
                auto dest_range = dests[index];
                // intersection
                if ((range.first < source_range.second) && (range.second > source_range.first)) {
                    auto intersection = Range(
                        max(range.first, source_range.first),
                        min(range.second, source_range.second));
                    assert(intersection.first <= intersection.second);
                    if (intersection.first != intersection.second) {
                        intersections.push_back(intersection);
                        auto dest_start = dest_range.first + (intersection.first - source_range.first);
                        auto dest_end = dest_range.first + (intersection.second - source_range.first);
                        auto new_range = Range(dest_start, dest_end);
                        new_ranges.push_back(new_range);
                    }
				}
                ++index;
            }
        }
        // Remove any intersected ranges from the range list
        for (auto const& intersection : intersections) {
            bool any_intersections = true;
            while (any_intersections) {
                any_intersections = false;
                for (auto& range : ranges) {
                    if ((range.first < intersection.second) && (range.second > intersection.first)) {
                        any_intersections = true;
                        if (range.first < intersection.first) {
                            split_ranges.push_back(Range(range.first, std::min(range.second, intersection.first)));
                        }
                        if (intersection.second < range.second) {
                            split_ranges.push_back(Range(std::max(intersection.second, range.first), range.second));
                        }
                    }
                    else {
                        split_ranges.push_back(range);
                    }
                }
                ranges.swap(split_ranges);
                split_ranges.clear();
            }
        }
        current = next;
        ranges.insert(ranges.end(), new_ranges.begin(), new_ranges.end());
        new_ranges.clear();
    }
    part2 = ranges[0].first;
    for (auto range : ranges) {
		part2 = min(part2, range.first);
    }
    // part2 3249904174 too high
    // 3249904174
    // 50716416

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
