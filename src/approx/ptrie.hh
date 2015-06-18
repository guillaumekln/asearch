#ifndef PTRIE_HH
# define PTRIE_HH

typedef struct
{
  unsigned int length;
  char strs;
} s_trie;

typedef struct
{
  unsigned int offset;
  unsigned int length;
  unsigned int frequency;
  unsigned int children_count;
  unsigned int children_offset;
} s_edge;

/**
 * \brief Load the trie.
 *
 * \param filename The path to the serialized trie.
 * \return On success, a pointer to the trie struct, NULL pointer otherwise.
 */
s_trie* load(const char* filename);

/**
 * \brief Unload the trie.
 *
 * \param trie The trie to unload.
 * \return true on success, false otherwise.
 */
bool unload(s_trie* trie);

/**
 * \brief Get the root edge of the trie.
 *
 * \param trie The trie.
 * \return A pointer to the root edge.
 */
const s_edge* get_root(const s_trie* trie);

/**
 * \brief Get the child of an edge.
 *
 * \param edge The edge to get the child of.
 * \return A pointer to the leftmost child edge.
 */
const s_edge* get_child(const s_edge* edge);

/**
 * \brief Get a char sequence.
 *
 * \param trie The trie.
 * \param offset The offset of the char sequence.
 * \return A pointer to the char sequence in the sequence buffer.
 */
const char* get_str(const s_trie* trie, unsigned int offset);

# endif /* !PTRIE_HH */
