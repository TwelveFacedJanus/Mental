#include <iostream>
#include "Mental.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.hpp"
#include "../External/imgui/imgui.h"
#include "../External/imgui/backends/imgui_impl_glfw.h"
#include "../External/imgui/backends/imgui_impl_opengl3.h"
#include <fstream>
#include <sstream>
#include "../External/TextEditor/TextEditor.h"
#include <stb_image.h>

MentalWindow::MentalWindow() {
    this->initialize_glfw();
    this->set_window_hints();
    this->create_window();
    this->initialize_glew();
    luaL_openlibs(this->L);
};

inline void MentalWindow::initialize_glfw() noexcept(false) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLEW.");
    }
}
inline void MentalWindow::initialize_glew() noexcept(false) {
    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW.");
    }
}
void MentalWindow::set_window_hints() noexcept(true) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void MentalWindow::create_window() noexcept(false) {
    this->window = glfwCreateWindow(800, 600, "Mental OpenGL renderer.", nullptr, nullptr);
    if (this->window == nullptr) {
        throw std::runtime_error("Failed to create GLFW window.");
    }
    glfwMakeContextCurrent(this->window);
}

void MentalWindow::main_loop() {
    // --- RUN ROOT LUA SCRIPT ---
    lua_State* rootL = luaL_newstate();
    luaL_openlibs(rootL);
    // Register C++ Lua bindings for root script
    Component::register_sprite2d(rootL);
    Component::register_triangle(rootL);
    Component::register_camera(rootL);
    if (luaL_dofile(rootL, "../Scripts/root.lua") != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(rootL, -1) << std::endl;
        lua_pop(rootL, 1);
    } else {
        lua_getglobal(rootL, "main");
        if (lua_isfunction(rootL, -1)) {
            if (lua_pcall(rootL, 0, 0, 0) != LUA_OK) {
                std::cerr << "Lua error in main: " << lua_tostring(rootL, -1) << std::endl;
                lua_pop(rootL, 1);
            }
        } else {
            lua_pop(rootL, 1);
            std::cerr << "No 'main' function found in RootObjectScript.lua" << std::endl;
        }
    }
    lua_close(rootL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glm::mat4 default_projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
    glm::mat4 default_view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    float delta = 0.0f;

    // ===== CAMERA ======
    Camera camera;
    camera.set_position(0.0f, 0.0f, 3.0f);
    camera.look_at(0.0f, 0.0f, 0.0f);
    g_active_camera = &camera;

    // ===== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ДОБАВЛЕНИЯ КОМПОНЕНТОВ =====
    auto add_sprite2d = [&](Sprite2D* sprite, float x, float y, float z, const std::string& script_path) {
        sprite->setShader();
        sprite->set_position(x, y, z);
        if (!script_path.empty()) {
            sprite->add_script(script_path);
            sprite->run_script_main();
        }
        g_lua_components.push_back(sprite);
        this->rp->add(std::bind(&Sprite2D::apply_shader, sprite));
        this->rp->add([sprite, this, &default_view, &default_projection]() {
            glm::mat4 view = g_active_camera ? g_active_camera->getView() : default_view;
            glm::mat4 projection = g_active_camera ? g_active_camera->getProjection() : default_projection;
            GLuint modelLoc = glGetUniformLocation(sprite->shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sprite->model));
            GLuint viewLoc = glGetUniformLocation(sprite->shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            GLuint projectionLoc = glGetUniformLocation(sprite->shaderProgram, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        });
        this->rp->add(std::bind(&Sprite2D::draw, sprite));
        this->rp->add(std::bind(&Sprite2D::reload_script_if_changed, sprite));
        this->rp->add([sprite, &delta]() { sprite->call_process(delta); });
    };
    auto add_triangle = [&](Triangle* tri) {
        tri->setShader();
        g_lua_components.push_back(tri);
        this->rp->add(std::bind(&Triangle::apply_shader, tri));
        this->rp->add([tri, this, &default_view, &default_projection]() {
            glm::mat4 view = g_active_camera ? g_active_camera->getView() : default_view;
            glm::mat4 projection = g_active_camera ? g_active_camera->getProjection() : default_projection;
            GLuint modelLoc = glGetUniformLocation(tri->shaderProgram, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tri->model));
            GLuint viewLoc = glGetUniformLocation(tri->shaderProgram, "view");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            GLuint projectionLoc = glGetUniformLocation(tri->shaderProgram, "projection");
            glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        });
        this->rp->add(std::bind(&Triangle::draw, tri));
        this->rp->add(std::bind(&Triangle::reload_script_if_changed, tri));
        this->rp->add([tri, &delta]() { tri->call_process(delta); });
    };

    // ===== BACKGROUND ======
    Sprite2D bg;
    add_sprite2d(&bg, 0.0f, 0.0f, 0.0f, "../Scripts/background_component.lua");

  
    // ===== PLAYER =====
    Sprite2D sprite;
    add_sprite2d(&sprite, 0.5f, 0.0f, 0.0f, "../Scripts/sprite_component.lua");

    // === IMGUI INIT ===
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(this->window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(this->window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        double currentTime = glfwGetTime();
        delta = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        Component::g_pressed_keys.clear();
        for (const auto& pair : Component::g_key_names) {
            if (glfwGetKey(this->window, pair.first) == GLFW_PRESS) {
                Component::g_pressed_keys.insert(pair.first);
            }
        }
        // --- CAMERA FOLLOWS PLAYER ---
        // Плавная интерполяция камеры к игроку
        glm::vec3 target_cam_pos(sprite.position.x, sprite.position.y, 3.0f);
        glm::vec3 target_cam_look(sprite.position.x, sprite.position.y, 0.0f);
        camera.update_interpolated(0.1f, target_cam_pos, target_cam_look);
        // === IMGUI FRAME START ===
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // === IMGUI DEMO ===
        ImGui::ShowDemoWindow();
        // === IMGUI: All Components Widget ===
        static int script_edit_index = -1;
        static std::string script_edit_path;
        static bool script_reload_requested = false;
        static TextEditor editor;
        static bool editor_initialized = false;
        static bool save_requested = false;
        if (ImGui::Begin("All Components")) {
            // --- Кнопки создания компонентов ---
            if (ImGui::Button("Add Sprite2D")) {
                auto* sprite = new Sprite2D();
                add_sprite2d(sprite, 0.0f, 0.0f, 0.0f, "");
            }
            ImGui::SameLine();
            if (ImGui::Button("Add Triangle")) {
                auto* tri = new Triangle();
                add_triangle(tri);
            }
            ImGui::SameLine();
            if (ImGui::Button("Add AnimatedSprite2D")) {
                auto* anim = new AnimatedSprite2D(4, 4, 0.1f);
                add_sprite2d(anim, 0.0f, 0.0f, 0.0f, "");
            }
            ImGui::SameLine();
            if (ImGui::Button("Add TileMap")) {
                auto* tilemap = new TileMap();
                tilemap->setMap(8, 8, std::vector<int>(64, 0));
                tilemap->setShader();
                g_lua_components.push_back(tilemap);
            }
            for (size_t i = 0; i < g_lua_components.size(); ++i) {
                Component* comp = g_lua_components[i];
                std::string label = "Component " + std::to_string(i);
                if (dynamic_cast<AnimatedSprite2D*>(comp)) label += " (AnimatedSprite2D)";
                else if (dynamic_cast<TileMap*>(comp)) label += " (TileMap)";
                else if (dynamic_cast<Sprite2D*>(comp)) label += " (Sprite2D)";
                else if (dynamic_cast<Triangle*>(comp)) label += " (Triangle)";
                if (ImGui::TreeNode(label.c_str())) {
                    ImGui::Text("Position: (%.2f, %.2f, %.2f)", comp->position.x, comp->position.y, comp->position.z);
                    ImGui::Text("Scale: (%.2f, %.2f, %.2f)", comp->scale.x, comp->scale.y, comp->scale.z);
                    ImGui::Text("Script: %s", comp->get_script_path().empty() ? "<none>" : comp->get_script_path().c_str());
                    if (ImGui::Button(("Edit Script##" + std::to_string(i)).c_str())) {
                        script_edit_index = (int)i;
                        script_edit_path = comp->get_script_path();
                        std::string script_text;
                        if (!script_edit_path.empty()) {
                            std::ifstream in(script_edit_path);
                            std::stringstream buffer;
                            buffer << in.rdbuf();
                            script_text = buffer.str();
                        } else {
                            script_text = "";
                        }
                        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
                        editor.SetText(script_text);
                        editor_initialized = true;
                    }
                    if (auto* anim = dynamic_cast<AnimatedSprite2D*>(comp)) {
                        ImGui::Separator();
                        ImGui::Text("AnimatedSprite2D");
                        ImGui::InputInt(("Rows##" + std::to_string(i)).c_str(), &anim->rows);
                        ImGui::InputInt(("Columns##" + std::to_string(i)).c_str(), &anim->cols);
                        ImGui::InputFloat(("Frame Duration##" + std::to_string(i)).c_str(), &anim->frame_duration);
                        char buf[256];
                        strncpy(buf, anim->tileset_path.c_str(), sizeof(buf));
                        if (ImGui::InputText(("Tileset Path##" + std::to_string(i)).c_str(), buf, sizeof(buf))) {
                            anim->tileset_path = buf;
                        }
                        if (ImGui::Button(("Load Tileset##" + std::to_string(i)).c_str())) {
                            int w=0,h=0,channels=0;
                            unsigned char* data = stbi_load(anim->tileset_path.c_str(), &w, &h, &channels, 4);
                            if (data) {
                                GLuint texID = 0;
                                glGenTextures(1, &texID);
                                glBindTexture(GL_TEXTURE_2D, texID);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                                glBindTexture(GL_TEXTURE_2D, 0);
                                anim->textureIDs.clear();
                                anim->textureIDs.push_back(texID);
                                stbi_image_free(data);
                            }
                        }
                        // === TileMap controls ===
                        if (auto* tilemap = dynamic_cast<TileMap*>(comp)) {
                            static char tileset_path[256] = "";
                            static char tilemap_save_path[256] = "tilemap.txt";
                            static char tilemap_load_path[256] = "tilemap.txt";
                            ImGui::Separator();
                            ImGui::Text("TileMap");
                            ImGui::InputInt("Rows", &tilemap->rows);
                            ImGui::InputInt("Columns", &tilemap->cols);
                            ImGui::InputInt("Tile Width", &tilemap->tile_width);
                            ImGui::InputInt("Tile Height", &tilemap->tile_height);
                            ImGui::InputInt("Tileset Rows", &tilemap->tileset_rows);
                            ImGui::InputInt("Tileset Columns", &tilemap->tileset_cols);
                            ImGui::InputFloat3("Position", (float*)&tilemap->position);
                            ImGui::InputFloat3("Scale", (float*)&tilemap->scale);
                            ImGui::InputText("Tileset Path", tileset_path, sizeof(tileset_path));
                            if (ImGui::Button("Load Tileset")) {
                                int w=0,h=0,channels=0;
                                unsigned char* data = stbi_load(tileset_path, &w, &h, &channels, 4);
                                if (data) {
                                    GLuint texID = 0;
                                    glGenTextures(1, &texID);
                                    glBindTexture(GL_TEXTURE_2D, texID);
                                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                                    glBindTexture(GL_TEXTURE_2D, 0);
                                    tilemap->tileset_texture = texID;
                                    tilemap->tileset_path = tileset_path;
                                    stbi_image_free(data);
                                }
                            }
                            ImGui::InputText("Save Path", tilemap_save_path, sizeof(tilemap_save_path));
                            if (ImGui::Button("Save TileMap")) {
                                tilemap->tileset_path = tileset_path;
                                tilemap->saveToFile(tilemap_save_path);
                            }
                            ImGui::InputText("Load Path", tilemap_load_path, sizeof(tilemap_load_path));
                            if (ImGui::Button("Load TileMap")) {
                                if (tilemap->loadFromFile(tilemap_load_path)) {
                                    strncpy(tileset_path, tilemap->tileset_path.c_str(), sizeof(tileset_path));
                                    // Автоматически загрузить текстуру
                                    int w=0,h=0,channels=0;
                                    unsigned char* data = stbi_load(tileset_path, &w, &h, &channels, 4);
                                    if (data) {
                                        GLuint texID = 0;
                                        glGenTextures(1, &texID);
                                        glBindTexture(GL_TEXTURE_2D, texID);
                                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                                        glBindTexture(GL_TEXTURE_2D, 0);
                                        tilemap->tileset_texture = texID;
                                        stbi_image_free(data);
                                    }
                                    tilemap->setShader();
                                }
                            }
                            // Обновить размер tiles при изменении rows/cols
                            if ((int)tilemap->tiles.size() != tilemap->rows * tilemap->cols) {
                                tilemap->tiles.resize(tilemap->rows * tilemap->cols, 0);
                            }
                            // --- Визуальный редактор тайлов ---
                            static int selected_tile_palette = 0;
                            if (tilemap->tileset_texture && tilemap->tileset_rows > 0 && tilemap->tileset_cols > 0) {
                                ImGui::Text("TileMap Editor:");
                                int grid_w = tilemap->cols;
                                int grid_h = tilemap->rows;
                                float cell_sz = 32.0f;
                                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                                ImVec2 start = ImGui::GetCursorScreenPos();
                                // Рисуем сетку и тайлы
                                for (int y = 0; y < grid_h; ++y) {
                                    for (int x = 0; x < grid_w; ++x) {
                                        int idx = tilemap->tiles[y*grid_w + x];
                                        int t_row = idx / tilemap->tileset_cols;
                                        int t_col = idx % tilemap->tileset_cols;
                                        float u0 = (float)t_col / tilemap->tileset_cols;
                                        float v0 = (float)t_row / tilemap->tileset_rows;
                                        float u1 = (float)(t_col+1) / tilemap->tileset_cols;
                                        float v1 = (float)(t_row+1) / tilemap->tileset_rows;
                                        ImVec2 uv0(u0, v1);
                                        ImVec2 uv1(u1, v0);
                                        ImVec2 p0 = ImVec2(start.x + x*cell_sz, start.y + y*cell_sz);
                                        ImVec2 p1 = ImVec2(p0.x + cell_sz, p0.y + cell_sz);
                                        draw_list->AddImage((void*)(intptr_t)tilemap->tileset_texture, p0, p1, uv0, uv1);
                                        draw_list->AddRect(p0, p1, IM_COL32(128,128,128,255));
                                        // Клик по клетке — рисуем выбранным тайлом
                                        ImGui::SetCursorScreenPos(p0);
                                        ImGui::InvisibleButton(("cell"+std::to_string(x)+"_"+std::to_string(y)).c_str(), ImVec2(cell_sz, cell_sz));
                                        if (ImGui::IsItemClicked()) {
                                            tilemap->tiles[y*grid_w + x] = selected_tile_palette;
                                        }
                                    }
                                }
                                ImGui::SetCursorScreenPos(ImVec2(start.x, start.y + grid_h*cell_sz));
                                ImGui::Dummy(ImVec2(grid_w*cell_sz, 0));
                                // --- Палитра тайлов ---
                                ImGui::Text("Tile Palette:");
                                float palette_cell = 28.0f;
                                int palette_cols = tilemap->tileset_cols;
                                int palette_rows = tilemap->tileset_rows;
                                ImVec2 palette_start = ImGui::GetCursorScreenPos();
                                for (int py = 0; py < palette_rows; ++py) {
                                    for (int px = 0; px < palette_cols; ++px) {
                                        int tidx = py*palette_cols + px;
                                        float u0 = (float)px / palette_cols;
                                        float v0 = (float)py / palette_rows;
                                        float u1 = (float)(px+1) / palette_cols;
                                        float v1 = (float)(py+1) / palette_rows;
                                        ImVec2 uv0(u0, v1);
                                        ImVec2 uv1(u1, v0);
                                        ImVec2 pp0 = ImVec2(palette_start.x + px*palette_cell, palette_start.y + py*palette_cell);
                                        ImVec2 pp1 = ImVec2(pp0.x + palette_cell, pp0.y + palette_cell);
                                        draw_list->AddImage((void*)(intptr_t)tilemap->tileset_texture, pp0, pp1, uv0, uv1);
                                        if (tidx == selected_tile_palette) {
                                            draw_list->AddRect(pp0, pp1, IM_COL32(255,255,0,255), 0, 0, 3.0f);
                                        } else {
                                            draw_list->AddRect(pp0, pp1, IM_COL32(128,128,128,255));
                                        }
                                        ImGui::SetCursorScreenPos(pp0);
                                        ImGui::InvisibleButton(("palette"+std::to_string(px)+"_"+std::to_string(py)).c_str(), ImVec2(palette_cell, palette_cell));
                                        if (ImGui::IsItemClicked()) {
                                            selected_tile_palette = tidx;
                                        }
                                    }
                                }
                                ImGui::SetCursorScreenPos(ImVec2(palette_start.x, palette_start.y + palette_rows*palette_cell));
                                ImGui::Dummy(ImVec2(palette_cols*palette_cell, 0));
                            }
                        }
                    }
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
        // === IMGUI: Lua Script Editor ===
        if (script_edit_index >= 0) {
            bool open = ImGui::Begin("Lua Script Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            static char new_script_path_buf[256] = "";
            if (open) {
                ImGui::Text("Editing: %s", script_edit_path.empty() ? "<none>" : script_edit_path.c_str());
                // If script path is empty, allow user to enter a new path
                if (script_edit_path.empty()) {
                    ImGui::InputText("New Script Path", new_script_path_buf, sizeof(new_script_path_buf));
                }
                editor.Render("EditorChild", ImVec2(600, 400));
                if (ImGui::Button("Save & Reload")) {
                    save_requested = true;
                }
                ImGui::SameLine();
                if (ImGui::Button("Close")) {
                    script_edit_index = -1;
                    editor_initialized = false;
                    new_script_path_buf[0] = '\0';
                }
                ImGui::EndChild();
            }
            ImGui::End();
            // Handle file I/O after ImGui::End()
            if (save_requested) {
                try {
                    std::string save_path = script_edit_path;
                    if (save_path.empty()) {
                        std::string user_filename = std::string(new_script_path_buf);
                        // Ensure the path starts with '../Scripts/'
                        const std::string scripts_prefix = "../Scripts/";
                        if (user_filename.rfind(scripts_prefix, 0) != 0) {
                            save_path = scripts_prefix + user_filename;
                        } else {
                            save_path = user_filename;
                        }
                    }
                    if (!save_path.empty()) {
                        std::ofstream out(save_path);
                        if (out.is_open()) {
                            out << editor.GetText();
                            out.close();
                            script_reload_requested = true;
                            // Assign new script path to component if it was empty
                            if (g_lua_components[script_edit_index]->get_script_path().empty()) {
                                g_lua_components[script_edit_index]->add_script(save_path);
                                g_lua_components[script_edit_index]->run_script_main();
                                script_edit_path = save_path;
                            }
                        }
                    }
                } catch (const std::exception& e) {
                    // Optionally, log error or show ImGui::OpenPopup("Save Error")
                }
                save_requested = false;
                new_script_path_buf[0] = '\0';
            }
        }
        // === IMGUI: Tilesheet Cutter Widget ===
        static char tilesheet_path[256] = "";
        static int tilesheet_rows = 1;
        static int tilesheet_cols = 1;
        static GLuint tilesheet_tex = 0;
        static int tilesheet_width = 0, tilesheet_height = 0, tilesheet_channels = 0;
        static bool tilesheet_loaded = false;
        static std::vector<int> selected_tiles;
        static std::vector<Sprite2D*> cut_sprites;
        static float preview_zoom = 2.0f;
        static int last_rows = 1, last_cols = 1;
        static std::string last_path = "";
        if (ImGui::Begin("Tilesheet Cutter")) {
            ImGui::InputText("Tilesheet Path", tilesheet_path, sizeof(tilesheet_path));
            ImGui::InputInt("Rows", &tilesheet_rows);
            ImGui::InputInt("Columns", &tilesheet_cols);
            tilesheet_rows = std::max(1, tilesheet_rows);
            tilesheet_cols = std::max(1, tilesheet_cols);
            if (ImGui::Button("Load Tilesheet") ||
                last_path != std::string(tilesheet_path) ||
                last_rows != tilesheet_rows ||
                last_cols != tilesheet_cols) {
                // Load image using stb_image
                if (tilesheet_tex) {
                    glDeleteTextures(1, &tilesheet_tex);
                    tilesheet_tex = 0;
                }
                unsigned char* data = stbi_load(tilesheet_path, &tilesheet_width, &tilesheet_height, &tilesheet_channels, 4);
                if (data) {
                    glGenTextures(1, &tilesheet_tex);
                    glBindTexture(GL_TEXTURE_2D, tilesheet_tex);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tilesheet_width, tilesheet_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    tilesheet_loaded = true;
                    selected_tiles.clear();
                } else {
                    tilesheet_loaded = false;
                }
                stbi_image_free(data);
                last_path = std::string(tilesheet_path);
                last_rows = tilesheet_rows;
                last_cols = tilesheet_cols;
            }
            if (tilesheet_loaded && tilesheet_tex) {
                ImGui::Text("Size: %d x %d", tilesheet_width, tilesheet_height);
                ImGui::SliderFloat("Zoom", &preview_zoom, 1.0f, 10.0f, "%.1fx");
                ImVec2 avail = ImGui::GetContentRegionAvail();
                float preview_w = tilesheet_width * preview_zoom;
                float preview_h = tilesheet_height * preview_zoom;
                ImVec2 img_size(preview_w, preview_h);
                ImVec2 cursor = ImGui::GetCursorScreenPos();
                ImGui::Image((void*)(intptr_t)tilesheet_tex, img_size);
                // Draw grid overlay and handle selection
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                float cell_w = preview_w / tilesheet_cols;
                float cell_h = preview_h / tilesheet_rows;
                // Mouse interaction
                ImVec2 mouse = ImGui::GetIO().MousePos;
                bool img_hovered = ImGui::IsItemHovered();
                if (img_hovered && ImGui::IsMouseClicked(0)) {
                    int col = (int)((mouse.x - cursor.x) / cell_w);
                    int row = (int)((mouse.y - cursor.y) / cell_h);
                    if (col >= 0 && col < tilesheet_cols && row >= 0 && row < tilesheet_rows) {
                        int idx = row * tilesheet_cols + col;
                        auto it = std::find(selected_tiles.begin(), selected_tiles.end(), idx);
                        if (it == selected_tiles.end())
                            selected_tiles.push_back(idx);
                        else
                            selected_tiles.erase(it);
                    }
                }
                // Draw grid
                for (int r = 0; r <= tilesheet_rows; ++r) {
                    float y = cursor.y + r * cell_h;
                    draw_list->AddLine(ImVec2(cursor.x, y), ImVec2(cursor.x + preview_w, y), IM_COL32(255,255,0,128));
                }
                for (int c = 0; c <= tilesheet_cols; ++c) {
                    float x = cursor.x + c * cell_w;
                    draw_list->AddLine(ImVec2(x, cursor.y), ImVec2(x, cursor.y + preview_h), IM_COL32(255,255,0,128));
                }
                // Highlight selected
                for (int idx : selected_tiles) {
                    int row = idx / tilesheet_cols;
                    int col = idx % tilesheet_cols;
                    ImVec2 p0(cursor.x + col * cell_w, cursor.y + row * cell_h);
                    ImVec2 p1(cursor.x + (col+1) * cell_w, cursor.y + (row+1) * cell_h);
                    draw_list->AddRectFilled(p0, p1, IM_COL32(0,255,0,64));
                }
                ImGui::Text("Selected: %d", (int)selected_tiles.size());
                if (ImGui::Button("Create Sprites from Selection")) {
                    // Remove old cut_sprites
                    for (auto* s : cut_sprites) delete s;
                    cut_sprites.clear();
                    for (int idx : selected_tiles) {
                        int row = idx / tilesheet_cols;
                        int col = idx % tilesheet_cols;
                        float u0 = (float)col / tilesheet_cols;
                        float v0 = (float)row / tilesheet_rows;
                        float u1 = (float)(col+1) / tilesheet_cols;
                        float v1 = (float)(row+1) / tilesheet_rows;
                        auto* sprite = new Sprite2D();
                        // Set custom UVs
                        float custom_vertices[20] = {
                            0.5f,  0.5f, 0.0f, u1, v1, // top right
                            0.5f, -0.5f, 0.0f, u1, v0, // bottom right
                           -0.5f, -0.5f, 0.0f, u0, v0, // bottom left
                           -0.5f,  0.5f, 0.0f, u0, v1  // top left
                        };
                        for (int i = 0; i < 20; ++i) sprite->vertices[i] = custom_vertices[i];
                        glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO);
                        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(custom_vertices), custom_vertices);
                        glBindBuffer(GL_ARRAY_BUFFER, 0);
                        // Use the loaded tilesheet texture
                        sprite->textureIDs.clear();
                        sprite->textureIDs.push_back(tilesheet_tex);
                        sprite->setShader();
                        cut_sprites.push_back(sprite);
                        // Immediately add to scene
                        add_sprite2d(sprite, 0.0f, 0.0f, 0.0f, "");
                    }
                }
            }
        }
        ImGui::End();
        rp->run();
        // === TileMap draw ===
        for (Component* comp : g_lua_components) {
            if (auto* tilemap = dynamic_cast<TileMap*>(comp)) {
                tilemap->draw();
            }
        }
        // === IMGUI RENDER ===
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(this->window);
        glfwPollEvents();
        // === Update animation for all AnimatedSprite2D ===
        for (Component* comp : g_lua_components) {
            if (auto* anim = dynamic_cast<AnimatedSprite2D*>(comp)) {
                anim->update_animation(delta);
            }
        }
    }
    // === IMGUI SHUTDOWN ===
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

MentalWindow::~MentalWindow() {
    if (this->window) glfwDestroyWindow(this->window);
    glfwTerminate();
}