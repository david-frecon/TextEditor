#include "include/Input/InputHandler.hh"
#include <iostream>
void InputHandler::handleEvent(const sf::Event &event, TextBuffer &textBuffer,
                               Cursor &cursor, float &pixelHeight,
                               int &bezierSteps) {
  if (event.type == sf::Event::KeyPressed)
    handleKeyPress(event.key, textBuffer, cursor, bezierSteps);
  if (event.type == sf::Event::TextEntered)
    handleTextInput(event.text, textBuffer, cursor, pixelHeight);
}

void InputHandler::handleKeyPress(const sf::Event::KeyEvent &keyEvent,
                                  TextBuffer &textBuffer, Cursor &cursor,
                                  int &bezierSteps) {
  switch (keyEvent.code) {
  case sf::Keyboard::Down:
    if (bezierSteps > 2)
      bezierSteps--;
    break;
  case sf::Keyboard::Up:
    bezierSteps++;
    break;
  case sf::Keyboard::Left:
    cursor.moveLeft();
    break;
  case sf::Keyboard::Right:
    cursor.moveRight(textBuffer.size());
    break;
  case sf::Keyboard::Tab:
    for (size_t i = 0; i < 4; i++) {
      textBuffer.insertChar(cursor.getIndex(), ' ');
      cursor.moveRight(textBuffer.size());
    }
    break;
  }
}

void InputHandler::handleTextInput(const sf::Event::TextEvent &textEvent,
                                   TextBuffer &textBuffer, Cursor &cursor,
                                   float &pixelHeight) {
  char32_t unicode = textEvent.unicode;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
      sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)) {

    if (unicode == '+' || unicode == '=') {
      pixelHeight += 10.f;
      return;
    } else if (unicode == '-') {
      if (pixelHeight > 20.f) {
        pixelHeight -= 10.f;
      }
      return;
    }
  }

  // Backspace
  if (unicode == 8) {
    if (cursor.getIndex() > 0) {
      textBuffer.deleteChar(cursor.getIndex() - 1);
      cursor.moveLeft();
    }
    return;
  }

  // Delete
  if (unicode == 127) {
    if (cursor.getIndex() < textBuffer.size()) {
      textBuffer.deleteChar(cursor.getIndex());
    }
    return;
  }

  // Character input
  if (unicode >= 32 || unicode == '\n' || unicode == '\r') {
    textBuffer.insertChar(cursor.getIndex(), unicode);
    cursor.moveRight(textBuffer.size());
  }
}
