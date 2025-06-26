#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderPipeline.hpp"

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

#include "Component.hpp"

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

class MentalWindow
{
    private:
        GLFWwindow* window = nullptr;
        RenderPipeline* rp = new RenderPipeline();
        lua_State* L = luaL_newstate();
    private:
        inline void initialize_glfw() noexcept(false);
        void initialize_glew() noexcept(false);
        void set_window_hints() noexcept(true);
        void create_window() noexcept(false);
    public:
        MentalWindow();
        void main_loop();
        ~MentalWindow();
};