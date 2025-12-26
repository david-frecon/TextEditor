#pragma once
#include "../Core/Cursor.hh"
#include "../Core/TextBuffer.hh"
#include "../Font/FontParser.hh"
#include "../Input/InputHandler.hh"
#include "../Rendering/TextRenderer.hh"
#include "include/Core/Selection.hh"
#include <SFML/Graphics.hpp>

class Editor {
public:
  Editor(Font &font);

  // Main loop
  void run();

  // Configuration
  void setPixelHeight(float height);
  float getPixelHeight() const { return m_pixelHeight; }

  void setBezierSteps(int steps);
  int getBezierSteps() const { return m_bezierSteps; }

private:
  sf::RenderWindow m_window;
  sf::Clock m_blinkClock;
  InputHandler m_inputHandler;
  TextBuffer m_textBuffer;
  Cursor m_cursor;
  Selection m_selection;
  TextRenderer m_textRenderer;

  // Configuration
  float m_pixelHeight;
  int m_bezierSteps;

  void processEvents();
  void update();
  void render();
};