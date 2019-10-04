#include "../header/Shader.h"


Shader::Shader(ShaderTypes vertex_shader_type, ShaderTypes fragment_shader_type)
{
    unsigned int vertex_shader;
    unsigned int fragment_shader;

    // Compile requested shaders
    switch (vertex_shader_type)
    {
    case ShaderTypes::NOTEXTURE_SHADER:
        vertex_shader = Shader::compileVertexShader_(no_texture_vertex_shader_source);
        break;
    case ShaderTypes::TEXTURE_SHADER:
        vertex_shader = Shader::compileVertexShader_(texture_vertex_shader_source);
        break;
    case ShaderTypes::FACEIDX_SHADER:
        vertex_shader = Shader::compileVertexShader_(face_idx_vertex_shader_source);
        break;
    case ShaderTypes::FLAT_SHADER:
        vertex_shader = Shader::compileVertexShader_(flat_vertex_shader_source);
        break;
    case ShaderTypes::DEFAULT_SHADER:
        vertex_shader = Shader::compileVertexShader_(default_vertex_shader_source_);
        break;
    }

    switch (fragment_shader_type)
    {
    case ShaderTypes::NOTEXTURE_SHADER:
        fragment_shader = Shader::compileFragmentShader_(no_texture_fragment_shader_source);
        break;
    case ShaderTypes::TEXTURE_SHADER:
        fragment_shader = Shader::compileFragmentShader_(texture_fragment_shader_source);
        break;
    case ShaderTypes::FACEIDX_SHADER:
        fragment_shader = Shader::compileFragmentShader_(face_idx_fragment_shader_source);
        break;
    case ShaderTypes::FLAT_SHADER:
        fragment_shader = Shader::compileFragmentShader_(flat_fragment_shader_source);
        break;
    case ShaderTypes::DEFAULT_SHADER:
        fragment_shader = Shader::compileFragmentShader_(default_fragment_shader_source_);
        break;
    }

    // create program
    createProgram_(vertex_shader, fragment_shader);

    // cleanup
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::Shader(const GLchar * vertex_path, const GLchar * fragment_path)
{
    unsigned int vertex_shader;
    unsigned int fragment_shader;
    // read & compile
    if (vertex_path != nullptr)
    {
        std::string vertex_code = Shader::readCodeFile_(vertex_path);
        vertex_shader = Shader::compileVertexShader_(vertex_code.c_str());
    }
    else
    {
        vertex_shader = Shader::compileVertexShader_(default_vertex_shader_source_);
    }
    
    if (fragment_path != nullptr)
    {
        std::string fragment_code = Shader::readCodeFile_(fragment_path);
        fragment_shader = Shader::compileFragmentShader_(fragment_code.c_str());
    }
    else
    {
        fragment_shader = Shader::compileFragmentShader_(default_fragment_shader_source_);
    }
    
    // create program
    createProgram_(vertex_shader, fragment_shader);

    // cleanup
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader()
{
    glDeleteProgram(ID_);
}

void Shader::use()
{
    glUseProgram(ID_);
}

void Shader::setUniform(const std::string & name, int value) const
{
    int location = glGetUniformLocation(ID_, name.c_str());
    glUniform1i(location, value);
}

void Shader::setUniform(const std::string & name, float value) const
{
    int location = glGetUniformLocation(ID_, name.c_str());
    glUniform1f(location, value);
}

void Shader::setUniform(const std::string & name, glm::mat4 value) const
{
    int location = glGetUniformLocation(ID_, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string & name, glm::vec3 value) const
{
    int location = glGetUniformLocation(ID_, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void Shader::setUniform(const std::string & name, glm::vec4 value) const
{
    int location = glGetUniformLocation(ID_, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(value));
}

void Shader::createProgram_(unsigned int vertex_shader, unsigned int fragment_shader)
{
    // link shaders
    ID_ = glCreateProgram();
    glAttachShader(ID_, vertex_shader);
    glAttachShader(ID_, fragment_shader);
    glLinkProgram(ID_);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(ID_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID_, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
}

std::string Shader::readCodeFile_(const GLchar * path)
{
    std::string code;
    std::ifstream file;

    // allow throuwing exeptions
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        code = stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE NOT SUCCESFULLY READ" << std::endl;
    }
    return code;
}

unsigned int Shader::compileVertexShader_(const char * shader_code)
{
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader_id, 1, &shader_code, NULL);
    glCompileShader(vertex_shader_id);

    // check if compiled ok
    int  success;
    char info_log[512];
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        // info before dying young 
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
        return 0;
    }

    return vertex_shader_id;
}

unsigned int Shader::compileFragmentShader_(const char * shader_code)
{
    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment_shader_id, 1, &shader_code, NULL);
    glCompileShader(fragment_shader_id);

    // check if compiled ok
    int  success;
    char info_log[512];
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        // info before dying young 
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        return 0;
    }

    return fragment_shader_id;
}
