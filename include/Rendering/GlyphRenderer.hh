#pragma once
#include "../Font/FontParser.hh"
#include <SFML/Graphics.hpp>

class GlyphRenderer {
public:
  GlyphRenderer();
  
  void setBezierSteps(int steps) { m_bezierSteps = steps; }
  int getBezierSteps() const { return m_bezierSteps; }
  
  void setColor(const sf::Color& color) { m_color = color; }
  
  // Render a single glyph at specified position
  void renderGlyph(
      sf::RenderWindow& window,
      const GlyphData& glyph,
      float scale,
      float offsetX,
      float offsetY
  );
  
private:
  int m_bezierSteps;
  sf::Color m_color;
};