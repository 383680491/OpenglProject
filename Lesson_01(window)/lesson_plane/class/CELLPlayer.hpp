#pragma once

#include    "class/CELLNode.hpp"
#include    "class/CELLInput.hpp"
#include    "class/CELLTimer.hpp"
#include    "class/CELLBullet.hpp"

namespace   CELL
{
    class   CELLPlayer :public CELLInput
    {
    public:
        typedef std::vector<CELLBullet*>    ArrayBullet;
    public:
        struct  Vertex
        {
            float2  _pos;
            float2  _uv;
        };
    public:
        CELLInstance&   _instance;
        CELLNode        _nodeBody;
        Texture2dId     _plant;
        bool            _lbuttonDown;
        float3          _downPos;
        CELLTimer       _timerEmitBullet;
        ArrayBullet     _arBullet;
    public:
        CELLPlayer(CELLInstance& instance)
            :_instance(instance)
            ,_lbuttonDown(false)
            ,_timerEmitBullet(instance._timerMgr)
        {
            _plant      =   instance._resource.getTexture("data/image/plane1.tex");
            _nodeBody.setSize(float2(_plant._width,_plant._height));
            _nodeBody.setPosition(float3(100,100,0));
            _nodeBody.update(0,true);
            _timerEmitBullet.setTimer(0.1f);
            _timerEmitBullet.bind(this,&CELLPlayer::eventEmitBullet);
        }
        virtual ~CELLPlayer()
        {
        }
        /**
        *   定时发射子弹函数
        */
        int     eventEmitBullet(CELLTimer* tm,float elasped)
        {
            CELLBullet* bullet  =   new CELLBullet();
            bullet->setPosition(_nodeBody.getPosition());
            bullet->setDir(float3(0,-1,0));
            bullet->setSize(float2(32,32));
            bullet->setMaxDistance(500);
            bullet->setSpeed(600);
            _arBullet.push_back(bullet);
            return  0;
        }
        /**
        *   鼠标移动
        */
        virtual void    onMouseMove(int absx, int absy, int absz)
        {
            if (_lbuttonDown)
            {
                float3  offset  =   float3(absx,absy,0) - _downPos;
                        _downPos=   float3(absx,absy,0);
                _nodeBody.setPosition(_nodeBody.getPosition() + offset);
                _nodeBody.update(0,true);
            }
        }
        /**
        *   鼠标按下
        */
        virtual void    onMousePress(int absx, int absy, CELL::MouseButton id)
        {
            if (_nodeBody.getAABB().contains(CELL::float3(absx,absy,0)))
            {
                _downPos        =   CELL::float3(absx,absy,0);
                _lbuttonDown    =   (id == CELL::MouseButton::Left);
            }
        }
        /**
        *   鼠标双击
        */
        virtual void    onMouseDbPress(int absx, int absy, CELL::MouseButton /*id*/)
        {
            
        }
        /**
        *   鼠标释放
        */
        virtual void    onMouseRelease(int absx, int absy, CELL::MouseButton id)
        {
            if (id == CELL::MouseButton::Left)
            {
                _lbuttonDown    =   false;
            }
        }
        /**
        *   键盘按下
        */
        virtual void    onKeyPress(CELL::KeyCode /*key*/, int /*text*/)
        {

        }
        /**
        *   键盘抬起
        */
        virtual void    onKeyRelease(CELL::KeyCode /*key*/)
        {

        }
        /**
        *   输入
        */
        virtual void    onChar(int /*ch*/)
        {

        }

        /**
        *   绘制
        */
        void    onRneder(const FrameEvent& evt,const CELL::matrix4& proj)
        {
            PROGRAM_P2_UV2& prg     =   _instance._resource._PROGRAM_P2_UV2;
            CELLOpenGL&     device  =   _instance._device;
            device.bindTexture2D(&_plant,0);

            CELL::float2    halfSz  =   _nodeBody.getSize() * 0.5f;

            CELL::float3    vMin    =   -CELL::float3(halfSz.x,halfSz.y,0);
            CELL::float3    vMax    =   CELL::float3(halfSz.x,halfSz.y,0);
            Vertex vert[]   =   
            {
                {   CELL::float2(vMin.x,vMin.y),    float2(0,0)},
                {   CELL::float2(vMax.x,vMin.y),    float2(1,0)},
                {   CELL::float2(vMin.x,vMax.y),    float2(0,1)},
                {   CELL::float2(vMax.x,vMax.y),    float2(1,1)},
            };

            CELL::matrix4   mvp     =   proj * _nodeBody.getMatrix();
            device.setUniformMatrix4fv(prg._MVP, 1, false, mvp.data());
            device.setUniform1i(prg._texture,0);
            device.attributePointer(prg._position,  2,  GL_FLOAT,   false,  sizeof(Vertex),vert);
            device.attributePointer(prg._uv,        2,  GL_FLOAT,   false,  sizeof(Vertex),&vert[0]._uv);

            device.drawArray(GL_TRIANGLE_STRIP,0,4);

            ArrayBullet::iterator itr = _arBullet.begin();
            for ( ;itr != _arBullet.end() ; )
            {
                CELLBullet* bullet  =   *itr;
                if (bullet->isDead())
                {
                    itr =   _arBullet.erase(itr);
                    continue;
                }
                else
                {
                    ++itr;
                }
                bullet->update(evt._sinceLastFrame,true);

                CELL::float2    halfSz  =   bullet->getSize() * 0.5f;

                CELL::float3    vMin    =   -CELL::float3(halfSz.x,halfSz.y,0);
                CELL::float3    vMax    =   CELL::float3(halfSz.x,halfSz.y,0);

                Vertex vert[]   =   
                {
                    {   CELL::float2(vMin.x,vMin.y),    float2(0,0)},
                    {   CELL::float2(vMax.x,vMin.y),    float2(1,0)},
                    {   CELL::float2(vMin.x,vMax.y),    float2(0,1)},
                    {   CELL::float2(vMax.x,vMax.y),    float2(1,1)},
                };

                mvp     =   proj * bullet->getMatrix();
                device.setUniformMatrix4fv(prg._MVP, 1, false, mvp.data());
           
                device.attributePointer(prg._position,  2,  GL_FLOAT,   false,  sizeof(Vertex),vert);
                device.attributePointer(prg._uv,        2,  GL_FLOAT,   false,  sizeof(Vertex),&vert[0]._uv);

                device.drawArray(GL_TRIANGLE_STRIP,0,4);


            }
        }
    };
}