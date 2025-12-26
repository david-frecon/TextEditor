#pragma once
#include <cstddef>

class Selection {
public:
  Selection(){};

  bool isActive() const { return m_isActive; }
  void setActive(bool active) { m_isActive = active; }

  size_t getStart() const { return m_startIndex; }
  void setStart(size_t start) { m_startIndex = start; }

  size_t getEnd() const { return m_endIndex; }
  void setEnd(size_t end) { m_endIndex = end; }

  void getRange(int &start, int &end);

  void begin(size_t position);
  void extend(size_t position);
  void clear();

  void selectAll(size_t textLength);

private:
  int m_startIndex;
  int m_endIndex;
  bool m_isActive;
};
