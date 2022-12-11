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
    std::deque<int>         items;
    std::function<int(int)> operation;
    int                     test;
    std::array<int, 2>      where;
    int                     inspected{};
    int to(int item) {
        return where[bool(item % test)];
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
                 m.items.push_back(item);
             }
         }
         if (std::string name1, op, name2; std::istringstream(lines[2]) >> equal >> name1 >> op >> name2) {
             if (name2 == "old") {
                 m.operation = [](int old){ return old * old; };
             }
             else if (int value; std::istringstream(name2) >> value) {
                  if (op == "+") {
                      m.operation = [value](int old){ return old + value; };
                  }
                  else if (op == "*") {
                      m.operation = [value](int old){ return old * value; };
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
        std::copy(m.items.begin(), m.items.end(), std::ostream_iterator<int>(out, ", "));
        return out;
    }
};

int main() {
    std::vector<monkey> monkeys;
    std::copy(std::istream_iterator<monkey>(std::cin), std::istream_iterator<monkey>(),
              std::back_inserter(monkeys));
    for (int i{}; i != 20; ++i) {
        for (monkey& m: monkeys) {
             while (!m.items.empty()) {
                 int item(m.items.front());
                 m.items.pop_front();
                 ++m.inspected;
                 int newitem(m.operation(item) / 3);
                 monkeys[m.to(newitem)].items.push_back(newitem);
             }
        }
    }
    std::sort(monkeys.begin(), monkeys.end());
    std::cout << (monkeys[0].inspected * monkeys[1].inspected) << "\n";
}
