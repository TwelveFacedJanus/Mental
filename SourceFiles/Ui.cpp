#include "Mental.hpp"
#include "Ui.hpp"
#include "Component.hpp"
#include <filesystem>
#include "../External/nativefiledialog/src/include/nfd.h"

void UI::set_custom_imgui_style() {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 4.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 1.0f;
    style.PopupRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.Colors[ImGuiCol_WindowBg]           = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_Header]             = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_HeaderHovered]      = ImVec4(0.30f, 0.32f, 0.37f, 1.00f);
    style.Colors[ImGuiCol_HeaderActive]       = ImVec4(0.25f, 0.27f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_Button]             = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]      = ImVec4(0.30f, 0.32f, 0.37f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]       = ImVec4(0.25f, 0.27f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_FrameBg]            = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_FrameBgActive]      = ImVec4(0.25f, 0.27f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_TitleBg]            = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]      = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg]          = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_Tab]                = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabHovered]         = ImVec4(0.30f, 0.32f, 0.37f, 1.00f);
    style.Colors[ImGuiCol_TabActive]          = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]       = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]          = ImVec4(0.30f, 0.32f, 0.37f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]         = ImVec4(0.30f, 0.32f, 0.37f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]   = ImVec4(0.25f, 0.27f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]         = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripHovered]  = ImVec4(0.30f, 0.32f, 0.37f, 1.00f);
    style.Colors[ImGuiCol_ResizeGripActive]   = ImVec4(0.25f, 0.27f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_Separator]          = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered]   = ImVec4(0.30f, 0.32f, 0.37f, 1.00f);
    style.Colors[ImGuiCol_SeparatorActive]    = ImVec4(0.25f, 0.27f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_Text]               = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]       = ImVec4(0.50f, 0.54f, 0.58f, 1.00f);
    style.Colors[ImGuiCol_Border]             = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
}

void UI::initialize_imgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = "../imgui.ini";
    io.Fonts->AddFontFromFileTTF("../Fonts/SFProText-Medium.ttf", 14.0f);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    ImGui::StyleColorsDark();
    set_custom_imgui_style();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::create_new_frame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::show_all_components() {
    // === IMGUI: All Components Widget ===
    if (ImGui::Begin("All Components")) {
        // --- Кнопка добавления компонента с выбором типа ---
        static int selected_type = 0;
        static const char* types[] = {"Sprite2D", "Triangle", "AnimatedSprite2D", "TileMap", "Camera"};
        if (ImGui::Button("Добавить компонент")) {
            ImGui::OpenPopup("component_type_popup");
        }
        if (ImGui::BeginPopup("component_type_popup")) {
            for (int i = 0; i < IM_ARRAYSIZE(types); ++i) {
                if (ImGui::Selectable(types[i], selected_type == i)) {
                    selected_type = i;
                    // Создание компонента по выбору
                    if (selected_type == 0) { // Sprite2D
                        auto* sprite = new Sprite2D();
                        window->addSprite2D(sprite, 0.0f, 0.0f, 0.0f, "", default_view, default_projection, delta);
                    } else if (selected_type == 1) { // Triangle
                        auto* tri = new Triangle();
                        window->addTriangle(tri, default_view, default_projection, delta);
                    } else if (selected_type == 2) { // AnimatedSprite2D
                        auto* anim = new AnimatedSprite2D(4, 4, 0.1f);
                        window->addSprite2D(anim, 0.0f, 0.0f, 0.0f, "", default_view, default_projection, delta);
                    } else if (selected_type == 3) { // TileMap
                        auto* tilemap = new TileMap();
                        tilemap->setMap(8, 8, std::vector<int>(64, 0));
                        tilemap->setShader();
                        g_lua_components.push_back(tilemap);
                    } else if (selected_type == 4) { // Camera
                        auto* cam = new Camera();
                        g_lua_components.push_back(cam);
                    }
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
        for (size_t i = 0; i < g_lua_components.size(); ++i) {
            Component* comp = g_lua_components[i];
            std::string label = "Component " + std::to_string(i);
            if (dynamic_cast<AnimatedSprite2D*>(comp)) label += " (AnimatedSprite2D)";
            else if (dynamic_cast<TileMap*>(comp)) label += " (TileMap)";
            else if (dynamic_cast<Sprite2D*>(comp)) label += " (Sprite2D)";
            else if (dynamic_cast<Triangle*>(comp)) label += " (Triangle)";
            // --- Выделение выбранного компонента ---
            ImGuiTreeNodeFlags node_flags = 0;
            if ((int)i == selected_component_index) node_flags |= ImGuiTreeNodeFlags_Selected;
            bool open = ImGui::TreeNodeEx(label.c_str(), node_flags);
            if (ImGui::IsItemClicked()) {
                selected_component_index = (int)i;
            }
            if (open) {
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
                    ImGui::InputText(("Tileset Path##" + std::to_string(i)).c_str(), buf, sizeof(buf));
                    ImGui::SameLine();
                    if (ImGui::Button(("Выбрать тайлсет##" + std::to_string(i)).c_str())) {
                        nfdchar_t *outPath = NULL;
                        nfdresult_t result = NFD_OpenDialog("png,jpg,jpeg,bmp,tga", NULL, &outPath);
                        if (result == NFD_OKAY && outPath) {
                            strncpy(buf, outPath, sizeof(buf));
                            buf[sizeof(buf)-1] = '\0';
                            anim->tileset_path = buf;
                            free(outPath);
                        }
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
}

void UI::show_script_editor() {
    if (script_edit_index >= 0) {
        bool open = ImGui::Begin("Lua Script Editor", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (open) {
            ImGui::Text("Editing: %s", script_edit_path.empty() ? "<none>" : script_edit_path.c_str());
            if (script_edit_path.empty()) {
                ImGui::InputText("New Script Path", new_script_path_buf, sizeof(new_script_path_buf));
            }
            editor.Render("EditorChild");
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
        if (save_requested) {
            try {
                std::string save_path = script_edit_path;
                if (save_path.empty()) {
                    std::string user_filename = std::string(new_script_path_buf);
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
}

void UI::show_tilesheet_cutter() {
    if (ImGui::Begin("Tilesheet Cutter")) {
        ImGui::InputInt("Rows", &tilesheet_rows);
        ImGui::InputInt("Columns", &tilesheet_cols);
        tilesheet_rows = std::max(1, tilesheet_rows);
        tilesheet_cols = std::max(1, tilesheet_cols);
        ImGui::InputText("Tilesheet Path", tilesheet_path, sizeof(tilesheet_path));
        ImGui::SameLine();
        if (ImGui::Button("Выбрать тайлсет")) {
            nfdchar_t *outPath = NULL;
            nfdresult_t result = NFD_OpenDialog("png,jpg,jpeg,bmp,tga", NULL, &outPath);
            if (result == NFD_OKAY && outPath) {
                strncpy(tilesheet_path, outPath, sizeof(tilesheet_path));
                tilesheet_path[sizeof(tilesheet_path)-1] = '\0';
                free(outPath);
            }
        }
        if (ImGui::Button("Load Tilesheet") ||
            last_path != std::string(tilesheet_path) ||
            last_rows != tilesheet_rows ||
            last_cols != tilesheet_cols) {
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
            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            float cell_w = preview_w / tilesheet_cols;
            float cell_h = preview_h / tilesheet_rows;
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
            for (int r = 0; r <= tilesheet_rows; ++r) {
                float y = cursor.y + r * cell_h;
                draw_list->AddLine(ImVec2(cursor.x, y), ImVec2(cursor.x + preview_w, y), IM_COL32(255,255,0,128));
            }
            for (int c = 0; c <= tilesheet_cols; ++c) {
                float x = cursor.x + c * cell_w;
                draw_list->AddLine(ImVec2(x, cursor.y), ImVec2(x, cursor.y + preview_h), IM_COL32(255,255,0,128));
            }
            for (int idx : selected_tiles) {
                int row = idx / tilesheet_cols;
                int col = idx % tilesheet_cols;
                ImVec2 p0(cursor.x + col * cell_w, cursor.y + row * cell_h);
                ImVec2 p1(cursor.x + (col+1) * cell_w, cursor.y + (row+1) * cell_h);
                draw_list->AddRectFilled(p0, p1, IM_COL32(0,255,0,64));
            }
            ImGui::Text("Selected: %d", (int)selected_tiles.size());
            if (ImGui::Button("Create Sprites from Selection")) {
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
                    float custom_vertices[20] = {
                        0.5f,  0.5f, 0.0f, u1, v1,
                        0.5f, -0.5f, 0.0f, u1, v0,
                       -0.5f, -0.5f, 0.0f, u0, v0,
                       -0.5f,  0.5f, 0.0f, u0, v1
                    };
                    for (int i = 0; i < 20; ++i) sprite->vertices[i] = custom_vertices[i];
                    glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO);
                    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(custom_vertices), custom_vertices);
                    glBindBuffer(GL_ARRAY_BUFFER, 0);
                    sprite->textureIDs.clear();
                    sprite->textureIDs.push_back(tilesheet_tex);
                    sprite->setShader();
                    cut_sprites.push_back(sprite);
                    window->addSprite2D(sprite, 0.0f, 0.0f, 0.0f, "", default_view, default_projection, delta);
                }
            }
        }
    }
    ImGui::End();
}

void UI::show_component_properties(Component* comp) {
    if (!comp) return;
    ImGui::Begin("Component Properties");
    ImGui::Text("Type: %s", typeid(*comp).name());
    ImGui::Separator();
    ImGui::InputFloat3("Position", (float*)&comp->position);
    ImGui::InputFloat3("Scale", (float*)&comp->scale);
    // Поворот (если есть)
    // ImGui::InputFloat4("Rotation (quat)", (float*)&comp->rotation);
    ImGui::Text("Shader Program: %u", comp->shaderProgram);
    ImGui::Text("Script: %s", comp->get_script_path().empty() ? "<none>" : comp->get_script_path().c_str());
    // Можно добавить больше информации по типу компонента
    if (auto* sprite = dynamic_cast<Sprite2D*>(comp)) {
        ImGui::Text("Sprite2D: %d textures", (int)sprite->textureIDs.size());
    }
    if (auto* anim = dynamic_cast<AnimatedSprite2D*>(comp)) {
        ImGui::Text("AnimatedSprite2D: %d frames", anim->total_frames);
        ImGui::Text("Current Animation: %s", anim->current_anim.c_str());
    }
    if (auto* tilemap = dynamic_cast<TileMap*>(comp)) {
        ImGui::Text("TileMap: %dx%d", tilemap->rows, tilemap->cols);
        ImGui::Text("Tileset: %s", tilemap->tileset_path.c_str());
    }
    ImGui::End();
}

void UI::show_component_selector() {
    if (g_lua_components.empty()) return;
    std::vector<const char*> names;
    for (auto* comp : g_lua_components) {
        names.push_back(typeid(*comp).name()); // Можно заменить на comp->get_name() если есть
    }
    ImGui::Text("Выберите компонент:");
    ImGui::Combo("Компоненты", &selected_component_index, names.data(), names.size());
}

void UI::show_file_browser() {
    static std::string current_path = ".";
    ImGui::Begin("File Browser");
    ImGui::Text("Current path: %s", current_path.c_str());
    if (ImGui::Button("Выбрать файл")) {
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog(NULL, NULL, &outPath);
        if (result == NFD_OKAY && outPath) {
            selected_file_path = outPath;
            // Открыть файл, если это текстовый
            std::string ext = std::filesystem::path(selected_file_path).extension().string();
            if (ext == ".txt" || ext == ".lua" || ext == ".glsl" || ext == ".vert" || ext == ".frag" || ext == ".md" || ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c") {
                std::ifstream in(selected_file_path);
                std::stringstream buffer;
                buffer << in.rdbuf();
                editor.SetText(buffer.str());
                editor_initialized = true;
                save_requested = false;
                if (ext == ".lua")
                    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
                else if (ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c")
                    editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
                else
                    editor.SetLanguageDefinition(TextEditor::LanguageDefinition());
            } else {
                editor_initialized = false;
            }
            free(outPath);
        }
    }
    if (current_path != ".") {
        if (ImGui::Button(".. (Up)")) {
            current_path = std::filesystem::path(current_path).parent_path().string();
        }
    }
    for (const auto& entry : std::filesystem::directory_iterator(current_path)) {
        std::string name = entry.path().filename().string();
        if (entry.is_directory()) {
            if (ImGui::Selectable((name + "/").c_str(), false)) {
                current_path = entry.path().string();
            }
        } else {
            bool selected = (selected_file_path == entry.path().string());
            if (ImGui::Selectable(name.c_str(), selected)) {
                selected_file_path = entry.path().string();
                // Если это текстовый файл, открыть в редакторе
                std::string ext = entry.path().extension().string();
                if (ext == ".txt" || ext == ".lua" || ext == ".glsl" || ext == ".vert" || ext == ".frag" || ext == ".md" || ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c") {
                    std::ifstream in(selected_file_path);
                    std::stringstream buffer;
                    buffer << in.rdbuf();
                    editor.SetText(buffer.str());
                    editor_initialized = true;
                    save_requested = false;
                    // Язык
                    if (ext == ".lua")
                        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
                    else if (ext == ".cpp" || ext == ".hpp" || ext == ".h" || ext == ".c")
                        editor.SetLanguageDefinition(TextEditor::LanguageDefinition::CPlusPlus());
                    else
                        editor.SetLanguageDefinition(TextEditor::LanguageDefinition());
                } else {
                    editor_initialized = false;
                }
            }
        }
    }
    ImGui::End();
    // Если выбран текстовый файл, показать редактор
    if (editor_initialized && !selected_file_path.empty()) {
        ImGui::Begin("Text Editor");
        ImGui::Text("Editing: %s", selected_file_path.c_str());
        bool child_open = ImGui::BeginChild("TextEditor");
        if (child_open) {
            editor.Render("TextEditor");
            ImGui::EndChild();
        }
        if (ImGui::Button("Save")) {
            save_requested = true;
        }
        if (save_requested) {
            std::ofstream out(selected_file_path);
            out << editor.GetText();
            save_requested = false;
        }
        ImGui::EndChild();
        ImGui::End();
    }
}