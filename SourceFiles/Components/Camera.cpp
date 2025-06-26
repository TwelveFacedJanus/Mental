#include "Component.hpp"

int Component::lua_camera_set_position(lua_State* L) {
    Camera* cam = *(Camera**)luaL_checkudata(L, 1, "CameraMeta");
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    float z = luaL_optnumber(L, 4, 0.0f);
    cam->set_position(x, y, z);
    return 0;
}
int Component::lua_camera_look_at(lua_State* L) {
    Camera* cam = *(Camera**)luaL_checkudata(L, 1, "CameraMeta");
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    float z = luaL_optnumber(L, 4, 0.0f);
    cam->look_at(x, y, z);
    return 0;
}
int Component::lua_camera_set_perspective(lua_State* L) {
    Camera* cam = *(Camera**)luaL_checkudata(L, 1, "CameraMeta");
    float fov = luaL_checknumber(L, 2);
    float aspect = luaL_checknumber(L, 3);
    float nearP = luaL_checknumber(L, 4);
    float farP = luaL_checknumber(L, 5);
    cam->set_perspective(fov, aspect, nearP, farP);
    return 0;
}
int Component::lua_create_camera(lua_State* L) {
    Camera* cam = new Camera();
    *(Camera**)lua_newuserdata(L, sizeof(Camera*)) = cam;
    luaL_getmetatable(L, "CameraMeta");
    lua_setmetatable(L, -2);
    return 1;
}
void Component::register_camera(lua_State* L) {
    luaL_newmetatable(L, "CameraMeta");
    lua_pushcfunction(L, lua_camera_set_position);
    lua_setfield(L, -2, "set_position");
    lua_pushcfunction(L, lua_camera_look_at);
    lua_setfield(L, -2, "look_at");
    lua_pushcfunction(L, lua_camera_set_perspective);
    lua_setfield(L, -2, "set_perspective");
    lua_pop(L, 1);
    lua_pushcfunction(L, lua_create_camera);
    lua_setglobal(L, "create_camera");
}
