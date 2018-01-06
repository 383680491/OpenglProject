#pragma once

#include "class/CELLOpenGL.hpp"
#include "class/CELLResource.hpp"
#include "class/CELLTimerMgr.hpp"


namespace   CELL
{
	/*
		基础数据和基础API。 方便其他界面调用 opengl的接口和 资源的管理
	*/
    class   CELLInstance
    {
    public:
        CELLOpenGL      _device;      //创建了三个主要的对象
        CELLResource    _resource;
        CELLTimerMgr    _timerMgr;
    };
}