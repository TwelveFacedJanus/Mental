#include "Component.hpp"

std::vector<Component*> g_lua_components;

std::string Component::loadShaderSource(const std::string& path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void Component::rotate(float angle, const glm::vec3& axis) {
    rotation = glm::rotate(rotation, angle, axis);
    update_model_matrix();
}

void Component::set_position(float x, float y, float z) {
    position = glm::vec3(x, y, z);
    update_model_matrix();
}

void Component::set_scale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
    update_model_matrix();
}

void Component::update_model_matrix() {
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model *= glm::mat4_cast(rotation);
    model = glm::scale(model, scale);
}

void Component::reset_model_matrix() {
    position = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    update_model_matrix();
}


void Component::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
    }
}
void Component::checkLinkErrors(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n" << infoLog << std::endl;
    }
}

void Component::_set_shader(const std::string& vertexPath, const std::string& fragmentPath, GLuint* shaderProgram)
{
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vShaderCode = vertexCode.c_str();
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fShaderCode = fragmentCode.c_str();
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    checkLinkErrors(program);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    *shaderProgram = program;
}

void Component::_apply_shader(GLuint* shaderProgram) {
    glUseProgram(*shaderProgram);
}

Component::~Component() {
    if (L) lua_close(L);
}

int Component::lua_rotate(lua_State* L) {
    Component* self = static_cast<Component*>(lua_touserdata(L, lua_upvalueindex(1)));
    float angle = luaL_checknumber(L, 1);
    int axis = luaL_checkinteger(L, 2);
    if (axis == 1) {
        self->rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
    } else if (axis == 2) {
        self->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    } else if (axis == 3) {
        self->rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
    }
    return 0;
}

int Component::lua_set_position(lua_State* L) {
    Component* self = static_cast<Component*>(lua_touserdata(L, lua_upvalueindex(1)));
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);
    float z = luaL_optnumber(L, 3, 0.0f);
    self->set_position(x, y, z);
    return 0;
}

int Component::lua_reset_model_matrix(lua_State* L) {
    Component* self = static_cast<Component*>(lua_touserdata(L, lua_upvalueindex(1)));
    self->reset_model_matrix();
    return 0;
}

int Component::lua_sprite2d_set_scale(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    float z = luaL_optnumber(L, 4, 1.0f);
    sprite->set_scale(x, y, z);
    return 0;
}

int Component::lua_apply_texture(lua_State* L) {
    Component* self = static_cast<Component*>(lua_touserdata(L, lua_upvalueindex(1)));
    const char* path = luaL_checkstring(L, 1);
    self->texture_path = path;
    self->applyTexture(path);
    return 0;
}

void Component::flip_h() {
    scale.x *= -1.0f;
    update_model_matrix();
}

int Component::lua_get_glfw_action(lua_State* L) {
    lua_newtable(L);
    int idx = 1;
    for (int key : g_pressed_keys) {
        auto it = g_key_names.find(key);
        if (it != g_key_names.end()) {
            lua_pushstring(L, it->second.c_str());
        } else {
            lua_pushfstring(L, "Key_%d", key);
        }
        lua_rawseti(L, -2, idx++);
    }
    return 1;
}

void Component::add_script(const std::string& path_to_script) {
    script_path = path_to_script;
    last_script_write_time = std::filesystem::last_write_time(script_path);
    L = luaL_newstate();
    luaL_openlibs(L);
    register_sprite2d(L);
    register_triangle(L);
    register_camera(L);
    lua_pushcfunction(L, lua_set_active_camera);
    lua_setglobal(L, "set_active_camera");
    lua_pushcfunction(L, lua_export_sprite_position);
    lua_setglobal(L, "export_sprite_position");
    lua_pushcfunction(L, lua_get_glfw_action);
    lua_setglobal(L, "get_glfw_action");
    lua_pushlightuserdata(L, this);
    lua_pushcclosure(L, &Component::lua_set_position, 1);
    lua_setglobal(L, "set_position");
    lua_pushlightuserdata(L, this);
    lua_pushcclosure(L, &Component::lua_rotate, 1);
    lua_setglobal(L, "rotate");
    lua_pushlightuserdata(L, this);
    lua_pushcclosure(L, &Component::lua_reset_model_matrix, 1);
    lua_setglobal(L, "reset_model_matrix");
    lua_pushlightuserdata(L, this);
    lua_pushcclosure(L, &Component::lua_apply_texture, 1);
    lua_setglobal(L, "apply_texture");

    // Inject 'self' as global in Lua
    // For Sprite2D, set metatable to Sprite2DMeta
    if (dynamic_cast<Sprite2D*>(this)) {
        Sprite2D** udata = (Sprite2D**)lua_newuserdata(L, sizeof(Sprite2D*));
        *udata = static_cast<Sprite2D*>(this);
        luaL_getmetatable(L, "Sprite2DMeta");
        lua_setmetatable(L, -2);
        lua_setglobal(L, "self");
    }
    // TODO: Add similar for Triangle/Camera if needed

    if (luaL_dofile(L, script_path.c_str()) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

void Component::reload_script_if_changed() {
    if (script_path.empty()) return;
    auto current_write_time = std::filesystem::last_write_time(script_path);
    if (current_write_time != last_script_write_time) {
        if (L) lua_close(L);
        L = nullptr;
        add_script(script_path);
        run_script_main();
        std::cout << "Script reloaded: " << script_path << std::endl;
    }
}

void Component::run_script_main() {
    if (!L) return;
    lua_getglobal(L, "main");
    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            std::cerr << "Lua error in main: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
    } else {
        lua_pop(L, 1);
        std::cerr << "No 'main' function found in script: " << script_path << std::endl;
    }
}

void Component::call_process(float delta) {
    if (!L) return;
    lua_getglobal(L, "process");
    if (lua_isfunction(L, -1)) {
        lua_pushnumber(L, delta);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            std::cerr << "Lua error in process: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }
    } else {
        lua_pop(L, 1); // remove non-function
    }
}
void Component::applyTexture(const std::string& path_to_texture) {
    if (textureID) {
        glDeleteTextures(1, &textureID);
    }
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path_to_texture.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << path_to_texture << std::endl;
    }
    stbi_image_free(data);
}



std::set<int> Component::g_pressed_keys;
std::map<int, std::string> Component::g_key_names = {
    {87, "Key_W"},
    {65, "Key_A"},
    {83, "Key_S"},
    {68, "Key_D"},
    {32, "Key_SPACE"},
    {256, "Key_ESCAPE"},
    {262, "Key_RIGHT"},
    {263, "Key_LEFT"},
    {264, "Key_DOWN"},
    {265, "Key_UP"},
    // ... другие клавиши ...
};

Camera* g_active_camera = nullptr;

float g_sprite_x = 0.0f, g_sprite_y = 0.0f, g_sprite_z = 0.0f;

int Component::lua_sprite2d_flip_h(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    sprite->flip_h();
    return 0;
}

void Component::register_sprite2d(lua_State* L) {
    luaL_newmetatable(L, "Sprite2DMeta");

    // Create a table for methods
    lua_newtable(L);
    lua_pushcfunction(L, lua_sprite2d_set_position);
    lua_setfield(L, -2, "set_position");
    lua_pushcfunction(L, lua_sprite2d_apply_texture);
    lua_setfield(L, -2, "apply_texture");
    lua_pushcfunction(L, lua_sprite2d_rotate);
    lua_setfield(L, -2, "rotate");
    lua_pushcfunction(L, lua_sprite2d_reset_model_matrix);
    lua_setfield(L, -2, "reset_model_matrix");
    lua_pushcfunction(L, lua_sprite2d_add_script);
    lua_setfield(L, -2, "add_script");
    lua_pushcfunction(L, lua_sprite2d_set_scale);
    lua_setfield(L, -2, "set_scale");
    lua_pushcfunction(L, lua_sprite2d_flip_h);
    lua_setfield(L, -2, "flip_h");
    // --- new animated texture methods ---
    lua_pushcfunction(L, Sprite2D::lua_sprite2d_add_texture);
    lua_setfield(L, -2, "add_texture");
    lua_pushcfunction(L, Sprite2D::lua_sprite2d_set_texture_index);
    lua_setfield(L, -2, "set_texture_index");
    // Set __index = methods table
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1);
    lua_pushcfunction(L, lua_create_sprite2d);
    lua_setglobal(L, "create_sprite2d");
    
}

void Component::register_triangle(lua_State* L) {
    luaL_newmetatable(L, "TriangleMeta");
    lua_pushcfunction(L, lua_triangle_set_position);
    lua_setfield(L, -2, "set_position");
    lua_pushcfunction(L, lua_triangle_rotate);
    lua_setfield(L, -2, "rotate");
    lua_pushcfunction(L, lua_triangle_reset_model_matrix);
    lua_setfield(L, -2, "reset_model_matrix");
    lua_pop(L, 1);
    lua_pushcfunction(L, lua_create_triangle);
    lua_setglobal(L, "create_triangle");
}


int Component::lua_set_active_camera(lua_State* L) {
    Camera* cam = *(Camera**)luaL_checkudata(L, 1, "CameraMeta");
    g_active_camera = cam;
    return 0;
}

int Component::lua_export_sprite_position(lua_State* L) {
    g_sprite_x = luaL_checknumber(L, 1);
    g_sprite_y = luaL_checknumber(L, 2);
    g_sprite_z = luaL_optnumber(L, 3, 0.0f);
    return 0;
}

int Component::lua_sprite2d_set_position(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    float z = luaL_optnumber(L, 4, 0.0f);
    sprite->set_position(x, y, z);
    return 0;
}
int Component::lua_sprite2d_apply_texture(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    const char* path = luaL_checkstring(L, 2);
    sprite->applyTexture(path);
    return 0;
}
int Component::lua_sprite2d_rotate(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    float angle = luaL_checknumber(L, 2);
    int axis = luaL_checkinteger(L, 3);
    if (axis == 1) sprite->rotate(angle, glm::vec3(1,0,0));
    else if (axis == 2) sprite->rotate(angle, glm::vec3(0,1,0));
    else if (axis == 3) sprite->rotate(angle, glm::vec3(0,0,1));
    return 0;
}
int Component::lua_sprite2d_reset_model_matrix(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    sprite->reset_model_matrix();
    return 0;
}

int Component::lua_create_sprite2d(lua_State* L) {
    std::cout << "Creating new sprite\n";
    Sprite2D* sprite = new Sprite2D();
    sprite->setShader();
    g_lua_components.push_back(sprite);
    *(Sprite2D**)lua_newuserdata(L, sizeof(Sprite2D*)) = sprite;
    std::cout << "[lua_create_sprite2d] Created Sprite2D: " << sprite << ", shader=" << sprite->shaderProgram << std::endl;
    luaL_getmetatable(L, "Sprite2DMeta");
    lua_setmetatable(L, -2);
    return 1;
}
int Component::lua_sprite2d_add_script(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    const char* script_path = luaL_checkstring(L, 2);
    sprite->add_script(script_path);
    sprite->run_script_main(); // чтобы сразу вызвать main() из скрипта
    return 0;
}

int Component::lua_triangle_set_position(lua_State* L) {
    Triangle* tri = *(Triangle**)luaL_checkudata(L, 1, "TriangleMeta");
    float x = luaL_checknumber(L, 2);
    float y = luaL_checknumber(L, 3);
    float z = luaL_optnumber(L, 4, 0.0f);
    tri->set_position(x, y, z);
    return 0;
}
int Component::lua_triangle_rotate(lua_State* L) {
    Triangle* tri = *(Triangle**)luaL_checkudata(L, 1, "TriangleMeta");
    float angle = luaL_checknumber(L, 2);
    int axis = luaL_checkinteger(L, 3);
    if (axis == 1) tri->rotate(angle, glm::vec3(1,0,0));
    else if (axis == 2) tri->rotate(angle, glm::vec3(0,1,0));
    else if (axis == 3) tri->rotate(angle, glm::vec3(0,0,1));
    return 0;
}
int Component::lua_triangle_reset_model_matrix(lua_State* L) {
    Triangle* tri = *(Triangle**)luaL_checkudata(L, 1, "TriangleMeta");
    tri->reset_model_matrix();
    return 0;
}
int Component::lua_create_triangle(lua_State* L) {
    Triangle* tri = new Triangle();
    g_lua_components.push_back(tri);
    *(Triangle**)lua_newuserdata(L, sizeof(Triangle*)) = tri;
    luaL_getmetatable(L, "TriangleMeta");
    lua_setmetatable(L, -2);
    return 1;
}