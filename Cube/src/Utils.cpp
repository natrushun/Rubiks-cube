#include "Utils.hpp"
#include "Exceptions.hpp"
#include "Camera.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <glm/gtc/epsilon.hpp>
using namespace glm;
using namespace std;


GLFWwindow* BaseInit(int width,int height) {

	if (!glfwInit())
	{
		throw GLFWInitException();
	}
	glfwWindowHint(GLFW_SAMPLES, 126);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	GLFWwindow* window;
	window = glfwCreateWindow(width, height, "Rubik's cube", NULL, NULL);
	if (window == NULL) {
		throw GLFWWindowException();
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	

	// Initialize GLEW
	glewExperimental = true;

	int profile;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	if (profile & GL_CONTEXT_CORE_PROFILE_BIT)
		std::cout << "Core Profile active" << std::endl;
	else
		std::cout << "Compatibility Profile active" << std::endl;


	if (glewInit() != GLEW_OK) {
		throw GLEWInitException();
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	//imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	SetImguiStyle();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();


	return window;

}
void SetImguiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	
	ImGui::StyleColorsDark();

	
	style.WindowRounding = 10.0f;
	style.FrameRounding = 8.0f;
	style.ScrollbarRounding = 12.0f;
	style.GrabRounding = 6.0f;
	style.FrameBorderSize = 1.0f;
	style.WindowBorderSize = 1.0f;
	style.PopupRounding = 6.0f;
	style.GrabMinSize = 20.0f;
	style.TabRounding = 4.0f;
	style.WindowPadding = ImVec2(12, 12);
	style.FramePadding = ImVec2(8, 4);
	style.ItemSpacing = ImVec2(8, 6);

	
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.28f, 0.32f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.32f, 0.36f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.18f, 0.22f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.26f, 0.29f, 0.32f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.11f, 0.64f, 0.92f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.08f, 0.50f, 0.72f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.13f, 0.54f, 0.84f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.13f, 0.54f, 0.84f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.08f, 0.45f, 0.78f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.13f, 0.54f, 0.84f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.13f, 0.54f, 0.84f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.13f, 0.54f, 0.84f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.13f, 0.54f, 0.84f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.13f, 0.54f, 0.84f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.10f, 0.31f, 0.53f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.15f, 0.47f, 0.76f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.18f, 0.50f, 0.78f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.13f, 0.54f, 0.84f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.13f, 0.54f, 0.84f, 0.90f);
}



GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {

	
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		printf("Can't to open %s.\n", vertex_file_path);
		getchar();
		return 0;
	}

	
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}
	else {
		printf("Can't to open %s.\n", vertex_file_path);
		getchar();
		return 0;
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	
	printf("Compiling shader : %s\n", vertex_file_path);
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE) {
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	
	printf("Compiling shader : %s\n", fragment_file_path);
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	if (Result == GL_FALSE) {
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	if (Result == GL_FALSE) {
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

Object InitObject(std::vector<Color> colors) {
	static GLuint id = 0;
	Object obj;
	obj.id = ++id;
	obj.vertex.position = {

		// Front face
		{-0.5f, -0.5f, 0.5f},
		{0.5f, -0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, 0.5f},
		// Back face
		{0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, 0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		// Left face
		{-0.5f, -0.5f, -0.5f},
		{-0.5f, -0.5f, 0.5f},
		{-0.5f, 0.5f, 0.5f},
		{-0.5f, 0.5f, -0.5f},
		// Right face
		{0.5f, -0.5f, 0.5f},
		{0.5f, -0.5f, -0.5f},
		{0.5f, 0.5f, -0.5f},
		{0.5f, 0.5f, 0.5f},
		// Top face
		{-0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, 0.5f},
		{0.5f, 0.5f, -0.5f},
		{-0.5f, 0.5f, -0.5f},
		// Bottom face
		{-0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, -0.5f},
		{0.5f, -0.5f, 0.5f},
		{-0.5f, -0.5f, 0.5f}

	};
	obj.vertex.uv = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};

	
	
	ColorInit(obj,colors);
	obj.indexArray = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20
	};
	obj.count = static_cast<GLsizei>(obj.indexArray.size());
	
	
	glCreateBuffers(1, &obj.vertexVboID);
	glNamedBufferData(obj.vertexVboID,
					obj.vertex.position.size() * sizeof(glm::vec3),
					obj.vertex.position.data(),
					GL_STATIC_DRAW);

	glCreateBuffers(1, &obj.colorVboID);
	glNamedBufferData(obj.colorVboID,
					obj.vertex.color.size() * sizeof(glm::vec3),
					obj.vertex.color.data(),
					GL_DYNAMIC_DRAW);

	glCreateBuffers(1, &obj.uvVboID);
	glNamedBufferData(obj.uvVboID,
		obj.vertex.uv.size() * sizeof(glm::vec2),
		obj.vertex.uv.data(),
		GL_STATIC_DRAW);

	glCreateBuffers(1, &obj.indexVboID);
	glNamedBufferData(obj.indexVboID,
		obj.indexArray.size() * sizeof(GLuint),
		obj.indexArray.data(),
		GL_STATIC_DRAW);

	glCreateVertexArrays(1, &obj.vaoID);

	glVertexArrayVertexBuffer(obj.vaoID, 0, obj.vertexVboID, 0, sizeof(glm::vec3));
	glVertexArrayVertexBuffer(obj.vaoID, 1, obj.uvVboID, 0, sizeof(glm::vec2));
	glVertexArrayVertexBuffer(obj.vaoID, 2, obj.colorVboID, 0, sizeof(glm::vec3));
	
	// Set the format of the attributes
	glVertexArrayAttribFormat(obj.vaoID, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(obj.vaoID, 1, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribFormat(obj.vaoID, 2, 3, GL_FLOAT, GL_FALSE, 0);

	// Bind the attributes
	glVertexArrayAttribBinding(obj.vaoID, 0, 0);
	glVertexArrayAttribBinding(obj.vaoID, 1, 1);
	glVertexArrayAttribBinding(obj.vaoID, 2, 2);

	// Enable the attributes
	glEnableVertexArrayAttrib(obj.vaoID, 0);
	glEnableVertexArrayAttrib(obj.vaoID, 1);
	glEnableVertexArrayAttrib(obj.vaoID, 2);


	// Set the index buffer
	glVertexArrayElementBuffer(obj.vaoID, obj.indexVboID);

	return obj;
}

void Destructor(Object& obj) {
	glDeleteBuffers(1, &obj.vertexVboID);
	glDeleteBuffers(1, &obj.colorVboID);
	glDeleteBuffers(1, &obj.indexVboID);
	glDeleteVertexArrays(1, &obj.vaoID);
}

void RefreshTime(TimeInfo* Time) {
	static double LastTime = glfwGetTime();

	double CurrentTime = glfwGetTime();
	
	Time->deltaTime = static_cast<float>(CurrentTime - LastTime);
	Time->elapsedTime = static_cast<float>(CurrentTime);
	
	LastTime = CurrentTime;
}

std::vector<Cubie> InitRubik() {

	std::vector<Object> rubik;
	
	std::vector<Cubie> cubies;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			for (int z = -1; z <= 1; z++) {
				std::vector<std::tuple<Color, glm::vec3,int>> colors;
				std::vector<Color> faceColors = {
					Color::BLACK, // Front 
					Color::BLACK, // Back  
					Color::BLACK, // Left  
					Color::BLACK, // Right 
					Color::BLACK, // Top   
					Color::BLACK  // Bottom
				};

				if (z == 1) {
					faceColors[0] = Color::YELLOW;  // Front 
					colors.push_back({ Color::YELLOW ,glm::vec3(0.0f,0.0f,1.0f),18+(1-y) * 3 + (x + 1) });
				}
				if (z == -1) {
					faceColors[1] = Color::WHITE;   // Back 
					colors.push_back({ Color::WHITE ,glm::vec3(0.0f,0.0f,-1.0f),36+ (1-y) * 3 + (1-x) });
				}
				if (x == -1) {
					faceColors[2] = Color::BLUE;    // Left 
					colors.push_back({ Color::BLUE ,glm::vec3(-1.0f,0.0f,0.0f),9+ (1 - y) * 3 + (z + 1) });
				}
				if (x == 1) {
					faceColors[3] = Color::GREEN;   // Right 
					colors.push_back({ Color::GREEN ,glm::vec3(1.0f,0.0f,0.0f), 27+(1 - y) * 3 + (1 - z) });
				}
				if (y == 1) {
					faceColors[4] = Color::ORANGE;     // Top 
					colors.push_back({ Color::ORANGE ,glm::vec3(0.0f,1.0f,0.0f), (z + 1)*3 + (x + 1) });
				}
				if (y == -1) {
					faceColors[5] = Color::RED;  // Bottom 
					colors.push_back({ Color::RED ,glm::vec3(0.0f,-1.0f,0.0f),45+ (1 - z) * 3 + (x + 1) });
				}
				Cubie cubie;
				cubie.cube = InitObject(faceColors);
				

				glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
				cubie.cube.modelMatrix = transform; 

				cubie.cube.faceColors = faceColors;
				cubie.colors = colors;

				cubies.push_back(cubie);
				
			}
		}
	}

	return cubies;
}


// Right | Left | Front | Back | Top | Bottom
void ColorInit(Object& obj, std::vector<Color> colors) {
	std::vector<glm::vec3> color;
	for (int i = 0; i < colors.size(); i++) {
		for (int j = 0; j < 4; j++) {
			color.push_back(getColor(colors[i]));
		}
	}
	obj.vertex.color = color;
}

void DrawObject(Object& obj, glm::mat4 world,GLuint programID)
{
    glProgramUniformMatrix4fv(programID, glGetUniformLocation(programID, "world"), 1, GL_FALSE, glm::value_ptr(world));
	glBindVertexArray(obj.vaoID);
	glDrawElements(GL_TRIANGLES, obj.count, GL_UNSIGNED_INT, nullptr);


}

static bool firstRightPress = true;
static bool directionY = false;
static bool directionX = false;
static Cubie* pickedObj;
static double lastX = 0.0, lastY = 0.0;
static std::vector<Cubie*> pickedNeighbors;
static float anglesum = 0;
static Direction direction;
static bool firstColorKeyPress = true;
static bool ShiftMouseKeyReleased = false;

void moveCubes(TimeInfo* Time, GLFWwindow* window,int height,int width,glm::mat4 viewProj,glm::vec3& rayPos,glm::vec3 &rayDir,const glm::vec3 eye, std::vector<Cubie>& rubik,bool& Check,bool& turn) {
	
	float speed = 100.0f;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos); 
		if (firstRightPress)
		{
			lastX = xpos;
			lastY = ypos;
			firstRightPress = false;

			glm::vec4 NDC(
				(static_cast<float>(xpos)) / width * 2.0f-1,
				1-(static_cast<float>(ypos)) /height * 2.0f,
				0.0f,
				1.0f
			);

			glm::vec4 worldPos = glm::inverse(viewProj) * NDC;
			worldPos /= worldPos.w;

			rayPos = eye;
			rayDir = glm::normalize(glm::vec3(worldPos) - rayPos);
			
			pickedObj = pickedCube(rayPos, rayDir, rubik);
			if (pickedObj != nullptr) {
				direction.hitDirection = getHitFaceint((pickedObj->cube), rayPos, rayDir);
				direction.viewDirection = getViewDirection(rayDir);
			}
			
		}
		else
		{
			if (pickedObj == nullptr) return;
			
			if (!directionX && !directionY)
			{
				double xoffset = xpos - lastX;
				double yoffset = lastY - ypos;
				lastX = xpos;
				lastY = ypos;

				if (fabs(xoffset) > fabs(yoffset)) {
					directionX = true;
					pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
				}
				else if (fabs(xoffset) < fabs(yoffset)) {
					directionY = true;
					pickedNeighbors = getPickedObjectNeighbors(&pickedObj->cube, rubik, direction, directionX);
				}
			}
			if (directionX)
			{
				turn = true;
				double xoffset = xpos - lastX;
				lastX = xpos;
				double speed = 30.0;

				anglesum += direction.prefix* xoffset * Time->deltaTime*speed;
				for (int i = 0; i < pickedNeighbors.size(); i++)
				{
					pickedNeighbors[i]->cube.modelMatrix = glm::rotate(glm::radians(static_cast<float>(direction.prefix * xoffset * Time->deltaTime*speed)), direction.axis) * pickedNeighbors[i]->cube.modelMatrix;
				}

			}
			else if (directionY)
			{
				turn = true;
				double yoffset = lastY - ypos;
				lastY = ypos;
				double speed = 30.0;

				anglesum += direction.prefix * yoffset * Time->deltaTime * speed;
				for (int i = 0; i < pickedNeighbors.size(); i++)
				{
					pickedNeighbors[i]->cube.modelMatrix = glm::rotate(glm::radians(static_cast<float>(direction.prefix * yoffset * Time->deltaTime * speed)), direction.axis) * pickedNeighbors[i]->cube.modelMatrix;
					
				}
			}
			
		}
		
	
	}
	else if (!firstRightPress &&glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{

		if (pickedObj != nullptr)
		{
			float snappedAngle = snapToNearest90(anglesum);
			float neededRotation = snappedAngle - anglesum;
			glm::vec3 rotationAxis;
			rotationAxis = direction.axis;


			for (Cubie* obj : pickedNeighbors)
			{	
				obj->cube.modelMatrix = glm::rotate(glm::radians(neededRotation), rotationAxis)*obj->cube.modelMatrix;
			}
		}

		directionX = false;
		directionY = false;
		firstRightPress = true;
		anglesum = 0;
		Check = true;
		turn = false;
	}
}
float snapToNearest90(float angle) {
	return round(angle / 90.0f) * 90.0f;
}

std::vector<int> GetFaceDirs(Cubie& pickedObj)
{
	std::vector<int> faceDirs;
	glm::vec3 objCenter = pickedObj.cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	if (objCenter.x < 0) faceDirs.push_back(1);
	else if (objCenter.x > 0) faceDirs.push_back(2);

	if (objCenter.y < 0) faceDirs.push_back(3);
	else if (objCenter.y > 0) faceDirs.push_back(4);

	if (objCenter.z < 0) faceDirs.push_back(5);
	else if (objCenter.z > 0) faceDirs.push_back(6);

	return faceDirs;
}

Cubie* getCubieByID(int Id, std::vector<Cubie>& rubik)
{
	for (Cubie& cubie : rubik)
	{
		if (cubie.cube.id == Id)
		{
			return &cubie;
		}
	}
	return nullptr;
}


void GetObjectAABB(Object& obj, glm::vec3& min, glm::vec3& max)
{
	glm::vec3 objCenter = obj.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	min = objCenter - glm::vec3(0.5f);
	max = objCenter + glm::vec3(0.5f);
}

bool hitCube(Object& obj, glm::vec3 rayPos, glm::vec3 rayDir,float& distance) {
	glm::vec3 minAABB = glm::vec3(0.0f);
	glm::vec3 maxAABB = glm::vec3(0.0f);
	GetObjectAABB(obj, minAABB, maxAABB);

	float tminx = (minAABB.x - rayPos.x) / rayDir.x;
	float tmaxx = (maxAABB.x - rayPos.x) / rayDir.x;
	if (rayDir.x < 0) std::swap(tminx, tmaxx);

	float tminy = (minAABB.y - rayPos.y) / rayDir.y;
	float tmaxy = (maxAABB.y - rayPos.y) / rayDir.y;
	if (rayDir.y < 0) std::swap(tminy, tmaxy);

	float tminz = (minAABB.z - rayPos.z) / rayDir.z;
	float tmaxz = (maxAABB.z - rayPos.z) / rayDir.z;
	if (rayDir.z<0) std::swap(tminz, tmaxz);

	float tmin = std::max({tminx,tminy,tminz});
	float tmax = std::min({tmaxx,tmaxy,tmaxz});
	
	if (tmin > tmax || tmax < 0)return false;
	distance = tmin;
	return true;
	
}

Cubie* pickedCube(glm::vec3 rayPos, glm::vec3 rayDir, std::vector<Cubie>& rubik) {
	float distance = FLT_MAX;
	Cubie* pickedCube =nullptr;
	for (int i = 0; i < rubik.size(); i++) {
		float tempDistance;
		if (hitCube(rubik[i].cube, rayPos, rayDir, tempDistance)) {
			if (tempDistance < distance) {
				distance = tempDistance;
				pickedCube = &rubik[i];
			}
		}
	}
	return pickedCube;
}

std::vector<Cubie*> getPickedObjectNeighbors(Object* pickedObj, std::vector<Cubie>& rubik, Direction& direction, bool horizontalMove) {
	std::vector<Cubie*> neighbors;
	glm::vec3 pickedCenter = pickedObj->modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	
	switch (direction.hitDirection)
	{
	case 1:		//Left
		if (horizontalMove) {
			for (int i = 0; i < rubik.size(); i++) {

				glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				if (fabsf(objCenter.y - pickedCenter.y) < 0.05) {

					neighbors.push_back(&rubik[i]);
					
				}
			}
			direction.axis = glm::vec3(0.0f, 1.0f, 0.0f);
			direction.prefix = 1;
		}
		else {
			for (int i = 0; i < rubik.size(); i++) {
				glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				if (fabsf(objCenter.z - pickedCenter.z) < 0.05) {

					neighbors.push_back(&rubik[i]);
				
				}
			}
			direction.axis = glm::vec3(0.0f, 0.0f, 1.0f);
			direction.prefix = -1;
		}
		
		break;
	case 2:		//Right
		if (horizontalMove) {
			for (int i = 0; i < rubik.size(); i++) {

				glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				if (fabsf(objCenter.y - pickedCenter.y) < 0.05) {

					neighbors.push_back(&rubik[i]);
					
				}
			}
			direction.axis = glm::vec3(0.0f, 1.0f, 0.0f);
			direction.prefix = 1;
		}
		else {
			for (int i = 0; i < rubik.size(); i++) {
				glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				if (fabsf(objCenter.z - pickedCenter.z) < 0.05) {

					neighbors.push_back(&rubik[i]);
					
				}
			}
			direction.axis = glm::vec3(0.0f, 0.0f, 1.0f);
			direction.prefix = 1;
		}
		break;
	case 3:		//Bottom
		if (horizontalMove) {
			if (direction.viewDirection == 1 || direction.viewDirection == 2) {
				for (int i = 0; i < rubik.size(); i++) {

					glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					if (fabsf(objCenter.x - pickedCenter.x) < 0.05) {

						neighbors.push_back(&rubik[i]);
						
					}
				}
				direction.axis = glm::vec3(1.0f, 0.0f, 0.0f);
				direction.prefix = direction.viewDirection==1 ? 1:-1 ;
			}
			else {
				for (int i = 0; i < rubik.size(); i++) {

					glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					if (fabsf(objCenter.z - pickedCenter.z) < 0.05) {

						neighbors.push_back(&rubik[i]);
						
					}
				}
				direction.axis = glm::vec3(0.0f, 0.0f, 1.0f);
				direction.prefix = direction.viewDirection == 4 ? -1 : 1;
			}
		}
		else {
			if (direction.viewDirection == 1 || direction.viewDirection == 2) {
				for (int i = 0; i < rubik.size(); i++) {

					glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					if (fabsf(objCenter.z - pickedCenter.z) < 0.05) {

						neighbors.push_back(&rubik[i]);
						
					}
				}
				direction.axis = glm::vec3(0.0f, 0.0f, 1.0f);
				direction.prefix = direction.viewDirection == 1 ? 1 : -1;
			}
			else {
				for (int i = 0; i < rubik.size(); i++) {

					glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					if (fabsf(objCenter.x - pickedCenter.x) < 0.05) {

						neighbors.push_back(&rubik[i]);
						
					}
				}
				direction.axis = glm::vec3(1.0f, 0.0f, 0.0f);
				direction.prefix = direction.viewDirection == 4 ? 1 : -1;
			}
		}
		break;
	case 4:		//Top
		if (horizontalMove) {
			if (direction.viewDirection == 1 || direction.viewDirection == 2) {
				for (int i = 0; i < rubik.size(); i++) {

					glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					if (fabsf(objCenter.x - pickedCenter.x) < 0.05) {

						neighbors.push_back(&rubik[i]);
						
					}
				}
				direction.axis = glm::vec3(1.0f, 0.0f, 0.0f);
				direction.prefix = direction.viewDirection == 1 ? -1 : 1;
			}
			else {
				for (int i = 0; i < rubik.size(); i++) {

					glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					if (fabsf(objCenter.z - pickedCenter.z) < 0.05) {

						neighbors.push_back(&rubik[i]);
						
					}
				}
				direction.axis = glm::vec3(0.0f, 0.0f, 1.0f);
				direction.prefix = direction.viewDirection == 4 ? 1 : -1;
			}	
		}
		else {
			if (direction.viewDirection == 1 || direction.viewDirection == 2) {
				for (int i = 0; i < rubik.size(); i++) {

					glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					if (fabsf(objCenter.z - pickedCenter.z) < 0.05) {

						neighbors.push_back(&rubik[i]);
						
					}
				}
				direction.axis = glm::vec3(0.0f, 0.0f, 1.0f);
				direction.prefix = direction.viewDirection == 1 ? 1 : -1;
			}
			else {
				for (int i = 0; i < rubik.size(); i++) {

					glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					if (fabsf(objCenter.x - pickedCenter.x) < 0.05) {

						neighbors.push_back(&rubik[i]);
						
					}
				}
				direction.axis = glm::vec3(1.0f, 0.0f, 0.0f);
				direction.prefix = direction.viewDirection == 4 ? 1 : -1;
			}
		}
		break;
	case 5:		//Back
		if (horizontalMove) {
			for (int i = 0; i < rubik.size(); i++) {

				glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				if (fabsf(objCenter.y - pickedCenter.y) < 0.05) {

					neighbors.push_back(&rubik[i]);
					
				}
			}
			direction.axis = glm::vec3(0.0f, 1.0f, 0.0f);
			direction.prefix = 1;
		}
		else {
			for (int i = 0; i < rubik.size(); i++) {
				glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				if (fabsf(objCenter.x - pickedCenter.x) < 0.05) {

					neighbors.push_back(&rubik[i]);
					
				}
			}
			direction.axis = glm::vec3(1.0f, 0.0f, 0.0f);
			direction.prefix = 1;
		}
		break;
	case 6:		//Front
		if (horizontalMove) {
			for (int i = 0; i < rubik.size(); i++) {

				glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				if (fabsf(objCenter.y - pickedCenter.y) < 0.05) {

					neighbors.push_back(&rubik[i]);
					
				}
			}
			direction.axis = glm::vec3(0.0f, 1.0f, 0.0f);
			direction.prefix = 1;
		}
		else {
			for (int i = 0; i < rubik.size(); i++) {
				glm::vec3 objCenter = rubik[i].cube.modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				if (fabsf(objCenter.x - pickedCenter.x) < 0.05) {

					neighbors.push_back(&rubik[i]);
					
				}
			}
			direction.axis = glm::vec3(1.0f, 0.0f, 0.0f);
			direction.prefix = -1;
		}
		break;
	default:
		break;
	}
	
	if (direction.axis.x != 0 && fabs(pickedCenter.x) < 0.1) {
		return {};
	}
	if (direction.axis.z != 0 && fabs(pickedCenter.z) < 0.1) {
		return {};
	}
	if (direction.axis.y != 0 && fabs(pickedCenter.y) < 0.1) {
		return {};
	}
	return neighbors;
}

int getHitFaceint(Object& obj, glm::vec3 rayPos, glm::vec3 rayDir) {
	glm::vec3 minAABB, maxAABB;
	GetObjectAABB(obj, minAABB, maxAABB);

	float tmin;
	if (!hitCube(obj, rayPos, rayDir, tmin)) {
		return -1;
	}

	glm::vec3 hitPoint = rayPos + tmin * rayDir;

	float epsilon = 0.01f;

	if (glm::abs(hitPoint.x - minAABB.x) < epsilon) return 1;		//Left
	if (glm::abs(hitPoint.x - maxAABB.x) < epsilon) return 2;		//Right
	if (glm::abs(hitPoint.y - minAABB.y) < epsilon) return 3;		//Bottom
	if (glm::abs(hitPoint.y - maxAABB.y) < epsilon) return 4;		//Top
	if (glm::abs(hitPoint.z - minAABB.z) < epsilon) return 5;		//Back
	if (glm::abs(hitPoint.z - maxAABB.z) < epsilon) return 6;		//Front

	return -1;
}
int getViewDirection(const glm::vec3& rayDir) {
	glm::vec3 absDir = glm::abs(rayDir); 

	if (absDir.x > absDir.z) {
		return (rayDir.x > 0) ? 2 : 1;	//Left, Right
	}
	else {
		return (rayDir.z > 0) ? 4 : 5;	//Front, Back
	}
}



