#pragma once

#include <Windows.h>
#include <tchar.h>
#include <math.h>

#include <EGL/egl.h>
#include <gles2/gl2.h>

#include "freeImage/FreeImage.h"

#include "CELLMath.hpp"
#include "CELLShader.hpp"

namespace   CELL
{

    class   FirstCameraInfor
    {
    public:
        FirstCameraInfor()
        {
            _moveSpeed  =   5;
            _eye        =   CELL::float3(0.0f, 10.0f, 0.0f);
            _look       =   CELL::float3(0.5f, -0.4f, -0.5f);
            _up         =   CELL::float3(0,1,0);
            _right      =   CELL::float3(1,0,0);
        }
        CELL::float3    _eye;
        CELL::float3    _look;
        CELL::float3    _up;
        CELL::float3    _right;
        float           _moveSpeed;
    public:

        void    updateCamera(float fElapsed)
        {

            CELL::float3    tmpLook =   _look;
            CELL::float3    dir     =   _look - _eye;
            dir =   normalize(dir);
            //! �������windows������ȡ���̵�״̬
            unsigned char keys[256];
            GetKeyboardState( keys );

            if( keys[VK_UP] & 0x80 )
            {
                _eye    -=  dir*-_moveSpeed * fElapsed;
                _look   -=  dir*-_moveSpeed * fElapsed;
            }

            if( keys[VK_DOWN] & 0x80 )
            {
                _eye    +=  (dir*-_moveSpeed) * fElapsed;
                _look   +=  (dir*-_moveSpeed) * fElapsed;
            }

            if( keys[VK_LEFT] & 0x80 )
            {
                _eye    -=  (_right*_moveSpeed) * fElapsed;
                _look   -=  (_right*_moveSpeed) * fElapsed;
            }

            if( keys[VK_RIGHT] & 0x80 )
            {
                _eye    +=  (_right*_moveSpeed) * fElapsed;
                _look   +=  (_right*_moveSpeed) * fElapsed;
            }
        }
    };


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
        PROGRAM_P2_T2_C3    _shader;
        unsigned            _textureId;
        unsigned            _texDynamic;
        FirstCameraInfor    _camera;
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
    
        virtual unsigned    createTexture(int width,int height)
        {
            unsigned    textureId;
            glGenTextures(1,&textureId);
            glBindTexture(GL_TEXTURE_2D,textureId);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

            glTexImage2D(
                GL_TEXTURE_2D
                ,0
                ,GL_RGB
                ,width
                ,height
                ,0
                ,GL_RGB
                ,GL_UNSIGNED_BYTE
                ,0);
            return  textureId;
        }
        virtual unsigned    buildMipMap(char* fileNames[],int size)
        {
            unsigned    textureId   =   0;
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
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
           


            for (int i = 0 ;i < size ; ++ i)
            {
                //1 ��ȡͼƬ��ʽ
                FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(fileNames[i], 0);

                //2 ����ͼƬ
                FIBITMAP    *dib = FreeImage_Load(fifmt, fileNames[i],0);

                //3 ת��Ϊrgb 24ɫ
                dib     =   FreeImage_ConvertTo24Bits(dib);



                //4 ��ȡ����ָ��
                BYTE    *pixels =   (BYTE*)FreeImage_GetBits(dib);

                int     width   =   FreeImage_GetWidth(dib);
                int     height  =   FreeImage_GetHeight(dib);

                for (int x = 0 ;x < width * height * 3 ; x+=3 )
                {
                    BYTE temp       =   pixels[x];
                    pixels[x]       =   pixels[x + 2];
                    pixels[x + 2]   =   temp;
                }
                

                /**
                *   ��ͼƬ��rgb�����ϴ���opengl.
                */
                glTexImage2D( 
                    GL_TEXTURE_2D,      //! ָ���Ƕ�άͼƬ
                    i,              //! ָ��Ϊ��һ�������������mipmap,��lod,����ľͲ��ü����ģ�Զ��ʹ�ý�С������
                    GL_RGB,             //! �����ʹ�õĴ洢��ʽ
                    width,              //! ��ȣ���һ����Կ�����֧�ֲ��������������Ⱥ͸߶Ȳ���2^n��
                    height,             //! ��ȣ���һ����Կ�����֧�ֲ��������������Ⱥ͸߶Ȳ���2^n��
                    0,                  //! �Ƿ�ı�
                    GL_RGB,             //! ���ݵĸ�ʽ��bmp�У�windows,����ϵͳ�д洢��������bgr��ʽ
                    GL_UNSIGNED_BYTE,   //! ������8bit����
                    pixels
                    );
                
                FreeImage_Unload(dib);
            }

            return  textureId;
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

            for (int i = 0 ;i < width * height * 3 ; i+=3 )
            {
                BYTE temp       =   pixels[i];
                pixels[i]       =   pixels[i + 2];
                pixels[i + 2]   =   temp;
            }
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
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
            
            
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
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            glViewport(0,0,_width,_width);

            struct Vertex
            {
                float x, y, z;
                float u,v;
                float r, g, b,a;
            };
           

            _camera.updateCamera(0.016f);

            CELL::matrix4   matView =   CELL::lookAt(_camera._eye,_camera._look,_camera._up);


            Vertex vertexs[] =
            {
                { -1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 1.0f, 1.0f,1.0f },
                {  1.0f,-1.0f, 1.0f,1.0f, 0.0f,1.0f, 1.0f, 1.0f,1.0f },
                {  1.0f, 1.0f, 1.0f,1.0f, 1.0f,1.0f, 1.0f, 1.0f,1.0f },

                { -1.0f,-1.0f, 1.0f,0.0f, 0.0f,1.0f, 1.0f, 1.0f,1.0f },
                {  1.0f, 1.0f, 1.0f,1.0f, 1.0f,1.0f, 1.0f, 1.0f,1.0f },
                { -1.0f, 1.0f, 1.0f,0.0f, 1.0f,1.0f, 1.0f, 1.0f,1.0f },
            };


            float   x   =   0;
            float   y   =   0;
            float   w   =   200;
            float   h   =   200;
            Vertex  vertex[]   =   
            {
                {x,      y,         0,  0,1,    1.0f, 1.0f, 1.0f,1.0f},
                {x + w,  y,         0,  1,1,    1.0f, 1.0f, 1.0f,1.0f},
                {x,      y + h,     0,  0,0,    1.0f, 1.0f, 1.0f,1.0f},
                {x + w,  y + h,     0,  1,0,    1.0f, 1.0f, 1.0f,1.0f},
            };                      
                                 
            float   gSize   =   100;
            float   gPos    =   -5;
            float   rept    =   100;

            Vertex grounds[] =
            {
                { -gSize, gPos,-gSize,0.0f, 0.0f,1.0f, 1.0f, 1.0f,1.0f },
                {  gSize, gPos,-gSize,rept, 0.0f,1.0f, 1.0f, 1.0f,1.0f },
                {  gSize, gPos, gSize,rept, rept,1.0f, 1.0f, 1.0f,1.0f },

                { -gSize, gPos,-gSize,0.0f, 0.0f,1.0f, 1.0f, 1.0f,1.0f },
                {  gSize, gPos, gSize,rept, rept,1.0f, 1.0f, 1.0f,1.0f },
                { -gSize, gPos, gSize,0.0f, rept,1.0f, 1.0f, 1.0f,1.0f },
            };


            _shader.begin();
            {

                CELL::matrix4   matWorld(1);
                CELL::matrix4   matProj =   CELL::perspective(45.0f, (GLfloat)_width / (GLfloat)_height, 0.1f, 100.0f);

                CELL::matrix4   MVP     =   matProj * matView * matWorld;

                glUniform1i(_shader._texture,0);
                //! ���Ƶ���
                glBindTexture(GL_TEXTURE_2D,_textureId);
                glUniformMatrix4fv(_shader._MVP, 1, false, MVP.data());

                glVertexAttribPointer(_shader._positionAttr,3,  GL_FLOAT,   false,  sizeof(Vertex),&grounds[0].x);
                glVertexAttribPointer(_shader._uvAttr,2,        GL_FLOAT,   false,  sizeof(Vertex),&grounds[0].u);
                glVertexAttribPointer(_shader._colorAttr,4,     GL_FLOAT,   false,  sizeof(Vertex),&grounds[0].r);

                glDrawArrays(GL_TRIANGLES,0,sizeof(grounds) / sizeof(grounds[0]) );

                CELL::matrix4   matRot;
                static  float   angle   =   0;
                matRot.rotateZ(angle);
                angle   +=  1;

                CELL::matrix4   MVP1     =   matProj * matView * matRot;

                glUniformMatrix4fv(_shader._MVP, 1, false, MVP1.data());
                glVertexAttribPointer(_shader._positionAttr,3,  GL_FLOAT,   false,  sizeof(Vertex),&vertexs[0].x);
                glVertexAttribPointer(_shader._uvAttr,2,        GL_FLOAT,   false,  sizeof(Vertex),&vertexs[0].u);
                glVertexAttribPointer(_shader._colorAttr,4,     GL_FLOAT,   false,  sizeof(Vertex),&vertexs[0].r);

                glDrawArrays(GL_TRIANGLES,0,sizeof(vertexs) / sizeof(vertexs[0]) );



                glBindTexture(GL_TEXTURE_2D,_texDynamic);
                glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,_width,_height);
                
                CELL::matrix4   screenProj  =   CELL::ortho<float>(0,float(_width),float(_height),0,-100.0f,100);

                glUniformMatrix4fv(_shader._MVP, 1, false, screenProj.data());

                glVertexAttribPointer(_shader._positionAttr,3,  GL_FLOAT,   false,  sizeof(Vertex),&vertex[0].x);
                glVertexAttribPointer(_shader._uvAttr,2,        GL_FLOAT,   false,  sizeof(Vertex),&vertex[0].u);
                glVertexAttribPointer(_shader._colorAttr,4,     GL_FLOAT,   false,  sizeof(Vertex),&vertex[0].r);

                glDrawArrays(GL_TRIANGLE_STRIP,0,sizeof(vertex) / sizeof(vertex[0]) );

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

            char*   szName[]    =   
            {
                "data/image/tex33X32.bmp",
                "data/image/tex16X16.bmp",
                "data/image/tex8x8.bmp",
                "data/image/tex4x4.bmp",
                "data/image/tex2X2.bmp",
                "data/image/tex1x1.bmp",
            };
            _textureId      =   buildMipMap(szName,6);
            _texDynamic     =   createTexture(_width,_height);
            _camera._eye    =   CELL::float3(1, 1, 1);
            _camera._look   =   CELL::float3(0.5f, -0.4f, -5.5f);
            _camera._up     =   CELL::float3(0.0f, 1.0f, 0.0f);
            _camera._right  =   CELL::float3(1.0f, 0.0f, 0.0f);


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
