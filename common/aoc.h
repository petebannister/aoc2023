#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <deque>
#include <iostream>
#include <memory>
#include <numeric>
#include <ranges>
#include <string_view>
#include <bitset>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template <typename...Args>
inline void print(Args&&...args) {
    (std::cout << ... << args);
}
template <typename...Args>
inline void println(Args&&...args) {
    (std::cout << ... << args) << '\n';
}

//-----------------------------------------------------------------------------
auto strings(auto view) {
    return view | views::transform([](auto const& r) {
        return string_view(r.begin(), r.end());
        });
}
//-----------------------------------------------------------------------------
inline string_view split(string_view& s, char delim = ' ') {
    auto pos = s.find(delim);
    if (pos == string_view::npos) {
        auto r = s;
        s.remove_prefix(s.size());
        return r;
    }
    auto r = s.substr(0, pos);
    s.remove_prefix(pos + 1);
    return r;
}
//-----------------------------------------------------------------------------
template<typename T>
inline T parse(string_view s) {
    T v;
    from_chars_result r = std::from_chars(s.data(), s.data() + s.size(), v);
    if (r.ec != std::errc()) {
        printf("Failed to parse: %s\n", s.data());
        exit(-1);
    }
    return v;
}
//-----------------------------------------------------------------------------
template<typename T>
inline T parseHex(string_view s) {
    T v;
    from_chars_result r = std::from_chars(s.data(), s.data() + s.size(), v, 16);
    if (r.ec != std::errc()) {
        printf("Failed to parse: %s\n", s.data());
        exit(-1);
    }
    return v;
}
//-----------------------------------------------------------------------------
inline void trim_left(string_view& s) {
    while (!s.empty() && isspace(s.front())) {
        s.remove_prefix(1);
    }
}
//-----------------------------------------------------------------------------
inline void trim_right(string_view& s) {
    while (!s.empty() && isspace(s.back())) {
        s.remove_suffix(1);
    }
}
//-----------------------------------------------------------------------------
inline string_view trimmed_left(string_view s) {
    trim_left(s);
    return s;
}
//-----------------------------------------------------------------------------
inline string_view trimmed_right(string_view s) {
    trim_right(s);
    return s;
}
//-----------------------------------------------------------------------------
inline string_view trim(string_view s) {
    s = trimmed_left(s);
    s = trimmed_right(s);
    return s;
}

//-----------------------------------------------------------------------------
template <typename T>
struct num_range {
    using self = num_range<T>;
    struct iterator {
        T v;
        iterator& operator++() {
            ++v;
            return *this;
        }
        T const& operator*() const {
            return v;
        }
        bool operator==(iterator const& r) const {
            return v == r.v;
        }
        bool operator!=(iterator const& r) const {
            return !(*this == r);
        }
    };
    T start_ = 0;
    T limit_ = 0;
    iterator begin() const {
        return iterator{ start_ };
    }
    iterator end() const {
        return iterator{ limit_ };
    }
    bool empty() const {
        return (start_ == limit_);
    }
    bool intersects(self const& other) const {
        return ((other.start_ < limit_) && (other.limit_ > start_));
    }
    bool contains(self const& other) const {
        return ((other.start_ >= start_) && (other.limit_ <= limit_));
    }
    bool contains(T const& v) const {
        return ((v >= start_) && (v < limit_));
    }
    T size() const {
        return (limit_ - start_);
    }
};
//-----------------------------------------------------------------------------
template <typename T>
inline num_range<T> integers(T limit) {
    return num_range<T>{0, limit};
}
//-----------------------------------------------------------------------------
template <typename T>
inline num_range<T> integers(T start, T limit) {
    return num_range<T>{start, limit};
}

//-----------------------------------------------------------------------------
inline auto equals(auto v) {
	return [v](auto const& r) { return r == v; };
}
//-----------------------------------------------------------------------------
inline bool all_of_v(auto&& view, auto v) {
	return ranges::all_of(view, equals(v));
}

//-----------------------------------------------------------------------------
class Scanner
{
private:
    string_view s_;
public:
    Scanner(string_view s) : s_(s) {}

    auto line() const {
        return s_;
    }

    template <typename T>
    T read() {
        T v;
        trim_left(s_);
        from_chars_result r = std::from_chars(s_.data(), s_.data() + s_.size(), v);
        if (r.ec != std::errc()) {
            throw runtime_error("Failed to parse: " + string(s_));
        }
        s_ = trimmed_left(string_view(r.ptr, (s_.data() + s_.size())));
        return v;
	}
    uint32_t u32() { return read<uint32_t>(); }
    uint64_t u64() { return read<uint64_t>(); }
    int32_t i32() { return read<int32_t>(); }
    int64_t i64() { return read<int64_t>(); }
    string_view str(char delim = ' ') {
        auto pos = s_.find(delim);
        if (pos == string_view::npos) {
			auto r = s_;
			s_.remove_prefix(s_.size());
			return r;
		}
        auto r = s_.substr(0, pos);
        s_.remove_prefix(pos + 1);
        trim_left(s_);
        return r;
    }
    void expect(char ch) {
        trim_left(s_);
        if (s_.empty() || s_.front() != ch) {
            throw runtime_error("Expected '" + string(1, ch) + "' but got '" + string(1, s_.empty() ? ' ' : s_.front()) + "'");
		}
		s_.remove_prefix(1);
	}
    template <typename T>
    std::vector<T> vec(char delim = ' ', char end_delim = ' ') {
        std::vector<T> v;
        while (!s_.empty()) {
            v.push_back(read<T>());

            if (!s_.empty()) {
                if (s_.front() == end_delim) {
					s_.remove_prefix(1);
					break;
				}
                if (delim != ' ') {
                    expect(delim);
                }
            }
            trim_left(s_);
        }
        return v;
    }
    std::vector<uint32_t> vec_u32(char delim = ' ', char end_delim = ' ') {
        return vec<uint32_t>(delim, end_delim);
    }
    std::vector<int32_t> vec_i32(char delim = ' ', char end_delim = ' ') {
        return vec<int32_t>(delim, end_delim);
    }
};

//-----------------------------------------------------------------------------
class Input
{
private:
    std::unique_ptr<char[]> data_;
    size_t size_;
public:
    Input(char const* fname = "input.txt", char const* mode = "rb") {
        FILE* f = nullptr;
        fopen_s(&f, fname, "rb");
        if (!f) {
            printf("Failed to open file: %s\n", fname);
            exit(-1);
        }
        fseek(f, 0, SEEK_END);
        size_ = ftell(f);
        fseek(f, 0, SEEK_SET);
        data_ = std::make_unique_for_overwrite<char[]>(size_ + 1);
        fread((void*)data_.get(), 1, size_, f);
        data_[size_] = '\0';
        if (data_[size_ - 1] == '\n') { // So that lines() does not return a blank final line
            --size_;
        }
        fclose(f);
    }
    Input(Input const&) = delete;
    Input& operator=(Input const&) = delete;
    Input(Input&&) = delete;
    Input& operator=(Input&&) = delete;

    size_t size() const {
		return size_;
	}

    char const* data() {
        return data_.get();
    }
    string_view str() const {
        return string_view(data_.get(), size_);
    }
    auto lines() const {
        return strings(str() | views::split('\n'));
    }
    auto lines_vec() const {
        auto l = lines();
        return std::vector<string_view>(l.begin(), l.end());
    }
    auto chars_vec() const {
        auto l = lines();
        std::vector<std::vector<char>> v;
        for (auto l : lines()) {
            v.push_back(std::vector<char>(l.begin(), l.end()));
        }
        return v;
    }
};
//-----------------------------------------------------------------------------
template <typename T>
inline T sgn(T x) {
    return (x < 0) ? -1 : (x == 0) ? 0 : 1;
}
//-----------------------------------------------------------------------------
template <typename T>
inline T
gcd(T x, T y)
{
    T g;
    if (x <= y) {
        T r = y % x;
        if (r == 0) {
            g = x;
        }
        else {
            g = gcd(x, r);
        }
    }
    else {
        g = gcd(y, x);
    }
    return g;
}
//-----------------------------------------------------------------------------
template <typename T>
inline T
lcm(T x, T y)
{
    return (x * y) / gcd(x, y);
}
//-----------------------------------------------------------------------------
template <typename T>
struct TPoint {
    T x = 0;
    T y = 0;

    using self = TPoint<T>;

    static constexpr self U() { return { 0, -1 }; }
    static constexpr self R() { return { 1, 0 }; }
    static constexpr self D() { return { 0, 1 }; }
    static constexpr self L() { return { -1, 0 }; }
    static constexpr array<self, 4> const Dirs() {
        return { {
			{ 0, -1 },
			{ 1, 0 },
			{ 0, 1 },
			{ -1, 0 },
		} };
    }

    constexpr bool operator== (self const& p) const {
        return (x == p.x) && (y == p.y);
    }
    constexpr bool operator!= (self const& p) const {
        return !(*this == p);
    }
    constexpr auto fields() const {
        return std::tie(x, y);
    }
    constexpr bool operator< (self const& p) const {
        return (fields() < p.fields());
    }
    constexpr self operator-(self const& rhs) const {
        return { x - rhs.x, y - rhs.y };
    }
    constexpr self operator-() const {
        return { -x, -y };
    }
    constexpr self operator+(self const& rhs) const {
        return { x + rhs.x, y + rhs.y };
    }
    constexpr self& operator+=(self const& rhs) {
        return (*this = *this + rhs);
    }
    constexpr self& operator-=(self const& rhs) {
        return (*this = *this - rhs);
    }
    constexpr self minimized() const {
        if (x == 0) {
            if (y == 0) {
                return *this;
            }
            return { 0, sgn(y) };
        }
        if (y == 0) {
            return { sgn(x), 0 };
        }
        auto p = *this;
        auto g = gcd(abs(x), abs(y));
        if (g != 0) {
            p.x /= g;
            p.y /= g;
        }
        return p;
    }
    constexpr self cw90() const {
        return self{ -y, x };
    }
    constexpr self ccw90() const {
        return self{ y, -x };
    }
    void swapxy() {
        std::swap(x, y);
    }

    double angle() const {
        auto d = minimized(); // for consistency
        d = d.cw90();
        auto a = atan2(d.y, d.x);
        if (a < 0) {
            return a + (3.14159265359 * 2);
        }
        return a;
    }
    T manhattan() const {
        return abs(x) + abs(y);
    }

    size_t hash() const {
        auto h = std::hash<T>();
        return h(x) ^ h(y);
    }
};
using Point = TPoint<int>;
using Point64 = TPoint<int64_t>;
using Points = std::vector<Point>;

namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(Point const& p) const {
            return p.hash();
        }
    };
    template <>
    struct hash<Point64> {
        size_t operator()(Point64 const& p) const {
            return p.hash();
        }
    };
}

struct Stopwatch
{
    using clock = std::chrono::high_resolution_clock;
	using time_point = clock::time_point;
	using duration = clock::duration;
	time_point start_;
	time_point end_;
	Stopwatch() {
		start();
	}
	void start() {
		start_ = clock::now();
	}
	void stop() {
		end_ = clock::now();
	}
	auto elapsed() const {
		return end_ - start_;
	}
	auto elapsed_ms() const {
		return std::chrono::duration_cast<std::chrono::milliseconds>(elapsed());
	}
	auto elapsed_us() const {
		return std::chrono::duration_cast<std::chrono::microseconds>(elapsed());
	}
	auto elapsed_ns() const {
		return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed());
	}
    void stop_print() {
        stop();
        println("elapsed: ", elapsed_us().count() * 0.001, "ms");
    }
};