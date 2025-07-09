#pragma once
#include <exception>
#include <string>

class GLFWInitException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Failed to initialize GLFW";
    }
};

class GLEWInitException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Failed to initialize GLEW";
    }
};

class GLFWWindowException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Failed to create GLFW window";
    }
};
