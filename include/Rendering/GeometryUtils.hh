#pragma once
#include "../Font/FontData.hh"
#include <array>
#include <vector>

namespace GeometryUtils {

std::vector<Point<float>>
computeBezier(const Point<int> &p1, const Point<int> &p2, const Point<int> &p3,
              int steps, float scale, float offsetX, float offsetY);

// Calculate signed area of a contour (hole detection)
float signedArea(const std::vector<std::array<double, 2>> &contour);

// Convert glyph contours to triangulated polygons
struct TriangulatedGlyph {
  std::vector<std::vector<std::vector<std::array<double, 2>>>> polygons;
};

TriangulatedGlyph triangulateGlyph(const GlyphData &glyphData, float scale,
                                   float offsetX, float offsetY,
                                   int bezierSteps);

} // namespace GeometryUtils