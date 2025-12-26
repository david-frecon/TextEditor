#include "include/Editor/Editor.hh"
#include "include/Font/FontParser.hh"

int main() {
  Font font = Parse("fonts/jetregular.ttf");
  Editor editor(font);
  editor.run();
}