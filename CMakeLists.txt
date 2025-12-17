cmake_minimum_required(VERSION 3.15)

project(TextEditor
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.5.1 
)



FetchContent_MakeAvailable(sfml)

set(FONT_SOURCES
    src/Font/FontParser.cpp
)

set(CORE_SOURCES
    src/Core/TextBuffer.cpp
    src/Core/Cursor.cpp
)

set(RENDERING_SOURCES
    src/Rendering/GeometryUtils.cpp
    src/Rendering/GlyphRenderer.cpp
    src/Rendering/TextRenderer.cpp
)

set(INPUT_SOURCES
    src/Input/InputHandler.cpp
)

set(EDITOR_SOURCES
    src/Editor/Editor.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)


add_executable(${PROJECT_NAME}
    src/main.cpp
    ${FONT_SOURCES}
    ${CORE_SOURCES}
    ${RENDERING_SOURCES}
    ${INPUT_SOURCES}
    ${EDITOR_SOURCES}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/fonts)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts/jetregular.ttf
    ${CMAKE_CURRENT_BINARY_DIR}/fonts/jetregular.ttf
    COPYONLY
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -g -O0)
endif()
