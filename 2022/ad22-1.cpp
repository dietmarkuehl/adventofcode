#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> read_map(std::istream& in) {
    std::vector<std::string> map;
    map.emplace_back();
    for (std::string line; std::getline(in, line) && !line.empty(); ) {
        map.push_back(" " + line);
    }
    map.emplace_back();
    auto max = std::max_element(map.begin(), map.end(), [](auto const& a, auto const& b){ return a.size() < b.size(); })->size();
    for (auto& l: map) {
        l.resize(max + 1, ' ');
    }
    return map;
}

std::string read_instructions(std::istream& in) {
    std::string instructions;
    if (!std::getline(in, instructions)) {
        std::cout << "bad input\n";
        std::exit(0);
    }
    return instructions;
}

auto read(std::istream& in) {
    return std::pair(read_map(in), read_instructions(in));
}

struct position {
    int x;
    int y;

    position& operator+= (position other) {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }
};

int main() {
    auto[tmap, ins] = read(std::cin);
    auto map = tmap;

    const char dir_char[] = { '>', 'v', '<', '^' };
    const position move[] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
    position pos{ 1, 1 };
    int      dir{};

    auto at = [&map](position pos)->char& { return map[pos.y][pos.x]; };
    auto rotate = [](auto dir, auto step){ return (dir + step) % 4; };

    auto next = [&]() {
        auto p = pos;
        p += move[dir];
        if (at(p) == ' ') {
            auto td = rotate(dir, 2);
            do { p += move[td]; } while (at(p) != ' ');
            p += move[dir];
        }
        return p;
    };
    auto print = [&]{
        char tmp{at(pos)};
        at(pos) = dir_char[dir];
        for (auto const& l: map) {
            std::cout << l << '\n';
        }
        at(pos) = tmp;
    };
    (void)print;

    while (at(pos) != '.') { pos += move[dir]; }
    
    int  dist;
    char turn;
    for (std::istringstream in(ins); in >> dist >> turn; ) {
        while (dist--) {
            auto np = next();
            if (at(np) == '#') {
                break;
            }
            pos = np;
        }
        dir = rotate(dir, turn == 'R'? 1: 3);
    }

    std::cout << (1000 * pos.y + 4 * pos.x + dir) << "\n";;
}
