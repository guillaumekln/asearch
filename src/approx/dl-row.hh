#ifndef DL_ROW_HH
# define DL_ROW_HH

# include <string>

/**
 * \brief DLRow class.
 *
 * This class represents a row of the matrix to compute the Damareau-Lenvenstein distance.
 * Like the trie itself, the matrix is implemented hierarchically. As we recurse
 * in the trie, for each character we add a row (1 row = 1 instance of DLRow)
 * that is connected to the parent row to avoid data duplication across all the children.
 */
class DLRow
{
public:
  /**
   * \brief Construct the first line of the matrix.
   *
   * \param width The width of the matrix (i.e. the length of the word to approximate + 1).
   * \param max_dist The maximal distance.
   */
  DLRow(size_t width, unsigned int max_dist);

  /**
   * \brief Construct a new row.
   *
   * It computes the distance between the word to approximate and the char
   * sequence that is build during the trie traversal.
   *
   * \param parent The parent, i.e. the previous row in the matrix.
   * \param word The word to approximate.
   * \param c The character associated to the row.
   * \param max_dist The maximal distance.
   */
  DLRow(const DLRow* parent,
        const std::string& word,
        char c,
        unsigned int max_dist);

  /**
   * \brief Get the distance computed so far.
   *
   * \return The current distance.
   */
  unsigned int get_dist() const;

  /**
   * \brief Get the complete word.
   *
   * It is the word in the trie that was compared to the word to approximate.
   *
   * \param word A reference to a string that will hold the result.
   */
  void get_word(std::string& word) const;

  /**
   * \brief Determine whether it useless to continue in this branch.
   *
   * In the current row, if all distances are greather than the maximal distance,
   * it will also be true in the next row, so we can just stop here in the trie traversal.
   *
   * \return true if we can stop, fals otherwise.
   */
  bool is_final() const;

  /**
   * \brief Get the offset of the current row in the total matrix.
   *
   * \return The offfset of the row.
   */
  size_t get_offset() const;

private:
  const DLRow* _parent;
  size_t _width;
  size_t _offset;
  unsigned int _dist[256];

  /**
   * Holds the index of the last column with a distance
   * lower or equal to the maximal distance.
   */
  int _maxcol;
  char _c;
};

# endif /* !DL_ROW_HH */
