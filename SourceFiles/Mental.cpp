#include "Mental.hpp"

MentalResult mentalEngineCreateViewportFBO(MentalEngineCore* instance, int width, int height) {
    if (instance == nullptr) {
        std::cerr << "Failed to create viewport FBO. Instance is nullptr.\n";
        return MENTAL_FATAL;
    }

    if (instance->viewport_fbo) {
        glDeleteFramebuffers(1, &instance->viewport_fbo);
        glDeleteTextures(1, &instance->viewport_texture);
        glDeleteRenderbuffers(1, &instance->viewport_depth);
    }

    glGenFramebuffers(1, &instance->viewport_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, instance->viewport_fbo);

    glGenTextures(1, &instance->viewport_texture);
    glBindTexture(GL_TEXTURE_2D, instance->viewport_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, instance->viewport_texture, 0);

    glGenRenderbuffers(1, &instance->viewport_depth);
    glBindRenderbuffer(GL_RENDERBUFFER, instance->viewport_depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, instance->viewport_depth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Failed to create viewport FBO. Framebuffer not complete.\n";
        return MENTAL_ERROR;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    instance->viewport_size[0] = width;
    instance->viewport_size[1] = height;
    
    return MENTAL_OK;
}

MentalResult mentalEngineDrawUI(MentalEngineCore* instance) {
    
}

MentalResult mentalEngineInitialize(MentalEngineCore* instance) {

    if (instance == nullptr) {
        std::cerr << "Failed to initialize mental engine. Instance is nullptr.\n";
        return MENTAL_FATAL;
    }
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize mental engine. Cannot initialize GLFW.\n";
        return MENTAL_FATAL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    instance->window = glfwCreateWindow(800, 600, "Mental OpenGL renderer.", nullptr, nullptr);
    if (instance->window == nullptr) {
        std::cerr << "Failed to create GLFW window.\n";
    }

    glfwMakeContextCurrent(instance->window);
    
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize mental engine. Cannot initialize GLEW.\n";
        return MENTAL_FATAL;
    }

    if (instance->viewport_fbo) {
        glDeleteFramebuffers(1, &instance->viewport_fbo);
        glDeleteTextures(1, &instance->viewport_texture);
        glDeleteRenderbuffers(1, &instance->viewport_depth);
    }
    mentalEngineUIInitialize(&instance->ui, instance->window);
    mentalEngineCreateViewportFBO(instance, instance->current_size[0], instance->current_size[1]);

    luaL_openlibs(instance->L);
    return MENTAL_OK;
}

MentalResult mentalEngineMainLoop(MentalEngineCore* instance)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    double lastTime = glfwGetTime();
    float delta = 0.0f;

    while (!glfwWindowShouldClose(instance->window))
    {
        double currentTime = glfwGetTime();
        delta = static_cast<float>(currentTime - lastTime);
        
        mentalEngineUICreateNewFrame();
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

        ImGui::Begin("Viewport");
        ImVec2 avail = ImGui::GetContentRegionAvail();
        int new_width = std::max(1, (int)avail.x);
        int new_height = std::max(1, (int)avail.y);
        if (new_width != instance->viewport_size[0] || new_height != instance->viewport_size[1]) {
            mentalEngineCreateViewportFBO(instance, new_width, new_height);
        }
        glBindFramebuffer(GL_FRAMEBUFFER, instance->viewport_fbo);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        ImGui::Image((void*)(intptr_t)instance->viewport_texture, avail, ImVec2(0,1), ImVec2(1,0));
        ImGui::End();

        /*
        _show_all_components(instance);
        _show_script_editor();
        _show_script_editor();
        _show_file_browser();
        _show_component_selector();
        */
        

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        glfwSwapBuffers(instance->window);
        glfwPollEvents();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (instance->window) glfwDestroyWindow(instance->window);
    glfwTerminate();
    return MENTAL_OK;
}

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
