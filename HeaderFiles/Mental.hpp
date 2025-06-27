#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "RenderPipeline.hpp"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


#include "Component.hpp"

#include "Ui.hpp"

#define OPENGL_VERSION_MAJOR 3
#define OPENGL_VERSION_MINOR 3

class MentalWindow : public UI
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
        void load_root_script();
    public:
        MentalWindow();
        void main_loop();
        void addSprite2D(Sprite2D* sprite, float x, float y, float z, const std::string& script_path, const glm::mat4& default_view, const glm::mat4& default_projection, float delta);
        void addTriangle(Triangle* tr, const glm::mat4& default_view, const glm::mat4& default_projection, float delta);
        ~MentalWindow();
        void remove_component_from_pipeline(Component* comp);
};