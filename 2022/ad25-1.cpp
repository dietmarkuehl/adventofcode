#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <numeric>

struct oddbase {
    std::uint64_t value{};
    oddbase& operator+= (oddbase const& other) { this->value += other.value; return *this; }
    oddbase  operator+ (oddbase other) { return other += *this; }

    friend std::istream& operator>> (std::istream& in, oddbase& o) {
        std::istreambuf_iterator<char> it(in.rdbuf()), end{};
        bool consumed{};
        std::uint64_t value{};
        for (bool done{}; !done && it != end; ++it) {
            value *= 5;
            switch (*it) {
            default: done = true; continue;
            case '=': value -= 2; break;
            case '-': value -= 1; break;
            case '0':           ; break;
            case '1': value += 1; break;
            case '2': value += 2; break;
            }
            consumed = true;
            o.value = value;
        }
        if (!consumed) {
            in.setstate(std::ios_base::failbit);
        }
        return in;
    }
    friend std::ostream& operator<< (std::ostream& out, oddbase const& o) {
         if (o.value == 0) {
             return out << 0;
         }
         char buffer[128];
         char* it{buffer};
         for (std::uint64_t v = o.value; v != 0; v /= 5) {
             switch (v % 5) {
                 case 0: *it++ = '0'; break;
                 case 1: *it++ = '1'; break;
                 case 2: *it++ = '2'; break;
                 case 3: *it++ = '='; v += 5; break;
                 case 4: *it++ = '-'; v += 5; break;
             }
         }
         std::reverse_copy(buffer, it, std::ostreambuf_iterator<char>(out));
         return out;
    }
};

std::pair<std::string, unsigned long> tests[] = {
    { "1=-0-2",    1747 },
    {  "12111",     906 },
    {   "2=0=",     198 },
    {     "21",      11 },
    {   "2=01",     201 },
    {    "111",      31 },
    {  "20012",    1257 },
    {    "112",      32 },
    {  "1=-1=",     353 },
    {   "1-12",     107 },
    {     "12",       7 },
    {     "1=",       3 },
    {    "122",      37 }
};

int main() {
    for (auto p: tests) {
        oddbase v;
        if (std::istringstream(p.first) >> std::ws >> v) {
            if (v.value != p.second) {
                std::cout << "snafu=" << p.first << " value=" << p.second << " read=" << v.value << "\n";
            }
            std::ostringstream out;
            out << v;
            if (out.str() != p.first) {
                std::cout << "snafu=" << p.first << " fmt='" << out.str() << "'\n";
            }
        }
        else {
            std::cout << "failed to read oddbase: " << p.first << "\n";
        }
    }
    auto v = std::accumulate(std::istream_iterator<oddbase>(std::cin), std::istream_iterator<oddbase>{}, oddbase{});
    std::cout << v << "\n";
}
