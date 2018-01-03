#pragma once

#include <Windows.h>
#include <tchar.h>

#include <EGL/egl.h>
#include <gles2/gl2.h>

#include "CELLMath.hpp"
#include "CELLShader.hpp"

namespace   CELL
{
    class   CELLWinApp
    {
    public:
        //! ʵ�����
        HINSTANCE   _hInstance;
        //! ���ھ��
        HWND        _hWnd;
        //! ���ڵĸ߶�
        int         _width;
        //! ���ڵĿ���
        int         _height;
        /// for gles2.0
        EGLConfig   _config;
        EGLSurface  _surface;
        EGLContext  _context;
        EGLDisplay  _display;
        //! ����shader
        PROGRAM_P2_C4   _shader;
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
        *   ��ʼ�� OpenGLES2.0
        */
        bool    initOpenGLES20()
        {
            const EGLint attribs[] =
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
            _display	    =	eglGetDisplay(EGL_DEFAULT_DISPLAY);

            //! 2init
            eglInitialize(_display, &major, &minor);

            //! 3
            eglChooseConfig(_display, attribs, &_config, 1, &numConfigs);

            eglGetConfigAttrib(_display, _config, EGL_NATIVE_VISUAL_ID, &format);
            //! 4 
            _surface	    = 	eglCreateWindowSurface(_display, _config, _hWnd, NULL);

            //! 5
            EGLint attr[]   =   { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
            _context 	    = 	eglCreateContext(_display, _config, 0, attr);
            //! 6
            if (eglMakeCurrent(_display, _surface, _surface, _context) == EGL_FALSE)
            {
                return false;
            }

            eglQuerySurface(_display, _surface, EGL_WIDTH,  &_width);
            eglQuerySurface(_display, _surface, EGL_HEIGHT, &_height);

            return  true;

        }
        /**
        *   ����OpenGLES2.0
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
            if (WM_CREATE == msg)
            {
                CREATESTRUCT*   pCreate =   (CREATESTRUCT*)lParam;
                SetWindowLong(hWnd,GWL_USERDATA,(DWORD_PTR)pCreate->lpCreateParams);
            }
            return  DefWindowProc( hWnd, msg, wParam, lParam );
        }
    public:
        /**
        *   �¼�����
        */
        virtual LRESULT onEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch (msg)
            {
            case WM_CLOSE:
            case WM_DESTROY:
                {
                    ::PostQuitMessage(0);
                }
                break;
            case WM_MOUSEMOVE:
                break;
            default:
                return  DefWindowProc( hWnd, msg, wParam, lParam ); 
            }
            return  S_OK;
            
        }

        virtual void    render()
        {
            //! ��ջ�����
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            //! �ӿڣ���Windows����ָ����λ�úʹ�С�ϻ���OpenGL����
            glViewport(0,0,_width,_height);

            //! ����һ��ͶӰ����

            CELL::matrix4   screenProj  =   CELL::ortho<float>(0,float(_width),float(_height),0,-100.0f,100);
            _shader.begin();
            {
                float   x   =   100;
                float   y   =   100;
                float   w   =   100;
                float   h   =   100;
                CELL::float2  pos[]   =   
                {
                    CELL::float2(x,y),
                    CELL::float2(x + w,y),
                    CELL::float2(y,y + h),
                    CELL::float2(x + w, y + h),
                };
                glUniformMatrix4fv(_shader._MVP, 1, false, screenProj.data());
                glUniform4f(_shader._color,1,0,0,1);
                glVertexAttribPointer(_shader._position,2,GL_FLOAT,false,sizeof(CELL::float2),pos);
                glDrawArrays(GL_TRIANGLE_STRIP,0,4);
            }
            _shader.end();

        }
        /**
        *   ������
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
            UpdateWindow(_hWnd);

            ShowWindow(_hWnd,SW_SHOW);

            if (!initOpenGLES20())
            {
                return  false;
            }
            _shader.initialize();
            MSG msg =   {0};
            while(msg.message != WM_QUIT)
            {
                if (msg.message == WM_DESTROY || 
                    msg.message == WM_CLOSE)
                {
                    break;
                }
                /**
                *   ����Ϣ��������Ϣ������Ϣ���������Ⱦ����
                */
                if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
                { 
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
                {
                    render();
                    eglSwapBuffers(_display,_surface);
                }
            }
            /**
            *   ����OpenGLES20
            */
            destroyOpenGLES20();

            return  0;
        }
    };
}