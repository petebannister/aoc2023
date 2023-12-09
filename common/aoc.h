#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
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
class Scanner
{
private:
    string_view s_;
public:
    Scanner(string_view s) : s_(s) {}
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

    char const* data() {
        return data_.get();
    }
    string_view str() const {
        return string_view(data_.get(), size_);
    }
    auto lines() const {
        return strings(str() | views::split('\n'));
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
struct Point {
    int x = 0;
    int y = 0;

    bool operator== (Point const& p) const {
        return (x == p.x) && (y == p.y);
    }
    bool operator!= (Point const& p) const {
        return !(*this == p);
    }
    auto fields() const {
        return std::tie(x, y);
    }
    bool operator< (Point const& p) const {
        return (fields() < p.fields());
    }
    Point operator-(Point const& rhs) const {
        return { x - rhs.x, y - rhs.y };
    }
    Point operator+(Point const& rhs) const {
        return { x + rhs.x, y + rhs.y };
    }
    Point& operator+=(Point const& rhs) {
        return (*this = *this + rhs);
    }
    Point& operator-=(Point const& rhs) {
        return (*this = *this - rhs);
    }
    Point minimized() const {
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
    Point cw90() const {
        return Point{ -y, x };
    }
    Point ccw90() const {
        return Point{ y, -x };
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
    int manhattan() const {
        return abs(x) + abs(y);
    }

    size_t hash() const {
        auto h = std::hash<int>();
        return h(x) ^ h(y);
    }
};
using points = std::vector<Point>;

namespace std {
    template <>
    struct hash<Point> {
        size_t operator()(Point const& p) const {
            return p.hash();
        }
    };
}
