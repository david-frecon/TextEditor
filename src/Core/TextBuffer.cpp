#include "../include/Core/TextBuffer.hh"
#include <iostream>
#include <stdexcept>

TextBuffer::TextBuffer() : m_text() {}

TextBuffer::TextBuffer(const std::u32string &initialText)
    : m_text(initialText) {}

void TextBuffer::insertChar(size_t index, char32_t c) {
  if (index > m_text.size()) {
    index = m_text.size();
  }
  m_undo.push_back(m_text);
  m_text.insert(m_text.begin() + index, c);
}

void TextBuffer::deleteChar(size_t index) {
  if (index < m_text.size()) {
    m_undo.push_back(m_text);
    m_text.erase(m_text.begin() + index);
  }
}

void TextBuffer::insertText(size_t index, const std::u32string &text) {
  if (index > m_text.size()) {
    index = m_text.size();
  }
  m_undo.push_back(m_text);
  m_text.insert(index, text);
}

void TextBuffer::deleteRange(size_t start, size_t end, bool copy) {
  m_undo.push_back(m_text);

  if (start < end && start < m_text.size()) {
    end = std::min(end, m_text.size());
    if (copy) {
      m_text.substr(start, end - start);
    }
    m_text.erase(start, end - start);
  }
}

void TextBuffer::copySelection(size_t start, size_t end) {
  if (start < end && start < m_text.size()) {
    end = std::min(end, m_text.size());
    m_copy = m_text.substr(start, end - start);
  }
}
void TextBuffer::undo() {
  if (!m_undo.empty()) {
    std::u32string last = m_undo.back();
    m_undo.pop_back();
    // m_redo.push_back(last);
    m_text = last;
  }
}
void TextBuffer::redo() {
  if (!m_redo.empty()) {
    std::u32string last = m_redo.back();
    m_redo.pop_back();
    m_undo.push_back(last);
    m_text = last;
  }
}

void TextBuffer::clear() { m_text.clear(); }
