#include "include/Editor/Editor.hh"

Editor::Editor(Font &font)
    : m_window(sf::VideoMode(1080, 920), "Text Editor"), m_pixelHeight(100.),
      m_bezierSteps(10), m_textRenderer(font) {}

void Editor::run() {
  while (m_window.isOpen()) {
    processEvents();
    update();
    render();
  }
}

void Editor::setPixelHeight(float height) { m_pixelHeight = height; }
void Editor::setBezierSteps(int steps) { m_bezierSteps = steps; }
void Editor::processEvents() {
  sf::Event event;
  while (m_window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      m_window.close();
    else {
      m_inputHandler.handleEvent(event, m_textBuffer, m_cursor, m_selection,
                                 m_pixelHeight, m_bezierSteps);
    }
  }
  m_textRenderer.setPixelHeight(m_pixelHeight);
  m_textRenderer.setBezierSteps(m_bezierSteps);
}

void Editor::update() {
  if (m_blinkClock.getElapsedTime().asMilliseconds() >= 500) {
    m_blinkClock.restart();
    m_cursor.toggleVisibility();
  }
}

void Editor::render() {
  m_window.clear(sf::Color::Black);
  m_textRenderer.renderText(m_window, m_textBuffer);
  m_textRenderer.renderCursor(m_window, m_cursor);
  m_textRenderer.renderSelection(m_window, m_selection);
  m_window.display();
}
