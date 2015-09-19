#include <iostream>
#include <fstream>
#include <string>
#include "ptrie.hh"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "usage: " << argv[0] << " /path/to/words.txt /path/to/dict.bin" << std::endl;
    return 1;
  }

  PTrie pt;

  std::ifstream in(argv[1]);
  std::string line;
  while (std::getline(in, line))
  {
    size_t delimiter = line.find_first_of('\t');
    std::string word(line, 0, delimiter);
    unsigned long freq = std::stoul(line.c_str() + delimiter);
    pt.add_word(word, freq);
  }

  pt.serialize(argv[2]);

  return 0;
}
