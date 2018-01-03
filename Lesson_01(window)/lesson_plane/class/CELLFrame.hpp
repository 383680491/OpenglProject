#pragma once

#include "class/CELLMath.hpp"
#include "class/CELLInput.hpp"
#include "class/CELLInstance.hpp"

namespace   CELL
{
	/*
	��Ϸ���  �̳���CELLInput���̺����� �Լ������render�Ⱥ�����֮���ShootingFrame�̳еľ���CELLFrame
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
        *   ����ƶ�
        */
        virtual void    onMouseMove(int /*absx*/, int /*absy*/, int /*absz*/){}
        /**
        *   ��갴��
        */
        virtual void    onMousePress(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   ���˫��
        */
        virtual void    onMouseDbPress(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   ����ͷ�
        */
        virtual void    onMouseRelease(int /*absx*/, int /*absy*/, CELL::MouseButton /*id*/){}
        /**
        *   ���̰���
        */
        virtual void    onKeyPress(CELL::KeyCode /*key*/, int /*text*/){}
        /**
        *   ����̧��
        */
        virtual void    onKeyRelease(CELL::KeyCode /*key*/){}
        /**
        *   ����
        */
        virtual void    onChar(int /*ch*/){}

        /**
        *   ���ƺ���
        */
        virtual void    onRender(const FrameEvent& /*evt*/,int /*width*/,int /*height*/){}
        /**
        *   ���ڴ�С�����仯�¼�
        */
        virtual void    onSize(int /*width*/,int /*height*/){}
        /**
        *   �����¼�
        */
        virtual void    onDestroy(){}
        /**
        *   ��ں���
        */
        virtual int     main(int /*argc*/,char** /*argv*/)  {   return 0;   }


    };

}
