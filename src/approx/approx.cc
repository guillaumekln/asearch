#include "approx.hh"

#include <iostream>
#include <algorithm>

Approx::Approx(const s_trie* trie)
  : _trie(trie)
  , _root(get_root(trie))
{
}

void Approx::handle_sequence(const DLRow* parent,
                             const s_edge* child,
                             const char* str,
                             size_t offset)
{
  // Build one row par chararacter in the sequence.
  DLRow childmat(parent, _word, str[offset], _max_dist);

  if (childmat.is_final())
    return;

  ++offset;

  if (offset == child->length)
  {
    unsigned int d;

    // If this child is a word (frequency != 0), check the distance.
    if (child->frequency != 0 && (d = childmat.get_dist()) <= _max_dist)
      _results.emplace_back(childmat, child->frequency, d);

    // Recurse on the child.
    if (child->children_count > 0)
      search_rec(child, &childmat);
  }
  else
    handle_sequence(&childmat, child, str, offset);
}

void Approx::search_rec(const s_edge* edge,
                        const DLRow* mat)
{
  const s_edge* child = get_child(edge);

  for (unsigned int i = 0; i < edge->children_count; ++i, ++child)
    handle_sequence(mat, child, get_str(_trie, child->offset), 0);
}

void Approx::search(std::string& word, unsigned int max_dist)
{
  DLRow mat(word.length() + 1, max_dist);

  _max_dist = max_dist;
  _word = std::move(word);

  search_rec(_root, &mat);

  std::sort(_results.begin(), _results.end());

  printf("[");

  if (_results.size() > 0)
  {
    _results.front().dump();

    for (auto it = _results.cbegin() + 1; it != _results.cend(); ++it)
    {
      printf(",");
      it->dump();
    }
  }

  printf("]\n");

  _results.clear();
}

Approx::Result::Result(const DLRow& mat, unsigned int frequency, unsigned int distance)
  : _frequency(frequency)
  , _distance(distance)
{
  _word.reserve(mat.get_offset());
  mat.get_word(_word);
}

bool Approx::Result::operator<(const Result& res) const
{
  return (_distance < res._distance
          || (_distance == res._distance && (_frequency > res._frequency ||
                                             (_frequency == res._frequency && _word < res._word))));
}

void Approx::Result::dump() const
{
  printf("{\"word\":\"%s\",\"freq\":%u,\"distance\":%u}", _word.c_str(), _frequency, _distance);
}
