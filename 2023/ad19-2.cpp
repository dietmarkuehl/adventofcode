#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <functional>
#include <numeric>
#include <bitset>

std::unordered_map<std::string, int> names{
    {"x", 0},
    {"m", 1},
    {"a", 2},
    {"s", 3},
    {"A", 4},
    {"R", 5}
};

enum condition { greater, less };
struct part
{
    using bs = std::bitset<4000>;
    std::array<bs, 4> bits{bs().flip(), bs().flip(), bs().flip(), bs().flip() };
};
using result = std::pair<std::uint64_t, part>;
using rule = std::function<result(part const&)>;
std::unordered_map<std::string, rule> workflows;

struct token
{
    rule        r;
    friend std::istream& operator>> (std::istream& in, token& t)
    {
        std::string name;
        for (std::istreambuf_iterator<char> it{in}, end{}; it != end; ++it)
        {
            switch (*it)
            {
            case '<':
            case '>':
                {
                    int index{names[name]};
                    condition cond{*it == '<'? condition::less: condition::greater};
                    ++it;
                    int value;
                    std::getline((in >> value).ignore(), name, ',');
                    if (name.back() == '}')
                        name.pop_back();
                    t.r = [index, cond, value, name](part const& p)->result{
                        part p0{p};
                        part p1{p};
                        if (cond == condition::less)
                        {
                            for (int i{0}; i < value - 1; ++i)
                                p1.bits[index].reset(i);
                            for (int i{value - 1}; i < 4000; ++i)
                                p0.bits[index].reset(i);
                        }
                        else
                        {
                            for (int i{0}; i < value; ++i)
                                p0.bits[index].reset(i);
                            for (int i{value}; i < 4000; ++i)
                                p1.bits[index].reset(i);
                        }

                        return { workflows[name](p0).first, p1 };
                    };
                    return in;
                }
                break;
            case '}':
            case ',':
                ++it;
                t.r = [name](part const& p){ return workflows[name](p); };
                return in;
            default:
                name.push_back(*it);
                break;
            }
        }
        in.setstate(std::ios_base::failbit);
        return in;
    }
};

int main() {
    workflows["in"] = [](part const&)->result{ return {}; };
    workflows["A"] = [](part const& p)->result{
        std::uint64_t factor{1};
        for (auto const& bs: p.bits)
        {
            factor *= bs.count();
        }
        return { factor, p };
    };
    workflows["R"] = [](part const& p)->result{ return {0, p}; };
    for (std::string line; std::getline(std::cin, line) && not line.empty(); )
    {
        std::istringstream lin(line);
        std::vector<rule> rules;

        std::string name;
        std::getline(lin, name, '{');
        for (token t; lin >> t; )
        {
            rules.push_back(t.r);
        }
        workflows[name] = [rules](part const& p)->result{
             std::uint64_t sum{};
             part          lp{p};
             for (rule const& r: rules)
             {
                 auto res{r(lp)};
                 sum += res.first;
                 lp = res.second;
             }
             return { sum, lp };
        };
    }
    std::cout << workflows["in"](part{}).first << "\n";
}
