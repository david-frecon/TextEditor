#include "include/Core/Selection.hh"

void Selection::begin(size_t position) {
  m_startIndex = position;
  m_endIndex = position;
  m_isActive = true;
}

void Selection::getRange(int &start, int &end) {
  if (m_startIndex < m_endIndex) {
    start = m_startIndex;
    end = m_endIndex;
  } else {
    start = m_endIndex;
    end = m_startIndex;
  }
}

void Selection::extend(size_t position) { m_endIndex = position; }

void Selection::clear() {
  m_startIndex = 0;
  m_endIndex = 0;
  m_isActive = false;
}

void Selection::selectAll(size_t textLength) {
  m_startIndex = 0;
  m_endIndex = textLength;
  m_isActive = true;
}
