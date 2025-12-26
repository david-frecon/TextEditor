#pragma once
#include <string>
#include <vector>

class TextBuffer {
public:
  TextBuffer();
  explicit TextBuffer(const std::u32string &initialText);

  // Text access
  const std::u32string &getText() const { return m_text; }
  const std::u32string &getCopy() const { return m_copy; }
  size_t size() const { return m_text.size(); }
  bool empty() const { return m_text.empty(); }

  // Modification
  void insertChar(size_t index, char32_t c);
  void deleteChar(size_t index);
  void insertText(size_t index, const std::u32string &text);
  void deleteRange(size_t start, size_t end, bool copy);
  void clear();

  // Shortcut
  void copySelection(size_t start, size_t end);
  void undo();
  void redo();

private:
  std::u32string m_text;
  std::u32string m_copy;
  std::vector<std::u32string> m_undo;
  std::vector<std::u32string> m_redo;
};