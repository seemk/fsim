#pragma once
#include <vector>
#include <memory>
#include <initializer_list>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace GL
{
	class Shader;

	enum ProgramType
	{
		Default, // The default shader which just colours the vertices
		ColorDefault,
		Blur,
		Fluid,

		COUNT
	};

	class Program
	{
	public:

		Program(const std::vector<Shader>& shaders);
		Program(std::initializer_list<std::shared_ptr<GL::Shader>> shaders);
		Program();

		static std::shared_ptr<Program> create(const std::vector<Shader>& shaders);

		void setSharedUniformBlockIndex(const std::string& uniformBlockName);
		GLuint getSharedUniformBlockIndex() const;

		GLint getUniformLocation(const std::string& location) const;
		GLint activeUniformCount() const;

		void setUniformValue(GLint location, const glm::vec4& vector) const;
		void setUniformValue(GLint location, GLint value) const;

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