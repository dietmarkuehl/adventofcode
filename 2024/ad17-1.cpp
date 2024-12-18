#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cinttypes>
#include <cassert>

struct cpu
{
    std::uint64_t A;
    std::uint64_t B;
    std::uint64_t C;
    std::uint64_t PC{};

    std::vector<int> program;
    friend std::ostream& operator<< (std::ostream& out, cpu const& c) {
        return out << "A=" << c.A << " B=" << c.B << " C=" << c.C << " PC=" << c.PC;
    }
    std::uint64_t literal() { assert(PC + 1 < program.size()); return program[PC + 1]; }
    std::uint64_t combo() {
        assert(PC + 1 < program.size());
        switch (program[PC + 1]) {
        case 7: assert(program[PC + 1] != 7);
        default: return program[PC + 1];
        case 4: return A;
        case 5: return B;
        case 6: return C;
        }
    }
    void adv() { A = A / (1 << combo()); PC += 2; }
    void bxl() { B = literal() ^ B; PC += 2; }
    void bst() { B = combo() % 8; PC += 2; }
    void jnz() { if (A) PC = literal(); else PC += 2; }
    void bxc() { B = B ^ C; PC += 2; }
    void out() { std::cout << (combo() % 8) << ","; PC += 2; }
    void bdv() { B = A / (1 << combo()); PC += 2; }
    void cdv() { C = A / (1 << combo()); PC += 2; }

    bool execute() {
        if (program.size() <= PC) {
            return false;
        }
        switch (program[PC]) {
        default: std::cout << "not implemented: " << program[PC] << "\n"; return false;
        case 0: adv(); return true;
        case 1: bxl(); return true;
        case 2: bst(); return true;
        case 3: jnz(); return true;
        case 4: bxc(); return true;
        case 5: out(); return true;
        case 6: bdv(); return true;
        case 7: cdv(); return true;
        }
    }
};

template <char C>
std::istream& skip(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), C);
}
auto colon = skip<':'>;
auto comma = skip<','>;

int main() {
    std::cout << std::unitbuf;
    cpu c;
    std::cin >> colon >> c.A >> colon >> c.B >> colon >> c.C >> colon;
    for (int inst; std::cin >> inst; std::cin >> comma) {
        c.program.push_back(inst);
    }
    //std::cout << c << "\n";
    //for (auto i: c.program) {
    //    std::cout << i << ", ";
    //}
    //std::cout << "\n";
    while (c.execute()) {
        //std::cout << c << "\n";
    }
    std::cout << "\n";
}
