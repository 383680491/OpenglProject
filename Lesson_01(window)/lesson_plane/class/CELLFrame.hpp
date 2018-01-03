#pragma once

#include "class/CELLMath.hpp"
#include "class/CELLInput.hpp"
#include "class/CELLInstance.hpp"

namespace   CELL
{
	/*
	游戏框架  继承了CELLInput键盘和输入 自己添加了render等函数，之后的ShootingFrame继承的就是CELLFrame
	*/
    class   CELLFrame   
            :public CELLInput
    {
    private:
        void    operator = (const CELLFrame& );
        CELLFrame(const CELLFrame& right);

    protected:
        CELLInstance& _instance;
    public:
        CELLFrame(CELLInstance& instance,void* user = 0) 
            :_instance(instance)
        {}
        virtual	~CELLFrame(){}
        /**
        *   鼠标移动
        */
        virtual void    onMouseMove(int /*absx*/, int /*absy*/, int /*absz*/){}
        /**
        *   鼠标按下
        */
        virtual void    onMousePress(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   鼠标双击
        */
        virtual void    onMouseDbPress(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   鼠标释放
        */
        virtual void    onMouseRelease(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   键盘按下
        */
        virtual void    onKeyPress(CELL::KeyCode /*key*/, int /*text*/){}
        /**
        *   键盘抬起
        */
        virtual void    onKeyRelease(CELL::KeyCode /*key*/){}
        /**
        *   输入
        */
        virtual void    onChar(int /*ch*/){}

        /**
        *   绘制函数
        */
        virtual void    onRender(const FrameEvent& /*evt*/,int /*width*/,int /*height*/){}
        /**
        *   窗口大小发生变化事件
        */
        virtual void    onSize(int /*width*/,int /*height*/){}
        /**
        *   销毁事件
        */
        virtual void    onDestroy(){}
        /**
        *   入口函数
        */
        virtual int     main(int /*argc*/,char** /*argv*/)  {   return 0;   }


    };

}
