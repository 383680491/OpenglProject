#pragma once

#include <Windows.h>
#include <tchar.h>

#include <EGL/egl.h>
#include <gles2/gl2.h>

#include "class/CELLMath.hpp"
#include "class/CELLTimestamp.hpp"
#include "class/CELLInstance.hpp"
#include "class/CELLFrame.hpp"

namespace   CELL
{
	/*
		处理windows界面，初始化opengl  分发事件给框架
	*/
    class   CELLWinApp
    {
    public:
        //! 实例句柄
        HINSTANCE   _hInstance;
        //! 窗口句柄
        HWND        _hWnd;
        //! 窗口的高度
        int         _width;
        //! 窗口的宽度
        int         _height;
        /// for gles2.0
        EGLConfig   _config;
        EGLSurface  _surface;
        EGLContext  _context;
        EGLDisplay  _display;
        /// OpenGL对象
        CELLInstance    _instance;
        CELLTimestamp   _timestap;
        CELLFrame*      _frame;
        FrameEvent      _event;
    public:
        CELLWinApp(HINSTANCE hInstance)
            :_hInstance(hInstance)
        {
            WNDCLASSEX  winClass;
            winClass.lpszClassName  =   _T("CELLWinApp");
            winClass.cbSize         =   sizeof(winClass);
            winClass.style          =   CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            winClass.lpfnWndProc    =   wndProc;
            winClass.hInstance      =   hInstance;
            winClass.hIcon	        =   0;
            winClass.hIconSm	    =   0;
            winClass.hCursor        =   LoadCursor(hInstance, IDC_ARROW);
            winClass.hbrBackground  =   (HBRUSH)GetStockObject(BLACK_BRUSH);
            winClass.lpszMenuName   =   NULL;
            winClass.cbClsExtra     =   0;
            winClass.cbWndExtra     =   0;
            RegisterClassEx(&winClass);
        }
        virtual ~CELLWinApp()
        {
            UnregisterClass(_T("CELLWinApp"),_hInstance);
        }

        /**
        *   初始化 OpenGLES2.0
        */
        bool    initOpenGLES20()
        {
            const EGLint attribs[] =                    //初始化属性
            {
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                EGL_BLUE_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8,
                EGL_DEPTH_SIZE,24,
                EGL_NONE
            };
            EGLint 	format(0);
            EGLint	numConfigs(0);
            EGLint  major;
            EGLint  minor;

            //! 1
            _display	    =	eglGetDisplay(EGL_DEFAULT_DISPLAY);  //跨平台 

            //! 2init
            eglInitialize(_display, &major, &minor);     //获得 ES 的大版本号 和小版本号

            //! 3
            eglChooseConfig(_display, attribs, &_config, 1, &numConfigs); //通过配置去初始化display

            eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);   
            //! 4 
            _surface	    = 	eglCreateWindowSurface(_display, _config, _hWnd, NULL);  //通过display 和 config获得窗口

            //! 5
            EGLint attr[]   =   { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
            _context 	    = 	eglCreateContext(_display, _config, 0, attr);   //创建上下文  相当于ES的状态机  程序的主要工作就是向Context提供图元、设置状态，偶尔也从Context里获取一些信息
            //! 6
            if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE) //使用当前的状态机  因为 状态机可以有多个
            {
                return false;
            }

            eglQuerySurface(_display, _surface, EGL_WIDTH,  &_width);
            eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

            return  true;

        }
        /**
        *   销毁OpenGLES2.0
        */
        void    destroyOpenGLES20()
        {
            if (_display != EGL_NO_DISPLAY)
            {
                eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
                if (_context != EGL_NO_CONTEXT) 
                {
                    eglDestroyContext(_display, _context);
                }
                if (_surface != EGL_NO_SURFACE) 
                {
                    eglDestroySurface(_display, _surface);
                }
                eglTerminate(_display);
            }
            _display    =   EGL_NO_DISPLAY;
            _context    =   EGL_NO_CONTEXT;
            _surface    =   EGL_NO_SURFACE;
        }
    protected:
        static  LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            
            CELLWinApp*  pThis   =   (CELLWinApp*)GetWindowLong(hWnd,GWL_USERDATA);
            if (pThis)
            {
                return  pThis->onEvent(hWnd,msg,wParam,lParam);
            }
            if (WM_CREATE == msg)   //窗口创建之后显示之前 系统分发的事件
            {
                CREATESTRUCT*   pCreate =   (CREATESTRUCT*)lParam;
                SetWindowLong(hWnd,GWL_USERDATA,(DWORD_PTR)pCreate->lpCreateParams);
            }
            return  DefWindowProc( hWnd, msg, wParam, lParam );
        }
    public:
        /**
        *   该函数可以被重写
        */
        virtual CELLFrame*  createFrame(CELLInstance& instance)
        {
            return  new CELLFrame(instance,0);
        }
        /**
        *   事件函数
        */
        virtual LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp)                        ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp)                        ((int)(short)HIWORD(lp))
#endif

#ifndef GET_WHEEL_DELTA_WPARAM
#define GET_WHEEL_DELTA_WPARAM(wParam)          (int)((short)HIWORD(wParam))
#endif

            switch( msg )
            {
            case WM_SIZE:
                {
                    RECT    rt;
                    GetClientRect(_hWnd,&rt);
                    _width   =   rt.right - rt.left;
                    _height  =   rt.bottom - rt.top;
                    if (_frame)
                    {
                        _frame->onSize(_width,_height);
                    }
                }
                break;
            case WM_LBUTTONDOWN:
                {
                    if (_frame)
                    {
                        _frame->onMousePress(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),CELL::MouseButton::Left);
                    }
                }
                break;
            case WM_LBUTTONUP:
                {
                    if (_frame)
                    {
                        _frame->onMouseRelease(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),CELL::MouseButton::Left);
                    }
                }
                break;
            case WM_RBUTTONDOWN:
                {
                    if (_frame)
                    {
                        _frame->onMousePress(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),CELL::MouseButton::Right);
                    }
                }
                break;
            case WM_RBUTTONUP:
                {
                    if (_frame)
                    {
                        _frame->onMouseRelease(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),CELL::MouseButton::Right);
                    }
                }
                break;
            case WM_MOUSEMOVE:
                {
                    if (_frame)
                    {
                        _frame->onMouseMove(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),0);
                    }
                }
                break;

            case WM_MOUSEWHEEL:
                {
                    if (_frame)
                    {
                        _frame->onMouseMove(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),GET_WHEEL_DELTA_WPARAM(wParam));
                    }
                }
                break;
            case WM_CHAR:
                {
                    if (_frame)
                    {
                        _frame->onChar(wParam);
                    }
                }
                break;
            case WM_KEYDOWN:
                {
                }
                break;
            case WM_CLOSE:
            case WM_DESTROY:
                {
                    ::PostQuitMessage(0);
                }
                break;
            default:
                return DefWindowProc(_hWnd, msg, wParam, lParam );
            }
            return  S_OK;
            
        }
        /**
        *   绘制函数,
        */
        virtual void    render()
        {
        }
        /**
        *   主函数
        */
        int     main(int width,int height)
        {
            _hWnd   =   CreateWindowEx( NULL,
                                        _T("CELLWinApp"),
                                        _T("CELLWinApp"),
                                        WS_OVERLAPPEDWINDOW,
                                        CW_USEDEFAULT,
                                        CW_USEDEFAULT,
                                        width,
                                        height, 
                                        NULL, 
                                        NULL,
                                        _hInstance, 
                                        this
                                        );

            if (_hWnd == 0)
            {
                return  -1;
            }
            ShowWindow(_hWnd,SW_SHOW);

            if (!initOpenGLES20())
            {
                return  false;
            }
            //! 增加资源初始化
            _instance._resource.initialize(_instance._device);   //初始化资源
            _frame  =   createFrame(_instance);                  //初始化框架

            MSG msg =   {0};
            while(msg.message != WM_QUIT)   //搞清楚PeekMessage 和 GetMessage函数的区别 PeekMessage是没有消息直接返回   GetMessage是没有消息就一直等待 阻塞
            {
                if (msg.message == WM_DESTROY || 
                    msg.message == WM_CLOSE)
                {
                    break;
                }

                _event._sinceLastFrame  =   (float)_timestap.getElapsedSecond();  //每次获得系统时间
                _timestap.update();
                /// 注意这里增加对定时器的支持
                _instance._timerMgr.update(_event._sinceLastFrame);
                /**
                *   有消息，处理消息，无消息，则进行渲染绘制
                */
                if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) //最后一个参数表示 获得消息后就直接删除 如果是 PM_NOREMOVE 则只获得消息，消息一直保存消息列表里面
                { 
                    TranslateMessage( &msg );  //将系统发的消息 翻译成我们能认识的消息，差不多就是这个意思
                    DispatchMessage( &msg );   //当我们创建WNDCLASSEX（可以理解为一个结构体数据）的时候，注册了一个监听函数wndProc 他会调用onEvent，当执行分发的时候，wndProc会收到消息
                }
               
                if (_frame)
                {
                    _frame->onRender(_event,_width,_height);
                }
                eglSwapBuffers(_display,_surface);
            
            }
            /**
            *   释放框架
            */
            delete  _frame;
            _frame  =   0;
            /**
            *   销毁OpenGLES20
            */
            destroyOpenGLES20();

            return  0;
        }
    };
}