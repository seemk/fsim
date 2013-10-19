#pragma once
#include <string>
#include <memory>
#include <GL\glew.h>

namespace GL
{
	class Shader
	{
	public:

		enum Type
		{
			Vertex = GL_VERTEX_SHADER,
			Fragment = GL_FRAGMENT_SHADER,
			Geometry = GL_GEOMETRY_SHADER
		};

		static std::shared_ptr<Shader> create(Type type, const std::string& shaderSource);

		~Shader();
		
		Shader() = delete;
		Shader(const Shader& o)	= delete;
		Shader& operator=(const Shader& o) = delete;

		std::string getInfo()	const;
		GLuint		getID()		const;
		Shader(Type type, const std::string& shaderSource);
	private:

		
		GLuint shader;
		Type type;

	};
}