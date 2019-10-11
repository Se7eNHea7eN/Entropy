#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
    {

		FILE* fp = fopen(vertexPath, "rb");
		fseek(fp, 0, SEEK_END);
		GLint vertexLen = (GLint)ftell(fp);
		unsigned char* vertexSpirv = new unsigned char[vertexLen];
		fseek(fp, 0, SEEK_SET);
		fread(vertexSpirv, vertexLen, 1, fp);
		fclose(fp);


		fp = fopen(fragmentPath, "rb");
		fseek(fp, 0, SEEK_END);
		GLint fragmentLen = (GLint)ftell(fp);
		unsigned char* fragmentSpirv = new unsigned char[fragmentLen];
		fseek(fp, 0, SEEK_SET);
		fread(fragmentSpirv, fragmentLen, 1, fp);
		fclose(fp);
  //   	
		// // Read our shaders into the appropriate buffers
		// std::vector<unsigned char> vertexSpirv = // Get SPIR-V for vertex shader.
		// 	std::vector<unsigned char> fragmentSpirv = // Get SPIR-V for fragment shader.

			// Create an empty vertex shader handle
			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Apply the vertex shader SPIR-V to the shader object.
		glShaderBinary(1, &vertexShader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertexSpirv, vertexLen);

		// Specialize the vertex shader.
		std::string vsEntrypoint = "main"; // Get VS entry point name
		glSpecializeShader(vertexShader, (const GLchar*)vsEntrypoint.c_str(), 0, nullptr, nullptr);

		// Specialization is equivalent to compilation.
		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}



		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Apply the fragment shader SPIR-V to the shader object.
		glShaderBinary(1, &fragmentShader, GL_SHADER_BINARY_FORMAT_SPIR_V, fragmentSpirv,fragmentLen);

		// Specialize the fragment shader.
		std::string fsEntrypoint = "main"; //Get VS entry point name
		glSpecializeShader(fragmentShader, (const GLchar*)fsEntrypoint.c_str(), 0, nullptr, nullptr);

		// Specialization is equivalent to compilation.
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		ID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);

		// Link our program
		glLinkProgram(ID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(ID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(ID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(ID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(ID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Use the infoLog as you see fit.

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(ID, vertexShader);
		glDetachShader(ID, fragmentShader);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
        glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    // void setVec2(const std::string &name, const glm::vec2 &value) const
    // { 
    //     glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    // }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y); 
    }
    // ------------------------------------------------------------------------
    // void setVec3(const std::string &name, const glm::vec3 &value) const
    // { 
    //     glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    // }
    // void setVec3(const std::string &name, float x, float y, float z) const
    // { 
    //     glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    // }
    // // ------------------------------------------------------------------------
    // void setVec4(const std::string &name, const glm::vec4 &value) const
    // { 
    //     glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
    // }
    // void setVec4(const std::string &name, float x, float y, float z, float w) 
    // { 
    //     glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w); 
    // }
    // // ------------------------------------------------------------------------
    // void setMat2(const std::string &name, const glm::mat2 &mat) const
    // {
    //     glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    // }
    // // ------------------------------------------------------------------------
    // void setMat3(const std::string &name, const glm::mat3 &mat) const
    // {
    //     glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    // }
    // // ------------------------------------------------------------------------
    // void setMat4(const std::string &name, const glm::mat4 &mat) const
    // {
    //     glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    // }

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(GLuint shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif