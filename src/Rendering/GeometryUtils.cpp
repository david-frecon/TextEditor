#include "include/Rendering/GeometryUtils.hh"

namespace GeometryUtils {

std::vector<Point<float>>
computeBezier(const Point<int> &p1, const Point<int> &p2, const Point<int> &p3,
              int steps, float scale, float offsetX, float offsetY) {
  std::vector<Point<float>> curve;

  for (int i = 0; i < steps; i++) {
    float t = static_cast<float>(i) / (steps - 1);
    float oneMinusT = 1.0f - t;

    float x =
        p1.x * oneMinusT * oneMinusT + 2 * p2.x * t * oneMinusT + p3.x * t * t;

    float y =
        p1.y * oneMinusT * oneMinusT + 2 * p2.y * t * oneMinusT + p3.y * t * t;

    curve.push_back({x * scale + offsetX, -y * scale + offsetY});
  }

  return curve;
}

float signedArea(const std::vector<std::array<double, 2>> &contour) {
  double area = 0;
  size_t n = contour.size();

  for (size_t i = 0; i < n; i++) {
    size_t j = (i + 1) % n;
    area += (contour[i][0] * contour[j][1]) - (contour[j][0] * contour[i][1]);
  }

  return area / 2.0;
}

TriangulatedGlyph triangulateGlyph(const GlyphData &glyphData, float scale,
                                   float offsetX, float offsetY,
                                   int bezierSteps) {
  TriangulatedGlyph result;

  // Create all contours
  std::vector<std::vector<std::array<double, 2>>> allContours;
  int firstPointContour = 0;

  for (size_t c = 0; c < glyphData.contourIndices.size(); c++) {
    int lastPoint = glyphData.contourIndices[c];
    std::vector<std::array<double, 2>> contour;

    // Generate Bezier curves for the contour
    for (int j = firstPointContour; j <= lastPoint - 2; j += 2) {
      auto curve = computeBezier(glyphData.coords[j], glyphData.coords[j + 1],
                                 glyphData.coords[j + 2], bezierSteps, scale,
                                 offsetX, offsetY);

      // Avoid duplicating points
      size_t startIdx = contour.empty() ? 0 : 1;
      for (size_t i = startIdx; i < curve.size(); i++) {
        contour.push_back({curve[i].x, curve[i].y});
      }
    }

    // Closing curve
    auto closing = computeBezier(glyphData.coords[lastPoint - 1],
                                 glyphData.coords[lastPoint],
                                 glyphData.coords[firstPointContour],
                                 bezierSteps, scale, offsetX, offsetY);

    for (size_t i = 1; i < closing.size(); i++) {
      contour.push_back({closing[i].x, closing[i].y});
    }

    allContours.push_back(contour);
    firstPointContour = lastPoint + 1;
  }

  // Separate polygons and holes based on winding order
  std::vector<std::vector<std::array<double, 2>>> holes;

  for (auto &contour : allContours) {
    float area = signedArea(contour);

    if (area > 0) {
      // This is an outer contour, save previous polygon with its holes
      if (!result.polygons.empty() && !holes.empty()) {
        result.polygons.back().insert(result.polygons.back().end(),
                                      holes.begin(), holes.end());
        holes.clear();
      }

      // Start new polygon
      std::vector<std::vector<std::array<double, 2>>> polygon;
      polygon.push_back(contour);
      result.polygons.push_back(polygon);
    } else {
      // This is a hole
      holes.push_back(contour);
    }
  }

  // Add remaining holes to last polygon
  if (!result.polygons.empty() && !holes.empty()) {
    result.polygons.back().insert(result.polygons.back().end(), holes.begin(),
                                  holes.end());
  }

  return result;
}

} // namespace GeometryUtils