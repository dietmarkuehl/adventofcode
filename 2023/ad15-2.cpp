#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

std::uint8_t hash(std::string_view s)
{
   std::uint8_t rc{};
   for (std::uint8_t c: s)
   {
       rc = 17 * (c + rc);
   }
   return rc;
}

int main() {
    std::uint64_t sum{};
    std::vector<std::pair<std::string, int>> boxes[256];
    for (std::string line; std::getline(std::cin, line); )
    {
        std::istringstream lin(line);
        for (std::string word; std::getline(lin, word, ','); )
        {
            auto it{std::find(word.begin(), word.end(), '=')};
            auto label{std::string_view(word.begin(), it - (it == word.end()? 1: 0))};
            auto h{hash(label)};
            auto lit{std::find_if(boxes[h].begin(), boxes[h].end(),
                         [label](auto const& p){
                             return p.first == label;
                         })};
            if (it == word.end())
            {
                if (lit != boxes[h].end())
                    boxes[h].erase(lit);
            }
            else
            {
                int f{std::stoi(std::string(it + 1, word.end()))};
                if (lit != boxes[h].end())
                    lit->second = f;
                else
                    boxes[h].emplace_back(std::string_view(word.begin(), it), f);
            }
        }
    }
    for (std::uint64_t i{}; i != 256; ++i)
    {
        for (std::size_t j{}; j != boxes[i].size(); ++j) {
            auto power = (i + 1ull) * (j + 1ull) * boxes[i][j].second;
            //std::cout << "box=" << i << " " << boxes[i][j].first << " -> " << power << "\n";
            sum += power;
        }
    }
    std::cout << sum << "\n";
}
