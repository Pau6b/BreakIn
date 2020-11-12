#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Text.h"
#include "Log.h"
#include <algorithm>


#define ATLAS_FONT_SIZE 64
#define INIT_TEXT 456

namespace game
{
	namespace gui
	{


		bool Text::m_bLibInit = false;
		FT_Library Text::m_library;


		Text::Text(const char *filename, int i_size)
			:m_size(i_size)
			,m_coins(0)
			,m_points(0)
		{
			m_texts.push_back(std::make_pair("COINS: ", &m_coins));
			m_texts.push_back(std::make_pair("POINTS: ", &m_points));
			m_quad = NULL;
			FT_Error error;

			if (!m_bLibInit)
			{
				error = FT_Init_FreeType(&Text::m_library);
				BreakIf(error, "Error Init FreeType");
				m_bLibInit = true;
			}
			error = FT_New_Face(Text::m_library, filename, 0, &m_face);
			BreakIf(error, "Error Init New Face");
			FT_Set_Pixel_Sizes(m_face, ATLAS_FONT_SIZE, ATLAS_FONT_SIZE);

			BreakIf(!extractCharSizes(&m_maxCharWidth, &m_maxCharHeight), "No size found");
			m_fontSize = m_maxCharHeight;
			m_textureSize = 256;
			if (floor(float(m_textureSize) / m_fontSize) * floor(float(m_textureSize) / m_fontSize) < (128 - 32))
				m_textureSize = 512;
			if (floor(float(m_textureSize) / m_fontSize) * floor(float(m_textureSize) / m_fontSize) < (128 - 32))
				BreakIf(true, "Error Init New Face");
			createTextureAtlas();
			initShaders();

			glm::vec2 geom[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
			glm::vec2 texCoords[2] = { glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };

			m_quad = TexturedQuad::createTexturedQuad(geom, texCoords, m_program);
		}

		Text::~Text()
		{
			destroy();
			if (m_quad != NULL)
			{
				m_quad->free();
				delete m_quad;
			}
		}

		void Text::destroy()
		{
			FT_Done_Face(m_face);
		}

		visuals::ShaderProgram &Text::getProgram()
		{
			return m_program;
		}

		void Text::SetSize(int32_t i_size)
		{
			m_size = i_size;
		}

		int Text::getFontSize() const
		{
			return m_fontSize;
		}

		void Text::render(char c, const glm::vec2 &pixel, const glm::vec4 &color)
		{
			int vp[4];
			glm::mat4 projection, modelview;
			glm::vec2 minTexCoord, maxTexCoord;

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			m_program.use();
			glGetIntegerv(GL_VIEWPORT, vp);
			projection = glm::ortho(0.f, float(vp[2] - 1), float(vp[3] - 1), 0.f);
			m_program.setUniformMatrix4f("projection", projection);
			m_program.setUniform4f("color", color.r, color.g, color.b, color.a);
			modelview = glm::mat4(1.0f);
			modelview = glm::translate(modelview, glm::vec3(pixel.x, pixel.y - m_size, 0.f));
			modelview = glm::scale(modelview, (float(m_size) / m_fontSize) * glm::vec3(m_chars[c - 32].sx, m_chars[c - 32].sy, 0.f));
			m_program.setUniformMatrix4f("modelview", modelview);
			minTexCoord = glm::vec2(float(m_chars[c - 32].tx) / m_textureSize, float(m_chars[c - 32].ty) / m_textureSize);
			maxTexCoord = glm::vec2(float(m_chars[c - 32].tx + m_chars[c - 32].sx) / m_textureSize, float(m_chars[c - 32].ty + m_chars[c - 32].sy) / m_textureSize);
			m_program.setUniform2f("minTexCoord", minTexCoord.s, minTexCoord.t);
			m_program.setUniform2f("maxTexCoord", maxTexCoord.s, maxTexCoord.t);
			m_quad->render(m_textureAtlas);
			glDisable(GL_BLEND);
		}


		void Text::render()
		{
			int n = 0;
			std::for_each(std::begin(m_texts), std::end(m_texts), [&n, this](auto it) {auto number = (it.first.find("ROOM") != std::string::npos) ? *it.second + 1 : *it.second;
																				  RenderString(it.first + std::to_string(number),
																				  glm::vec2(INIT_TEXT, 80 + 65 * n),
																				  glm::vec4(1, 1, 1, 1));
																				  ++n; });
		}


		uint32_t Text::GetCurrentPoints() const
		{
			return m_coins;
		}

		void Text::linkStr(std::string i_text, uint32_t* value)
		{
			m_texts.push_back(std::make_pair(i_text, value));
		}

		void Text::RenderString(const std::string &str, const glm::vec2 &pixel, const glm::vec4 &color)
		{
			int vp[4];
			glm::mat4 projection, modelview;
			glm::vec2 minTexCoord, maxTexCoord, pos = pixel;

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_BLEND);
			m_program.use();
			glGetIntegerv(GL_VIEWPORT, vp);
			projection = glm::ortho(0.f, float(vp[2] - 1), float(vp[3] - 1), 0.f);
			m_program.setUniformMatrix4f("projection", projection);
			m_program.setUniform4f("color", color.r, color.g, color.b, color.a);

			for (unsigned int i = 0; i < str.length(); i++)
			{
				modelview = glm::mat4(1.0f);
				modelview = glm::translate(modelview, glm::vec3(pos.x + (float(m_size) / m_fontSize) * m_chars[str[i] - 32].bl, pos.y - (float(m_size) / m_fontSize) * m_chars[str[i] - 32].bt, 0.f));
				modelview = glm::scale(modelview, (float(m_size) / m_fontSize) * glm::vec3(m_chars[str[i] - 32].sx, m_chars[str[i] - 32].sy, 0.f));
				m_program.setUniformMatrix4f("modelview", modelview);
				minTexCoord = glm::vec2(float(m_chars[str[i] - 32].tx) / m_textureSize, float(m_chars[str[i] - 32].ty) / m_textureSize);
				maxTexCoord = glm::vec2(float(m_chars[str[i] - 32].tx + m_chars[str[i] - 32].sx) / m_textureSize, float(m_chars[str[i] - 32].ty + m_chars[str[i] - 32].sy) / m_textureSize);
				m_program.setUniform2f("minTexCoord", minTexCoord.s, minTexCoord.t);
				m_program.setUniform2f("maxTexCoord", maxTexCoord.s, maxTexCoord.t);
				m_quad->render(m_textureAtlas);
				pos.x += (float(m_size) / m_fontSize) * m_chars[str[i] - 32].ax;
			}

			glDisable(GL_BLEND);
		}


		void Text::UpdateCoins()
		{
			m_coins = m_coins + 50;
		}


		void Text::UpdatePoints()
		{
			m_points = m_points + 100;
		}

		void Text::initShaders()
		{
			visuals::Shader vShader, fShader;

			vShader.free();
			fShader.free();
			vShader.initFromFile(visuals::ShaderType::VERTEX_SHADER, "shaders/text.vert");
			if (!vShader.isCompiled())
			{
				std::cout << "Vertex Shader Error" << std::endl;
				std::cout << "" << vShader.log() << std::endl << std::endl;
			}
			fShader.initFromFile(visuals::ShaderType::FRAGMENT_SHADER, "shaders/text.frag");
			if (!fShader.isCompiled())
			{
				std::cout << "Fragment Shader Error" << std::endl;
				std::cout << "" << fShader.log() << std::endl << std::endl;
			}
			m_program.init();
			m_program.addShader(vShader);
			m_program.addShader(fShader);
			m_program.link();
			if (!m_program.isLinked())
			{
				std::cout << "Shader Linking Error" << std::endl;
				std::cout << "" << m_program.log() << std::endl << std::endl;
			}
			m_program.bindFragmentOutput("outColor");
		}

		bool Text::extractCharSizes(int *maxCharWidth, int *maxCharHeight)
		{
			unsigned char c;

			*maxCharWidth = 0;
			*maxCharHeight = 0;
			for (c = 32; c < 128; c++)
			{
				if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
					return false;
				*maxCharWidth = glm::max(*maxCharWidth, (int)m_face->glyph->bitmap.width);
				*maxCharHeight = glm::max(*maxCharHeight, (int)m_face->glyph->bitmap.rows);
			}

			return true;
		}

		void Text::createTextureAtlas()
		{
			unsigned char c;
			int x = 0, y = 0;

			m_textureAtlas.createEmptyTexture(m_textureSize, m_textureSize);
			for (c = 32; c < 128; c++)
			{
				FT_Load_Char(m_face, c, FT_LOAD_RENDER);
				m_chars[c - 32].tx = x;
				m_chars[c - 32].ty = y;
				m_chars[c - 32].sx = m_face->glyph->bitmap.width;
				m_chars[c - 32].sy = m_face->glyph->bitmap.rows;
				m_chars[c - 32].ax = m_face->glyph->advance.x >> 6;
				m_chars[c - 32].ay = m_face->glyph->advance.y >> 6;
				m_chars[c - 32].bl = m_face->glyph->bitmap_left;
				m_chars[c - 32].bt = m_face->glyph->bitmap_top;
				m_textureAtlas.loadSubtextureFromGlyphBuffer(m_face->glyph->bitmap.buffer, x, y, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows);
				x += m_maxCharWidth;
				if ((x + m_maxCharWidth) >= m_textureSize)
				{
					x = 0;
					y += m_maxCharHeight;
				}
			}
			m_textureAtlas.generateMipmap();
			m_textureAtlas.setWrapS(GL_CLAMP_TO_EDGE);
			m_textureAtlas.setWrapT(GL_CLAMP_TO_EDGE);
		}

		uint32_t Text::GetCurrentCoins() const
		{
			return m_coins;
		}

		void Text::SetCurrentPoints(uint32_t i_currentPoints)
		{
			m_points = i_currentPoints;
		}

		void Text::SetCurrentCoins(uint32_t i_currentCoins)
		{
			m_coins = i_currentCoins;
		}

	}
}

