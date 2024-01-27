#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "imgui_stdlib.h"
#include <stdio.h>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif


#include "..\libs\glfw\glfw-3.3.8.bin.WIN64\include\GLFW\glfw3.h"

#include "../includes/schedulBuilder/scheduleBuilder.h"
#include "../includes/database/db.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Schedule Builder", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    static ScheduleBuilder* scheduleBuilder = ScheduleBuilder::getInstance();

    bool showTaskCreationWindow = false;
    static float pointsSum = scheduleBuilder->checkedPoints();
    static float totalPoints = scheduleBuilder->totalPoints();

    // Main loop
#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;

            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            
            ImGui::SetNextWindowBgAlpha(1);
            ImGui::SetNextWindowSizeConstraints(ImVec2(1500, 800), ImVec2(FLT_MAX, FLT_MAX));

            ImGui::Begin(" ", nullptr, flags);
            ImGui::SetWindowFontScale(1.8);
            //ImGui::SetWindowFontScale(1.2f);
            if (ImGui::Button("Add Task", ImVec2(viewport->WorkSize.x / 7, ImGui::GetFontSize() * 1.3))) {
                showTaskCreationWindow = true;
            }

            ImGui::SameLine();

            ImGui::Text("Total points: %d/%d (%.1f%%)", int(pointsSum), int(totalPoints), (totalPoints == 0) ? 0.0f : pointsSum / totalPoints * 100);
            
            std::vector<Task*> tasks = scheduleBuilder->getTasks();
            totalPoints = scheduleBuilder->totalPoints();

            for (int i = 0; i < tasks.size(); i+=1) {
                ImGui::PushID(i);
                if (ImGui::Checkbox(tasks[i]->toString().c_str(), tasks[i]->isChecked())) {
                    scheduleBuilder->updateTaskCheck(tasks[i]);
                    if (*(tasks[i]->isChecked())) {
                        pointsSum += tasks[i]->getPoints();
                    }
                    else {
                        pointsSum -= tasks[i]->getPoints();
                    }
                }
                
                ImGui::SameLine();
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0, 0.8f, 0.8f));
                if (ImGui::Button("X")) {
                    if (*(tasks[i]->isChecked())) {
                        pointsSum -= tasks[i]->getPoints();
                    }
                    scheduleBuilder->deleteTask(tasks[i]);
                }
                ImGui::PopStyleColor(3);
                ImGui::PopID();
            }
            
            ImGui::End();
        }

        if (showTaskCreationWindow) {
            ImVec2 winSize = ImGui::GetMainViewport()->Size;
            static ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;
            
            ImGui::SetNextWindowSizeConstraints(ImVec2(270, 185 + ImGui::GetFontSize() * 3), ImVec2(FLT_MAX, 185 + ImGui::GetFontSize() * 3));
            ImGui::SetNextWindowSize(ImVec2(winSize.x / 4, 185 + ImGui::GetFontSize() * 3));
            ImGui::Begin("New Task", &showTaskCreationWindow, flags);
            ImGui::SetWindowFontScale(1.4);

            static std::string name;
            static int points = 0;

            ImGui::PushItemWidth(winSize.x / 5);
            ImGui::InputText("Name", &name);
            ImGui::PushItemWidth(winSize.x / 20);
            ImGui::InputInt("Points", &points, 0);
            static int startHours = 0;
            static int startMinutes = 0;
            static int startSeconds = 0;
            static int endHours = 0;
            static int endMinutes = 0;
            static int endSeconds = 0;
            ImGui::BeginGroup();
            ImGui::PushID(1);
            ImGui::Text("Start Time:");
            ImGui::InputInt("H", &startHours, 0);
            ImGui::SameLine();
            ImGui::InputInt("M", &startMinutes, 0);
            ImGui::SameLine();
            ImGui::InputInt("S", &startSeconds, 0);
            ImGui::PopID();
            ImGui::EndGroup();

            ImGui::BeginGroup();
            ImGui::PushID(2);
            ImGui::Text("End Time:");
            ImGui::InputInt("H", &endHours, 0);
            ImGui::SameLine();
            ImGui::InputInt("M", &endMinutes, 0);
            ImGui::SameLine();
            ImGui::InputInt("S", &endSeconds, 0);
            ImGui::PopID();
            ImGui::EndGroup();

            if (ImGui::Button("Confirm", ImVec2(winSize.x / 10, ImGui::GetFontSize() * 1.3)) && !name.empty() && points > 0 && Time::correctSequence(startHours, startMinutes, startSeconds, endHours, endMinutes, endSeconds)) {
                Time* startTime = new Time((startHours < 10 ? "0" : "") + std::to_string(startHours), (startMinutes < 10 ? "0" : "") + std::to_string(startMinutes), (startSeconds < 10 ? "0" : "") + std::to_string(startSeconds));
                Time* endTime = new Time((endHours < 10 ? "0" : "") + std::to_string(endHours), (endMinutes < 10 ? "0" : "") + std::to_string(endMinutes), (endSeconds < 10 ? "0" : "") + std::to_string(endSeconds));
                Task* task = new Task(name, points, startTime, endTime, false);
                scheduleBuilder->addTask(task);
                name = "";
                points = 0;
                startHours = 0;
                startMinutes = 0;
                startSeconds = 0;
                endHours = 0;
                endMinutes = 0;
                endSeconds = 0;
                showTaskCreationWindow = false;
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    delete scheduleBuilder;

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
