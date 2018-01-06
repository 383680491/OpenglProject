#pragma once

#include <gles2/gl2.h>
#include <assert.h>


namespace   CELL
{
    /**
    *   时间
    */
    class   FrameEvent    //给定时器使用的
    {
    public:
        /**
        *   单位秒
        */
        float    _sinceLastFrame;
        /**
        *   单位秒
        */
        float    _sinceLastEvent;
    };
	/*
		将shade处理分为了3个部分  
			1、  ShaderId 是指shader代码传给ES 编译之后的返回值
			2、  ProgramId 是指 创建一个着色器 然后 Attach 顶点shader和 片源shader
			3、  Program 代码里面真正使用的着色器的逻辑操作，将shader代码API化，方便后续代码使用
	*/

	/*
		关联vsh 和 psh后生成的 GLSL语言 ID
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
		生成着色器ID
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
        *   舒适化函数，做基本的OpenGL和应用程序直接的接口
        */
        virtual void    initialize(CELLOpenGL* device);
        /**
        *   使用程序
        */
        virtual void    begin();
        /**
        *   使用完成
        */
        virtual void    end();
    };

    /**
    *   图片纹理数据格式
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
        *   清空缓冲区函数
        */
        static  void    clear(unsigned mask);
        /**
        *   指定清空颜色
        */
        static  void    clearColor(float r,float g,float b,float a);
        /**
        *   设置视口
        */
        static  void    setViewport(int x,int y,int width,int height);
        /**
        *   创建程序
        */
        static  ProgramId   createProgram(const char* vertex,const char* fragment);
        /**
        *   销毁程序
        */
        static  void    destroyProgram(ProgramId& prg);
        /**
        *   使用程序
        */
        static  void    useProgram(const ProgramId* program);

        static  int     getUniformLocation(const ProgramId& program,const char *name);  

        static  int     getAttribLocation(const ProgramId& program,const char* name);

        static  void    drawArray( unsigned mode,int first,int count );

        /**
        *   获取shader中的属性字段
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
        *   创建2D纹理
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
        *   通过文件加载文理
        */
        static Texture2dId  createTexture2DFromFile(const char* fileName);

        static  void        bindTexture2D(  const Texture2dId* textureId,int stage);

        static  void        enableState(unsigned  state);
        static  void        blendFunction(unsigned sfactor, unsigned dfactor);


    };
}