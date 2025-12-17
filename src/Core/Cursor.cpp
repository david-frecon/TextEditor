#include "include/Core/Cursor.hh"

void Cursor::setIndex(size_t index) { m_index = index; }

void Cursor::setScreenPosition(float x, float y) {
  m_screenX = x;
  m_screenY = y;
}

void Cursor::moveLeft() {
  if (m_index > 0) {
    m_index--;
  }
}

void Cursor::moveRight(size_t textLength) {
  if (m_index < textLength) {
    m_index++;
  }
}
