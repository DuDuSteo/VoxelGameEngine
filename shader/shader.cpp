#include "shader.hpp"

void Shader::Init(const std::string &vertFileName, const std::string &fragFileName)
{
    std::ifstream vertFile(std::string(FILES_PATH) + vertFileName + GLSL_VERTEX_FILE_EXTENSION);
    std::ifstream fragFile(std::string(FILES_PATH) + fragFileName + GLSL_FRAGMENT_FILE_EXTENSION);

    if (vertFile.bad() || fragFile.bad())
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;

    std::stringstream vertSStream, fragSStream;

    vertSStream << vertFile.rdbuf();
    fragSStream << fragFile.rdbuf();

    vertFile.close();
    fragFile.close();

    std::string vertexCode = vertSStream.str();
    std::string fragmentCode = fragSStream.str();

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    uint32_t vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vShaderCode, NULL);
    glCompileShader(vert);
    checkCompileErrors(vert, "VERTEX");

    uint32_t frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fShaderCode, NULL);
    glCompileShader(frag);
    checkCompileErrors(frag, "FRAGMENT");

    shaderID = glCreateProgram();

    glAttachShader(shaderID, vert);
    glAttachShader(shaderID, frag);

    glLinkProgram(shaderID);
    checkCompileErrors(shaderID, "PROGRAM");

    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::Use()
{
    glUseProgram(shaderID);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &vec[0]);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &vec) const
{
    glUniform4fv(glGetUniformLocation(shaderID, name.c_str()), 1, &vec[0]);
}

void Shader::SetFloat(const std::string &name, const float &value) const
{
    glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::checkCompileErrors(uint32_t shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << std::endl
                      << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << std::endl
                      << infoLog << std::endl;
        }
    }
}
