#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

struct expr {
    std::function<std::int64_t(std::unordered_map<std::string, expr> const&)> op;
    std::string ex0;
    std::string ex1;
    static std::int64_t eval(std::string const& name, std::unordered_map<std::string, expr> const& names) {
        auto it = names.find(name);
        if (it == names.end()) {
            std::cout << "name not found '" << name << "'\n";
            return -1;
        }
        return it->second.op(names);
    }
    friend std::istream& operator>> (std::istream& in, expr& e) {
        if (std::int64_t value; !(in >> value)) {
            in.clear();
            char op;
            std::string ex0, ex1;
            if (in >> ex0 >> op >> ex1) {
                switch (op) {
                default:
                    std::cout << "unexpected op '" << op << "'\n";
                    break;
                case '+': 
                    e.op = [ex0, ex1](std::unordered_map<std::string, expr> const& names){
                        return eval(ex0, names) + eval(ex1, names);
                    };
                    break;
                case '-': 
                    e.op = [ex0, ex1](std::unordered_map<std::string, expr> const& names){
                        return eval(ex0, names) - eval(ex1, names);
                    };
                    break;
                case '*': 
                    e.op = [ex0, ex1](std::unordered_map<std::string, expr> const& names){
                        return eval(ex0, names) * eval(ex1, names);
                    };
                    break;
                case '/': 
                    e.op = [ex0, ex1](std::unordered_map<std::string, expr> const& names){
                        return eval(ex0, names) / eval(ex1, names);
                    };
                    break;
                }
            }
        }
        else {
            e.op = [value](std::unordered_map<std::string, expr> const&){ return value; };
        }
        return in;
    }
};

int main() {
    std::cout << std::unitbuf;
    std::unordered_map<std::string, expr> names;
    std::string  name;
    expr         expr;
    while (std::getline(std::cin >> std::ws, name, ':') >> expr) {
        names[name] = expr;
    }
    std::cout << expr::eval("root", names) << "\n";
}
