#ifndef APPROX_HH
# define APPROX_HH

# include <string>
# include <vector>

# include "ptrie.hh"
# include "dl-matrix.hh"

class Approx
{
public:
  /**
   * \brief The class used to store a result and that implements operator<.
   */
  class Result
  {
  public:
    /**
     * \brief Construct a result.
     *
     * \param mat The matrix row that holds the word.
     * \param frequency The frequency of the word.
     * \param distance The distance between this word and the word to approximate.
     */
    Result(const DLMatrix& mat, unsigned int frequency, unsigned int distance);

    /**
     * \brief Order the result.
     *
     * The result must be ordered:
     * - by increasing distance;
     * - then by decreasing frequency;
     * - then by increasing lexicographical order.
     */
    bool operator<(const Result& res) const;

    /**
     * \brief Output the result in the JSON format.
     *
     * Example: {"word":"test","freq":49216987,"distance":0}
     */
    void dump() const;

  private:
    std::string _word;
    unsigned int _frequency;
    unsigned int _distance;
  };

  /**
   * \brief Construct an approx object.
   *
   * \param trie The trie to work with.
   */
  Approx(const s_trie* trie);

  /**
   * \brief Approximative search.
   *
   * This function outputs on std::cout all words in the trie with a distance to
   * the word argument lower or equal to the max_dist argument.
   *
   * The output is a JSON array, for example:
   * [{"word":"test","freq":49216987,"distance":0},{"word":"est","freq":1991137112,"distance":1}]
   * followed by a newline.
   *
   * \param word The word to approximate.
   * \param max_dist The maximal distance.
   */
  void search(std::string& word, unsigned int max_dist);

private:
  const s_trie* _trie;

  /*
   * Root edge: offset = 0, length = 0, frequency = 0
   * get_child(_root) = the leftmost edge of the root node
   */
  const s_edge* _root;

  unsigned int _max_dist;
  std::string _word;

  std::vector<Result> _results;

  /**
   * \brief Handle a string sequence from the trie.
   *
   * It will recursively create a new row for each character to compute the distance.
   *
   * \param parent The previous row.
   * \param child The current child in the trie.
   * \param str The current sequence.
   * \param offset The current offset in the sequence.
   */
  void handle_sequence(const DLMatrix* parent,
                       const s_edge* child,
                       const char* str,
                       size_t offset);


  /**
   * \brief Recursive approximative search.
   *
   * This function recursively find all the words that match the condition.
   *
   * \param edge The edge to recurse on.
   * \param mat The last row of the distance matrix (it will be shared by all children).
   */
  void search_rec(const s_edge* edge,
                  const DLMatrix* mat);
};

# endif /* !APPROX_HH */
