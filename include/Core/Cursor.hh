#pragma once
#include "TextBuffer.hh"
#include <cstddef>

class Cursor {
public:
  Cursor(){};

  // Position management
  size_t getIndex() const { return m_index; }
  void setIndex(size_t index);

  // Screen position
  float getScreenX() const { return m_screenX; }
  float getScreenY() const { return m_screenY; }
  void setScreenPosition(float x, float y);

  // Visibility
  bool isVisible() const { return m_isVisible; }
  void setVisible(bool visible) { m_isVisible = visible; }
  void toggleVisibility() { m_isVisible = !m_isVisible; }

  // Movement
  void moveUp(TextBuffer &buffer);
  void moveDown(TextBuffer &buffer);
  void moveLeft();
  void moveRight(size_t textLength);

  void moveWord(std::u32string &buffer, bool toEnd);

private:
  size_t m_index = 0;      // Character index in text
  float m_screenX = 0.;    // Screen X position
  float m_screenY = 0.;    // Screen Y position
  bool m_isVisible = true; // For blinking effect
};