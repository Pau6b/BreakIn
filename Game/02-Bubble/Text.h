#pragma once


#include <ft2build.h>
#include FT_FREETYPE_H
#include "Texture.h"
#include "ShaderProgram.h"
#include <string>
#include <vector>
#include "TexturedQuad.h"

#define NUM_CHARACTERS (128 - 32)


// Using Freetype the Text class is capable of building a texture atlas
// for a given font and size. Combining with TexturedQuad allows it 
// to render text strings

namespace game 
{
	namespace gui
	{
		class Text
		{

		public:
			Text(const char *i_filename, int i_size);
			~Text();

			void destroy();

			visuals::ShaderProgram &getProgram();
			void SetSize(int32_t i_size);
			int getFontSize() const;
			void render(char c, const glm::vec2 &pixel, const glm::vec4 &color);
			void RenderString(const std::string &str, const glm::vec2 &pixel, const glm::vec4 &color);
			void UpdateCoins();
			void UpdatePoints();
			void render();

			void linkStr(std::string i_text, uint32_t* value);

		private:
			void initShaders();
			bool extractCharSizes(int *maxCharWidth, int *maxCharHeight);
			void createTextureAtlas();

		private:
			struct CharMetrics
			{
				int tx, ty; 		// Pos x, y inside texture in pixels
				int sx, sy; 		// Size of char in pixels
				int ax, ay; 		// Advance for each character in a string sequence
				int bl, bt; 		// Bitmap left and top adjusting
			};
			int m_fontSize, m_textureSize, m_maxCharWidth, m_maxCharHeight;
			FT_Face m_face;
			CharMetrics m_chars[NUM_CHARACTERS];
			visuals::Texture n_textureAtlas;
			visuals::ShaderProgram m_program;
			TexturedQuad *m_quad;
			int m_size;
			static bool m_bLibInit;
			static FT_Library m_library;
			uint32_t m_points;
			uint32_t m_coins;
			std::vector<std::pair<std::string, uint32_t*>> m_texts;

		};
	}

}

