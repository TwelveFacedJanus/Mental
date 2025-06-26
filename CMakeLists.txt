cmake_minimum_required(VERSION 3.11)
project(Mental LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_language(C)
set(SFD SourceFiles)
set(HFD HeaderFiles)
set(EXECUTABLE ${SFD}/main.cpp
    ${SFD}/Mental.cpp
    ${SFD}/stb_image.cpp
    ${SFD}/Component.cpp
    ${SFD}/Components/Sprite2D.cpp
    ${SFD}/Components/Camera.cpp
    ${SFD}/Components/AnimatedSprite2D.cpp
    ${SFD}/Ui.cpp
)

# Find GLFW
find_path(GLFW_INCLUDE_DIR NAMES GLFW/glfw3.h)
find_library(GLFW_LIBRARY NAMES glfw)
find_package(glm)

# Find GLEW
find_path(GLEW_INCLUDE_DIR NAMES GL/glew.h)
find_library(GLEW_LIBRARY NAMES GLEW glew32 glew32s)

# Set Lua paths for Homebrew
set(LUA_INCLUDE_DIR "/opt/homebrew/include")
set(LUA_LIBRARY "/opt/homebrew/lib/liblua.dylib")

# Find Lua
find_path(LUA_INCLUDE_DIR NAMES lua.h PATHS ${LUA_INCLUDE_DIR})
find_library(LUA_LIBRARY NAMES lua PATHS ${LUA_LIBRARY})

# Dear ImGui
set(IMGUI_DIR "External/imgui")
file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)
include_directories(${IMGUI_DIR})

# TextEditor
set(TEXTEDITOR_DIR "External/TextEditor")
set(TEXTEDITOR_SOURCES
    ${TEXTEDITOR_DIR}/TextEditor.cpp
)
include_directories(${TEXTEDITOR_DIR})

# Native File Dialog (NFD)
set(NFD_DIR "External/nativefiledialog/src")
set(NFD_SOURCES
    # nfd_common.c не добавляем сюда!
)
include_directories(${NFD_DIR}/include)

include_directories(${GLFW_INCLUDE_DIR} ${GLEW_INCLUDE_DIR} ${HFD} ${LUA_INCLUDE_DIR})

enable_language(OBJC)

add_executable(${PROJECT_NAME} ${EXECUTABLE} ${IMGUI_SOURCES} ${TEXTEDITOR_SOURCES} ${NFD_SOURCES})

set_source_files_properties(${NFD_DIR}/nfd_common.c PROPERTIES LANGUAGE C)
set_source_files_properties(${NFD_DIR}/nfd_cocoa.m PROPERTIES LANGUAGE OBJC)
target_sources(${PROJECT_NAME} PRIVATE ${NFD_DIR}/nfd_common.c ${NFD_DIR}/nfd_cocoa.m)

target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY} ${GLEW_LIBRARY} "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" ${LUA_LIBRARY})

