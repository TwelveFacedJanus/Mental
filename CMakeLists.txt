cmake_minimum_required(VERSION 3.11)
project(Mental LANGUAGES C CXX VERSION 1.0.0)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform-specific settings
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Source and header files
set(SFD SourceFiles)
set(HFD HeaderFiles)
set(EXECUTABLE 
    ${SFD}/main.cpp
    ${SFD}/Mental.cpp
    ${SFD}/stb_image.cpp
    ${SFD}/Component.cpp
    ${SFD}/Components/Sprite2D.cpp
    ${SFD}/Components/Camera.cpp
    ${SFD}/Components/AnimatedSprite2D.cpp
    ${SFD}/Ui.cpp
)

# ==================== Dependency Configuration ====================

# GLFW
set(GLFW_DIR "External/glfw")
set(GLFW_INCLUDE_DIR "${GLFW_DIR}/include")
if(WIN32)
    set(GLFW_LIBRARY glfw3)
    link_directories(${GLFW_DIR}/lib-vc2022)
elseif(APPLE)
    set(GLFW_LIBRARY glfw)
    # Check for Homebrew (Apple Silicon and Intel)
    if(EXISTS /opt/homebrew/lib/libglfw.3.dylib)
        include_directories(/opt/homebrew/include)
        link_directories(/opt/homebrew/lib)
    elseif(EXISTS /usr/local/lib/libglfw.3.dylib)
        include_directories(/usr/local/include)
        link_directories(/usr/local/lib)
    endif()
    # If you want to use a local build, uncomment the next line and provide the library in lib-macos
    # link_directories(${GLFW_DIR}/lib-macos)
    # Otherwise, Homebrew's glfw will be found automatically by the linker
endif()

# GLEW
set(GLEW_DIR "External/glew")
set(GLEW_INCLUDE_DIR "${GLEW_DIR}/include")
if(WIN32)
    set(GLEW_LIBRARY glew32s)
    link_directories(${GLEW_DIR}/lib/Release/x64)
    add_definitions(-DGLEW_STATIC)
elseif(APPLE)
    set(GLEW_LIBRARY GLEW)
    # Check for Homebrew (Apple Silicon and Intel)
    if(EXISTS /opt/homebrew/lib/libGLEW.dylib)
        include_directories(/opt/homebrew/include)
        link_directories(/opt/homebrew/lib)
    elseif(EXISTS /usr/local/lib/libGLEW.dylib)
        include_directories(/usr/local/include)
        link_directories(/usr/local/lib)
    endif()
else()
    set(GLEW_LIBRARY "${GLEW_DIR}/lib/libGLEW.so")
endif()

# GLM (header-only)
set(GLM_DIR "External/glm")
set(GLM_INCLUDE_DIR "${GLM_DIR}")

# Lua
set(LUA_DIR "External/lua")
set(LUA_INCLUDE_DIR "${LUA_DIR}/lua/include") # Where lua/lua.h is located
if(WIN32)
    set(LUA_LIBRARY lua54)
    link_directories(${LUA_DIR}/lib)
elseif(APPLE)
    set(LUA_LIBRARY lua)
    # Check for Homebrew (Apple Silicon and Intel)
    if(EXISTS /opt/homebrew/lib/liblua.dylib)
        include_directories(/opt/homebrew/include)
        link_directories(/opt/homebrew/lib)
    elseif(EXISTS /usr/local/lib/liblua.dylib)
        include_directories(/usr/local/include)
        link_directories(/usr/local/lib)
    endif()
else()
    set(LUA_LIBRARY \"${LUA_DIR}/lib/liblua.a\")
endif()

# ImGui
set(IMGUI_DIR "External/imgui")
file(GLOB IMGUI_SOURCES
    "${IMGUI_DIR}/*.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_glfw.cpp"
    "${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp"
)

# TextEditor
set(TEXTEDITOR_DIR "External/TextEditor")
set(TEXTEDITOR_SOURCES ${TEXTEDITOR_DIR}/TextEditor.cpp)

# Native File Dialog
if(WIN32)
    set(NFD_DIR "External/nativefiledialog/src")
    set(NFD_SOURCES ${NFD_DIR}/nfd_win.cpp ${NFD_DIR}/nfd_common.c)
elseif(APPLE)
    enable_language(OBJC)
    set(NFD_DIR "External/nativefiledialog/src")
    set(NFD_SOURCES ${NFD_DIR}/nfd_cocoa.m ${NFD_DIR}/nfd_common.c)
endif()

# ==================== Include Directories ====================
include_directories(
    ${HFD}
    ${GLFW_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${IMGUI_DIR}
    ${TEXTEDITOR_DIR}
    ${NFD_DIR}/include
)

# ==================== Executable ====================
add_executable(${PROJECT_NAME} 
    ${EXECUTABLE} 
    ${IMGUI_SOURCES} 
    ${TEXTEDITOR_SOURCES} 
    ${NFD_SOURCES}
)

# ==================== Linking ====================
if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        ${LUA_LIBRARY}
        opengl32.lib
        gdi32.lib
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        ${LUA_LIBRARY}
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
else()
    target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARY}
        ${GLEW_LIBRARY}
        ${LUA_LIBRARY}
        GL
        dl
    )
endif()

# Ensure nfd_cocoa.m is compiled as Objective-C on macOS
if(APPLE)
    set_source_files_properties(${NFD_DIR}/nfd_cocoa.m PROPERTIES LANGUAGE OBJC)
endif()

# ==================== Post-Build (Windows DLLs) ====================
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/External/glew/bin/Release/x64/glew32.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/imgui.ini"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/External/lua/lib/lua54.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Fonts"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Fonts
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Scripts"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Scripts
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Textures"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Textures
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Windows/System32/vcruntime140.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Windows/System32/msvcp140d.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Windows/System32/vcruntime140d.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Windows/System32/vcruntime140_1d.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Windows/System32/ucrtbased.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
elseif(APPLE)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/imgui.ini"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Fonts"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Fonts
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Scripts"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Scripts
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Textures"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/Textures
    )
endif()