#include <CanvasShader.hpp>

CanvasShader::CanvasShader()
{
    const char* cstr_fragmentShader = _fragmentShaderSource.c_str();
    const char* cstr_vertexShader = _vertexShaderSource.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &cstr_vertexShader, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &cstr_fragmentShader, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    _glShaderProgramId = glCreateProgram();

    glAttachShader(_glShaderProgramId, vertexShader);
    glAttachShader(_glShaderProgramId, fragmentShader);
    glLinkProgram(_glShaderProgramId);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

CanvasShader::~CanvasShader()
{
    glDeleteProgram(_glShaderProgramId);
}

void CanvasShader::Use()
{
    glUseProgram(_glShaderProgramId);
}

void CanvasShader::UseWithOrtho(const glm::mat4& ortho)
{
    glUseProgram(_glShaderProgramId);
    SetOrthoTransform(ortho);
}

void CanvasShader::SetOrthoTransform(const glm::mat4& ortho)
{
    unsigned int orthoLocation = glGetUniformLocation(_glShaderProgramId, "ortho");
    glUniformMatrix4fv(orthoLocation, 1, GL_FALSE, glm::value_ptr(ortho));
}