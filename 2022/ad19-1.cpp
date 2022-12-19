#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

// ----------------------------------------------------------------------------

std::istream& colon(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), ':');
}

std::istream& skip(std::istream& in) {
    std::string dummy;
    return in >> dummy;
}

// ----------------------------------------------------------------------------

constexpr int max_time{24};
std::string const material[] = { "ore", "clay", "obsidian", "geode" };
std::unordered_map<std::string, int> type = []{
    std::unordered_map<std::string, int> rc;
    int index{};
    for (auto const& m: material) {
        rc[m] = index++;
    }
    return rc;
}();

struct blueprint {
     // cost[what-to-make][what-is-needed]
     std::array<std::array<int, 4>, 4> costs{};

     friend std::istream& operator>> (std::istream& in, blueprint& b) {
         std::string line;
         std::getline(in, line);

         std::istringstream sin(line);
         sin >> colon;
         for (std::string ms; std::getline(sin, ms, '.'); ) {
             std::istringstream min(ms);
             std::string to;
             min >> skip >> to >> skip;
             int amount;
             std::string from;
             while (min >> skip >> amount >> from) {
                 b.costs[type[to]][type[from]] = amount;
             }
         }

         return in;
     }
     friend std::ostream& operator<< (std::ostream& out, blueprint const& b) {
         for (int i{}; i != 4; ++i) {
             out << material[i] << ": ";
             for (int j{}; j != 4; ++j) {
                 out << b.costs[i][j] << " " << material[j] << ", ";
             }
             out << "\n";
         }
         return out;
     }
};

struct factory {
    std::array<int, 4>  robots{1, 0, 0, 0};
    std::array<int, 4>  material{};
    std::array<bool, 4> possible{false, false, false, false};
    int                 num_possible{};

    factory(blueprint const& b) { update_possible(b); }
    bool   test_robot(blueprint const& b, int mat) const {
        if (!possible[mat]) {
            return false;
        }
        for (int i{}; i != 3; ++i) {
            if (material[i] < b.costs[mat][i]) {
                return false;
            }
        }
        return true;
    }
    void update_possible(blueprint const& b) {
        for (int i{}; i != 4; ++i) {
            if (!this->possible[i]) {
                this->possible[i] = true;
                for (int j{}; j != 4; ++j) {
                    if (this->robots[j] == 0 && b.costs[i][j] != 0) {
                        this->possible[i] = false;
                    }
                }
                if (this->possible[i]) {
                    ++this->num_possible;
                }
            }
        }
    }
    factory make_robot(blueprint const& b, int mat) const {
        factory rc(*this);
        for (int i{}; i != 3; ++i) {
            rc.material[i] -= b.costs[mat][i];
        }
        ++rc.robots[mat];
        rc.update_possible(b);

        return rc;
    }
    factory next() const {
        factory rc(*this);
        for (int i{}; i != 4; ++i) {
            rc.material[i] += robots[i];
        }
        return rc;
    }

    friend std::ostream& operator<< (std::ostream& out, factory const& f) {
        out << "robots: ";
        for (int i{}; i != 4; ++i) {
            out << ::material[i] << "=" << f.robots[i] << " ";
        }
        out << "material: ";
        for (int i{}; i != 4; ++i) {
            out << ::material[i] << "=" << f.material[i] << " ";
        }
        out << "possible(" << f.num_possible << "): ";
        for (int i{}; i != 4; ++i) {
            if (f.possible[i]) {
                out << ::material[i] << " ";
            }
        }
        return out;
    }
};

int process(blueprint const& b, factory f, int time) {
    if (time == max_time) {
        return f.material[3];
    }
    int max = 0;
    int build{};
    for (int i{4}; i-- != 0; ) {
        if (f.test_robot(b, i)) {
            ++build;
            max = std::max(max, process(b, f.make_robot(b, i), time));
        }
    }
    if (build < f.num_possible) {
        max = std::max(max, process(b, f.next(), time + 1));
    }
    return max;
}

int main() {
    std::cout << std::unitbuf;
    std::vector<blueprint> bps(std::istream_iterator<blueprint>(std::cin), std::istream_iterator<blueprint>{});
    for (auto const& b: bps) {
        std::cout << "blueprint:\n" << b << "\n";
        std::cout << process(b, factory(b), 0) << "\n";
    }
}
