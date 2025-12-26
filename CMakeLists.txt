cmake_minimum_required(VERSION 3.15)
project(TextEditor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Source files
set(FONT_SOURCES
    src/Font/FontParser.cpp
)

set(CORE_SOURCES
    src/Core/TextBuffer.cpp
    src/Core/Cursor.cpp
    src/Core/Selection.cpp
)

set(RENDERING_SOURCES
    src/Rendering/GeometryUtils.cpp
    src/Rendering/GlyphRenderer.cpp
    src/Rendering/AnalyticRenderer.cpp
    src/Rendering/TextRenderer.cpp
)

set(INPUT_SOURCES
    src/Input/InputHandler.cpp
)

set(EDITOR_SOURCES
    src/Editor/Editor.cpp
)

# Create executable
add_executable(${PROJECT_NAME}
    src/main.cpp
    ${FONT_SOURCES}
    ${CORE_SOURCES}
    ${RENDERING_SOURCES}
    ${INPUT_SOURCES}
    ${EDITOR_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# Copy font file to build directory
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fonts/jetregular.ttf 
               ${CMAKE_CURRENT_BINARY_DIR}/fonts/jetregular.ttf COPYONLY)

# Compiler warnings and Sanitizers
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    # GCC / Clang
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -Wall -Wextra -pedantic -g
        -fsanitize=address        # <--- Enable ASan compilation
        -fno-omit-frame-pointer   # <--- Better stack traces for ASan
    )
    
    # You must also link against ASan
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address) 
endif()