#include "ptrie.hh"

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <iostream>

static size_t g_data_size = 0;

s_trie* load(const char* filename)
{
  int fd;
  struct stat sbuf;

  // Open file.
  if ( (fd = open(filename, O_RDONLY)) == -1)
  {
    std::cerr << "open failed." << std::endl;
    return NULL;
  }

  // Get file size.
  if (stat(filename, &sbuf) == -1)
  {
    std::cerr << "stat failed." << std::endl;
    return NULL;
  }

  // Map file in memory
  g_data_size = sbuf.st_size;
  void* map = mmap(0, g_data_size, PROT_READ, MAP_SHARED, fd, 0);

  if (map == MAP_FAILED)
  {
    std::cerr << "mmap failed." << std::endl;
    return NULL;
  }

  s_trie* trie = (s_trie*) map;

  // Close file.
  if (close(fd) == -1)
  {
    unload(trie);
    std::cerr << "close failed." << std::endl;
    return NULL;
  }

  return trie;
}

bool unload(s_trie* trie)
{
  if (munmap(trie, g_data_size) == -1)
  {
    std::cerr << "munmap failed." << std::endl;
    return false;
  }

  return true;
}

const s_edge* get_root(const s_trie* trie)
{
  return (const s_edge*) (&trie->strs + trie->length);
}

const s_edge* get_child(const s_edge* edge)
{
  return edge + edge->children_offset;
}

const char* get_str(const s_trie* trie, unsigned int offset)
{
  return &trie->strs + offset;
}
