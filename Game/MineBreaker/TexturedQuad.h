#ifndef _TEXTURED_QUAD_INCLUDE
#define _TEXTURED_QUAD_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

namespace game
{
	namespace gui
	{

		class TexturedQuad
		{

		public:
			// Textured quads can only be created inside an OpenGL context
			static TexturedQuad *createTexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], visuals::ShaderProgram &program);

			TexturedQuad(glm::vec2 geom[2], glm::vec2 texCoords[2], visuals::ShaderProgram &program);

			void render(const visuals::Texture &tex) const;
			void free();

		private:
			GLuint vao;
			GLuint vbo;
			GLint posLocation, texCoordLocation;

		};
	}
}

#endif // _TEXTURED_QUAD_INCLUDE

