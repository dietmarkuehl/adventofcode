#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

std::istream& ignore(std::istream& in) {
    return in.ignore(1, ',');
}

struct droplet {
    int x{}, y{}, z{};
    friend std::istream& operator>>(std::istream& in, droplet& d) {
        in >> d.x >> ignore >> d.y >> ignore >> d.z;
        if (in) {
            ++d.x;
            ++d.y;
            ++d.z;
        }
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, droplet& d) {
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
        max.x = std::max(d.x + 2, max.x);
        max.y = std::max(d.y + 2, max.y);
        max.z = std::max(d.z + 2, max.z);
    }

    std::vector<std::vector<std::vector<bool>>> droplets(max.x,
        std::vector<std::vector<bool>>(max.y, std::vector<bool>(max.z)));
    for (droplet const& d: input) {
        droplets[d.x][d.y][d.z] = true;
    }
    std::size_t total{};
    for (droplet const& d: input) {
        total += std::size_t(droplets[d.x - 1][d.y][d.z] == false)
              +  std::size_t(droplets[d.x + 1][d.y][d.z] == false)
              +  std::size_t(droplets[d.x][d.y - 1][d.z] == false)
              +  std::size_t(droplets[d.x][d.y + 1][d.z] == false)
              +  std::size_t(droplets[d.x][d.y][d.z - 1] == false)
              +  std::size_t(droplets[d.x][d.y][d.z + 1] == false)
              ;
    }
    std::cout << total << "\n";
}
