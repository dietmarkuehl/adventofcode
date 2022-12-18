#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <queue>

std::istream& ignore(std::istream& in) {
    return in.ignore(1, ',');
}

struct droplet {
    unsigned int x{}, y{}, z{};
    friend std::istream& operator>>(std::istream& in, droplet& d) {
        in >> d.x >> ignore >> d.y >> ignore >> d.z;
        if (in) {
            ++d.x;
            ++d.y;
            ++d.z;
        }
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, droplet const& d) {
        return out << "(" << d.x << ", " << d.y << ", " << d.z << ")";
    }
};

int main(int ac, char* av[]) {
    droplet max{};
    std::ifstream fin(ac == 2? av[1]: "");
    std::istream& in(fin? fin: std::cin);
    std::vector<droplet> input(std::istream_iterator<droplet>(in),
                               std::istream_iterator<droplet>{});
    for (droplet const& d: input) {
        max.x = std::max(d.x, max.x);
        max.y = std::max(d.y, max.y);
        max.z = std::max(d.z, max.z);
    }
    std::vector<std::vector<std::vector<bool>>> droplets(max.x + 2,
        std::vector<std::vector<bool>>(max.y + 2, std::vector<bool>(max.z + 2)));
    std::vector<std::vector<std::vector<bool>>> water(droplets);

    for (droplet const& d: input) {
        droplets[d.x][d.y][d.z] = true;
    }
    std::queue<droplet> q;
    auto push = [&](droplet const& d) {
        if (0 <= d.x && d.x < water.size()
            && 0 <= d.y && d.y < water[d.x].size()
            && 0 <= d.z && d.z < water[d.x][d.y].size()
            && !water[d.x][d.y][d.z] && !droplets[d.x][d.y][d.z])
        {
            water[d.x][d.y][d.z] = true;
            q.push(d);
        }
    };

    push(droplet{});
    while (!q.empty()) {
        droplet d{q.front()};

        q.pop();
        push({ d.x - 1, d.y, d.z });
        push({ d.x + 1, d.y, d.z });
        push({ d.x, d.y - 1, d.z });
        push({ d.x, d.y + 1, d.z });
        push({ d.x, d.y, d.z - 1 });
        push({ d.x, d.y, d.z + 1 });
    }
    std::size_t total{};
    for (droplet const& d: input) {
        total += std::size_t(water[d.x - 1][d.y][d.z])
              +  std::size_t(water[d.x + 1][d.y][d.z])
              +  std::size_t(water[d.x][d.y - 1][d.z])
              +  std::size_t(water[d.x][d.y + 1][d.z])
              +  std::size_t(water[d.x][d.y][d.z - 1])
              +  std::size_t(water[d.x][d.y][d.z + 1])
              ;
    }
    std::cout << total << "\n";
}
