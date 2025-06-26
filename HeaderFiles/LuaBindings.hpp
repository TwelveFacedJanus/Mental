#pragma once

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

#include "Component.hpp"


static int lua_set_position(Component* component, lua_State* L) {
    Component* self = static<lua>
}
static int lua_rotate(Component* component, lua_State* L);
static int lua_apply_texture(Component* component, lua_State* L);
static int lua_get_glfw_action(Component* component, lua_State* L);
static int lua_reset_model_matrix(Component* component, lua_State* L);
