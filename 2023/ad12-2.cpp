#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>
#include <span>

struct fmt
{
    std::span<std::int64_t> v;
    fmt(std::span<std::int64_t> v): v(v) {}
    friend std::ostream& operator<< (std::ostream& out, fmt const& f)
    {
        for (auto x: f.v)
            out << x << " ";
        return out;
    }
};

std::int64_t check(auto& memo, std::string_view damage, std::span<std::int64_t> counts, std::span<std::int64_t> tail);

std::int64_t check_aux(auto& memo, std::string_view damage, std::span<std::int64_t> counts, std::span<std::int64_t> tail)
{
    auto it{std::find_if(damage.begin(), damage.end(), [](char c){ return c != '.'; })};
    if (counts.empty())
        return std::find_if(it, damage.end(), [](char c){ return c == '#'; }) == damage.end();

    std::int64_t sum{};
    if (it != damage.end())
    {
        if (std::distance(it, damage.end()) < tail.front())
        {
            return 0;
        }
        if (*it == '?')
        {
            sum += check(memo, std::string_view(it + 1, damage.end()), counts, tail);
        }
        if (counts.front() <= std::distance(it, damage.end()))
        {
            auto nit = it + counts.front();
            if (std::find(it, nit, '.') != nit)
                return sum;
            if (nit == damage.end())
                return sum + (counts.size() == 1);
            if (*nit == '#')
                return sum;

            sum += check(memo,
                         std::string_view(nit + 1, damage.end()),
                         std::span(counts.begin() + 1, counts.end()),
                         std::span(tail.begin() + 1, tail.end()));
        }
    }
    return sum;
}

std::int64_t check(auto& memo, std::string_view damage, std::span<std::int64_t> counts, std::span<std::int64_t> tail)
{
    std::int64_t& rc = memo[damage.size()][counts.size()];
    if (rc == -1)
        rc = check_aux(memo, damage, counts, tail);
    return rc;
}

int main() {
    std::int64_t sum{};
    for (std::string line; std::getline(std::cin, line); )
    {
        std::replace(line.begin(), line.end(), ',', ' ');
        std::istringstream lin(line);
        std::string damage;
        lin >> damage;
        std::vector<std::int64_t> counts{std::istream_iterator<std::int64_t>(lin), {}};

        std::string      dtmp(damage);
        std::vector<std::int64_t> ctmp(counts);
        for (std::int64_t i{0}; i != 4; ++i)
        {
            damage += '?' + dtmp;
            counts.insert(counts.end(), ctmp.begin(), ctmp.end());
        }
        std::vector<std::int64_t> tail;
        std::int64_t tsum{};
        for (auto it = counts.rbegin(); it != counts.rend(); ++it)
        {
            tsum += *it;
            tail.push_back(tsum);
            tsum += 1;
        }
        std::reverse(tail.begin(), tail.end());
        std::vector<std::vector<std::int64_t>> memo(damage.size() + 1, std::vector<std::int64_t>(counts.size() + 1, -1));
        std::int64_t rc = check(memo, damage, counts, tail);
        sum += rc;
    }
    std::cout << sum << "\n";
}
