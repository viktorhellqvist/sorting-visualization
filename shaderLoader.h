#ifndef SHADER_LOADER_H_
#define SHADER_LOADER_H_

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    Shader& use();

private:
    void Compile(const char* vertexSource, const char* fragmentSource);
    // checks if compilation or linking failed and if so, print the error logs
    void checkCompileErrors(unsigned int object, std::string type);
};

#endif /* SHADER_LOADER_H_ */