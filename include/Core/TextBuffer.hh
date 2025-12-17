#pragma once
#include <string>

class TextBuffer {
public:
  TextBuffer();
  explicit TextBuffer(const std::u32string &initialText);

  // Text access
  const std::u32string &getText() const { return m_text; }
  size_t size() const { return m_text.size(); }
  bool empty() const { return m_text.empty(); }

  // Modification
  void insertChar(size_t index, char32_t c);
  void deleteChar(size_t index);
  void insertText(size_t index, const std::u32string &text);
  void deleteRange(size_t start, size_t end);
  void clear();

private:
  std::u32string m_text;
};