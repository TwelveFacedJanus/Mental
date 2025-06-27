#include <iostream>
#include "Mental.hpp"
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Component.hpp"
#include "Ui.hpp"
#include <stb_image.h>
#include <GL/glew.h>

// === Глобальные/члены класса для viewport FBO ===
GLuint viewport_fbo = 0;
GLuint viewport_texture = 0;
GLuint viewport_depth = 0;
int viewport_width = 800, viewport_height = 600;

void create_viewport_fbo(int width, int height) {
    if (viewport_fbo) {
        glDeleteFramebuffers(1, &viewport_fbo);
        glDeleteTextures(1, &viewport_texture);
        glDeleteRenderbuffers(1, &viewport_depth);
    }
    glGenFramebuffers(1, &viewport_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, viewport_fbo);
    glGenTextures(1, &viewport_texture);
    glBindTexture(GL_TEXTURE_2D, viewport_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewport_texture, 0);
    glGenRenderbuffers(1, &viewport_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, viewport_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, viewport_depth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    viewport_width = width;
    viewport_height = height;
}

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

void MentalWindow::load_root_script() {
    // --- RUN ROOT LUA SCRIPT ---
    lua_State* rootL = luaL_newstate();
    luaL_openlibs(rootL);
    // Register C++ Lua bindings for root script
    Component::register_sprite2d(rootL);
    Component::register_triangle(rootL);
    Component::register_camera(rootL);
    Node::register_node(rootL);
    if (luaL_dofile(rootL, "./Scripts/root.lua") != LUA_OK) {
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
}

void MentalWindow::addSprite2D(Sprite2D* sprite, float x, float y, float z, const std::string& script_path, const glm::mat4& default_view, const glm::mat4& default_projection, float delta)
{
    sprite->setShader();
    sprite->set_position(x, y, z);
    if (!script_path.empty()) {
        sprite->add_script(script_path);
        sprite->run_script_main();
    }
    g_lua_components.push_back(sprite);
    this->rp->add(sprite, std::bind(&Sprite2D::apply_shader, sprite));
    this->rp->add(sprite, [sprite, this, &default_view, &default_projection]() {
        glm::mat4 view = g_active_camera ? g_active_camera->getView() : default_view;
        glm::mat4 projection = g_active_camera ? g_active_camera->getProjection() : default_projection;
        GLuint modelLoc = glGetUniformLocation(sprite->shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(sprite->model));
        GLuint viewLoc = glGetUniformLocation(sprite->shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        GLuint projectionLoc = glGetUniformLocation(sprite->shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    });
    this->rp->add(sprite, std::bind(&Sprite2D::draw, sprite));
    this->rp->add(sprite, std::bind(&Sprite2D::reload_script_if_changed, sprite));
    this->rp->add(sprite, [sprite, delta]() { sprite->call_process(delta); });
}

void MentalWindow::addTriangle(Triangle* tri, const glm::mat4& default_view, const glm::mat4& default_projection, float delta)
{
    tri->setShader();
    g_lua_components.push_back(tri);
    this->rp->add(tri, std::bind(&Triangle::apply_shader, tri));
    this->rp->add(tri, [tri, this, &default_view, &default_projection]() {
        glm::mat4 view = g_active_camera ? g_active_camera->getView() : default_view;
        glm::mat4 projection = g_active_camera ? g_active_camera->getProjection() : default_projection;
        GLuint modelLoc = glGetUniformLocation(tri->shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(tri->model));
        GLuint viewLoc = glGetUniformLocation(tri->shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        GLuint projectionLoc = glGetUniformLocation(tri->shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    });
    this->rp->add(tri, std::bind(&Triangle::draw, tri));
    this->rp->add(tri, std::bind(&Triangle::reload_script_if_changed, tri));
    this->rp->add(tri, [tri, delta]() { tri->call_process(delta); });
}

void MentalWindow::main_loop() {
    initialize_imgui(this->window);
    this->load_root_script();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glm::mat4 default_projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
    glm::mat4 default_view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    float delta = 0.0f;

    auto root_node = std::make_shared<Node>();
    g_lua_components.push_back(root_node.get());

    // ===== CAMERA ======
    auto camera_ptr = std::make_shared<Camera>();
    camera_ptr->set_position(0.0f, 0.0f, 3.0f);
    camera_ptr->look_at(0.0f, 0.0f, 0.0f);
    g_active_camera = camera_ptr.get();
    g_lua_components.push_back(g_active_camera);
    root_node->add_component("camera", camera_ptr);

    // ===== FBO INIT =====
    create_viewport_fbo(viewport_width, viewport_height);
    auto sprite_ptr = std::make_shared<Sprite2D>();
    addSprite2D(sprite_ptr.get(), 0.5f, 0.0f, 0.0f, "./Scripts/sprite_component.lua", default_view, default_projection, delta);
    root_node->add_component("sprite", sprite_ptr);

    // ===== BACKGROUND ======
    auto bg_ptr = std::make_shared<Sprite2D>();
    addSprite2D(bg_ptr.get(), 0.0f, 0.0f, 0.0f, "./Scripts/background_component.lua", default_view, default_projection, delta);
    root_node->add_component("background", bg_ptr);

    // ===== CUBE3D =====
    auto cube_ptr = std::make_shared<Cube3D>();
    cube_ptr->setShader();
    cube_ptr->applyTexture("Textures/minecraft_block.jpg");
    cube_ptr->set_position(-1.0f, 0.0f, 0.0f);
    g_lua_components.push_back(cube_ptr.get());
    this->rp->add(cube_ptr.get(), std::bind(&Cube3D::apply_shader, cube_ptr.get()));
    this->rp->add(cube_ptr.get(), [cube_ptr, this, &default_view, &default_projection]() {
        glm::mat4 view = g_active_camera ? g_active_camera->getView() : default_view;
        glm::mat4 projection = g_active_camera ? g_active_camera->getProjection() : default_projection;
        GLuint modelLoc = glGetUniformLocation(cube_ptr->shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cube_ptr->model));
        GLuint viewLoc = glGetUniformLocation(cube_ptr->shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        GLuint projectionLoc = glGetUniformLocation(cube_ptr->shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    });
    this->rp->add(cube_ptr.get(), std::bind(&Cube3D::draw, cube_ptr.get()));
    root_node->add_component("cube", cube_ptr);

    // ===== PLAYER =====
    UI ui;

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(this->window)) {
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
        glm::vec3 target_cam_pos(sprite_ptr->position.x, sprite_ptr->position.y, 3.0f);
        glm::vec3 target_cam_look(sprite_ptr->position.x, sprite_ptr->position.y, 0.0f);
        camera_ptr->update_interpolated(0.1f, target_cam_pos, target_cam_look);
        // === IMGUI FRAME START ===
        ui.set_context(this, default_view, default_projection, delta);
        create_new_frame();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        ImGui::PopStyleVar(2);
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();

        // === VIEWPORT WIDGET ===
        ImGui::Begin("Viewport");
        ImVec2 avail = ImGui::GetContentRegionAvail();
        int new_width = std::max(1, (int)avail.x);
        int new_height = std::max(1, (int)avail.y);
        if (new_width != viewport_width || new_height != viewport_height) {
            create_viewport_fbo(new_width, new_height);
        }
        // 1. Рендер в FBO
        glBindFramebuffer(GL_FRAMEBUFFER, viewport_fbo);
        glViewport(0, 0, viewport_width, viewport_height);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        rp->run();
        root_node->draw();
        for (Component* comp : g_lua_components) {
            if (auto* tilemap = dynamic_cast<TileMap*>(comp)) tilemap->draw();
            comp->call_process(delta);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // 2. Показываем текстуру
        ImGui::Image((void*)(intptr_t)viewport_texture, avail, ImVec2(0,1), ImVec2(1,0));
        ImGui::End();

        // Остальные виджеты
        ui.show_all_components();
        ui.show_script_editor();
        ui.show_tilesheet_cutter();
        ui.show_file_browser();
        ui.show_component_selector();
        if (!g_lua_components.empty()) {
            int idx = ui.get_selected_component_index();
            if (idx >= 0 && idx < g_lua_components.size()) {
                ui.show_component_properties(g_lua_components[idx]);
            }
        }

        // === IMGUI RENDER ===
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
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

void MentalWindow::remove_component_from_pipeline(Component* comp) {
    if (rp) rp->remove_for_component(comp);
}