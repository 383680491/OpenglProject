#pragma once

#include "class/CELLOpenGL.hpp"
#include "class/CELLResource.hpp"
#include "class/CELLTimerMgr.hpp"


namespace   CELL
{
	/*
		�������ݺͻ���API�� ��������������� opengl�Ľӿں� ��Դ�Ĺ���
	*/
    class   CELLInstance
    {
    public:
        CELLOpenGL      _device;      //������������Ҫ�Ķ���
        CELLResource    _resource;
        CELLTimerMgr    _timerMgr;
    };
}