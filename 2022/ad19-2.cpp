#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <initializer_list>

// ----------------------------------------------------------------------------

std::istream& colon(std::istream& in) {
    return in.ignore(std::numeric_limits<std::streamsize>::max(), ':');
}

std::istream& skip(std::istream& in) {
    std::string dummy;
    return in >> dummy;
}

// ----------------------------------------------------------------------------

namespace ad {
    template <typename T, int S>
    class vector {
        std::array<T, S> values{};
    public:
        using value_type = T;
        vector() = default;
        vector(std::initializer_list<T> list){ assert(list.size() == 4); std::copy(list.begin(), list.end(), this->begin()); }
        auto begin() { return this->values.begin(); }
        auto end() { return this->values.end(); }
        auto begin() const { return this->values.begin(); }
        auto end() const { return this->values.end(); }
        T&       operator[](int index) { return this->values[index]; }
        T const& operator[](int index) const { return this->values[index]; }

        vector& operator+= (vector const& other) { std::transform(this->begin(), this->end(), other.begin(), this->begin(), std::plus<>()); return *this; }
        vector& operator-= (vector const& other) { std::transform(this->begin(), this->end(), other.begin(), this->begin(), std::minus<>()); return *this; }

        vector<bool, S> operator!() const {
            vector<bool, S> rc;
            std::transform(this->begin(), this->end(), rc.begin(), [](bool b){ return !b; });
            return rc;
        }
        vector<bool, S> operator== (vector const& other) const {
            vector<bool, S> rc;
            std::transform(this->begin(), this->end(), other.begin(), rc.begin(), std::equal_to<>());
            return rc;
        }
        vector<bool, S> operator!= (vector const& other) const {
            vector<bool, S> rc;
            std::transform(this->begin(), this->end(), other.begin(), rc.begin(), [](auto const& a, auto const& b){ return a != b; });
            return rc;
        }
        vector<bool, S> operator== (value_type const& other) const {
            vector<bool, S> rc;
            std::transform(this->begin(), this->end(), rc.begin(), [&other](vector const& v){ return v == other; });
            return rc;
        }
        vector<bool, S> operator!= (value_type const& other) const {
            vector<bool, S> rc;
            std::transform(this->begin(), this->end(), rc.begin(), [&other](auto const& v){ return any(v != other); });
            return rc;
        }
        vector<bool, S> operator< (vector const& other) const {
            vector<bool, S> rc;
            std::transform(this->begin(), this->end(), other.begin(), rc.begin(), std::less<>());
            return rc;
        }
        vector<bool, S> operator< (value_type const& other) const {
            vector<bool, S> rc;
            std::transform(this->begin(), this->end(), rc.begin(), [&other](auto const& v) { return all(v < other); });
            return rc;
        }
        friend vector<bool, S> operator< (value_type const& a, vector const& b) {
            vector<bool, S> rc;
            std::transform(b.begin(), b.end(), rc.begin(), [&a](auto const& v) { return all(a < v); });
            return rc;
        }
        vector<bool, S> operator<= (vector const& other) const { return !(other < *this); }
        vector<bool, S> operator<= (value_type const& other) const { return !(other < *this); }

        friend vector operator+(vector v, vector const& w) { return v += w; }
        friend bool all(vector const& v) { return std::all_of(v.values.begin(), v.values.end(), [](auto v){ return v; }); }
        friend bool any(vector const& v) { return std::any_of(v.values.begin(), v.values.end(), [](auto v){ return v; }); }
        explicit operator bool() const { return all(*this); }
        friend vector max(vector v, vector const& w) {
            std::transform(v.begin(), v.end(), w.begin(), v.begin(), [](auto a, auto b){ return std::max(a, b); });
            return v;
        }

        friend std::ostream& operator<< (std::ostream& out, vector const& v) {
            out << "[";
            std::copy(v.begin(), v.end() -1, std::ostream_iterator<T>(out, ", "));
            return out << *(v.end() - 1) << "]";
        }
    };
}

void test_vector() {
    ad::vector<int, 4> v{1, 1, 1, 1};
    ad::vector<int, 4> w{1, 2, 3, 4};
    ad::vector<int, 4> x{2, 2, 3, 4};
    std::cout << "v=" << v << "\n";
    std::cout << "w=" << w << "\n";
    std::cout << "(v+w)=" << (v + w) << "\n";
    std::cout << "(v<w)=" << std::boolalpha << (v < w) << "\n";
    std::cout << "all(v<w)=" << std::boolalpha << all(v < w) << "\n";
    std::cout << "all(v<x)=" << std::boolalpha << all(v < x) << "\n";
}

// ----------------------------------------------------------------------------

constexpr int max_time{32}; // {24};
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
     int                               id;
     // cost[what-to-make][what-is-needed]
     ad::vector<ad::vector<int, 4>, 4> costs{};
     ad::vector<int, 4>                max_costs{};
     int                               max_geode{0};

     friend std::istream& operator>> (std::istream& in, blueprint& b) {
         std::string line;
         std::getline(in, line);

         std::istringstream sin(line);
         std::string dummy;
         sin >> dummy >> b.id >> colon;
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

         for (int i{}; i != 4; ++i) {
             b.max_costs = max(b.max_costs, b.costs[i]);
         }
         b.max_costs[3] = max_time;

         return in;
     }
     friend std::ostream& operator<< (std::ostream& out, blueprint const& b) {
         out << "id=" << b.id << "\n";
         for (int i{}; i != 4; ++i) {
             out << material[i] << ": ";
             for (int j{}; j != 4; ++j) {
                 out << b.costs[i][j] << " " << material[j] << ", ";
             }
             out << "\n";
         }
         out << "max:";
         for (int i{}; i != 4; ++i) {
             out << " " << b.max_costs[i];
         }
         out << "\n";
         return out;
     }
};

struct factory {
    blueprint&          bp;
    ad::vector<int, 4>  robots{1, 0, 0, 0};
    ad::vector<int, 4>  material{};

    factory(blueprint& b): bp(b) {}

    factory next() {
         factory rc(*this);
         rc.material += robots;
         bp.max_geode = std::max(bp.max_geode, rc.material[3]);
         return rc;
    }
    factory build(int i) {
        factory rc(this->next());
        ++rc.robots[i];
        rc.material -= rc.bp.costs[i];
        return rc;
    }

    friend std::ostream& operator<< (std::ostream& out, factory const& f) {
        out << "robots: " << f.robots << " ";
        out << "material: " << f.material << " ";
        return out;
    }
    bool all_enough() {
        for (int i{}; i != 4; ++i) {
            // if ((this->bp.costs[i] != ad::vector<int, 4>{}) == (this->robots != ad::vector<int, 4>{})) {
            //     if (any(this->material < this->bp.costs[i])) {
            for (int j{}; j != 4; ++j) {
                if (this->bp.costs[i][j] != this->material[j] && this->robots[j] != 0) {
                    return false;
                }
            }
        }
        return true;
    }
};

std::array<int, max_time + 1> const max_add{[]{
    std::array<int, max_time + 1> rc{};
    int sum{};
    for (int i{0}; i != max_time; ++i) {
        rc[i + 1] = (sum += i);
    }
    return rc;
}()};

int process(factory f, int time, ad::vector<bool, 4> delayed = {}) {
    if (time == max_time
        || (max_time - time < 10 && max_add[max_time - time] + f.robots[3] * (max_time - time) + f.material[3] <= f.bp.max_geode)
        ) {
        //std::cout << f << "\n";
        return f.material[3];
    }

    int max{};
    for (int i{0}; i != 4; ++i) {
        if (all(f.bp.costs[i] <= f.material) && f.robots[i] < f.bp.max_costs[i]) {
            if (!delayed[i]) {
            max = std::max(max, process(f.build(i), time + 1));
            }
        }
    }
    if (!f.all_enough()) {
        ad::vector<bool, 4> nd{};
        for (int i{}; i != 4; ++i) {
            if (f.bp.costs[i] <= f.material) {
                nd[i] = true;
            }
        }
        max = std::max(max, process(f.next(), time + 1, nd));
    }
    return max;
}

int main() {
    std::cout << std::unitbuf;
    std::vector<blueprint> bps(std::istream_iterator<blueprint>(std::cin), std::istream_iterator<blueprint>{});
    int total{1};
    for (std::size_t i{}; i != std::min(std::size_t{3}, bps.size()); ++i) {
        int result{process(factory(bps[i]), 0)};
        total *= result;
    }
    std::cout << total << "\n";
}
