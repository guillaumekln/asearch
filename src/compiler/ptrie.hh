#ifndef PTRIE_HH
# define PTRIE_HH

# include <string>
# include <list>
# include <ostream>

class PTrie
{
public:
  static std::string strs;

  /**
   * \brief Add a new word in the trie.
   *
   * \param word The new word.
   * \param frequency The frequency of the word.
   */
  void addWord(const std::string& word, unsigned int frequency);

  /**
   * \brief Serialize the trie.
   *
   * \param filename The path to the serialized trie.
   */
  void serialize(const std::string& filename) const;

  /**
   * \brief Debug output.
   *
   * Print the trie in the dot format.
   */
  friend std::ostream& operator<<(std::ostream& os, const PTrie& pt);

private:
  // Forward declaration.
  class Edge;

  /**
   * \brief Node class.
   *
   * A node can be the end of a word.
   * It stores a list of edges to access its children.
   */
  class Node
  {
  public:
    /**
     * \brief Construct a node.
     *
     * \param frequency The frequency of the word that terminates here.
     */
    Node(unsigned int frequency);

    /**
     * \brief Construct a node.
     *
     * The frequency is set to 0 (i.e. no word terminates here).
     */
    Node();

    /**
     * \brief Insert a word in the node.
     *
     * \param word The word.
     * \param frequency The frequency of the word.
     */
    void insert(const std::string& word, unsigned int frequency);

    /**
     * \brief Get the edges associated to this node (const version).
     *
     * \return The list of edges.
     */
    const std::list<Edge>& getEdges() const;

    /**
     * \brief Get the edges associated to this node.
     *
     * \return The list of edges.
     */
    std::list<Edge>& getEdges();

    /**
     * \brief Get the frequency of the node.
     *
     * A null frequency indicated that the node does not represent a word.
     *
     * \return The frequency.
     */
    unsigned int getFrequency() const;

    /**
     * \brief Output this node in the dot format.
     */
    void toDot(std::ostream& os) const;

  private:
    unsigned int _frequency;
    std::list<Edge> _edges;
  };

  /**
   * \brief Edge class.
   *
   * An edge holds a prefix and a target node.
   */
  class Edge
  {
  public:
    /**
     * \brief Construct an edge.
     *
     * \param offset The offset of the char sequence in the string buffer.
     * \param length The length of the char sequence in the string buffer.
     * \param frequency The frequency of the word that terminates with this char sequence.
     */
    Edge(unsigned int offset, unsigned int length, unsigned int frequency);

    /**
     * \brief Split the edge.
     *
     * For example, with split(2) on this edge:
     * [] -- earth --> []  becomes  [] -- ea --> [] -- rth --> []
     *
     * \param size The size of the new char sequence.
     */
    void split(unsigned int size);

    /**
     * \brief Get the target node (const version).
     */
    const Node& getTargetNode() const;

    /**
     * \brief Get the target node (const version).
     */
    Node& getTargetNode();

    /**
     * \brief Get the offset of the char sequence.
     */
    unsigned int getOffset() const;

    /**
     * \brief Get the length of the char sequence.
     */
    unsigned int getLength() const;

  private:
    unsigned int _offset;
    unsigned int _length;
    Node _targetNode;
  };

  Node _root;

  /**
   * \brief Helper function to serialize an edge.
   *
   * \param out The stream to write into.
   * \param offset The offset of the char sequence.
   * \param length The length of the char sequence.
   * \param frequency The frequency of the word that terminates with this char sequence (if it exists).
   * \param children_count The number of children in the trie.
   * \param children_offset The offset to access its leftmost child.
   */
  void serialize(std::ostream& out,
                 unsigned int offset,
                 unsigned int length,
                 unsigned int frequency,
                 unsigned int children_count,
                 unsigned int children_offset) const;
};


# endif /* !PTRIE_HH */
