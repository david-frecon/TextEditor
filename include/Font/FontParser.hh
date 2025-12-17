#pragma once
#include "FontData.hh"
#include "FontReader.hh"
#include <map>

Font Parse(std::string path);
uint ReadMaxGlyph(FontReader &reader, int maxpOffset);
std::vector<int> ReadLocGlyph(FontReader &reader, int locaOffset, int nbGlyph,
                              bool length);
std::map<std::string, int> ReadLookupTable(FontReader &reader);
std::vector<GlyphData> ReadGlyphs(FontReader &reader, int glyphOffset,
                                  std::vector<int> loca,
                                  std::map<int, int> cmap);
GlyphData ReadSimpleGlyph(FontReader &reader, int glyphOffset);
CompoundData ReadCompoundGlyph(FontReader &reader, int glyOffset);
bool BitSet(char flag, int index);

void ReadCoord(FontReader &reader, bool ReadX, std::vector<char> &flags,
               std::vector<Point<int>> &points);

void AddImpliedPoints(std::vector<Point<int>> &points,
                      std::vector<int> &contour);
void ReadFontSpacing(FontReader &reader, int htmxOffset, int hheaOffset,
                     std::vector<GlyphData> &glypths);

std::map<int, int> ReadCmap(FontReader &reader, int cmapOffset);