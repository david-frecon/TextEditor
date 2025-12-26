#include "include/Input/InputHandler.hh"
#include <iostream>
void InputHandler::handleEvent(const sf::Event &event, TextBuffer &textBuffer,
                               Cursor &cursor, Selection &selection,
                               float &pixelHeight, int &bezierSteps) {
  if (event.type == sf::Event::KeyPressed)
    handleKeyPress(event.key, textBuffer, cursor, selection, bezierSteps);
  else if (event.type == sf::Event::TextEntered)
    handleTextInput(event.text, textBuffer, cursor, selection, pixelHeight);
}

void InputHandler::handleKeyPress(const sf::Event::KeyEvent &keyEvent,
                                  TextBuffer &textBuffer, Cursor &cursor,
                                  Selection &selection, int &bezierSteps) {
  bool shiftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                      sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);

  bool ctrlPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

  switch (keyEvent.code) {
  case sf::Keyboard::Down:
    if (shiftPressed) {
      if (!selection.isActive()) {
        selection.begin(cursor.getIndex());
      }

      cursor.moveDown(textBuffer);
      selection.extend(cursor.getIndex());
    } else {
      if (selection.isActive()) {
        selection.clear();
      }
      cursor.moveDown(textBuffer);
    }
    break;
  case sf::Keyboard::Up:

    if (shiftPressed) {
      if (!selection.isActive()) {
        selection.begin(cursor.getIndex());
      }

      cursor.moveUp(textBuffer);
      selection.extend(cursor.getIndex());
    } else {
      if (selection.isActive()) {
        selection.clear();
      }
      cursor.moveUp(textBuffer);
    }
    break;
  case sf::Keyboard::Left:
    if (shiftPressed) {
      if (!selection.isActive()) {
        selection.begin(cursor.getIndex());
      }

      cursor.moveLeft();
      selection.extend(cursor.getIndex());
    } else {
      if (selection.isActive()) {
        selection.clear();
      }
      cursor.moveLeft();
    }
    break;
  case sf::Keyboard::Right:
    if (shiftPressed) {
      if (!selection.isActive()) {
        selection.begin(cursor.getIndex());
      }

      cursor.moveRight(textBuffer.size());
      selection.extend(cursor.getIndex());
    } else {
      if (selection.isActive()) {
        int start, end;
        selection.getRange(start, end);
        cursor.setIndex(end);
        selection.clear();
      } else
        cursor.moveRight(textBuffer.size());
    }
    break;
  case sf::Keyboard::Tab:
    for (size_t i = 0; i < 4; i++) {
      textBuffer.insertChar(cursor.getIndex(), ' ');
      cursor.moveRight(textBuffer.size());
    }
    break;
  case sf::Keyboard::Z:
    if (ctrlPressed) {
      textBuffer.undo();
      if (cursor.getIndex() > textBuffer.size()) {
        cursor.setIndex(textBuffer.size());
      }
    }
    break;
  case sf::Keyboard::X:
    if (ctrlPressed) {
      int start, end;
      selection.getRange(start, end);
      textBuffer.deleteRange(start, end, true);
    }
    break;

  case sf::Keyboard::C:
    if (ctrlPressed) {
      int start, end;
      selection.getRange(start, end);
      textBuffer.copySelection(start, end);
    }
    break;
  case sf::Keyboard::V:
    if (ctrlPressed) {
      std::u32string copy = textBuffer.getCopy();
      textBuffer.insertText(cursor.getIndex(), copy);
      cursor.setIndex(cursor.getIndex() + copy.size());
    }
    break;
  default:
    break;
  }
}

void InputHandler::handleTextInput(const sf::Event::TextEvent &textEvent,
                                   TextBuffer &textBuffer, Cursor &cursor,
                                   Selection &selection, float &pixelHeight) {
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
    return;
  }

  // Backspace
  if (unicode == 8) {
    if (cursor.getIndex() > 0) {
      if (selection.isActive()) {
        int start, end;
        selection.getRange(start, end);
        selection.clear();
        textBuffer.deleteRange(start, end, false);
      } else {
        textBuffer.deleteChar(cursor.getIndex() - 1);
        cursor.moveLeft();
      }
    }
    return;
  }

  // Delete
  if (unicode == 127) {
    if (cursor.getIndex() < textBuffer.size()) {
      if (selection.isActive()) {
        int start, end;
        selection.getRange(start, end);
        selection.clear();
        textBuffer.deleteRange(start, end, false);
      } else {
        textBuffer.deleteChar(cursor.getIndex() - 1);
      }
    }
    return;
  }

  // Character input
  if (unicode >= 32 || unicode == '\n' || unicode == '\r') {
    if (selection.isActive()) {
      int start, end;
      selection.getRange(start, end);
      textBuffer.deleteRange(start, end, false);
      cursor.setIndex(start);
      selection.clear();
    }
    textBuffer.insertChar(cursor.getIndex(), unicode);
    cursor.moveRight(textBuffer.size());
  }
}
