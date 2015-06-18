#ifndef DL_MATRIX_HXX
# define DL_MATRIX_HXX

# include <iostream>
# include <algorithm>

inline
DLMatrix::DLMatrix(size_t width, unsigned int max_dist)
  : _parent(nullptr)
  , _width(width)
  , _offset(0)
  , _maxcol(max_dist)
{
  for (size_t i = 0; i < width; ++i)
    _dist[i] = i;
}

inline
DLMatrix::DLMatrix(const DLMatrix* parent,
                   const std::string& word,
                   char c,
                   unsigned int max_dist)
  : _parent(parent)
  , _width(parent->_width)
  , _offset(parent->_offset + 1)
  , _maxcol(_offset <= max_dist ? 0 : -1)
  , _c(c)
{
  _dist[0] = _offset;

  size_t j;
  size_t upper_bound = std::min(_width, (size_t) _parent->_maxcol + 2);

  for (j = 1; j < upper_bound; ++j)
  {
    _dist[j] = std::min(std::min(parent->_dist[j] + 1, // delete
                                 _dist[j-1] + 1), // insert
                        parent->_dist[j-1] + (c != word[j-1])); // equal or substitution

    if (_offset > 1 && j > 1 && c == word[j-2] && parent->_c == word[j-1])
      _dist[j] = std::min(_dist[j], parent->_parent->_dist[j-2] + (c != word[j-1])); // transposition

    if (_dist[j] <= max_dist)
      _maxcol = j;
  }

  for (; j < _width; ++j)
    _dist[j] = max_dist + 1;
}

inline
unsigned int DLMatrix::get_dist() const
{
  return _dist[_width - 1];
}

inline
void DLMatrix::get_word(std::string& w) const
{
  // Recursively retrieving the word we computed the distance with.
  if (_parent != nullptr)
  {
    _parent->get_word(w);
    w.push_back(_c);
  }
}

inline
bool DLMatrix::final() const
{
  return _maxcol < 0;
}

inline
size_t DLMatrix::offset() const
{
  return _offset;
}

# endif /* !DL_MATRIX_HXX */
