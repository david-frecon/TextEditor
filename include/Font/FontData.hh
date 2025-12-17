#pragma once
#include <iostream>
#include <map>
#include <vector>

struct CompoundData {
  std::vector<u_int16_t> GlyphIndex;
  std::vector<int> xOffset;
  std::vector<int> yOffset;
};

template <typename T> struct Point {
  T x, y;
  int onCurve = 0;
};

struct GlyphData {
  int unicode;
  int xmin, ymin, xmax, ymax;
  std::vector<int> contourIndices;
  std::vector<Point<int>> coords;
  unsigned int advanceWidth = 0;
};

struct Font {
  std::map<int, GlyphData> font;
  int _unitPerEm;
  std::vector<GlyphData> glyphs_;

  Font(std::vector<GlyphData> &glyphs, int unitPerEm) {
    for (size_t i = 0; i < glyphs.size(); i++) {
      font[glyphs[i].unicode] = glyphs[i];
    }
    glyphs_ = glyphs;
    _unitPerEm = unitPerEm;
  }

  GlyphData getGlyph(int c) { return font[c]; }
};
