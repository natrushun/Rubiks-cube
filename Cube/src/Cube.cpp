#include "Utils.hpp";


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Camera.hpp"
#include "Solver.hpp"
#include <thread>
#include <chrono>
#include <windows.h>

struct WindowData {
	Camera camera;
	int width;
	int height;
};

void framebuffer_size_callback(GLFWwindow* window, int newWidth, int newHeight) {
	if (newHeight == 0) return; 

	WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
	if (data) {
		data->width = newWidth;
		data->height = newHeight;

		
		glViewport(0, 0, newWidth, newHeight);
		data->camera.SetAspect(static_cast<float>(newWidth) / newHeight);
	}
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Initialize GLFW,GLEW
	GLFWwindow* window;
	WindowData windowData;
	windowData.width = 1024;
	windowData.height = 768;
	try {
		window = BaseInit(1024,768);
		
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint programID = LoadShaders("./shaders/VertexShader.vert", "./shaders/FragmentShader.frag");
	
	TimeInfo Time;

	windowData.camera = Camera();
	windowData.camera.SetAspect(static_cast<float>(windowData.width) / windowData.height);

	glfwSetWindowUserPointer(window, &windowData);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	std::vector<Cubie> rubik= InitRubik();
	

	bool spacePressed = false;
	bool qpress = false;
	bool Check = false;
	bool turn = false;

	Solver solver= Solver();
	float TimeDur = 1.0f/ 120.0f;
	solver.FetchCubeState(rubik);

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(windowData.width * 0.3,windowData.height));
		ImGui::SetNextWindowPos(ImVec2(5, 5));
		if (ImGui::Begin("Rubik",nullptr, ImGuiWindowFlags_NoMove)) {
			ImGui::PushTextWrapPos();
			if (ImGui::CollapsingHeader("Description")) {
				ImGui::PushTextWrapPos();
				ImGui::Text("Moves:");
				ImGui::Text("U = Up face (orange center)");
				ImGui::Text("D = Down face (red center)");
				ImGui::Text("L = Left face (blue center)");
				ImGui::Text("R = Right face (green center)");
				ImGui::Text("F = Front face (yellow center)");
				ImGui::Text("B = Back face (white center)");

				ImGui::Text("Variants:");
				ImGui::Text("X  = 90 deg clockwise turn");
				ImGui::Text("X' = 90 deg anti-clockwise turn");
				ImGui::Text("X2 = 180 deg turn");

				ImGui::Separator();

				ImGui::Text("Controls:");
				ImGui::Text("Left mouse button : rotate face");
				ImGui::Text("Right mouse button : rotate camera");
				ImGui::Text("S : Solve rubik's cube");
				ImGui::Text("Space : Random shuffle");
				ImGui::Separator();
			}
			
			ImGui::Text("solution: %s", solver.GetSolutionString().c_str());
			ImGui::Text("Random generated shuffle: %s", solver.GetDumbSolutionString().c_str());
		}
		ImGui::End();

		windowData.camera.Move(&Time, window);

		RefreshTime(&Time);
		glProgramUniformMatrix4fv(programID, glGetUniformLocation(programID, "viewProj"), 1, GL_FALSE, glm::value_ptr(windowData.camera.GetViewProj()));
		glProgramUniform3fv(programID, glGetUniformLocation(programID, "view"), 1, glm::value_ptr(windowData.camera.GetEye()));
		glUseProgram(programID);
		
		int i = 0;
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				for (int z = -1; z <= 1; z++) {
					DrawObject(rubik[i].cube, rubik[i].cube.modelMatrix, programID);
					i++;
				}
			}
		}
		glm::vec3 rayPos;
		glm::vec3 rayDir;


		bool mouseInUse = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
		
		moveCubes(&Time, window, windowData.height, windowData.width, windowData.camera.GetViewProj(), rayPos, rayDir, windowData.camera.GetEye(), rubik,Check,turn);
		
		if (Check) {
			solver.CheckDifferences(rubik);
			Check = false;
		}


		if (!turn &&  solver.Solved(rubik)) {
			glClearColor(0.067, 0.671, 0.059, 0.0f);
		}
		else {
			glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		}
		
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
			spacePressed = false;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !spacePressed && !mouseInUse) {
			solver.RandomMoveSequence(rubik, 20);
			spacePressed = true;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
			qpress = false;
			
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !qpress && !mouseInUse) {
			solver.CheckDifferences(rubik);
			solver.Solve();
			qpress = true;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (Time.deltaTime < TimeDur) {
			std::this_thread::sleep_for(std::chrono::duration<double>(TimeDur - Time.deltaTime));
		}
	} 
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	int i = 0;
	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				Destructor(rubik[i].cube);
				i++;
			}
		}
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}


