#include <utility>
#include <iterator>
#include <fstream>
#include <queue>
#include "ptrie.hh"

std::string PTrie::strs;

void PTrie::addWord(const std::string& word, unsigned int frequency)
{
  _root.insert(word, frequency);
}

void PTrie::serialize(std::ostream& out,
                      unsigned int offset,
                      unsigned int length,
                      unsigned int frequency,
                      unsigned int children_count,
                      unsigned int children_offset) const
{
  out.write((char*) &offset, sizeof (unsigned int));
  out.write((char*) &length, sizeof (unsigned int));
  out.write((char*) &frequency, sizeof (unsigned int));
  out.write((char*) &children_count, sizeof (unsigned int));
  out.write((char*) &children_offset, sizeof (unsigned int));
}

void PTrie::serialize(const std::string& filename) const
{
  std::ofstream out(filename, std::ios::out | std::ios::binary);

  unsigned int tmp = strs.size();
  out.write((char*) &tmp, sizeof (unsigned int));
  out.write(strs.c_str(), strs.size());

  std::queue<const Node*> queue;
  queue.push(&_root);
  queue.push(nullptr);

  size_t i = 0;
  size_t children = _root.getEdges().size();
  size_t children_next = 0;

  // Virtual edge to represent the trie's root
  serialize(out, 0, 0, 0, children, children ? 1 : 0);

  do {
    const Node* n = queue.front();
    queue.pop();

    if (n == nullptr)
    {
      i = 0;
      children = children_next;
      children_next = 0;
      if (!queue.empty())
        queue.push(nullptr);
    }
    else if (!n->getEdges().empty())
    {
      for (const Edge& e: n->getEdges())
      {
        serialize(out,
                  e.getOffset(),
                  e.getLength(),
                  e.getTargetNode().getFrequency(),
                  e.getTargetNode().getEdges().size(),
                  e.getTargetNode().getEdges().empty() ? 0 : ((children - i) + children_next));

        i++;
        children_next += e.getTargetNode().getEdges().size();
        queue.push(&e.getTargetNode());
      }
    }
  } while (!queue.empty());
}

std::ostream& operator<<(std::ostream& os, const PTrie& pt)
{
  os << "# " << PTrie::strs << std::endl;
  os << "digraph g {" << std::endl;
  pt._root.toDot(os);
  os << "}";
  return os;
}


PTrie::Node::Node(unsigned int frequency)
  : _frequency(frequency)
{
}

PTrie::Node::Node()
  : _frequency(0)
{
}

void PTrie::Node::insert(const std::string& word, unsigned int frequency)
{
  // Easy case, word is a prefix of another word in the trie.
  if (word.empty())
  {
    _frequency = frequency;
    return;
  }

  std::list<Edge>::iterator it;

  // Search for a common prefix
  for (it = _edges.begin(); it != _edges.end(); ++it)
  {
    if (strs[it->getOffset()] == word[0]) // Found one.
    {
      unsigned int prefixlen = 1;

      // Computing the prefix length.
      for (;
           prefixlen < it->getLength() && prefixlen < word.length()
             && strs[it->getOffset() + prefixlen] == word[prefixlen];
           ++prefixlen)
      {
      }

      // Split the edge if the common prefix is not the complete substring.
      if (it->getLength() > prefixlen)
        it->split(prefixlen);

      // Insert the word without the prefix in the child node.
      it->getTargetNode().insert(word.substr(prefixlen, word.length() - prefixlen), frequency);
      break;
    }
  }

  if (it != _edges.end())
    return;

  // No prefix found, create a new edge.
  unsigned int offset = strs.length();
  unsigned int length = word.length();
  strs += word;

  _edges.emplace_back(offset, length, frequency);
}

unsigned int PTrie::Node::getFrequency() const
{
  return _frequency;
}

const std::list<PTrie::Edge>& PTrie::Node::getEdges() const
{
  return _edges;
}

std::list<PTrie::Edge>& PTrie::Node::getEdges()
{
  return _edges;
}

void PTrie::Node::toDot(std::ostream& os) const
{
  os << "n" << this;
  if (_frequency != 0)
    os << " [shape=box]";
  os << ";" << std::endl;

  for (const Edge& e: _edges)
  {
    os << "n" << this << " -> " << "n" << &e.getTargetNode()
       << " [label=\"" << std::string(strs, e.getOffset(), e.getLength()) << "\"];" << std::endl;
    e.getTargetNode().toDot(os);
  }
}


PTrie::Edge::Edge(unsigned int offset, unsigned int length, unsigned int frequency)
  : _offset(offset)
  , _length(length)
  , _targetNode(frequency)
{
}

void PTrie::Edge::split(unsigned int size)
{
  Edge e(_offset + size, _length - size, 0);
  std::swap(_targetNode, e.getTargetNode());
  _length = size;
  _targetNode.getEdges().push_back(std::move(e));
}

const PTrie::Node& PTrie::Edge::getTargetNode() const
{
  return _targetNode;
}

PTrie::Node& PTrie::Edge::getTargetNode()
{
  return _targetNode;
}

unsigned int PTrie::Edge::getOffset() const
{
  return _offset;
}

unsigned int PTrie::Edge::getLength() const
{
  return _length;
}
