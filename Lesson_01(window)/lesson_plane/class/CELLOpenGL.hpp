#pragma once

#include <gles2/gl2.h>
#include <assert.h>


namespace   CELL
{
    /**
    *   ʱ��
    */
    class   FrameEvent    //����ʱ��ʹ�õ�
    {
    public:
        /**
        *   ��λ��
        */
        float    _sinceLastFrame;
        /**
        *   ��λ��
        */
        float    _sinceLastEvent;
    };
	/*
		��shade�����Ϊ��3������  
			1��  ShaderId ��ָshader���봫��ES ����֮��ķ���ֵ
			2��  ProgramId ��ָ ����һ����ɫ�� Ȼ�� Attach ����shader�� ƬԴshader
			3��  Program ������������ʹ�õ���ɫ�����߼���������shader����API���������������ʹ��
	*/

	/*
		����vsh �� psh�����ɵ� GLSL���� ID
	*/
    class    ShaderId
    {
    public:
        ShaderId()
        {
            _shaderId   =   -1;
        }
        int _shaderId;
    };
	/*
		������ɫ��ID
	*/
    class    ProgramId
    {
    public:
        ProgramId()
        {
            _programId  =   -1;
        }
        int         _programId;
        ShaderId    _vertex;
        ShaderId    _fragment;
    };

    class   CELLOpenGL;
    class   Program :public ProgramId
    {
    public:
        typedef     int uniform;
        typedef     int atrribute;
    protected:
        CELLOpenGL* _device;
    public:
        Program();

        virtual ~Program();

        virtual	void 	destroy();
        /**
        *   ���ʻ���������������OpenGL��Ӧ�ó���ֱ�ӵĽӿ�
        */
        virtual void    initialize(CELLOpenGL* device);
        /**
        *   ʹ�ó���
        */
        virtual void    begin();
        /**
        *   ʹ�����
        */
        virtual void    end();
    };

    /**
    *   ͼƬ�������ݸ�ʽ
    */
    enum	PIXEL_FORMAT
    {
        PIXEL_UNKNOW	,
        PIXEL_I8		,
        PIXEL_A8        ,
        PIXEL_R8G8B8	,
        PIXEL_R8G8B8A8	,
        PIXEL_B8G8R8	,
        PIXEL_B8G8R8A8	,
        PIXEL_B8G8R8E8	,
        PIXEL_R8G8B8E8	,			//!	hdr
        PIXEL_RGBA4     ,           //! 16bit
        PIXEL_IA8       ,           //! 16bit
        PIXEL_RGB565    ,           //! 16bit
        PIXEL_I16       ,           //! 16bit
        PIXEL_RGB10A2   ,           //! 32bit 
        PIXEL_YUY2		,
        PIXEL_YUYV		,
        PIXEL_UYVY		,
        PIXEL_RGBA_S3TC_DXT1,
        PIXEL_RGBA_S3TC_DXT3,
        PIXEL_RGBA_S3TC_DXT5,
        PIXEL_FORCE_DWORD  =   0x7fffffff,
    };


    class   Texture2dId
    {
    public:
        Texture2dId()
        {
            _texture    =   0;
            _width      =   0;
            _height     =   0;
            _user       =   0;
        }
        unsigned    _texture;
        unsigned    _width;
        unsigned    _height;
        void*       _user;
    };

    class   CELLOpenGL
    {
    public:
        /**
        *   ��ջ���������
        */
        static  void    clear(unsigned mask);
        /**
        *   ָ�������ɫ
        */
        static  void    clearColor(float r,float g,float b,float a);
        /**
        *   �����ӿ�
        */
        static  void    setViewport(int x,int y,int width,int height);
        /**
        *   ��������
        */
        static  ProgramId   createProgram(const char* vertex,const char* fragment);
        /**
        *   ���ٳ���
        */
        static  void    destroyProgram(ProgramId& prg);
        /**
        *   ʹ�ó���
        */
        static  void    useProgram(const ProgramId* program);

        static  int     getUniformLocation(const ProgramId& program,const char *name);  

        static  int     getAttribLocation(const ProgramId& program,const char* name);

        static  void    drawArray( unsigned mode,int first,int count );

        /**
        *   ��ȡshader�е������ֶ�
        */
        static  void    enableVertexAttributeArray(unsigned attrIndex);
        static  void    disableVertexAttributeArray(unsigned attrIndex);

        static  void    attributePointer( int index, int size, int type, int normalized, int stride, const void* pointer );

        static  void    setUniform1f(int index,float v0);
        static  void    setUniform2f(int index,float v0, float v1);
        static  void    setUniform3f(int index,float v0, float v1, float v2);;
        static  void    setUniform4f(int index,float v0, float v1, float v2, float v3);;

        static  void    setUniform1i(int index,int v0);
        static  void    setUniform2i(int index,int v0, int v1);
        static  void    setUniform3i(int index,int v0, int v1, int v2);
        static  void    setUniform4i(int index,int v0, int v1, int v2, int v3);

        // Arrays
        static  void    setUniform1fv(int index,int count, const float *value);
        static  void    setUniform2fv(int index,int count, const float *value);
        static  void    setUniform3fv(int index,int count, const float *value);
        static  void    setUniform4fv(int index,int count, const float *value);

        static  void    setUniform1iv(int index,int count, const int *value);
        static  void    setUniform2iv(int index,int count, const int *value);
        static  void    setUniform3iv(int index,int count, const int *value);
        static  void    setUniform4iv(int index,int count, const int *value);

        static  void    setUniformMatrix2fv(int index,int count, bool transpose, const float *value);
        static  void    setUniformMatrix3fv(int index,int count, bool transpose, const float *value);
        static  void    setUniformMatrix4fv(int index,int count, bool transpose, const float *value);

        /**
        *   ����2D����
        */
        static  Texture2dId createTexure2D(
                                            int level,
                                            int intFmt,
                                            int width,
                                            int height,
                                            int border,
                                            int srcFmt,
                                            int type,
                                            const void* data,
                                            void* userData
                                            );

        static  Texture2dId createTexture2DFromStream(const char* stream,unsigned length);
        /**
        *   ͨ���ļ���������
        */
        static Texture2dId  createTexture2DFromFile(const char* fileName);

        static  void        bindTexture2D(  const Texture2dId* textureId,int stage);

        static  void        enableState(unsigned  state);
        static  void        blendFunction(unsigned sfactor, unsigned dfactor);


    };
}