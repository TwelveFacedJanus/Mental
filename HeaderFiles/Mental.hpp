#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderPipeline.hpp"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#include <filesystem>
#include <unordered_map>
#include <functional>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../External/imgui/backends/imgui_impl_opengl3.h"
#include "../External/imgui/backends/imgui_impl_glfw.h"
#include "../External/TextEditor/TextEditor.h"

#include <string>
#include <vector>

#include <stb_image.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "../External/nativefiledialog/src/include/nfd.h"
#ifdef __cplusplus
}
#endif

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

typedef enum MentalStructureTypes {
    MENTAL_ENGINE_CORE_INFO = 0,
    MENTAL_ENGINE_CORE      = 1,

    MENTAL_ENGINE_UI_INFO = 2,
    MENTAL_ENGINE_UI = 3,
} MentalStructureTypes; 

typedef enum MentalComponentTypes {
    MENTAL_COMPONENT_TRIANGLE = 0,
    MENTAL_COMPONENT_RECTANGLE = 1,
    MENTAL_COMPONENT_CUBE = 2,
    MENTAL_COMPONENT_SPRITE2D = 3,
    MENTAL_COMPONENT_CAMERA = 4,
    MENTAL_COMPONENT_NODE = 5,
} MentalComponentTypes;

typedef enum MentalResult {
    MENTAL_OK,
    MENTAL_FATAL,
    MENTAL_ERROR,
} MentalResult;

typedef struct MentalEngineUI MentalEngineUI;
typedef struct MentalEngineCore MentalEngineCore;

#include "Component.hpp"
#include "./Ui.hpp"

typedef struct MentalEngineCore {
    MentalStructureTypes sType = MENTAL_ENGINE_CORE;
    GLFWwindow* window = nullptr;
    lua_State* L = luaL_newstate();
    RenderPipeline* rp = new RenderPipeline();
    
    GLuint viewport_fbo = 0;
    GLuint viewport_texture = 0;
    GLuint viewport_depth = 0;

    int viewport_size[2] = {800, 600};
    int current_size[2] = {800, 600};
    MentalEngineUI ui;
} MentalEngineCore;

MentalResult mentalEngineInitialize(MentalEngineCore* instance);
MentalResult mentalEngineMainLoop(MentalEngineCore* instance);
MentalResult mentalEngineCreateViewportFBO(MentalEngineCore* instance, int width, int height);
