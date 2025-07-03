#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <filesystem>
#include <vector>
#include <set>
#include <map>

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

#include "stb_image.h"
#include "Mental.hpp"

typedef struct MentalComponentInfo {
    MentalComponentTypes cType = MENTAL_COMPONENT_TRIANGLE;
} MentalComponentInfo;

typedef struct MentalComponent {
    MentalComponentInfo info;
    lua_State* L = nullptr;
    std::filesystem::file_time_type last_script_write_time;
    GLuint textureID = 0;
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // или glm::vec3 для углов
    std::string script_path;
    std::string texture_path;
    GLuint shaderProgram = 0;
    GLuint VBO, VAO, EBO;
    bool flipped_h = false;
    static std::set<int> g_pressed_keys;
    static std::map<int, std::string> g_key_names;
} MentalComponent;

MentalResult mentalComponentCreate(MentalComponentInfo* info, MentalComponent* component);

void _mentalCreateTriangle(MentalComponent* component);

class Component {
private:
    static std::string loadShaderSource(const std::string& path);
    static void checkCompileErrors(GLuint shader, const std::string& type);
    static void checkLinkErrors(GLuint program);
protected:
    lua_State* L = nullptr;
    std::filesystem::file_time_type last_script_write_time;
    GLuint textureID = 0;
    
    void init_buffers(const float* vertices, size_t vert_size, const unsigned int* indices, size_t ind_size, int pos_size, int tex_offset = -1, int tex_size = 0);
public:
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // или glm::vec3 для углов
    std::string script_path;
    std::string texture_path;
    GLuint shaderProgram = 0;
    GLuint VBO, VAO, EBO;
    bool flipped_h = false;

    virtual void setModelMatrix(const glm::mat4& m) {model = m;}
    virtual void rotate(float angle, const glm::vec3& axis);
    virtual void set_position(float x, float y, float z);
    virtual void setShader(const std::string& vertexPath, const std::string& fragmentPath) = 0;
    virtual void apply_shader() = 0;
    virtual void draw() = 0;
    virtual void set_scale(float x, float y, float z);
    virtual void _set_shader(const std::string& vertexPath, const std::string& fragmentPath, GLuint* shaderProgram);
    virtual void _apply_shader(GLuint* shaderProgram);
    virtual void reset_model_matrix();
    virtual void add_script(const std::string& path_to_script);
    virtual void reload_script_if_changed();
    virtual void run_script_main();
    virtual void call_process(float delta);
    virtual void applyTexture(const std::string& path_to_texture);
    virtual void flip_h();
    virtual void update_model_matrix();
    virtual ~Component();
    virtual GLuint getShaderProgram() const = 0;

    
    static std::set<int> g_pressed_keys;
    static std::map<int, std::string> g_key_names;

    static int lua_rotate(lua_State* L);
    static int lua_set_position(lua_State* L);
    static int lua_reset_model_matrix(lua_State* L);
    static int lua_apply_texture(lua_State* L);
    static int lua_get_glfw_action(lua_State* L);
    
    
    static int lua_triangle_set_position(lua_State* L);
    static int lua_triangle_rotate(lua_State* L);
    static int lua_triangle_reset_model_matrix(lua_State* L);
    static int lua_create_triangle(lua_State* L);
    static void register_triangle(lua_State* L);
    
    static int lua_camera_set_position(lua_State* L);
    static int lua_camera_look_at(lua_State* L);
    static int lua_camera_set_perspective(lua_State* L);
    static int lua_create_camera(lua_State* L);
    static void register_camera(lua_State* L);
    static int lua_set_active_camera(lua_State* L);

    static int lua_export_sprite_position(lua_State* L);
    static int lua_sprite2d_add_script(lua_State* L);
    static int lua_sprite2d_set_scale(lua_State* L);
    static int lua_sprite2d_set_position(lua_State* L);
    static int lua_sprite2d_apply_texture(lua_State* L);
    static int lua_sprite2d_rotate(lua_State* L);
    static int lua_sprite2d_reset_model_matrix(lua_State* L);
    static int lua_create_sprite2d(lua_State* L);
    static int lua_sprite2d_flip_h(lua_State* L);
    static void register_sprite2d(lua_State* L);
    
    const std::string& get_script_path() const { return script_path; }
};

extern std::vector<Component*> g_lua_components;
class Camera;
extern Camera* g_active_camera;
extern float g_sprite_x, g_sprite_y, g_sprite_z;



class Triangle : public Component
{
public:
    float vertices[15] = {
        // positions         // texcoords
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.0f,  0.5f, 0.0f,  0.5f, 1.0f
    };
    Triangle();
    void apply_shader() override;
    void setShader(const std::string& vertexPath = "./Shaders/triangle.vert", const std::string& fragmentPath = "./Shaders/triangle.frag") override;
    void draw() override;
    ~Triangle();
    GLuint getShaderProgram() const override;
};

class Rect : public Component
{
public:
    float vertices[12] = {
        // Positions for a rectangle (two triangles)
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
       -0.5f, -0.5f, 0.0f,  // bottom left
       -0.5f,  0.5f, 0.0f   // top left
    };
    unsigned int indices[6] = {
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };
    Rect();
    void setShader(const std::string& vertexPath = "./Shaders/rectangle.vert", const std::string& fragmentPath = "./Shaders/rectangle.frag") override;
    void draw() override;
    ~Rect();
    GLuint getShaderProgram() const override;
};

class Sprite2D : public Component {
public:
    float vertices[20] = {
        // positions        // texcoords
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[6] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    std::vector<GLuint> textureIDs;
    int current_texture_index = 0;
    Sprite2D();
    void setShader(const std::string& vertexPath = "./Shaders/triangle.vert", const std::string& fragmentPath = "./Shaders/triangle.frag") override;
    void draw() override;
    ~Sprite2D();
    GLuint getShaderProgram() const override;
    void flip_h() override {
        flipped_h = !flipped_h;
        update_vertices();
    }
    void addTexture(const std::string& path);
    void setTextureIndex(int idx);
    // Lua wrappers
    static int lua_sprite2d_add_texture(lua_State* L);
    static int lua_sprite2d_set_texture_index(lua_State* L);
    void apply_shader() override {
        _apply_shader(&this->shaderProgram);
        glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
    }
private:
    void update_vertices();
};


class Camera : public Component {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov, aspect, nearPlane, farPlane;
    glm::mat4 view, projection;
private:
    glm::vec3 interp_position;
    glm::vec3 interp_target;
public:
    Camera( glm::vec3 pos = glm::vec3(0,0,3), glm::vec3 tgt = glm::vec3(0,0,0), glm::vec3 upv = glm::vec3(0,1,0),
            float fov_ = 45.0f, float aspect_ = 4.0f/3.0f, float near_ = 0.1f, float far_ = 100.0f);
    void set_position(float x, float y, float z) override;
    void look_at(float x, float y, float z);
    void set_perspective(float fov_, float aspect_, float near_, float far_);
    void update_matrices();
    void update_interpolated(float alpha, const glm::vec3& new_pos, const glm::vec3& new_target);
    glm::vec3 getInterpPosition() const;
    glm::vec3 getInterpTarget() const;
    glm::mat4 getView() const;
    glm::mat4 getProjection() const;
    void setShader(const std::string&, const std::string&) override {}
    void apply_shader() override {}
    void draw() override {}
    GLuint getShaderProgram() const override;
};

class AnimatedSprite2D : public Sprite2D {
public:
    struct Animation {
        int start_frame = 0;
        int end_frame = 0;
        float frame_time = 0.1f;
        bool loop = true;
    };
    int rows = 1;
    int cols = 1;
    int total_frames = 1;
    int current_frame = 0;
    float time_accum = 0.0f;
    bool playing = true;
    std::map<std::string, Animation> animations;
    std::string current_anim;
    std::string tileset_path;
    float frame_duration = 0.1f;
    AnimatedSprite2D(int rows_ = 1, int cols_ = 1, float frame_time_ = 0.1f);
    void set_frame(int frame);
    void update_animation(float delta);
    void play(const std::string& anim_name = "");
    void pause();
    void stop();
    void add_animation(const std::string& name, int start, int end, float frame_time, bool loop);
    void remove_animation(const std::string& name);
    void set_animation_params(const std::string& name, int start, int end, float frame_time, bool loop);
};

class TileMap : public Component {
public:
    int rows = 1;
    int cols = 1;
    int tile_width = 32;
    int tile_height = 32;
    std::vector<int> tiles; // Индексы тайлов (размер rows*cols)
    GLuint tileset_texture = 0;
    int tileset_rows = 1;
    int tileset_cols = 1;
    std::string tileset_path;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    TileMap() {}
    // Установить тайлсет (текстура, сетка)
    void setTileset(GLuint tex, int t_rows, int t_cols) {
        tileset_texture = tex;
        tileset_rows = t_rows;
        tileset_cols = t_cols;
    }
    // Установить карту (размер и индексы)
    void setMap(int r, int c, const std::vector<int>& t) {
        rows = r; cols = c; tiles = t;
        if ((int)tiles.size() != rows*cols) tiles.resize(rows*cols, 0);
    }
    // Рендеринг тайлмапа
    void draw() override {
        if (!tileset_texture || !shaderProgram || !VAO) return;
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        if (g_active_camera) {
            view = g_active_camera->getView();
            projection = g_active_camera->getProjection();
        }
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tileset_texture);
        GLuint texLoc = glGetUniformLocation(shaderProgram, "ourTexture");
        glUniform1i(texLoc, 0);
        glBindVertexArray(VAO);
        // Нарисовать только один тайл в центре
        int x = 0, y = 0;
        int idx = tiles[y*cols + x];
        float quad_vertices[20];
        float px = (x - cols/2.0f) * tile_width;
        float py = (y - rows/2.0f) * tile_height;
        // Сгенерировать quad с этим смещением
        int t_row = idx / tileset_cols;
        int t_col = idx % tileset_cols;
        float u0 = (float)t_col / tileset_cols;
        float v0 = (float)t_row / tileset_rows;
        float u1 = (float)(t_col+1) / tileset_cols;
        float v1 = (float)(t_row+1) / tileset_rows;
        float uv_top = v0, uv_bot = v1;
        // top left
        quad_vertices[0] = px;              quad_vertices[1] = py + tile_height; quad_vertices[2] = 0.0f; quad_vertices[3] = u0; quad_vertices[4] = uv_top;
        // top right
        quad_vertices[5] = px + tile_width; quad_vertices[6] = py + tile_height; quad_vertices[7] = 0.0f; quad_vertices[8] = u1; quad_vertices[9] = uv_top;
        // bottom right
        quad_vertices[10] = px + tile_width; quad_vertices[11] = py; quad_vertices[12] = 0.0f; quad_vertices[13] = u1; quad_vertices[14] = uv_bot;
        // bottom left
        quad_vertices[15] = px; quad_vertices[16] = py; quad_vertices[17] = 0.0f; quad_vertices[18] = u0; quad_vertices[19] = uv_bot;
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad_vertices), quad_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::scale(model, scale);
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
    void setShader(const std::string& vertexPath = "./Shaders/triangle.vert", const std::string& fragmentPath = "./Shaders/triangle.frag") override {
        _set_shader(vertexPath, fragmentPath, &shaderProgram);
        // quad (2 triangles)
        float quad_vertices[20] = {
            // pos         // uv
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top left
            1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f  // bottom left
        };
        unsigned int quad_indices[6] = { 0, 1, 2, 0, 2, 3 };
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void update_tile_vertices(int x, int y, int tile_idx, float* out_vertices) {
        float px = x * tile_width;
        float py = y * tile_height;
        int t_row = tile_idx / tileset_cols;
        int t_col = tile_idx % tileset_cols;
        float u0 = (float)t_col / tileset_cols;
        float v0 = (float)t_row / tileset_rows;
        float u1 = (float)(t_col+1) / tileset_cols;
        float v1 = (float)(t_row+1) / tileset_rows;
        // flip v for OpenGL (исправлено)
        float uv_top = v0, uv_bot = v1;
        // top left
        out_vertices[0] = px;              out_vertices[1] = py + tile_height; out_vertices[2] = 0.0f; out_vertices[3] = u0; out_vertices[4] = uv_top;
        // top right
        out_vertices[5] = px + tile_width; out_vertices[6] = py + tile_height; out_vertices[7] = 0.0f; out_vertices[8] = u1; out_vertices[9] = uv_top;
        // bottom right
        out_vertices[10] = px + tile_width; out_vertices[11] = py; out_vertices[12] = 0.0f; out_vertices[13] = u1; out_vertices[14] = uv_bot;
        // bottom left
        out_vertices[15] = px; out_vertices[16] = py; out_vertices[17] = 0.0f; out_vertices[18] = u0; out_vertices[19] = uv_bot;
    }
    void apply_shader() override {}
    GLuint getShaderProgram() const override { return shaderProgram; }
    void saveToFile(const std::string& path) {
        std::ofstream out(path);
        out << rows << ' ' << cols << ' ' << tile_width << ' ' << tile_height << ' ' << tileset_rows << ' ' << tileset_cols << '\n';
        out << tileset_path << '\n';
        for (int i = 0; i < (int)tiles.size(); ++i) {
            out << tiles[i];
            if ((i+1) % cols == 0) out << '\n';
            else out << ' ';
        }
    }
    bool loadFromFile(const std::string& path) {
        std::ifstream in(path);
        if (!in) return false;
        in >> rows >> cols >> tile_width >> tile_height >> tileset_rows >> tileset_cols;
        in.ignore();
        std::getline(in, tileset_path);
        tiles.resize(rows*cols);
        for (int i = 0; i < rows*cols; ++i) in >> tiles[i];
        return true;
    }
    ~TileMap() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
        if (shaderProgram) glDeleteProgram(shaderProgram);
    }
};

class Cube3D : public Component {
public:
    // 8 vertices, 36 indices (12 triangles)
    float vertices[120] = {
        // positions         // texcoords
        // Front face
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
        // Back face
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    };
    unsigned int indices[36] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Back face
        7, 6, 5, 5, 4, 7,
        // Left face
        4, 0, 3, 3, 7, 4,
        // Bottom face
        4, 5, 1, 1, 0, 4,
        // Top face
        3, 2, 6, 6, 7, 3
    };
    Cube3D() {
        init_buffers(vertices, sizeof(vertices), indices, sizeof(indices), 3, 3, 2);
    }
    void setShader(const std::string& vertexPath = "./Shaders/triangle.vert", const std::string& fragmentPath = "./Shaders/triangle.frag") override {
        _set_shader(vertexPath, fragmentPath, &this->shaderProgram);
    }
    void apply_shader() override {
        _apply_shader(&this->shaderProgram);
        glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
    }
    void draw() override {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    ~Cube3D() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        if (shaderProgram) glDeleteProgram(shaderProgram);
    }
    GLuint getShaderProgram() const override { return shaderProgram; }
};

class Node : public Component
{
public:
    std::map<std::string, std::shared_ptr<Component>> child_components;
    std::map<std::string, std::shared_ptr<Node>> child_nodes;

    void add_component(const std::string& name, std::shared_ptr<Component> component) {
        child_components[name] = component;
    }
    void remove_component(const std::string& name) {
        child_components.erase(name);
    }
    std::shared_ptr<Component> get_component(const std::string& name) {
        auto it = child_components.find(name);
        if (it != child_components.end()) return it->second;
        return nullptr;
    }
    void add_node(const std::string& name, std::shared_ptr<Node> node) {
        child_nodes[name] = node;
    }
    void remove_node(const std::string& name) {
        child_nodes.erase(name);
    }
    std::shared_ptr<Node> get_node(const std::string& name) {
        auto it = child_nodes.find(name);
        if (it != child_nodes.end()) return it->second;
        return nullptr;
    }
    void draw() override {
        for (auto& [name, comp] : child_components) {
            if (comp) comp->draw();
        }
        for (auto& [name, node] : child_nodes) {
            if (node) node->draw();
        }
    }
    void setShader(const std::string& vertexPath, const std::string& fragmentPath) override {
        for (auto& [name, comp] : child_components) {
            if (comp) comp->setShader(vertexPath, fragmentPath);
        }
        for (auto& [name, node] : child_nodes) {
            if (node) node->setShader(vertexPath, fragmentPath);
        }
    }
    void apply_shader() override {
        for (auto& [name, comp] : child_components) {
            if (comp) comp->apply_shader();
        }
        for (auto& [name, node] : child_nodes) {
            if (node) node->apply_shader();
        }
    }
    GLuint getShaderProgram() const override {
        if (!child_components.empty()) {
            auto it = child_components.begin();
            if (it->second) return it->second->getShaderProgram();
        }
        if (!child_nodes.empty()) {
            auto it = child_nodes.begin();
            if (it->second) return it->second->getShaderProgram();
        }
        return 0;
    }

    // Lua bindings
    static int lua_create_node(lua_State* L);
    static int lua_node_add_component(lua_State* L);
    static int lua_node_add_node(lua_State* L);
    static void register_node(lua_State* L);
};

