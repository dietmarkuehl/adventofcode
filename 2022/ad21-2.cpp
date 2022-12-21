#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>
#include <variant>

struct expr;
using res_fun = std::function<std::int64_t(std::int64_t)>;

struct result {
    std::variant<std::int64_t, res_fun> value;

    result(std::int64_t value): value(value) {}
    result(auto const& fun): value(fun) {}
    int index() const { return value.index(); }
    std::int64_t eval() const;

    result operator+ (result) const;
    result operator- (result) const;
    result operator* (result) const;
    result operator/ (result) const;

    friend std::ostream& operator<< (std::ostream& out, result const& r) {
        return out << r.eval();
    }
};

struct expr {
    std::function<result(std::unordered_map<std::string, expr> const&)> op;
    std::string ex0;
    std::string ex1;
    static result eval(std::string const& name, std::unordered_map<std::string, expr> const& names) {
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
            e.op = [value](std::unordered_map<std::string, expr> const&){ return result(value); };
        }
        return in;
    }
};

std::int64_t result::eval() const {
    if (this->value.index() == 0) {
        return std::get<0>(this->value);
    }
    else {
        return std::get<1>(this->value)(0);
    }
}

result result::operator+ (result other) const {
    if (this->index() == 0 && other.index() == 0) {
        return std::get<0>(this->value) + std::get<0>(other.value);
    }
    return res_fun([r0=*this, r1=other](std::int64_t value) {
        if (r0.index() == 0 && r1.index() == 1) {
            // r0 + fun(x) = value  => fun(x) == value - r0
            return std::get<1>(r1.value)(value - std::get<0>(r0.value));
        }
        else if (r0.index() == 1 && r1.index() == 0) {
            // fun(x) + r1 == value  => fun(x) = value - r1
            return std::get<1>(r0.value)(value - std::get<0>(r1.value));
        }
        else {
            std::cout << "op+" << "unexpected expression: r0=" << r0.index() << " r1=" << r1.index() << "\n";
            return std::int64_t();
        }
    });
}
result result::operator- (result other) const {
    if (this->index() == 0 && other.index() == 0) {
        return std::get<0>(this->value) - std::get<0>(other.value);
    }
    return res_fun([r0=*this, r1=other](std::int64_t value) {
        if (r0.index() == 0 && r1.index() == 1) {
            // r0 - fun(x) == value  => fun(x) == r0 - value
            return std::get<1>(r1.value)(std::get<0>(r0.value) - value);
        }
        else if (r0.index() == 1 && r1.index() == 0) {
            // fun(x) - r1 == value  => fun(x) == value + r1
            return std::get<1>(r0.value)(value + std::get<0>(r1.value));
        }
        else {
            std::cout << "op-" << "unexpected expression: r0=" << r0.index() << " r1=" << r1.index() << "\n";
            return std::int64_t();
        }
    });
}
result result::operator* (result other) const {
    if (this->index() == 0 && other.index() == 0) {
        return std::get<0>(this->value) * std::get<0>(other.value);
    }
    return res_fun([r0=*this, r1=other](std::int64_t value) {
        if (r0.index() == 0 && r1.index() == 1) {
            // r0 * fun(x) == value  => fun(x) == value / r0
            return std::get<1>(r1.value)(value / std::get<0>(r0.value));
        }
        else if (r0.index() == 1 && r1.index() == 0) {
            // fun(x) * r1 == value  => fun(x) == value / r1
            return std::get<1>(r0.value)(value / std::get<0>(r1.value));
        }
        else {
            std::cout << "op*" << "unexpected expression: r0=" << r0.index() << " r1=" << r1.index() << "\n";
            return std::int64_t();
        }
    });
}
result result::operator/ (result other) const {
    if (this->index() == 0 && other.index() == 0) {
        return std::get<0>(this->value) / std::get<0>(other.value);
    }
    return res_fun([r0=*this, r1=other](std::int64_t value) {
        if (r0.index() == 0 && r1.index() == 1) {
            // r0 / fun(x) == value  => fun(x) == r0 / value
            return std::get<1>(r1.value)(std::get<0>(r0.value) / value);
        }
        else if (r0.index() == 1 && r1.index() == 0) {
            // fun(x) / r1 == value => fun(x) == value * r1
            return std::get<1>(r0.value)(value * std::get<0>(r1.value));
        }
        else {
            std::cout << "op/" << "unexpected expression: r0=" << r0.index() << " r1=" << r1.index() << "\n";
            return std::int64_t();
        }
    });
}

int main() {
    std::unordered_map<std::string, expr> names;
    std::string  name;
    while (std::getline(std::cin >> std::ws, name, ':')) {
        if (name == "root") {
            std::string n0, n1;
            char op;
            if (std::cin >> n0 >> op >> n1) {
                names["root"].op = [n0, n1](std::unordered_map<std::string, expr> const& names) {
                    return result(res_fun([n0, n1, &names](int64_t) {
                        result r0 = expr::eval(n0, names);
                        result r1 = expr::eval(n1, names);
                        if (r0.index() == 0 && r1.index() == 1) {
                            return std::get<1>(r1.value)(std::get<0>(r0.value));
                        }
                        else if (r0.index() == 1 && r1.index() == 0) {
                            return std::get<1>(r0.value)(std::get<0>(r1.value));
                        }
                        else {
                            std::cout << "op==: " << "unexpected expression: r0=" << r0.index() << " r1=" << r1.index() << "\n";
                        }
                        return std::int64_t();
                    }));
                };
            }
            else {
                std::cout << "couldn't read root\n";
                break;
            }
        }
        else {
            expr exp;
            if (std::cin >> exp) {
                names[name] = exp;
            }
            else {
                std::cout << "couldn't read expr\n";
                break;
            }
        }
    }
    names["humn"].op = [](std::unordered_map<std::string, expr> const&) {
        return result([](int64_t value){
            return value;
        });
    };
    std::cout << expr::eval("root", names) << "\n";
}
