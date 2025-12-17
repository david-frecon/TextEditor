#include "../include/Core/TextBuffer.hh"
#include <stdexcept>

TextBuffer::TextBuffer() : m_text() {}

TextBuffer::TextBuffer(const std::u32string &initialText)
    : m_text(initialText) {}

void TextBuffer::insertChar(size_t index, char32_t c) {
  if (index > m_text.size()) {
    index = m_text.size();
  }
  m_text.insert(m_text.begin() + index, c);
}

void TextBuffer::deleteChar(size_t index) {
  if (index < m_text.size()) {
    m_text.erase(m_text.begin() + index);
  }
}

void TextBuffer::insertText(size_t index, const std::u32string &text) {
  if (index > m_text.size()) {
    index = m_text.size();
  }
  m_text.insert(index, text);
}

void TextBuffer::deleteRange(size_t start, size_t end) {
  if (start < end && start < m_text.size()) {
    end = std::min(end, m_text.size());
    m_text.erase(start, end - start);
  }
}

void TextBuffer::clear() { m_text.clear(); }
