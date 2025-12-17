#include "include/Font/FontParser.hh"

Font Parse(std::string path) {
  FontReader reader(path);
  std::map<std::string, int> lookupTable = ReadLookupTable(reader);
  reader.GoTo(lookupTable["head"]);
  reader.SkipBytes(18);
  int unitPerEm = reader.ReadInt16();
  reader.SkipBytes(30);
  int nbBytesByLoc = reader.ReadInt16() == 0;
  int nbGlyphs = ReadMaxGlyph(reader, lookupTable["maxp"]);
  std::vector<int> loc =
      ReadLocGlyph(reader, lookupTable["loca"], nbGlyphs, nbBytesByLoc);
  std::map<int, int> cmap = ReadCmap(reader, lookupTable["cmap"]);

  std::vector<GlyphData> glyphs =
      ReadGlyphs(reader, lookupTable["glyf"], loc, cmap);
  ReadFontSpacing(reader, lookupTable["hmtx"], lookupTable["hhea"], glyphs);
  return Font(glyphs, unitPerEm);
}

void ReadFontSpacing(FontReader &reader, int htmxOffset, int hheaOffset,
                     std::vector<GlyphData> &glyphs) {
  reader.GoTo(hheaOffset);
  reader.SkipBytes(34);
  unsigned int numAdvanceMetrics = reader.ReadUInt16();

  // aller à la table hmtx
  reader.GoTo(htmxOffset);

  if (glyphs.empty())
    return;

  numAdvanceMetrics = std::min<unsigned int>(numAdvanceMetrics, glyphs.size());

  // Lire les "longHorMetrics" (advanceWidth + lsb)
  for (size_t i = 0; i < numAdvanceMetrics; i++) {
    glyphs[i].advanceWidth = reader.ReadUInt16();
    reader.SkipInt16(); // lsb
  }

  // Si la police contient plus de glyphes, utiliser la dernière advanceWidth
  unsigned int lastAdvanceWidth = glyphs[numAdvanceMetrics - 1].advanceWidth;
  for (size_t i = numAdvanceMetrics; i < glyphs.size(); i++) {
    glyphs[i].advanceWidth = lastAdvanceWidth;
  }
}

std::map<int, int> ReadCmap(FontReader &reader, int cmapOffset) {
  reader.GoTo(cmapOffset);
  reader.ReadUInt16();
  int nbSubtables = reader.ReadUInt16();
  u_int32_t offset = 0;
  for (size_t i = 0; i < nbSubtables; i++) {
    int platformID = reader.ReadInt16();
    int platformSpecificID = reader.ReadInt16();
    offset = reader.ReadUInt32();
    if (platformID == 0 && platformSpecificID == 3)
      break;
  }
  reader.GoTo(cmapOffset + offset);
  u_int16_t formati = reader.ReadUInt16();

  reader.SkipBytes(4);
  int SegCount = reader.ReadUInt16() / 2;
  reader.SkipBytes(6);
  std::vector<u_int16_t> endCode;
  for (size_t i = 0; i < SegCount; i++) {
    endCode.push_back(reader.ReadUInt16());
  }
  reader.SkipInt16();

  std::vector<u_int16_t> startCode;
  for (size_t i = 0; i < SegCount; i++) {
    startCode.push_back(reader.ReadUInt16());
  }

  std::vector<u_int16_t> idDelta;
  for (size_t i = 0; i < SegCount; i++) {
    idDelta.push_back(reader.ReadUInt16());
  }

  std::vector<u_int16_t> idRangeOffset;
  std::vector<u_int32_t> idRangeOffsetAddress;
  for (size_t i = 0; i < SegCount; i++) {
    idRangeOffsetAddress.push_back(reader.Location());
    idRangeOffset.push_back(reader.ReadUInt16());
  }
  std::map<int, int> glyphMapLoc{};
  for (size_t i = 0; i < startCode.size(); i++) {

    int c = startCode[i];
    if (c == 65536)
      break;
    u_int16_t GlyphIndex;
    while (c <= endCode[i]) {
      if (idRangeOffset[i] == 0) {
        GlyphIndex = (idDelta[i] + c) % 65536;
      } else {
        int GlyphIndexaddress =
            idRangeOffset[i] + 2 * (c - startCode[i]) + idRangeOffsetAddress[i];
        reader.GoTo(GlyphIndexaddress);
        GlyphIndex = reader.ReadUInt16();
        if (GlyphIndex != 0)
          GlyphIndex += idDelta[i];
      }
      glyphMapLoc[GlyphIndex] = c;
      c++;
    }
  }
  return glyphMapLoc;
}

uint ReadMaxGlyph(FontReader &reader, int maxpOffset) {
  reader.GoTo(maxpOffset);
  reader.SkipInt32();
  return reader.ReadInt16();
}

std::vector<int> ReadLocGlyph(FontReader &reader, int locaOffset, int nbGlyph,
                              bool shortOffset) {
  std::vector<int> location{};
  for (size_t i = 0; i < nbGlyph + 1; i++) {
    reader.GoTo(locaOffset + i * (shortOffset ? 2 : 4));
    if (shortOffset)
      location.push_back(reader.ReadUInt16() * 2);
    else
      location.push_back(reader.ReadInt32());
  }
  return location;
}

std::vector<GlyphData> ReadGlyphs(FontReader &reader, int glyphOffset,
                                  const std::vector<int> loca,
                                  const std::map<int, int> cmap) {
  std::vector<GlyphData> Glyphs{};
  // loca size should be nbGlyph + 1. We'll use bounds checks before indexing.
  if (loca.size() < 2)
    return Glyphs;

  for (const auto &entry : cmap) {
    int glyphIndex = entry.first; // key = glyph index
    int unicode = entry.second;   // value = code point

    // validate glyphIndex
    if (glyphIndex < 0 || static_cast<size_t>(glyphIndex + 1) >= loca.size()) {
      // out-of-range glyph index in cmap (robustness: skip it)
      continue;
    }

    int offsetInGlyf = loca[glyphIndex];
    int nextOffsetInGlyf = loca[glyphIndex + 1];

    // If offsets are identical, glyph has zero length (empty glyph) -> skip
    if (offsetInGlyf == nextOffsetInGlyf) {
      continue;
    }

    // Additional sanity: negative offsets are invalid
    if (offsetInGlyf < 0 || nextOffsetInGlyf < 0) {
      continue;
    }

    // Go to glyph data and read number of contours
    reader.GoTo(glyphOffset + offsetInGlyf);

    // It's possible the font is truncated; try/catch or additional file-size
    // checks
    short nbContour = reader.ReadInt16();

    // Only handle simple glyphs (nbContour >= 0). Compound glyphs have
    // nbContour < 0.

    if (nbContour >= 0) {
      // ReadSimpleGlyph expects the reader positioned at glyph start, so pass
      // proper offset
      GlyphData data = ReadSimpleGlyph(reader, glyphOffset + offsetInGlyf);
      data.unicode = unicode;
      Glyphs.push_back(std::move(data));
    } else {

      CompoundData compoundGlyphData =
          ReadCompoundGlyph(reader, glyphOffset + offsetInGlyf);
      std::vector<GlyphData> GlyphsCompound;
      GlyphData glyph;
      for (int i = 0; i < compoundGlyphData.GlyphIndex.size(); i++) {

        offsetInGlyf = loca[compoundGlyphData.GlyphIndex[i]];
        auto simpleCompound =
            ReadSimpleGlyph(reader, glyphOffset + offsetInGlyf);
        int endContour = glyph.coords.size();
        for (auto indice : simpleCompound.contourIndices) {
          glyph.contourIndices.push_back(endContour + indice);
        }

        for (auto coord : simpleCompound.coords) {
          coord.x += compoundGlyphData.xOffset[i];
          coord.y += compoundGlyphData.yOffset[i];
          glyph.coords.push_back(coord);
        }
      }
      glyph.unicode = unicode;
      Glyphs.push_back(glyph);
    }
  }

  return Glyphs;
}

GlyphData ReadSimpleGlyph(FontReader &reader, int glyphOffset) {
  GlyphData data{};

  reader.GoTo(glyphOffset);
  short nbContour = reader.ReadInt16();
  data.xmin = reader.ReadInt16();
  data.ymin = reader.ReadInt16();
  data.xmax = reader.ReadInt16();
  data.ymax = reader.ReadInt16();
  int nbPoints = 0;
  for (size_t i = 0; i < nbContour; i++) {
    short EndIndices = reader.ReadInt16();
    data.contourIndices.push_back(EndIndices);
    if (EndIndices + 1 > nbPoints)
      nbPoints = EndIndices + 1;
  }
  int instructionLength = reader.ReadInt16();
  reader.SkipBytes(instructionLength);
  int i = 0;
  std::vector<char> flags(nbPoints);
  // Read all the flags
  while (i < nbPoints) {
    u_char flag = reader.ReadByte();
    flags[i] = flag;
    if (BitSet(flag, 3)) {
      u_char repeat = reader.ReadByte();
      for (size_t j = 0; j < repeat; j++) {
        i++;
        flags[i] = flag;
      }
    }
    i++;
  }

  std::vector<Point<int>> points(nbPoints);
  ReadCoord(reader, true, flags, points);
  ReadCoord(reader, false, flags, points);
  data.coords = points;

  AddImpliedPoints(data.coords, data.contourIndices);
  return data;
}

CompoundData ReadCompoundGlyph(FontReader &reader, int glyphOffset) {
  bool remainCompound = true;
  reader.GoTo(glyphOffset);
  reader.SkipInt16();
  reader.SkipBytes(8);
  std::vector<u_int16_t> glyphs{};
  CompoundData compoundData{};
  while (remainCompound) {
    u_int16_t flags = reader.ReadUInt16();
    u_int16_t glyphIndex = reader.ReadUInt16();
    remainCompound = (flags >> 5) & 1;

    bool isWord = flags & (1 << 0);

    int arg1, arg2;

    if (isWord) {
      arg1 = reader.ReadInt16();
      arg2 = reader.ReadInt16();
    } else {
      arg1 = reader.ReadByte();
      arg2 = reader.ReadByte();
    }

    compoundData.GlyphIndex.push_back(glyphIndex);
    compoundData.xOffset.push_back(arg1);
    compoundData.yOffset.push_back(arg2);
  }
  return compoundData;
}

void ReadCoord(FontReader &reader, bool ReadX, std::vector<char> &flags,
               std::vector<Point<int>> &points) {
  int flagShort = ReadX ? 1 : 2;
  int flagIns = ReadX ? 4 : 5;
  int coord = 0;
  for (size_t i = 0; i < points.size(); i++) {
    Point<int> p;
    int size;
    // check if the offset is a char

    if (BitSet(flags[i], flagShort)) {
      u_char coordOff = reader.ReadByte();
      // If the flag is
      int sign = 1;
      if (!BitSet(flags[i], flagIns))
        sign = -1;
      coord += coordOff * sign;
    } else if (!BitSet(flags[i], flagIns)) {
      coord += reader.ReadInt16();
    }

    if (ReadX)
      points[i].x = coord;
    else
      points[i].y = coord;

    // Check is on the curve
    if (BitSet(flags[i], 0))
      points[i].onCurve = 1;
  }
}

void AddImpliedPoints(std::vector<Point<int>> &points,
                      std::vector<int> &contour) {
  std::vector<Point<int>> newPoints;
  std::vector<int> newContour;

  int contourStart = 0;
  for (size_t i = 0; i < contour.size(); i++) {
    int contourEnd = contour[i];
    int countourCount = contourEnd - contourStart + 1;

    // Boucle sur tous les segments du contour (fermé)
    for (int j = 0; j < countourCount; j++) {
      const Point<int> &p1 = points[contourStart + j];
      const Point<int> &p2 = points[contourStart + ((j + 1) % countourCount)];

      newPoints.push_back(p1);

      // Si les deux points sont off-curve, on ajoute un point implicite au
      // milieu
      if (p1.onCurve == p2.onCurve) {
        Point<int> mid{};
        mid.x = (p1.x + p2.x) / 2;
        mid.y = (p1.y + p2.y) / 2;
        mid.onCurve = true;
        newPoints.push_back(mid);
      }
    }

    newContour.push_back(newPoints.size() - 1);
    contourStart = contourEnd + 1;
  }

  points = std::move(newPoints);
  contour = std::move(newContour);
}

bool BitSet(char flag, int index) { return ((flag >> index) & 1) == 1; }

std::map<std::string, int> ReadLookupTable(FontReader &reader) {
  reader.SkipInt32();
  int nbTable = reader.ReadInt16();
  reader.SkipBytes(6);
  std::map<std::string, int> lookupTable{};
  for (size_t i = 0; i < nbTable; i++) {
    std::string tag = reader.ReadString(4);
    reader.SkipBytes(4);
    int offset = reader.ReadInt32();
    reader.SkipBytes(4);
    lookupTable[tag] = offset;
  }
  return lookupTable;
}