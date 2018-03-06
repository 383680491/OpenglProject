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

    public:
		struct  Vertex
		{
			float2  _pos;
			float2  _uv;
			float4 _color;
		};

		CELLLittleMap(CELLInstance&   instance):
			_instance(instance)
        {
			CELLOpenGL& device = _instance._device;
			GLubyte* TexData = new GLubyte[_instance.screenWidth * _instance.screenHeight * 4];
			memset(TexData, 255, sizeof(TexData));
			textId = device.createTexure2D(0, GL_RGBA, _instance.screenWidth, _instance.screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexData, 0);
			//textId = instance._resource.getTexture("data/image/plane1.tex");





        }
		virtual ~CELLLittleMap()
        {

		}

		void onRneder(const FrameEvent& evt)
		{
			CELL::matrix4   screenProj = CELL::ortho<float>(0, float(_instance.screenWidth), float(_instance.screenHeight), 0, -100.0f, 100);
			PROGRAME_P2_UV_C4& shader = _instance._resource._PROGRAME_P2_UV_C4;
			CELLOpenGL& device = _instance._device;
			float   x = 0;
			float   y = 0;
			float   w = 100;
			float   h = 100;
			Vertex  vertex[] =
			{
				{ CELL::float2(0, 0), float2(0, 0), float4(1.0f, 0.0f, 0.0f, 0.0f) },
				{ CELL::float2(100, 0), float2(1, 0), float4(1.0f, 0.0f, 0.0f, 0.0f) },
				{ CELL::float2(0, 100), float2(0, 1), float4(1.0f, 0.0f, 0.0f, 0.0f) },
				{ CELL::float2(100, 100), float2(1, 1), float4(1.0f, 0.0f, 0.0f, 0.0f) },
			};

			shader.begin();
			{
				device.setUniformMatrix4fv(shader._MVP, 1, false, screenProj.data());
				device.bindTexture2D(&textId, 0);
				//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, textId._width, textId._height);
				glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, textId._width, textId._height, 0);

				device.setUniform1i(shader._texture, 0);
				glVertexAttribPointer(shader._position, 2, GL_FLOAT, false, sizeof(Vertex), vertex);
				glVertexAttribPointer(shader._uv, 2, GL_FLOAT, false, sizeof(Vertex), &vertex[0]._uv);
				glVertexAttribPointer(shader._color, 4, GL_FLOAT, false, sizeof(Vertex), &vertex[0]._color);

				device.drawArray(GL_TRIANGLE_STRIP, 0, 4);
			}
			shader.end();
		}
    };
}