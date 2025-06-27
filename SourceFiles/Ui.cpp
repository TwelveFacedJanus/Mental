#include "Mental.hpp"
#include "Ui.hpp"
#include "Component.hpp"
#include <filesystem>
#include <unordered_map>

#ifdef __cplusplus
extern "C" {
#endif
#include "../External/nativefiledialog/src/include/nfd.h"
#ifdef __cplusplus
}
#endif

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
    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    io.IniFilename = "./imgui.ini";
    
    io.Fonts->AddFontFromFileTTF("./Fonts/SFProText-Medium.ttf", 14.0f);
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

void UI::show_node_hierarchy(Node* node, const std::string& label_prefix) {
    std::string label = label_prefix + " (Node)";
    ImGuiTreeNodeFlags node_flags = 0;
    bool open = ImGui::TreeNodeEx(label.c_str(), node_flags);
    bool node_selected = false;
    if (ImGui::IsItemClicked()) {
        // Можно добавить выделение Node, если нужно
        // node_selected = true; // если нужно выделять
    }
    if (open) {
        // Кнопка добавления дочернего компонента
        if (ImGui::Button(("Добавить дочерний компонент##" + label_prefix).c_str())) {
            ImGui::OpenPopup(("add_child_popup##" + label_prefix).c_str());
        }
        static int child_type = 0;
        static char child_name[64] = "";
        static const char* child_types[] = {"Sprite2D", "Triangle", "AnimatedSprite2D", "TileMap", "Camera", "Node"};
        if (ImGui::BeginPopup(("add_child_popup##" + label_prefix).c_str())) {
            ImGui::InputText("Имя", child_name, sizeof(child_name));
            for (int i = 0; i < IM_ARRAYSIZE(child_types); ++i) {
                if (ImGui::Selectable(child_types[i], child_type == i)) {
                    child_type = i;
                }
            }
            if (ImGui::Button("Добавить") && strlen(child_name) > 0) {
                std::shared_ptr<Component> new_comp;
                if (child_type == 0) { // Sprite2D
                    new_comp = std::make_shared<Sprite2D>();
                } else if (child_type == 1) { // Triangle
                    new_comp = std::make_shared<Triangle>();
                } else if (child_type == 2) { // AnimatedSprite2D
                    new_comp = std::make_shared<AnimatedSprite2D>(4, 4, 0.1f);
                } else if (child_type == 3) { // TileMap
                    auto tilemap = std::make_shared<TileMap>();
                    tilemap->setMap(8, 8, std::vector<int>(64, 0));
                    tilemap->setShader();
                    new_comp = tilemap;
                } else if (child_type == 4) { // Camera
                    new_comp = std::make_shared<Camera>();
                } else if (child_type == 5) { // Node
                    auto new_node = std::make_shared<Node>();
                    node->add_node(child_name, new_node);
                    ImGui::CloseCurrentPopup();
                    child_name[0] = '\0';
                    child_type = 0;
                    return;
                }
                node->add_component(child_name, new_comp);
                ImGui::CloseCurrentPopup();
                child_name[0] = '\0';
                child_type = 0;
            }
            ImGui::EndPopup();
        }
        // Минимальный рабочий пример для первого дочернего компонента
        static std::string comp_unique_id, comp_name_cache;
        static bool open_rename = false, open_delete = false, open_script = false;
        static std::string rename_buf, script_buf;
        for (auto it = node->child_components.begin(); it != node->child_components.end(); ++it) {
            const std::string& comp_name = it->first;
            auto& comp_ptr = it->second;
            std::string comp_label = comp_name;
            if (dynamic_cast<AnimatedSprite2D*>(comp_ptr.get())) comp_label += " (AnimatedSprite2D)";
            else if (dynamic_cast<TileMap*>(comp_ptr.get())) comp_label += " (TileMap)";
            else if (dynamic_cast<Sprite2D*>(comp_ptr.get())) comp_label += " (Sprite2D)";
            else if (dynamic_cast<Triangle*>(comp_ptr.get())) comp_label += " (Triangle)";
            else if (dynamic_cast<Camera*>(comp_ptr.get())) comp_label += " (Camera)";
            std::string unique_id = label_prefix + comp_name;
            ImGuiTreeNodeFlags comp_flags = ImGuiTreeNodeFlags_Leaf;
            bool open = ImGui::TreeNodeEx((comp_label + "##" + unique_id).c_str(), comp_flags);
            // Выделение компонента при клике
            if (ImGui::IsItemClicked()) {
                // Найти индекс этого компонента в g_lua_components
                auto itg = std::find(g_lua_components.begin(), g_lua_components.end(), comp_ptr.get());
                if (itg != g_lua_components.end()) {
                    selected_component_index = static_cast<int>(std::distance(g_lua_components.begin(), itg));
                }
            }
            if (ImGui::BeginPopupContextItem(("context_comp_" + unique_id).c_str())) {
                if (ImGui::MenuItem("Переименовать")) {
                    open_rename = true;
                    comp_unique_id = unique_id;
                    comp_name_cache = comp_name;
                    rename_buf = comp_name;
                }
                if (ImGui::MenuItem("Удалить")) {
                    open_delete = true;
                    comp_unique_id = unique_id;
                    comp_name_cache = comp_name;
                }
                if (!comp_ptr->get_script_path().empty()) {
                    if (ImGui::MenuItem("Отключить скрипт")) {
                        comp_ptr->add_script("");
                    }
                    if (ImGui::MenuItem("Редактировать скрипт")) {
                        auto itg = std::find(g_lua_components.begin(), g_lua_components.end(), comp_ptr.get());
                        if (itg != g_lua_components.end()) {
                            script_edit_index = static_cast<int>(std::distance(g_lua_components.begin(), itg));
                            script_edit_path = comp_ptr->get_script_path();
                        }
                    }
                } else {
                    if (ImGui::MenuItem("Создать скрипт")) {
                        auto itg = std::find(g_lua_components.begin(), g_lua_components.end(), comp_ptr.get());
                        if (itg != g_lua_components.end()) {
                            script_edit_index = static_cast<int>(std::distance(g_lua_components.begin(), itg));
                            script_edit_path = "";
                        }
                    }
                }
                ImGui::EndPopup();
            }
            ImGui::TreePop();
        }
        // Popup'ы вне цикла
        if (open_rename) {
            ImGui::OpenPopup(("rename_comp_popup_" + comp_unique_id).c_str());
            open_rename = false;
        }
        if (open_delete) {
            ImGui::OpenPopup(("delete_comp_popup_" + comp_unique_id).c_str());
            open_delete = false;
        }
        if (open_script) {
            ImGui::OpenPopup(("attach_script_popup_" + comp_unique_id).c_str());
            open_script = false;
        }
        // Переименование
        if (ImGui::BeginPopupModal(("rename_comp_popup_" + comp_unique_id).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            char input_buf[64];
            strncpy(input_buf, rename_buf.c_str(), sizeof(input_buf));
            if (ImGui::InputText("Новое имя", input_buf, sizeof(input_buf))) {
                rename_buf = input_buf;
            }
            if (ImGui::Button("OK") && !rename_buf.empty() && comp_name_cache != rename_buf) {
                auto it = node->child_components.find(comp_name_cache);
                if (it != node->child_components.end()) {
                    auto node_ptr = it->second;
                    node->child_components.erase(it);
                    node->child_components[rename_buf] = node_ptr;
                }
                rename_buf.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Отмена")) {
                rename_buf.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        // Удаление
        if (ImGui::BeginPopupModal(("delete_comp_popup_" + comp_unique_id).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Удалить компонент '%s'?", comp_name_cache.c_str());
            if (ImGui::Button("Да")) {
                auto it = node->child_components.find(comp_name_cache);
                if (it != node->child_components.end()) {
                    Component* ptr = it->second.get();
                    node->child_components.erase(it);
                    // Удалить из g_lua_components
                    auto git = std::find(g_lua_components.begin(), g_lua_components.end(), ptr);
                    if (git != g_lua_components.end()) {
                        int idx = std::distance(g_lua_components.begin(), git);
                        g_lua_components.erase(git);
                        if (selected_component_index == idx) selected_component_index = -1;
                        else if (selected_component_index > idx) selected_component_index--;
                    }
                    if (window) window->remove_component_from_pipeline(ptr);
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Нет")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        // Прикрепление скрипта
        if (ImGui::BeginPopupModal(("attach_script_popup_" + comp_unique_id).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            char input_buf[256];
            strncpy(input_buf, script_buf.c_str(), sizeof(input_buf));
            if (ImGui::InputText("Путь к скрипту", input_buf, sizeof(input_buf))) {
                script_buf = input_buf;
            }
            if (ImGui::Button("OK") && !script_buf.empty()) {
                auto it = node->child_components.find(comp_name_cache);
                if (it != node->child_components.end()) {
                    it->second->add_script(script_buf);
                    it->second->run_script_main();
                }
                script_buf.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Отмена")) {
                script_buf.clear();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        // Показываем дочерние Node
        for (auto it = node->child_nodes.begin(); it != node->child_nodes.end(); ++it) {
            const std::string& child_name = it->first;
            auto& child_node = it->second;
            ImGui::PushID((label_prefix + child_name).c_str());
            ImGui::TreeNodeEx((child_name + " (Node)##" + label_prefix + child_name).c_str(), 0);
            // Контекстное меню по ПКМ
            if (ImGui::BeginPopupContextItem(("context_node_" + label_prefix + child_name).c_str())) {
                if (ImGui::MenuItem("Переименовать")) {
                    ImGui::OpenPopup(("rename_node_popup_" + label_prefix + child_name).c_str());
                }
                if (ImGui::MenuItem("Удалить")) {
                    ImGui::OpenPopup(("delete_node_popup_" + label_prefix + child_name).c_str());
                }
                ImGui::EndPopup();
            }
            // Popup для переименования Node
            std::string rename_node_popup_id = "rename_node_popup_" + label_prefix + child_name;
            if (ImGui::BeginPopupModal(rename_node_popup_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                static std::unordered_map<std::string, std::string> rename_node_buffers;
                std::string& buf = rename_node_buffers[rename_node_popup_id];
                if (buf.empty()) buf = child_name;
                char input_buf[64];
                strncpy(input_buf, buf.c_str(), sizeof(input_buf));
                if (ImGui::InputText("Новое имя", input_buf, sizeof(input_buf))) {
                    buf = input_buf;
                }
                if (ImGui::Button("OK") && !buf.empty() && child_name != buf) {
                    auto node_ptr = it->second;
                    node->child_nodes.erase(it);
                    node->child_nodes[buf] = node_ptr;
                    buf.clear();
                    ImGui::CloseCurrentPopup();
                    ImGui::TreePop();
                    ImGui::PopID();
                    break;
                }
                ImGui::SameLine();
                if (ImGui::Button("Отмена")) {
                    buf.clear();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
            // Popup для удаления Node
            std::string delete_node_popup_id = "delete_node_popup_" + label_prefix + child_name;
            if (ImGui::BeginPopupModal(delete_node_popup_id.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
                bool delete_confirmed = false;
                ImGui::Text("Удалить Node '%s'?", child_name.c_str());
                if (ImGui::Button("Да")) {
                    delete_confirmed = true;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Нет")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
                if (delete_confirmed) {
                    ImGui::TreePop();
                    ImGui::PopID();
                    node->child_nodes.erase(it);
                    break;
                }
            }
            if (child_node) {
                this->show_node_hierarchy(child_node.get(), child_name);
            }
            ImGui::TreePop();
            ImGui::PopID();
        }
        ImGui::TreePop();
    }
}

void UI::show_all_components() {
    if (ImGui::Begin("All Components")) {
        // Найти все Node в g_lua_components и показать иерархию
        for (size_t i = 0; i < g_lua_components.size(); ++i) {
            Component* comp = g_lua_components[i];
            if (auto* node = dynamic_cast<Node*>(comp)) {
                std::string label = "Node " + std::to_string(i) + " (Node)";
                ImGuiTreeNodeFlags node_flags = 0;
                if ((int)i == selected_component_index) node_flags |= ImGuiTreeNodeFlags_Selected;
                bool open = ImGui::TreeNodeEx(label.c_str(), node_flags);
                if (ImGui::IsItemClicked()) {
                    selected_component_index = (int)i;
                }
                if (open) {
                    this->show_node_hierarchy(node, "Root");
                    ImGui::TreePop();
                }
            }
        }
        // Кнопка добавления компонента с выбором типа
        static int selected_type = 0;
        static const char* types[] = {"Sprite2D", "Triangle", "AnimatedSprite2D", "TileMap", "Camera", "Node"};
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
                    } else if (selected_type == 5) { // Node
                        auto* node = new Node();
                        g_lua_components.push_back(node);
                    }
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void UI::show_script_editor() {
    if (script_edit_index >= 0) {
        // Если редактор не инициализирован и есть путь к скрипту — загрузить содержимое
        if (!editor_initialized && !script_edit_path.empty()) {
            std::ifstream in(script_edit_path);
            if (in) {
                std::stringstream buffer;
                buffer << in.rdbuf();
                editor.SetText(buffer.str());
                editor_initialized = true;
            } else {
                editor.SetText("");
                editor_initialized = true;
            }
        }
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