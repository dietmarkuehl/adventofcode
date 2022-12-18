#include <algorithm>
#include <bitset>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <cstdint>

struct fmt {
    unsigned char value;
    fmt(unsigned char v): value(v) {}
    friend std::ostream& operator<< (std::ostream& out, fmt const& f) {
        return out << std::bitset<7>(f.value).to_string('.', '#');
    }
};

struct shape {
    static constexpr std::uint64_t test_left { std::uint64_t{0b10000000'10000000'10000000'10000000} << 24 };
    static constexpr std::uint64_t test_right{ std::uint64_t{0b00000010'00000010'00000010'00000010} << 24 };
    std::uint64_t d_row;
    shape(std::uint32_t r): d_row(std::uint64_t(r) << 24) {}
    friend std::ostream& operator<< (std::ostream& out, shape const& s) {
        for (int i{4}; 0 < i--; ) {
            out << "|" << fmt((s.d_row >> (24 + i * 8 + 1)) & 0xFF) << "|\n";
        }
        return out;
    }
    void left(std::uint64_t state)  { if ((this->d_row & (test_left  | (state >> 1))) == 0u) { this->d_row <<= 1; } }
    void right(std::uint64_t state) { if ((this->d_row & (test_right | (state << 1))) == 0u) { this->d_row >>= 1; } }
    bool intersects(std::uint64_t x) const { return this->d_row & x; }
    std::uint32_t row() const { return this->d_row >> 24; }
};

std::vector<shape> shapes{
    { 0b00000000'00000000'00000000'00111100 },
    { 0b00000000'00010000'00111000'00010000 },
    { 0b00000000'00001000'00001000'00111000 },
    { 0b00100000'00100000'00100000'00100000 },
    { 0b00000000'00000000'00110000'00110000 }
};

struct playground {
    using buffer = std::array<unsigned char, 2048>;
    static constexpr std::size_t cut{1024};

    std::string      instructions;
    std::size_t      next_instruction{};
    buffer           area{};
    buffer::iterator it{area.end() - 6};
    std::size_t      d_size{};
    
    playground(std::string const& instructions)
        : instructions(instructions) {
        for (int i = 1; i != 6; ++i) {
            *(this->area.end() - i) = 0b11111110;
        }
    }
    auto start_piece() {
        if (this->it < this->area.begin() + 8) {
            std::memmove(&*this->area.begin() + cut, &*this->area.begin(), this->area.size() - cut);
            std::memset(&*this->area.begin(), 0, cut);
            this->it += cut;
            this->d_size += cut;
        }
        return this->it - 3;
    }

    friend std::ostream& operator<< (std::ostream& out, playground const& p) {
        for (int i{}; i != 3; ++i) {
            out << "|.......|\n";
        }
        for (auto it{p.it}; it != p.area.end() - 5; ++it) {
            out << "|" << fmt(*it >> 1) << "|" << (it == p.it? "<--": "") << "\n";
        }
        out << "+-------+\n";
        return out;
    }

    char next() {
        return this->instructions[this->next_instruction++ % this->instructions.size()];
    }
    void move(shape s) {
        auto it{this->it};
        std::uint64_t rows{};

        while (true) {
            this->next() == '<'? s.left(rows): s.right(rows);
            rows = (rows << 8) | *++it;
            if (s.intersects(rows)) {
                it -= 3;
                std::uint32_t row(s.row());
                for (int i{}; i != 8; ++i) {
                    *--it |= row & 0xFF;
                    row >>= 8;
                }
                while (*this->it) {
                    --this->it;
                }
                break;
            }
        }
    }
    std::size_t size() {
        return this->d_size + std::distance(this->it, this->area.end()) - 6;
    }
};

int main() {
    std::cout << std::unitbuf;
    std::string instructions;
    if (!(std::cin >> instructions)) {
        std::cout << "couldn't read instructions\n";
        return 0;
    }

    playground p(instructions);

    static std::size_t split{10000};
    static std::size_t target{1000000000000};
    static std::size_t limit{target / split};
    std::size_t current{};
    std::size_t next_shape{};

    for (std::size_t i{}; i != split; ++i) {
        current += limit;
        for (; next_shape != current; ++next_shape) {
            shape s(shapes[next_shape % shapes.size()]);
            p.start_piece();
            p.move(s);
        }
        std::cout << std::setw(4) << i << " " << next_shape << " " << p.size() << "\n";
    }
}
