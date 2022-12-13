#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

enum class order { none, less, equal, greater };
std::ostream& operator<< (std::ostream& out, order o) {
    switch (o) {
    case order::none: return out << "none";
    case order::less: return out << "less";
    case order::equal: return out << "equal";
    case order::greater: return out << "greater";
    }
}

enum class kind { value, list };
struct element {
    kind                 type{kind::value};                 
    int                  value{};
    std::vector<element> list;
    friend std::ostream& operator<< (std::ostream& out, element const& e) {
        if (e.type == kind::value) {
            return out << e.value;
        }
        else {
             out << "[";
             if (!e.list.empty()) {
                 out << e.list.front();
                 for (auto it = e.list.begin() + 1; it != e.list.end(); ++it) {
                    out << "," << *it;
                 }
             }
             return out << "]";
        }
    }
    bool operator< (element const& e) const {
        if (this->type == e.type) {
            return this->type == kind::value? this->value < e.value: this->list < e.list;
        }
        else if (this->type == kind::value) {
            return std::vector<element>(1, *this) < e.list;
        }
        else {
            return this->list < std::vector<element>(1, e);
        }
    }
};

std::string unpack(std::string s) {
    return !s.empty() && s[0] == '['? s.substr(1, s.size() - 2): s;
}

std::pair<std::string, std::string> extract(std::string const& s) {
    if (s.empty()) {
       return std::make_pair(s, s);
    }
    else if (s[0] == '[') {
        auto it = s.begin() + 1;
        int count = 1;
        for (; count; ++it) {
           if (*it == '[') {
               ++count;
           }
           else if (*it == ']') {
               --count;
           }
        }
        return std::make_pair(std::string(s.begin(), it), std::string(it + (it != s.end() && *it == ','? 1: 0),  s.end()));
    }
    else {
        auto it = std::find(s.begin(), s.end(), ',');
        return std::make_pair(std::string(s.begin(), it), std::string(it + (it != s.end()? 1: 0), s.end()));
    }
}

element parse(std::string part) {
    element e;
    if (part[0] == '[') {
        e.type = kind::list;
        part = unpack(part);
        while (!part.empty()) {
            auto[head, tail] = extract(part);
            e.list.push_back(parse(head));
            part = tail;
        }
    }
    else if (std::istringstream(part) >> e.value) {
    }
    else {
        std::cout << "failed to parse '" << part << "'\n";
    }
    return e;
}

int main() {
    int total{};
    int pair(1);
    for (std::string line1, line2; std::getline(std::getline(std::cin >> std::ws, line1), line2); ++pair) {
        //std::cout << "input\n" << line1 << "\n" << line2 << "\n";
        element e1 = parse(line1);
        element e2 = parse(line2);
        //std::cout << "elements\n" << e1 << "\n" << e2 << "\n\n";
        if (e1 < e2) {
            total += pair;
        }
    }
    std::cout << total << "\n";
}
