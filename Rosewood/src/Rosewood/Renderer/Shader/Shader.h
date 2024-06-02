#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Rosewood
{
    class Shader
    {
    public:
        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);

        ~Shader();

        // use/activate the shader
        void Bind();

        void Unbind();

        // utility uniform functions
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec3(const std::string& name, glm::vec3 value) const;
        void SetMat4(const std::string& name, glm::mat4 value) const;
    private:
        // the program ID
        unsigned int m_RendererID;
    };
}