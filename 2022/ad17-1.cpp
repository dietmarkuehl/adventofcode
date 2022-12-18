#include <algorithm>
#include <bitset>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

struct row {
    std::bitset<9> r;
    row(std::bitset<9> const& r): r(r) {}
    row(std::string const& r): r(r) {}
    bool operator== (row const&) const = default;
    friend std::ostream& operator<< (std::ostream& out, row const& r) {
        return out << r.r.to_string('.', '#');
    }
    row operator|(row const& other) const { return this->r | other.r; }
    bool operator& (row const& other) const { return (this->r & other.r) != std::bitset<9>(); }
};

row const empty("000000000");
row const border("100000001");
row const full("111111111");

struct shape {
    std::array<row, 4> rows;
    friend std::ostream& operator<< (std::ostream& out, shape const& s) {
        std::copy(s.rows.begin(), s.rows.end(), std::ostream_iterator<row>(out, "\n"));
        return out;
    }
    auto begin() const { return rows.begin(); }
    auto end() const   { return rows.end(); }
    void left() { for (row& r: this->rows) r.r <<= 1; }
    void right() { for (row& r: this->rows) r.r >>= 1; }
};

std::vector<shape> shapes{
    { row("000000000"), row("000000000"), row("000000000"), row("000111100") },
    { row("000000000"), row("000010000"), row("000111000"), row("000010000") },
    { row("000000000"), row("000001000"), row("000001000"), row("000111000") },
    { row("000100000"), row("000100000"), row("000100000"), row("000100000") },
    { row("000000000"), row("000000000"), row("000110000"), row("000110000") }
};

struct playground {
    std::vector<row> area{ full, border };
    std::string      instructions;
    std::size_t      next_instruction{};

    playground(std::string const& instructions): instructions(instructions) {}

    friend std::ostream& operator<< (std::ostream& out, playground const& p) {
        std::copy(p.area.rbegin(), p.area.rend(), std::ostream_iterator<row>(out, "\n"));
        return out;
    }

    char instruction() const { return this->instructions[this->next_instruction % instructions.size()]; }
    bool intersects(auto it, shape const& s) const {
        for (row const& r: s) {
            if (r & *it++) {
                return true;
            }
        }
        return false;
    }
    bool move(auto it, shape& s) {
        char instruction{this->instruction()};
        ++this->next_instruction;

        instruction == '<'? s.left(): s.right();
        if (this->intersects(it, s)) {
            instruction == '>'? s.left(): s.right();
        }
        if (this->intersects(++it, s)) {
            return false;
        }
        return true;
    }
    playground& apply(auto it, shape& s) {
        for (row const& r: s) {
           *it = *it | r;
           ++it;
        }
        return *this;
    }
    auto find() {
        return std::find_if(this->area.rbegin(), this->area.rend(),
                            [](row const& r){ return r != border; });
    }
    auto start_piece() {
        auto it = find();
        this->area.resize(this->area.size() + 7u - std::distance(this->area.rbegin(), it), border);
        return this->area.rbegin();
    }
    std::size_t size() {
        return std::distance(this->find(), this->area.rend()) - 1u;
    }
};

int main() {
    std::string instructions;
    if (!(std::cin >> instructions)) {
        std::cout << "couldn't read instructions\n";
        return 0;
    }

    playground p(instructions);
    for (std::size_t next_shape{}; true; ) {
        if (next_shape == 2022u) {
            break;
        }
        auto  it{p.start_piece()};
        shape s(shapes[next_shape++ % shapes.size()]);
        
        while (p.move(it, s)) {
            ++it;
        }
        p.apply(it, s);
    }
    std::cout << p.size() << "\n";
}
