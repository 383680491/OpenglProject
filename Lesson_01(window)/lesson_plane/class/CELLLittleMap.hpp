#pragma once
#include "CELLInstance.hpp"
#include "CELLOpenGL.hpp"
#include "CELLMath.hpp"
#include "CELLProgramLibrary.hpp"
namespace   CELL
{
    class   CELLLittleMap
    {
    private:
		CELLInstance&   _instance;
		Texture2dId textId;

		struct Vertex
		{
			float x, y, z;
			float u, v;
			float r, g, b, a;
		};
    public:
		CELLLittleMap(CELLInstance&   instance):
			_instance(instance)
        {
			CELLOpenGL& device = _instance._device;
			textId = device.createTexure2D(0, GL_RGB, _instance.screenWidth, _instance.screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0, 0);
        }
		virtual ~CELLLittleMap()
        {

		}

		void onRneder(const FrameEvent& evt)
		{
			CELL::matrix4   screenProj = CELL::ortho<float>(0, float(_instance.screenWidth), float(_instance.screenHeight), 0, -100.0f, 100);
			PROGRAM_P2_UV2& shader = _instance._resource._PROGRAM_P2_UV2;
			CELLOpenGL& device = _instance._device;
			float   x = 0;
			float   y = 0;
			float   w = 200;
			float   h = 200;
			Vertex  vertex[] =
			{
				{ x, y, 0, 0 },
				{ x + w, y, 0 },
				{ x, y + h, 0, 0 },
				{ x + w, y + h, 0, 1 },
			};

			shader.begin();
			{
				glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, _instance.screenWidth, _instance.screenHeight);

				device.bindTexture2D(&textId, 0);
				device.setUniformMatrix4fv(shader._MVP, 1, false, screenProj.data());
				glVertexAttribPointer(shader._position, 3, GL_FLOAT, false, sizeof(Vertex), &vertex[0].x);
				glVertexAttribPointer(shader._uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex[0].u);

				device.drawArray(GL_TRIANGLE_STRIP, 0, sizeof(vertex) / sizeof(vertex[0]));
			}
			shader.end();
		}
    };
}