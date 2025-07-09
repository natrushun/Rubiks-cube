#pragma once
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <unordered_map>
#include <iostream>
#include <map>
#include <GLFW/glfw3.h> 


struct TimeInfo
{
    float elapsedTime = 0.0f;  
    float deltaTime = 0.0f;    
};
enum class Color {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    ORANGE,
    WHITE,
    BLACK
};


struct Vertex {
    std::vector<glm::vec3> position;
    std::vector<glm::vec3> color;
    std::vector<glm::vec2> uv;
};

struct Object {
    GLuint id;
    GLuint count;
    Vertex vertex;
    std::vector<GLuint> indexArray;
    std::vector<Color> faceColors;
    GLuint vertexVboID = 0;
    GLuint colorVboID = 0;
	GLuint uvVboID = 0;
    GLuint indexVboID = 0;
    GLuint vaoID = 0;
    glm::mat4 modelMatrix;
};

struct Direction {
    int hitDirection;
    int viewDirection;
    int prefix;
    glm::vec3 axis;
};



struct Cubie {
    Object cube;
    std::vector<std::tuple<Color,glm::vec3,int>> colors;
};


static std::unordered_map<Color, glm::vec3> ColorMap = {
    {Color::RED, glm::vec3(1.0f, 0.0f, 0.0f)},
    {Color::GREEN, glm::vec3(0.0f, 1.0f, 0.0f)},
    {Color::BLUE, glm::vec3(0.0f, 0.0f, 0.804f)},
    {Color::YELLOW, glm::vec3(1.0f, 1.0f, 0.0f)},
    {Color::ORANGE, glm::vec3(1.0f, 0.271f, 0.0f)},
    {Color::WHITE, glm::vec3(1.0f, 1.0f, 1.0f)},
    {Color::BLACK, glm::vec3(0.0f, 0.0f, 0.0f)}
};

inline glm::vec3 getColor(Color color) {
    return ColorMap.at(color);
}

GLFWwindow* BaseInit(int width, int height);

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

Object InitObject(std::vector<Color> colors);

void Destructor(Object& obj);

void RefreshTime(TimeInfo* Time);

void ColorInit(Object& obj, std::vector<Color> colors);

void DrawObject(Object& obj, glm::mat4 world, GLuint programID);

void GetObjectAABB(Object& obj, glm::vec3& min, glm::vec3& max);

bool hitCube(Object& obj, glm::vec3 rayPos, glm::vec3 rayDir, float& distance);

Cubie* pickedCube(glm::vec3 rayPos, glm::vec3 rayDir, std::vector<Cubie>& rubik);

void moveCubes(TimeInfo* Time, GLFWwindow* window, int height, int width, glm::mat4 viewProj, glm::vec3& rayPos, glm::vec3& rayDir, const glm::vec3 eye, std::vector<Cubie>& rubik, bool& Check,bool& turn);

std::vector<Cubie*> getPickedObjectNeighbors(Object* pickedObj, std::vector<Cubie>& rubik, Direction& direction, bool horizontalMove);

int getHitFaceint(Object& obj, glm::vec3 rayPos, glm::vec3 rayDir);

int getViewDirection(const glm::vec3& rayDir);

std::vector<Cubie> InitRubik();

float snapToNearest90(float angle);

std::vector<int> GetFaceDirs(Cubie& pickedObj);

Cubie* getCubieByID(int Id, std::vector<Cubie>& rubik);

void SetImguiStyle();

