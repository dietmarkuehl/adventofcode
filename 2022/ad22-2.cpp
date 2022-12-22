#include <algorithm>
#include <fstream>
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
    if (!in) {
       std::cout << "error reading map\n";
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
    friend std::ostream& operator<< (std::ostream& out, position const& p) {
        return out << "(" << p.x << ", " << p.y << ")";
    }
};

int main(int ac, char* av[]) {
    std::ifstream in;
    if (ac == 2) {
        in.open(av[1]);
    }
    auto[tmap, ins] = read((ac == 2 && in)? in: std::cin);
    auto map = tmap;

    const int  size = (map[0].size() - 2) / 3;
    const char dir_char[] = { '>', 'v', '<', '^' };
    const position move[] = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
    position pos{ 1, 1 };
    int      dir{};

    auto at = [&map](position pos)->char& { return map[pos.y][pos.x]; };
    auto rotate = [](auto dir, auto step){ return (dir + step) % 4; };
    auto print = [&]{
        char tmp{at(pos)};
        at(pos) = dir_char[dir];
        for (auto const& l: map) {
            std::cout << l << '\n';
        }
        at(pos) = tmp;
    };
    (void)print;

    auto next = [&]() {
        auto p = pos;
        auto d = dir;
        p += move[d];
        if (at(p) == ' ') {
            switch (d) {
            case 0: // >
                //       6   6
                //      111 222
                //     5111 222 4
                //      111 222
                //      333  3
                //     5333 2 
                //   3  333
                //  555 444
                // 1555 444 2
                //  555 444
                //  666  6
                // 1666 4
                //  666
                //   2
                if (p.y <= 1 * size) {
                    d = rotate(d, 2);
                    p.x = 2 * size;
                    p.y = 2 * size + 1 * size - p.y + 1;
                    //std::cout << "case 1: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.y <= 2 * size) {
                    d = rotate(d, 3);
                    p.x = 2 * size + 2 * size - p.y;
                    p.y = 1 * size;
                    //std::cout << "case 2: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.y <= 3 * size) {
                    d = rotate(d, 2);
                    p.x = p.x + 1 * size - 1;
                    p.y = 3 * size - p.y + 1;
                    //std::cout << "case 3: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.y <= 4 * size) {
                    d = rotate(d, 3);
                    p.x = p.y - 3 * size + 1 * size;
                    p.y = 3 * size;
                    //std::cout << "case 4: " << p << " " << dir_char[d] << "\n";
                }
                else {
                   std::cout << "right out of bound: " << p.y << "\n";
                }
                break;
            case 1: // v
                //       6   6
                //      111 222
                //     5111 222 4
                //      111 222
                //      333  3
                //     5333 2 
                //   3  333
                //  555 444
                // 1555 444 2
                //  555 444
                //  666  6
                // 1666 4
                //  666
                //   2

                if (p.x <= 1 * size) {
                    d = rotate(d, 0);
                    p.x = 2 * size + p.x;
                    p.y = 1;
                    //std::cout << "case 1: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.x <= 2 * size) {
                    d = rotate(d, 1);
                    p.y = p.x - 1 * size + 3 * size; 
                    p.x = 1 * size;
                    //std::cout << "case 2: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.x <= 3 * size) {
                    d = rotate(d, 1);
                    p.y = p.x - 2 * size + 1 * size;
                    p.x = 2 * size;
                    //std::cout << "case 3: " << p << " " << dir_char[d] << "\n";
                }
                else  {
                   std::cout << "down out of bound: " << p.x << "\n";
                }
                break;
            case 2: // <
                //       6   6
                //      111 222
                //     5111 222 4
                //      111 222
                //      333  3
                //     5333 2 
                //   3  333
                //  555 444
                // 1555 444 2
                //  555 444
                //  666  6
                // 1666 4
                //  666
                //   2
                if (p.y <= 1 * size) {
                    d = rotate(d, 2);
                    p.y = 1 * size - p.y + 2 * size + 1;
                    p.x = 1;
                    std::cout << "case 1: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.y <= 2 * size) {
                    d = rotate(d, 3);
                    p.x = p.y - 1 * size;
                    p.y = 2 * size + 1;
                    //std::cout << "case 2: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.y <= 3 * size) {
                    d = rotate(d, 2);
                    p.x = 1 * size + 1;
                    p.y = 3 * size - p.y + 1;
                    //std::cout << "case 3: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.y <= 4 * size) {
                    d = rotate(d, 3);
                    p.x = p.y - 3 * size + 1 * size;
                    p.y = 1;
                    //std::cout << "case 4: " << p << " " << dir_char[d] << "\n";
                }
                else {
                    std::cout << "left out of bounds: " << p.y << "\n";
                }
                break;
            case 3: // ^
                //       6   6
                //      111 222
                //     5111 222 4
                //      111 222
                //      333  3
                //     5333 2 
                //   3  333
                //  555 444
                // 1555 444 2
                //  555 444
                //  666  6
                // 1666 4
                //  666
                //   2
                if (p.x <= 1 * size) {
                    d = rotate(d, 1);
                    p.y = p.x - 0 * size + 1 * size;
                    p.x = 1 * size + 1;
                    //std::cout << "case 1: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.x <= 2 * size) {
                    d = rotate(d, 1);
                    p.y = p.x - size + 3 * size;
                    p.x = 1;
                    //std::cout << "case 2: " << p << " " << dir_char[d] << "\n";
                }
                else if (p.x <= 3 * size) {
                    d = rotate(d, 0);
                    p.y = 4 * size;
                    p.x = p.x - 2 * size;
                    //std::cout << "case 3: " << p << " " << dir_char[d] << "\n";
                }
                else {
                    std::cout << "up out of bounds: " << p.x << "\n";
                }
                break;
            }
        }
        return std::pair(p, d);
    };
    auto forward = [&](int dist) {
        while (dist--) {
            auto[np, nd] = next();
            if (at(np) == '#') {
                break;
            }
            pos = np;
            dir = nd;
            at(pos) = dir_char[dir];
        }
    };
    auto advance = [&](int dist, char turn) {
        forward(dist);
        dir = rotate(dir, turn == 'R'? 1: 3);
    };
    while (at(pos) != '.') { pos += move[dir]; }

    auto test = [&](int x, int y, int d) {
        pos = position{x, y};
        dir = d;

        at(pos) = dir_char[dir];
        auto tmp = map;
        at(pos) = '.';

        auto[np, nd] = next();
        pos = np;
        dir = nd;
        at(pos) = dir_char[dir];

        for (std::size_t i{}; i != tmp.size(); ++i) {
            std::cout << tmp[i] << "    " << map[i] << "\n";
        }

        at(pos) = '.';
    };
    (void)test;
    //  12
    //  3
    // 54
    // 6
    // right:
    //test(12,  2, 0);
    //test( 8,  6, 0);
    //test( 8, 10, 0);
    //test( 4, 14, 0);
    // down
    //test( 2, 16, 1);
    //test( 6, 12, 1);
    //test(10,  4, 1);
    // left
    //test( 5, 2, 2);
    //test( 5, 6, 2);
    //test( 1, 10, 2);
    //test( 1, 14, 2);
    // up
    //test( 1, 9, 3);
    //test( 6, 1, 3);
    //test(10, 1, 3);

    int  dist;
    char turn;
    for (std::istringstream in(ins); in >> dist; ) {
        if (in >> turn) {
            advance(dist, turn);
        }
        else {
            forward(dist);
        }
    }

    std::cout << "x=" << pos.x << " y=" << pos.y << " dir=" << dir << "\n";
    std::cout << (1000 * pos.y + 4 * pos.x + dir) << "\n";;
}
