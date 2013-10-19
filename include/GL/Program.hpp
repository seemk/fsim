#pragma once
#include <vector>
#include <memory>
#include <initializer_list>
#include <GL\glew.h>

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

		GLuint getMVPUniform() const;
		void setMVPUniform(GLuint location);
		void destroy();
		bool initialized() const;
		void use() const;
		std::string getInfo() const;
		GLuint getID() const;


	private:
		
		GLuint mvpUniformLocation;
		GLuint program;

	};
}