#pragma once

#include <Windows.h>
#include <tchar.h>
#include <math.h>

#include <EGL/egl.h>
#include <gles2/gl2.h>

#include "freeImage/FreeImage.h"
#include "CELLFont.h"

#include "CELLMath.hpp"
#include "CELLShader.hpp"
#include "CELLFont.h"

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
        //! ���ڵĿ��
        int         _height;
        /// for gles2.0
        EGLConfig   _config;
        EGLSurface  _surface;
        EGLContext  _context;
        EGLDisplay  _display;
        //! ����shader
        PROGRAM_P2_AC4  _shader;
        unsigned        _textureId;
        CELLFont        _font;
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

            _textureId              =   0;
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

        virtual unsigned    loadTexture(const char* fileName)
        {
            unsigned    textureId   =   0;
            //1 ��ȡͼƬ��ʽ
            FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileName, 0);

            //2 ����ͼƬ
            FIBITMAP    *dib = FreeImage_Load(fifmt, fileName,0);

            //3 ת��Ϊrgb 24ɫ
            dib     =   FreeImage_ConvertTo24Bits(dib);

            //4 ��ȡ����ָ��
            BYTE    *pixels =   (BYTE*)FreeImage_GetBits(dib);

            int     width   =   FreeImage_GetWidth(dib);
            int     height  =   FreeImage_GetHeight(dib);

            /**
            *   ����һ������Id,������Ϊ��������������Ĳ����������������id
            */
            glGenTextures( 1, &textureId );

            /**
            *   ʹ���������id,���߽а�(����)
            */
            glBindTexture( GL_TEXTURE_2D, textureId );
            /**
            *   ָ������ķŴ�,��С�˲���ʹ�����Է�ʽ������ͼƬ�Ŵ��ʱ���ֵ��ʽ 
            */
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            /**
            *   ��ͼƬ��rgb�����ϴ���opengl.
            */
            glTexImage2D( 
                GL_TEXTURE_2D,      //! ָ���Ƕ�άͼƬ
                0,                  //! ָ��Ϊ��һ�������������mipmap,��lod,����ľͲ��ü����ģ�Զ��ʹ�ý�С������
                GL_RGB,             //! �����ʹ�õĴ洢��ʽ
                width,              //! ��ȣ���һ����Կ�����֧�ֲ��������������Ⱥ͸߶Ȳ���2^n��
                height,             //! ��ȣ���һ����Կ�����֧�ֲ��������������Ⱥ͸߶Ȳ���2^n��
                0,                  //! �Ƿ�ı�
                GL_RGB,             //! ���ݵĸ�ʽ��bmp�У�windows,����ϵͳ�д洢��������bgr��ʽ
                GL_UNSIGNED_BYTE,   //! ������8bit����
                pixels
                );
            /**
            *   �ͷ��ڴ�
            */
            FreeImage_Unload(dib);

            return  textureId;
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

            struct  Vertex
            {
                CELL::float2    pos;
                CELL::float2    uv;
                CELL::Rgba4Byte color;
            };
            //! ��ջ�����
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            //! �ӿڣ���Windows����ָ����λ�úʹ�С�ϻ���OpenGL����
            glViewport(0,0,_width,_height);

            //! ����һ��ͶӰ����

            CELL::matrix4   screenProj  =   CELL::ortho<float>(0,float(_width),float(_height),0,-100.0f,100);
           

            _font.beginText(screenProj);

            _font.drawText(100,100,0,Rgba4Byte(255,255,0),L"����",-1);

            _font.endText();

        }
        /**
        *   ������
		���ڸ������ԭ��   ���ȴ���һ��1024*1024��С������fontTexture�����ǲ�û���κ����ݣ�Ȼ��ͨ���ַ�ͨ��freeType���ø����ֵ�λͼ��bitmap��
        Ȼ��bitmap������ͨ��GLTexSubImage2D��������bitmap��fontTexture,����ڴ����������ݽ�fontTexture���Ƴ������ܵ���˵����ƫ��
		��һ����Ҫ�Լ�ȥ�������
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

            _font.buildSystemFont("data/font/simsun.ttc",40);

            _textureId =   loadTexture("data/image/main.jpg");
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
