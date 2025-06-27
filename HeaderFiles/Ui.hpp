#pragma once

#include "Component.hpp"
// #include "Mental.hpp" // Удалено для устранения циклической зависимости
#include "../External/TextEditor/TextEditor.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../External/imgui/backends/imgui_impl_opengl3.h"
#include "../External/imgui/backends/imgui_impl_glfw.h"
#include "Mental.hpp"

class MentalWindow; // Forward declaration

class UI {
public:
    int script_edit_index = -1;
    std::string script_edit_path;
    bool script_reload_requested = false;
    TextEditor editor;
    bool editor_initialized = false;
    bool save_requested = false;
    MentalWindow* window = nullptr;
    glm::mat4 default_view;
    glm::mat4 default_projection;
    float delta = 0.0f;
    // Script editor state
    static inline char new_script_path_buf[256] = "";
    // Tilesheet cutter state
    static inline char tilesheet_path[256] = "";
    static inline int tilesheet_rows = 1;
    static inline int tilesheet_cols = 1;
    static inline GLuint tilesheet_tex = 0;
    static inline int tilesheet_width = 0, tilesheet_height = 0, tilesheet_channels = 0;
    static inline bool tilesheet_loaded = false;
    static inline std::vector<int> selected_tiles;
    static inline std::vector<Sprite2D*> cut_sprites;
    static inline float preview_zoom = 2.0f;
    static inline int last_rows = 1, last_cols = 1;
    static inline std::string last_path = "";
    int selected_component_index = 0;
    int get_selected_component_index() const { return selected_component_index; }
    void set_selected_component_index(int idx) { selected_component_index = idx; }
    void initialize_imgui(GLFWwindow* window);
    void create_new_frame();
    void set_context(MentalWindow* win, const glm::mat4& view, const glm::mat4& proj, float d) {
        window = win;
        default_view = view;
        default_projection = proj;
        delta = d;
    }
    
    void show_all_components();
    void show_script_editor();
    void show_tilesheet_cutter();
    void show_component_properties(Component* comp);
    void show_component_selector();
    void show_file_browser();
    void show_node_hierarchy(Node* node, const std::string& label_prefix = "Root");
    // Здесь можно добавить другие методы для отдельных виджетов
    std::string selected_file_path;
    const std::string& get_selected_file_path() const { return selected_file_path; }
    void set_selected_file_path(const std::string& path) { selected_file_path = path; }
    void set_custom_imgui_style();
};