// ImGui - standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
// (GL3W is a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc.)

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include <GLFW/glfw3.h>
#include <gurptest.cpp>
#include <Inconsolata-Regular.cpp>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
	
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    //GLFWwindow* window = glfwCreateWindow(795, 950, "ImGui GURPSapp Game Aid Using OpenGL3", NULL, NULL);
	GLFWwindow* window = glfwCreateWindow(975, 950, "ImGui GURPSapp Game Aid Using OpenGL3", NULL, NULL);

	glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync
    gl3wInit();
	
    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.Fonts->AddFontDefault();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	ImGui_ImplGlfwGL3_Init(window, true);
    //ImGui_ImplGlfwGL3_Init(window, false);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them. 
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple. 
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'misc/fonts/README.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //ENABLE DEFAULT FONTS FOR RELEASE.
	//io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    
	//ImFont* Inconsolata = io.Fonts->AddFontFromMemoryCompressedTTF(inCon_fontTest_compressed_data, inCon_fontTest_compressed_size, 19.6f);
	ImFont* Inconsolata = io.Fonts->AddFontFromMemoryCompressedTTF(inCon_fontTest_compressed_data, inCon_fontTest_compressed_size, 24.4f);

	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    bool show_demo_window = false;
	//bool show_demo_window = true;

    bool show_another_window = false;
	//bool my_tool_active = true;
	bool my_tool_active = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        ImGui_ImplGlfwGL3_NewFrame();

        // 1. Show a simple window.
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
        {
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Text("This is a Debug Function.");                           // Display some text (you can use a format string too)
           // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        // 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
        if (show_demo_window)
        {
            //ImGui::SetNextWindowPos(ImVec2(680, 775), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
			ImGui::SetNextWindowPos(ImVec2(680, 150), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
			ImGui::ShowDemoWindow(&show_demo_window);
        }

		//BEGIN MENU THING FOR PROJECT


		//One thing I really, REALLY Need to do, is standardize my line size to a maximum of 80 Columns.it looks better and keeps up with tradition.

		ImGui::SetNextWindowSize(ImVec2(580, 640), ImGuiCond_FirstUseEver);
		//ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
		//ImGui::SetNextWindowPos(ImVec2(660, 0), ImGuiCond_FirstUseEver);
		/*
		This is an 80Column set of dots. Nice and clean.
		We will use this to better type-set the rest of the writing.
		...............................................................................

		*/

		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::Begin("GURPSapp Roleplaying Toolset", &my_tool_active, ImGuiWindowFlags_MenuBar);

		legalMatters();
		//if (ImGui::BeginMenuBar())
		//{
			// Menu Bar is not complete. So, it remains unused.
			//if (ImGui::BeginMenu("File"))
			//{
				//if (ImGui::MenuItem("INACTIVE", "IN")) { /* Do stuff */ }
				//if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
				//if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
				//if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			//	ImGui::EndMenu();
			//}
			//ImGui::EndMenuBar();

			
		//}
		
		//Each Function below calls the presence of collapsible headers that are used in the Game Aid.

		//Debug Tool for demonstrating Tool structure.
		

		defaultTool();

		glossTable();

		successRoll();

		damageRoll();

		//combatStatusTool();

		//frightCheck();

		reactionRolls();

		//speedrangeTbl();

		
		
		ImGui::End();
		//END MENU THINGS



        // Rendering
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();

    return 0;
}
