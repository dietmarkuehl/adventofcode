#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

int match(auto access, unsigned width, unsigned height)
{
    int rc{-1};
    auto comp = [&access, width](auto r0, auto r1){
        for (unsigned c{}; c != width; ++c)
            if (access(c, r0) != access(c, r1))
                return false;
        return true;
    };
    for (unsigned r{}; r != height - 1u; ++r)
    {
        if (comp(r, r + 1))
        {
            bool match{true};
            for (unsigned i{r}; 0 < i && r + 1 + (r - i + 1) < height; )
            {
                --i;
                if (!comp(i, r + 1 + (r - i)))
                {
                    match = false;
                    break;
                }
            }
            if (match)
                rc = r;
        }
    }
    return rc;
}

std::uint64_t compute(auto const& block)
{
    int v{match([&block](unsigned c, unsigned r){ return block[r][c]; }, block[0].size(), block.size())};
    int h{match([&block](unsigned r, unsigned c){ return block[r][c]; }, block.size(), block[0].size())};
    return 100 * (v + 1) + h + 1;
}

int main() {
    std::cout << std::unitbuf;
    std::uint64_t sum{};
    std::vector<std::string> block;
    for (std::string line; std::getline(std::cin, line); )
    {
        if (line.empty())
        {
            sum += compute(block);
            block.clear();
        }
        else
        {
            block.push_back(line);
        }
    }
    std::cout << sum << "\n";
}
