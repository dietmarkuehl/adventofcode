#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

std::uint8_t hash(std::string const& s)
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
    for (std::string line; std::getline(std::cin, line); )
    {
        std::istringstream lin(line);
        for (std::string word; std::getline(lin, word, ','); )
        {
            sum += hash(word);
        }
    }
    std::cout << sum << "\n";
}
