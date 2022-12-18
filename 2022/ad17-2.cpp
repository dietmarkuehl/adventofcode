#include <algorithm>
#include <chrono>
#include <bitset>
#include <iomanip>
#include <fstream>
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
    shape(std::uint32_t r = 0): d_row(std::uint64_t(r) << 24) {}
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
    using buffer = std::array<unsigned char, 1024>;
    static constexpr std::size_t cut{512};

    std::array<std::array<shape, 8>, 5> init{};

    std::string      instructions;
    std::string      init_ins;
    std::size_t      next_instruction{};
    buffer           area{};
    buffer::iterator it{area.end() - 6};
    std::size_t      d_size{};
    
    std::size_t move_to_index(std::string const& s) {
        return 4 * (s[0] == '<') + 2 * (s[1] == '<') + 1 * (s[2] == '<');
    }
    playground(std::string const& instructions)
        : instructions(instructions) {
        for (int i = 1; i != 6; ++i) {
            *(this->area.end() - i) = 0b11111110;
        }
        using namespace std::string_literals;
        for (std::size_t i{}; i != shapes.size(); ++i) {
            for (std::string m: { "<<<"s,"<<>"s,"<><"s,"<>>"s,"><<"s,"><>"s,">><"s,">>>"s }) {
                shape s{shapes[i]};
                m[0] == '<'? s.left(0u): s.right(0u);
                m[1] == '<'? s.left(0u): s.right(0u);
                m[2] == '<'? s.left(0u): s.right(0u);
                init[i][move_to_index(m)] = s;
            }
        }

        std::string tmp;
        std::size_t index{};
        auto next = [&]{ return instructions[index++ % instructions.size()]; };
        tmp.push_back(next());
        tmp.push_back(next());
        tmp.push_back(next());
        while (init_ins.size() != instructions.size()) {
            init_ins.push_back(move_to_index(tmp));
            tmp.erase(tmp.begin());
            tmp.push_back(next());
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
    void move(std::size_t index) {
        auto it{this->it};

        shape s{init[index][this->init_ins[this->next_instruction % this->instructions.size()]]};
        this->next_instruction += 3;

        std::uint64_t rows{};
        rows = (rows << 8) | *++it;
        rows = (rows << 8) | *++it;
        rows = (rows << 8) | *++it;

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

int main(int ac, char *av[]) {
    std::cout << std::unitbuf;
    std::ifstream fin(ac == 2? av[1]: "");
    std::istream&  in(fin? fin: std::cin);
    std::string instructions;
    if (!(in >> instructions)) {
        std::cout << "couldn't read instructions\n";
        return 0;
    }

    playground p(instructions);

    static constexpr bool small{true};
    static constexpr std::size_t split{small? 1: 10000};
    static constexpr std::size_t target{small? 2022: 1000000000000};
    static constexpr std::size_t limit{target / split};

    std::size_t current{};
    std::size_t next_shape{};

    auto start = std::chrono::steady_clock::now();
    for (std::size_t i{}; i != split; ++i) {
        current += limit;
        for (; next_shape != current; ++next_shape) {
            p.start_piece();
            p.move(next_shape % shapes.size());
        }

        auto now = std::chrono::steady_clock::now();
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count()
                  << " " << std::setw(4) << i << " " << p.size() << "\n";
        start = now;
    }
}
