#include "include/Rendering/GlyphRenderer.hh"
#include "include/Rendering/GeometryUtils.hh"
#include "include/Rendering/earcut.hpp"

GlyphRenderer::GlyphRenderer() : m_bezierSteps(10), m_color(sf::Color::White) {}

void GlyphRenderer::renderGlyph(sf::RenderWindow &window,
                                const GlyphData &glyph, float scale,
                                float offsetX, float offsetY) {

  // Define for earcut lib
  using Coord = double;
  using N = uint32_t;

  auto triangulated = GeometryUtils::triangulateGlyph(glyph, scale, offsetX,
                                                      offsetY, m_bezierSteps);

  for (auto &polygon : triangulated.polygons) {
    std::vector<N> indices = mapbox::earcut<N>(polygon);
    sf::VertexArray triangles(sf::Triangles, indices.size());
    std::vector<std::array<Coord, 2>> flatten;
    for (auto &contour : polygon) {
      flatten.insert(flatten.end(), contour.begin(), contour.end());
    }
    for (size_t i = 0; i < indices.size(); i++) {
      triangles[i].position =
          sf::Vector2f(flatten[indices[i]][0], flatten[indices[i]][1]);
      triangles[i].color = m_color;
    }
    window.draw(triangles);
  }
}