#pragma once

#include "CELLOpenGL.hpp"
#include "CELLResource.hpp"
#include "CELLTimerMgr.hpp"


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
		int screenWidth;
		int screenHeight;

    };
}