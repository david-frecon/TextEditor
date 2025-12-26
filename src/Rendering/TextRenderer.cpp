#include "include/Rendering/TextRenderer.hh"

TextRenderer::TextRenderer(Font &font)
    : m_font(font), m_pixelHeight(100.), m_lineSpacing(1), m_startX(0),
      m_startY(300) {}
void TextRenderer::renderText(sf::RenderWindow &window,
                              const TextBuffer &buffer) {

  float penX = m_startX;
  float penY = m_startY;
  float scale = getScale();
  float maxWidth = window.getSize().x - 10.0f;
  m_letterPosition.clear();

  std::u32string text = buffer.getText();
  for (int i = 0; i <= text.size(); i++) {
    char32_t c = text[i];
    // m_letterPosition.push_back({penX, penY});

    if (c == '\n' || c == '\r') {
      m_letterPosition.push_back({penX, penY, 0});
      penX = m_startX;
      penY += getLineHeight();

      continue;
    }

    GlyphData g = m_font.getGlyph(c);

    if (g.advanceWidth == 0 && c == ' ')
      g.advanceWidth = m_font._unitPerEm / 4;

    // Make line wrapping
    int charWidth = g.advanceWidth * scale;
    ;
    if (penX + charWidth > maxWidth) {
      penX = m_startX;         // Reset to left
      penY += getLineHeight(); // Move down
    }
    m_letterPosition.push_back({penX, penY, 1});
    m_glyphRenderer.renderGlyph(window, g, scale, penX, penY);
    // m_analyticRenderer.renderGlyph(window, g, scale, penX, penY);
    penX += charWidth;
  }
}

void TextRenderer::renderSelection(sf::RenderWindow &window,
                                   Selection &selection) {
  if (!selection.isActive())
    return;

  float selectionHeight = m_pixelHeight;
  sf::Color selectionColor(100, 150, 255, 128);

  int start, end;
  selection.getRange(start, end);
  for (size_t i = start; i < end; i++) {
    float x1 = m_letterPosition[i][0];
    float y1 = m_letterPosition[i][1];
    float x2 = m_letterPosition[i + 1][0];
    float y2 = m_letterPosition[i + 1][1];
    if (!m_letterPosition[i][2])
      continue;

    float width = x2 - x1;
    if (y2 > y1) {
      width = window.getSize().x - 10.0f;
    }
    if (width > 0) {
      sf::RectangleShape rect;
      rect.setPosition(x1, y1 - selectionHeight);
      rect.setSize(sf::Vector2f(width, selectionHeight));
      rect.setFillColor(selectionColor);
      window.draw(rect);
    }
  }
}

void TextRenderer::renderCursor(sf::RenderWindow &window,
                                const Cursor &cursor) {
  if (!cursor.isVisible())
    return;
  sf::RectangleShape caret;

  int screenX = m_letterPosition[cursor.getIndex()][0];
  int screenY = m_letterPosition[cursor.getIndex()][1];
  caret.setSize({2.f, m_pixelHeight});
  caret.setPosition(screenX, screenY - m_pixelHeight);
  caret.setFillColor(sf::Color::White);

  window.draw(caret);
}

float TextRenderer::getLineHeight() const {
  return m_font._unitPerEm * getScale() * m_lineSpacing;
}

float TextRenderer::getScale() const {
  return m_pixelHeight / m_font._unitPerEm;
}
