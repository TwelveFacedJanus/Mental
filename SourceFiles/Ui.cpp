#include "Mental.hpp"

MentalResult mentalEngineSetCustomUIStyle()
{
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

    return MENTAL_OK;
}

MentalResult mentalEngineUIInitialize(MentalEngineUI* ui, GLFWwindow* window)
{
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
    mentalEngineSetCustomUIStyle();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return MENTAL_OK;
}

MentalResult mentalEngineUICreateNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    return MENTAL_OK;
}

MentalResult mentalEngineUISetContext(MentalEngineCore* instance, MentalEngineUI* ui, const glm::mat4& view, const glm::mat4& proj, float d) {
    ui->window = instance->window;
    ui->default_view = view;
    ui->default_projection = proj;
    ui->delta = d;
    return MENTAL_OK;
}
