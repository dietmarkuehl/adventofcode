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

std::unordered_map<std::string, int> names{
    {"x", 0},
    {"m", 1},
    {"a", 2},
    {"s", 3},
    {"A", 4},
    {"R", 5}
};

enum condition { greater, less };
using part = std::array<int, 4>;
using result = std::optional<std::uint64_t>;
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
                       return (cond == condition::less? p[index] < value: p[index] > value)
                           ? workflows[name](p): result{};
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
    workflows["in"] = [](part const&){ return result{}; };
    workflows["A"] = [](part const& p){ return std::accumulate(p.begin(), p.end(), 0); };
    workflows["R"] = [](part const&){ return result{0}; };
    for (std::string line; std::getline(std::cin, line) && not line.empty(); )
    {
        std::istringstream lin(line);
        std::vector<rule> rules;

        // px{a<2006:qkq,m>2090:A,rfg}
        std::string name;
        std::getline(lin, name, '{');
        for (token t; lin >> t; )
        {
            rules.push_back(t.r);
        }
        workflows[name] = [rules](part const& p)->result{
             for (rule const& r: rules)
             {
                 auto res{r(p)};
                 if (res)
                     return res;
             }
             return {};
        };
    }
    //  {x=787,m=2655,a=1222,s=2876}
    std::uint64_t sum{};
    for (std::string line; std::getline(std::cin, line); )
    {
        std::istringstream lin(line);

        part p;
        for (std::size_t i{}; lin.ignore(100, '=') >> p[i]; ++i)
        {
        }
        auto res{workflows["in"](p).value_or(0u)};
        sum += res;
    }
    std::cout << sum << "\n";
}
