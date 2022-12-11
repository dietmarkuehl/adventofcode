#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>
#include <cctype>
#include <deque>

struct BigInt {
    static constexpr unsigned long long scaleback{2ull * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23};
    unsigned long long value;

    BigInt(): value() {}
    explicit BigInt(unsigned long long value)
        : value(value) {
    }
    friend std::ostream& operator<< (std::ostream& out, BigInt const& b) {
        return out << b.value;
    }
    friend unsigned long long operator% (BigInt a, unsigned b) {
        return a.value % b;
    }
    friend BigInt operator* (BigInt a, BigInt b) {
        a.value = (a.value * b.value) % scaleback;
        return a;
    }
    friend BigInt operator+ (BigInt a, int b) {
        a.value += b;
        return a;
    }
    bool operator== (BigInt const&) const = default;
};

std::istream& equal(std::istream& in) {
    in.ignore(std::numeric_limits<std::streamsize>::max(), '=');
    return in;
}
std::istream& colon(std::istream& in) {
    in.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    return in;
}
std::istream& comma(std::istream& in) {
    in.ignore(1, ',');
    return in;
}
std::istream& ignore(std::istream& in) {
   std::string s;
   return in >> s;
}

struct monkey {
    std::deque<BigInt>         items;
    std::function<BigInt(BigInt)> operation;
    int                     test;
    std::array<int, 2>      where;
    unsigned long long      inspected{};
    int to(BigInt item) {
        return where[(item % test) != 0];
    }
    bool operator< (monkey const& m) const { return m.inspected < this->inspected; }

    friend std::istream& operator>>(std::istream& in, monkey& m) {
         m.items.clear();
         std::array<std::string, 6> lines;
         for (int i{}; i != 6; ++i) {
             std::getline(in >> std::ws, lines[i]);
         }
         if (auto lin = std::istringstream(lines[1]) >> colon) {
             for (int item; lin >> item; lin >> comma) {
                 m.items.push_back(BigInt(item));
             }
         }
         if (std::string name1, op, name2; std::istringstream(lines[2]) >> equal >> name1 >> op >> name2) {
             if (name2 == "old") {
                 m.operation = [](BigInt old)->BigInt{ return old * old; };
             }
             else if (int value; std::istringstream(name2) >> value) {
                  if (op == "+") {
                      m.operation = [value](BigInt old)->BigInt{ return old + value; };
                  }
                  else if (op == "*") {
                      m.operation = [value](BigInt old)->BigInt{ return old * BigInt(value); };
                  }
                  else {
                      std::cout << "unknown operation\n";
                      exit(1);
                  }
             }
             else {
                 std::cout << "unknown value\n";
                 exit(1);
             }
         }
         if (!(std::istringstream(lines[3]) >> colon >> ignore >> ignore >> m.test)
             || !(std::istringstream(lines[4]) >> colon >> ignore >> ignore >> ignore >> m.where[0])
             || !(std::istringstream(lines[5]) >> colon >> ignore >> ignore >> ignore >> m.where[1])) {
             in.setstate(std::ios::failbit);
         }
         return in;
    }
    friend std::ostream& operator<< (std::ostream& out, monkey const& m) {
        std::copy(m.items.begin(), m.items.end(), std::ostream_iterator<BigInt>(out, ", "));
        return out;
    }
};

int main() {
    std::vector<monkey> monkeys;
    std::copy(std::istream_iterator<monkey>(std::cin), std::istream_iterator<monkey>(),
              std::back_inserter(monkeys));
    for (int i{}; i != 10000; ++i) {
        for (monkey& m: monkeys) {
             while (!m.items.empty()) {
                 BigInt item(m.items.front());
                 m.items.pop_front();
                 ++m.inspected;
                 BigInt newitem(m.operation(item));
                 monkeys[m.to(newitem)].items.push_back(newitem);
             }
        }
    }
    std::sort(monkeys.begin(), monkeys.end());
    std::cout << (monkeys[0].inspected * monkeys[1].inspected) << "\n";
}
