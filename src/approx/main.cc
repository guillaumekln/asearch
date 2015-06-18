#include <cstdlib>
#include <climits>
#include <string>
#include <iostream>
#include "ptrie.hh"
#include "approx.hh"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "usage: " << argv[0] << " /path/to/dict.bin" << std::endl;
    return 1;
  }

  s_trie* trie = load(argv[1]);

  if (trie == NULL)
    return 1;

  Approx approx(trie);

  std::string line;
  size_t delimiter;

  while (std::getline(std::cin, line))
  {
    // Get the command.
    delimiter = line.find_first_of(' ');
    if (delimiter == std::string::npos)
      continue;

    std::string cmd(line, 0, delimiter);
    if (cmd != "approx")
      continue;
    line.erase(0, delimiter + 1);

    // Get the maximal distance.
    delimiter = line.find_first_of(' ');
    if (delimiter == std::string::npos)
      continue;

    std::string strdist(line, 0, delimiter);
    char* offset;
    unsigned long dist = strtoul(strdist.c_str(), &offset, 10);
    if (offset == strdist.c_str() || dist == ULONG_MAX)
      continue;

    // Get the word to approximate.
    line.erase(0, delimiter + 1);
    if (line.empty())
      continue;

    approx.search(line, dist);
  }

  unload(trie);
  return 0;
}
