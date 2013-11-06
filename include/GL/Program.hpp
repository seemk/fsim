#pragma once
#include <vector>
#include <memory>
#include <initializer_list>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace GL
{
	class Shader;
	class Program
	{
	public:

		Program(const std::vector<Shader>& shaders);
		Program(std::initializer_list<std::shared_ptr<GL::Shader>> shaders);
		Program();

		static std::shared_ptr<Program> create(const std::vector<Shader>& shaders);

		void setSharedUniformBlockIndex(const std::string& uniformBlockName);
		GLuint getSharedUniformBlockIndex() const;

		GLuint getUniformLocation(const std::string& location) const;

		void setUniformValue(GLuint location, const glm::vec4& vector) const;

		void destroy();
		bool initialized() const;
		void use() const { glUseProgram(program); }
		void unuse() const { glUseProgram(0); }
		std::string getInfo() const;
		GLuint getID() const;


	private:

		GLuint program;
		GLuint sharedUniformsBlockIndex;
	};
}