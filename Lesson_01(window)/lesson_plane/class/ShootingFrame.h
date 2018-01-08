#pragma once
#include "class/CELLFrame.hpp"
#include "class/CELLNode.hpp"
#include "class/CELLPlayer.hpp"
#include "class/CELLLittleMap.hpp"

namespace   CELL
{
    class   ShootingFrame :public CELLFrame
    {
    public:
        struct  Vertex
        {
            float2  _pos;
            float2  _uv;
        };
    public:
        Texture2dId     _texture;
        CELLPlayer      _player;
		CELLLittleMap*   map;
    public:
        ShootingFrame(CELLInstance& instance)
            :CELLFrame(instance)
            ,_player(instance)
        {
            _texture    =   instance._resource.getTexture("data/image/main.tex");
            
            instance._device.enableState(GL_BLEND);
            instance._device.blendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			map = new CELLLittleMap(instance);
        }

		~ShootingFrame()
		{
			if (map){
				delete map;
			}
		}

        /**
        *   鼠标移动
        */
        virtual void    onMouseMove(int absx, int absy, int absz)
        {
            _player.onMouseMove(absx,absy,absz);
        }
        /**
        *   鼠标按下
        */
        virtual void    onMousePress(int absx, int absy, CELL::MouseButton id)
        {
            _player.onMousePress(absx,absy,id);
        }

        /**
        *   鼠标双击
        */
        virtual void    onMouseDbPress(int absx, int absy, CELL::MouseButton id)
        {
            _player.onMouseDbPress(absx,absy,id);
        }

        /**
        *   鼠标释放
        */
        virtual void    onMouseRelease(int absx, int absy, CELL::MouseButton id)
        {
            _player.onMouseRelease(absx,absy,id);
        }

        /**
        *   键盘按下
        */
        virtual void    onKeyPress(CELL::KeyCode key, int text)
        {
            _player.onKeyPress(key,text);
        }
        /**
        *   键盘抬起
        */
        virtual void    onKeyRelease(CELL::KeyCode key)
        {
            _player.onKeyRelease(key);
        }
        /**
        *   输入
        */
        virtual void    onChar(int ch)
        {
            _player.onChar(ch);
        }
        
        virtual void    onRender(const FrameEvent& evt,int width,int height)
        {

            PROGRAM_P2_UV2& prg     =   _instance._resource._PROGRAM_P2_UV2;
            CELLOpenGL&     device  =   _instance._device;

            device.clear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            device.setViewport(0,0,width,height);

           
            prg.begin();
            {
                //! 创建一个投影矩阵
                CELL::matrix4   screenProj  =   CELL::ortho<float>(0,float(width),float(height),0,-100.0f,100);

                static  float   vv  =   0;
                float   x   =   0;
                float   y   =   0;
                float   w   =   (float)width;
                float   h   =   (float)height;
                Vertex vert[]   =   
                {
                    {   CELL::float2(x,y),          float2(0,vv)},
                    {   CELL::float2(x + w,y),      float2(1,vv)},
                    {   CELL::float2(x,y + h),      float2(0,1 + vv)},
                    {   CELL::float2(x + w, y + h), float2(1,1 + vv)},
                };
                vv  +=  0.001f;
                device.bindTexture2D(&_texture,0);
                device.setUniformMatrix4fv(prg._MVP, 1, false, screenProj.data());
                device.setUniform1i(prg._texture,0);
                device.attributePointer(prg._position,  2,  GL_FLOAT,   false,  sizeof(Vertex),vert);
                device.attributePointer(prg._uv,        2,  GL_FLOAT,   false,  sizeof(Vertex),&vert[0]._uv); 
                device.drawArray(GL_TRIANGLE_STRIP,0,4);

                _player.onRneder(evt,screenProj);
            }
            prg.end();

			//map->onRneder(evt);
        }


    };
}