#pragma once
#include "../Core/Cursor.hh"
#include "../Core/TextBuffer.hh"
#include "include/Core/Selection.hh"
#include <SFML/Graphics.hpp>

class InputHandler {
public:
  InputHandler(){};

  void handleEvent(const sf::Event &event, TextBuffer &textBuffer,
                   Cursor &cursor, Selection &selection, float &pixelHeight,
                   int &bezierSteps);

private:
  void handleKeyPress(const sf::Event::KeyEvent &keyEvent,
                      TextBuffer &textBuffer, Cursor &cursor,
                      Selection &selection, int &bezierSteps);

  void handleTextInput(const sf::Event::TextEvent &textEvent,
                       TextBuffer &textBuffer, Cursor &cursor,
                       Selection &selection, float &pixelHeight);
};