#include "include/Rendering/TextRenderer.hh"

TextRenderer::TextRenderer(Font &font)
    : m_font(font), m_pixelHeight(100.), m_lineSpacing(1.2), m_startX(0),
      m_startY(300) {}
void TextRenderer::renderText(sf::RenderWindow &window,
                              const TextBuffer &buffer, Cursor &cursor) {

  float penX = m_startX;
  float penY = m_startY;
  float scale = getScale();

  std::u32string text = buffer.getText();
  for (int i = 0; i <= text.size(); i++) {
    char32_t c = text[i];

    // Update cursor position
    if (cursor.getIndex() == i)
      cursor.setScreenPosition(penX, penY);

    if (c == '\n' || c == '\r') {
      penX = m_startX;
      penY += getLineHeight();
      continue;
    }

    GlyphData g = m_font.getGlyph(c);
    if (g.advanceWidth == 0 && c == ' ')
      g.advanceWidth = m_font._unitPerEm / 4;

    m_glyphRenderer.renderGlyph(window, g, scale, penX, penY);
    penX += g.advanceWidth * scale;
  }
}

void TextRenderer::renderCursor(sf::RenderWindow &window,
                                const Cursor &cursor) {
  if (!cursor.isVisible())
    return;
  sf::RectangleShape caret;
  caret.setSize({2.f, m_pixelHeight});
  caret.setPosition(cursor.getScreenX(), cursor.getScreenY() - m_pixelHeight);
  caret.setFillColor(sf::Color::White);

  window.draw(caret);
}

float TextRenderer::getLineHeight() const {
  return m_font._unitPerEm * getScale() * m_lineSpacing;
}

float TextRenderer::getScale() const {
  return m_pixelHeight / m_font._unitPerEm;
}
