#pragma once
#include "../Core/Cursor.hh"
#include "../Core/Selection.hh"
#include "../Core/TextBuffer.hh"
#include "../Font/FontParser.hh"
#include "AnalyticRenderer.hh"
#include "GlyphRenderer.hh"
#include <SFML/Graphics.hpp>

class TextRenderer {
public:
  TextRenderer(Font &font);
  // Configuration
  void setPixelHeight(float height) { m_pixelHeight = height; }
  float getPixelHeight() const { return m_pixelHeight; }

  void setLineSpacing(float spacing) { m_lineSpacing = spacing; }
  void setStartPosition(float x, float y);

  void setBezierSteps(int steps) { m_glyphRenderer.setBezierSteps(steps); }
  int getBezierSteps() const { return m_glyphRenderer.getBezierSteps(); }

  float getLineHeight() const;

  // Rendering
  void renderText(sf::RenderWindow &window, const TextBuffer &buffer);

  void renderCursor(sf::RenderWindow &window, const Cursor &cursor);
  void renderSelection(sf::RenderWindow &window, Selection &selection);

private:
  Font &m_font;
  GlyphRenderer m_glyphRenderer;
  AnalyticGlyphRenderer m_analyticRenderer;

  std::vector<std::array<float, 3>> m_letterPosition;

  float m_pixelHeight;
  float m_lineSpacing;
  float m_startX;
  float m_startY;

  float getScale() const;
};