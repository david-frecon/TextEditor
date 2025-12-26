#include "include/Core/Cursor.hh"
#include <algorithm>
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

void Cursor::moveWord(std::u32string &buffer, bool toEnd)
{
  if (toEnd){
    while (m_index < buffer.size()){
      if (buffer[m_index])
      continue;
    }

  }

}

void Cursor::moveDown(TextBuffer &buffer)
{
    const std::u32string &text = buffer.getText();
    size_t endOfCurrentLine = text.find(U'\r', m_index);

    if (endOfCurrentLine == std::u32string::npos)
    {
        return;
    }
    size_t startOfCurrentLine = text.rfind(U'\r', m_index);

    if (startOfCurrentLine == std::u32string::npos)
    {
        startOfCurrentLine = 0;
    } else {
    startOfCurrentLine++;
  }

  size_t currentColumn = m_index - startOfCurrentLine;

  size_t startOfNextLine = endOfCurrentLine + 1;
  size_t endOfNextLine = text.find(U'\r', startOfNextLine);

  if (endOfNextLine == std::u32string::npos) {
    endOfNextLine = text.size();
  }

  size_t targetIndex = startOfNextLine + currentColumn;

  m_index = std::min(targetIndex, endOfNextLine);
}

void Cursor::moveUp(TextBuffer &buffer) {
  const std::u32string &text = buffer.getText();

  size_t startOfCurrentLine = text.rfind(U'\r', m_index);

  if (startOfCurrentLine == std::u32string::npos) {
    return;
  }

  size_t endOfPrevLine = startOfCurrentLine;

  size_t realStartOfCurrentLine = endOfPrevLine + 1;

  if (m_index < realStartOfCurrentLine) {
    realStartOfCurrentLine = 0; 
  }

  size_t currentColumn = m_index - realStartOfCurrentLine;

  size_t startOfPrevLine = 0;
  if (endOfPrevLine > 0) {
    size_t found = text.rfind(U'\r', endOfPrevLine - 1);
    if (found != std::u32string::npos) {
      startOfPrevLine = found + 1;
    }
  }

  size_t targetIndex = startOfPrevLine + currentColumn;

  m_index = std::min(targetIndex, endOfPrevLine);
}