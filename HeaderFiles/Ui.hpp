#pragma once
#include "Mental.hpp"


typedef struct MentalEngineUI {
    int script_edit_index = -1;
    std::string script_edit_path;
    bool script_reload_requested = false;
    TextEditor editor;
    bool editor_initialized = false;
    bool save_requested = false;
    GLFWwindow* window = nullptr;
    glm::mat4 default_view;
    glm::mat4 default_projection;
    float delta = 0.0f;
    static inline char new_script_path_buf[256] = "";
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
    std::string selected_file_path;
} MentalEngineUI;

MentalResult mentalEngineSetCustomUIStyle();
MentalResult mentalEngineUIInitialize(MentalEngineUI* ui, GLFWwindow* window);
MentalResult mentalEngineUICreateNewFrame();
MentalResult mentalEngineUISetContext(GLFWwindow* window, const glm::mat4& view, const glm::mat4& proj, float d);

