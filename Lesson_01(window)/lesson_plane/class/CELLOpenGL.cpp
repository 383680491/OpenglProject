#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CELLOpenGL.hpp"
#include "CELLImageReader.hpp"



namespace   CELL
{

    char*	readFile(const char* fileName,unsigned& length)
    {
        char    szFile[256];
        sprintf_s(szFile,"%s",fileName);
		FILE*   pFile = NULL;
		errno_t err = fopen_s(&pFile, fileName, "rb");
		if (0 == err)
        {
            fseek( pFile, 0, SEEK_END );
            length = ftell( pFile );
            fseek(pFile, 0, SEEK_SET );
            char* buffer =   new char[length + 1];
            fread( buffer, 1, length, pFile );
            buffer[length] = 0;
            fclose( pFile );
            return  buffer;
        }
        else
        {
            char    buffer[256];
            sprintf_s(buffer,"read %s  failed",fileName);
            assert(pFile != 0 && buffer);
        }
        return  0;

    }

    GLenum      getSrcFormat(const char* fmt)
    {
        if (strcmp(fmt,"PIXEL_R8G8B8A8") == 0)
        {
            return  GL_RGBA;
        }
        else if(strcmp(fmt,"PIXEL_R8G8B8") == 0)
        {
            return  GL_RGB;
        }
        else if(strcmp(fmt,"PIXEL_RGBA4") == 0)
        {
            return  GL_RGBA;
        }
        else if(strcmp(fmt,"PIXEL_RGB565") == 0)
        {
            return  GL_RGB;
        }
        else if(strcmp(fmt,"PIXEL_A8") == 0)
        {
            return  GL_ALPHA;
        }
        else if(strcmp(fmt,"PIXEL_I8") == 0)
        {
            return  GL_LUMINANCE;
        }
        else if(strcmp(fmt,"PIXEL_IA8") == 0)
        {
            return  GL_LUMINANCE_ALPHA;
        }
        else
        {
            return  GL_RGB;
        }
    }

    GLenum      getSrcType(const char* fmt)
    {
        if (strcmp(fmt,"PIXEL_RGBA4") == 0)
        {
            return  GL_UNSIGNED_SHORT_4_4_4_4;
        }
        if (strcmp(fmt,"PIXEL_RGB565") == 0)
        {
            return  GL_UNSIGNED_SHORT_5_6_5;
        }
        else
        {
            return  GL_UNSIGNED_BYTE;
        }
    }


    GLenum      getSrcFormat(PIXEL_FORMAT fmt)
    {
        switch(fmt)
        {
        case PIXEL_A8:
            return  GL_ALPHA;
        case PIXEL_I8:
            return  GL_LUMINANCE;
        case PIXEL_IA8:
            return  GL_LUMINANCE_ALPHA;
        case PIXEL_R8G8B8:
            return  GL_RGB;
        case PIXEL_R8G8B8A8:
            return  GL_RGBA;
        case PIXEL_YUYV:
        case PIXEL_UYVY:
        case PIXEL_YUY2:
            return  GL_LUMINANCE_ALPHA;
        default:
            return  GL_LUMINANCE_ALPHA;
        }
    }
    GLenum      getSrcType(PIXEL_FORMAT fmt)
    {
        switch(fmt)
        {
        case PIXEL_R8G8B8:
        case PIXEL_R8G8B8A8:
        case PIXEL_B8G8R8:
        case PIXEL_B8G8R8A8:
            return	GL_UNSIGNED_BYTE;
        case PIXEL_YUYV:
        case PIXEL_UYVY:
        case PIXEL_YUY2:
            return	GL_UNSIGNED_BYTE;
        default:
            return	GL_UNSIGNED_BYTE;
        }
    }


    GLenum      getInternalFormat(const char* fmt)
    {
        if (strcmp(fmt,"PIXEL_R8G8B8A8") == 0)
        {
            return  GL_RGBA;
        }
        else if(strcmp(fmt,"PIXEL_R8G8B8") == 0)
        {
            return  GL_RGB;
        }
        else if(strcmp(fmt,"PIXEL_RGBA4") == 0)
        {
            return  GL_RGBA4;
        }
        else if(strcmp(fmt,"PIXEL_A8") == 0)
        {
            return  GL_ALPHA;
        }
        else if(strcmp(fmt,"PIXEL_I8") == 0)
        {
            return  GL_LUMINANCE;
        }
        else if(strcmp(fmt,"PIXEL_IA8") == 0)
        {
            return  GL_LUMINANCE_ALPHA;
        }
        else
        {
            return  GL_RGB;
        }
    }

    GLenum      getInternalFormat(PIXEL_FORMAT fmt)
    {
        switch(fmt)
        {
        case PIXEL_A8:
            return  GL_ALPHA;
        case PIXEL_I8:
            return  GL_LUMINANCE;
        case PIXEL_IA8:
            return  GL_LUMINANCE_ALPHA;
        case PIXEL_R8G8B8:
            return	GL_RGB;
        case PIXEL_R8G8B8A8:
            return	GL_RGBA;
        case PIXEL_B8G8R8:
            return	GL_RGB;
        case PIXEL_B8G8R8A8:
            return	GL_RGBA;
        case PIXEL_YUYV:
        case PIXEL_UYVY:
        case PIXEL_YUY2:
            return	GL_LUMINANCE_ALPHA;
        default:
            return	GL_LUMINANCE_ALPHA;
        }
    }



    Program::Program() :_device(0)
    {

    }

    Program::~Program()
    {
        destroy();
    }


    void Program::destroy()
    {
        if (_device)
        {
            _device->destroyProgram(*this);
        }
    }


    void Program::initialize( CELLOpenGL*)
    {
    }

    void Program::begin()
    {
    }

    void Program::end()
    {
    }

    void CELLOpenGL::clear(unsigned mask)
    {
        //! GL_DEPTH_BUFFER_BIT
        //! GL_COLOR_BUFFER_BIT
        //! GL_COLOR_BUFFER_BIT
        //! GL_STENCIL_BUFFER_BIT
        //! GL_ACCUM_BUFFER_BIT;
        //! GL_TRANSFORM_BIT
        //! GL_ENABLE_BIT
        //! GL_HINT_BIT
        //! GL_EVAL_BIT
        glClear(mask);
    }

    void CELLOpenGL::clearColor(float r,float g,float b,float a)
    {
        glClearColor(r,g,b,a);
    }

    void CELLOpenGL::setViewport( int x,int y,int width,int height )
    {
        glViewport(x,y,width,height);
    }


    ProgramId CELLOpenGL::createProgram( const char* vertex,const char* fragment )
    {
        ProgramId   program;
        bool        error   =   false;
        do 
        {
            if (vertex)
            {
                program._vertex._shaderId   = glCreateShader( GL_VERTEX_SHADER );
                glShaderSource( program._vertex._shaderId, 1, &vertex, 0 );
                glCompileShader( program._vertex._shaderId );

                GLint   compileStatus;
                glGetShaderiv( program._vertex._shaderId, GL_COMPILE_STATUS, &compileStatus );
                error   =   compileStatus == GL_FALSE;
                if( error )
                {
                    GLchar messages[256];
                    glGetShaderInfoLog( program._vertex._shaderId, sizeof(messages), 0,messages);
                    assert( messages && 0 != 0);
                    break;
                }
            }
            if (fragment)
            {
                program._fragment._shaderId   = glCreateShader( GL_FRAGMENT_SHADER );
                glShaderSource( program._fragment._shaderId, 1, &fragment, 0 );
                glCompileShader( program._fragment._shaderId );

                GLint   compileStatus;
                glGetShaderiv( program._fragment._shaderId, GL_COMPILE_STATUS, &compileStatus );
                error   =   compileStatus == GL_FALSE;

                if( error )
                {
                    GLchar messages[256];
                    glGetShaderInfoLog( program._fragment._shaderId, sizeof(messages), 0,messages);
                    assert( messages && 0 != 0);
                    break;
                }
            }

            program._programId  =   glCreateProgram( );

            if (program._vertex._shaderId)
            {
                glAttachShader( program._programId, program._vertex._shaderId);
            }
            if (program._fragment._shaderId)
            {
                glAttachShader( program._programId, program._fragment._shaderId);
            }

            glLinkProgram( program._programId );

            GLint linkStatus;
            glGetProgramiv( program._programId, GL_LINK_STATUS, &linkStatus );
            if (linkStatus == GL_FALSE)
            {
                GLchar messages[256];
                glGetProgramInfoLog( program._programId, sizeof(messages), 0, messages);
                break;
            }
            glUseProgram(program._programId);

        } while(false);

        if (error)
        {
            if (program._fragment._shaderId != -1)
            {
                glDeleteShader(program._fragment._shaderId);
                program._fragment._shaderId =   0;
            }
            if (program._vertex._shaderId != -1)
            {
                glDeleteShader(program._vertex._shaderId);
                program._vertex._shaderId   =   0;
            }
            if (program._programId != -1)
            {
                glDeleteProgram(program._programId);
                program._programId          =   0;
            }
        }
        return  program;
    }

    void CELLOpenGL::destroyProgram( ProgramId& program )
    {
        if (program._fragment._shaderId)
        {
            glDeleteShader(program._fragment._shaderId);
            program._fragment._shaderId =   -1;
        }
        if (program._vertex._shaderId)
        {
            glDeleteShader(program._vertex._shaderId);
            program._vertex._shaderId   =   -1;
        }
        if (program._programId)
        {
            glDeleteProgram(program._programId);
            program._programId          =   -1;
        }
    }
    void CELLOpenGL::useProgram( const ProgramId* program )
    {
        if (program)
        {
            glUseProgram(program->_programId);
        }
        else
        {
            glUseProgram(0);
        }
    }

    int CELLOpenGL::getUniformLocation( const ProgramId& program,const char *name )
    {
        return  glGetUniformLocation(program._programId, name);
    }

    int CELLOpenGL::getAttribLocation( const ProgramId& program,const char* name )
    {
        return  glGetAttribLocation(program._programId,name);
    }


    void CELLOpenGL::setUniform1f( int index,float v0 )
    {
        glUniform1f(index, v0);
    }

    void CELLOpenGL::setUniform2f( int index,float v0, float v1 )
    {
        glUniform2f(index, v0,v1);
    }

    void CELLOpenGL::setUniform3f( int index,float v0, float v1, float v2 )
    {
        glUniform3f(index, v0,v1,v2);
    }

    void CELLOpenGL::setUniform4f( int index,float v0, float v1, float v2, float v3 )
    {
        glUniform4f(index, v0,v1,v2,v3);
    }

    void CELLOpenGL::setUniform1i( int index,int v0 )
    {
        glUniform1i(index, v0);
    }

    void CELLOpenGL::setUniform2i( int index,int v0, int v1 )
    {
        glUniform2i(index, v0,v1);
    }

    void CELLOpenGL::setUniform3i( int index,int v0, int v1, int v2 )
    {
        glUniform3i(index, v0,v1,v2);
    }

    void CELLOpenGL::setUniform4i( int index,int v0, int v1, int v2, int v3 )
    {
        glUniform4i(index, v0,v1,v2,v3);
    }

    void CELLOpenGL::setUniform1fv( int index,int count, const float *value )
    {
        glUniform1fv(index, count, value);
    }

    void CELLOpenGL::setUniform2fv( int index,int count, const float *value )
    {
        glUniform2fv(index, count, value);
    }

    void CELLOpenGL::setUniform3fv( int index,int count, const float *value )
    {
        glUniform3fv(index, count, value);
    }

    void CELLOpenGL::setUniform4fv( int index,int count, const float *value )
    {
        glUniform4fv(index, count, value);
    }

    void CELLOpenGL::setUniform1iv( int index,int count, const int *value )
    {
        glUniform1iv(index, count, value);
    }

    void CELLOpenGL::setUniform2iv( int index,int count, const int *value )
    {
        glUniform2iv(index, count, value);
    }

    void CELLOpenGL::setUniform3iv( int index,int count, const int *value )
    {
        glUniform3iv(index, count, value);
    }

    void CELLOpenGL::setUniform4iv( int index,int count, const int *value )
    {
        glUniform4iv(index, count, value);
    }

    void CELLOpenGL::setUniformMatrix2fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix2fv(index, count, (GLboolean) transpose, value);
    }

    void CELLOpenGL::setUniformMatrix3fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix3fv(index, count, (GLboolean) transpose, value);
    }

    void CELLOpenGL::setUniformMatrix4fv( int index,int count, bool transpose, const float *value )
    {
        glUniformMatrix4fv(index, count, (GLboolean) transpose, value);
    }

    void CELLOpenGL::attributePointer( int index, int size, int type, int normalized, int stride, const void* pointer )
    {
        glVertexAttribPointer(index,size,type,(GLboolean)normalized,stride,pointer);
    }

    void CELLOpenGL::enableVertexAttributeArray( unsigned attrIndex )
    {
        glEnableVertexAttribArray(attrIndex);
    }

    void CELLOpenGL::disableVertexAttributeArray( unsigned attrIndex )
    {
        glDisableVertexAttribArray(attrIndex);
    }

    void CELLOpenGL::drawArray( unsigned mode,int first,int count )
    {
        glDrawArrays(mode,first,count);
    }

    
    Texture2dId CELLOpenGL::createTexure2D(
        int level,
        int intFmt,
        int width,
        int height,
        int border,
        int srcFmt,
        int type,
        const void* data,
        void* userData
        )
    {
        Texture2dId textureId;
        textureId._width    =   width;
        textureId._height   =   height;
        textureId._user     =   userData;
        glGenTextures(1,&textureId._texture);
        glBindTexture( GL_TEXTURE_2D, textureId._texture );
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

        glTexImage2D(
            GL_TEXTURE_2D, 
            level, 
            intFmt,
            width, 
            height,
            border, 
            srcFmt , 
            type,
            data
            );
        return  textureId;
    }

    CELL::Texture2dId CELLOpenGL::createTexture2DFromStream( const char* stream,unsigned length )
    {
        Texture2dId texId;
        CELLImageReader reader((char*)stream,(int)length);
        if (!reader.parse())
        {
            return  texId;
        }

        glGenTextures(1, &texId._texture);
        glBindTexture(GL_TEXTURE_2D, texId._texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


        ImageHeader*header  =   (ImageHeader*)reader.getCursor();
        texId._width        =   (unsigned)header->getWidth();
        texId._height       =   (unsigned)header->getHeight();


        GLenum  intFormat   =   getInternalFormat(reader.getFormat());
        GLenum  srcType     =   getSrcType(reader.getFormat());
        GLint   srcFmt      =   getSrcFormat(reader.getFormat());
        GLint   level       =   0;
        while(!reader.eof())
        {
            header  =   (ImageHeader*)reader.getCursor();
            reader.skip(sizeof(ImageHeader));
            char*           pixel   =   reader.getCursor();

            glTexImage2D(
                GL_TEXTURE_2D,
                level, 
                intFormat, 
                header->getWidth(),
                header->getHeight(), 
                0,
                srcFmt,
                srcType,
                pixel
                );
            ++level;
            int itt = glGetError();
            reader.skip(header->getLength());
        }
        return  texId;
    }


    CELL::Texture2dId CELLOpenGL::createTexture2DFromFile( const char* fileName )
    {
        Texture2dId texId;
        unsigned    length  =   0;
        char*       buffer  =   readFile(fileName,length);
        if (buffer)
        {
            texId   =   createTexture2DFromStream(buffer,length);
            delete [] buffer;
        }
        return  texId;
    }

    void CELLOpenGL::bindTexture2D( const Texture2dId* textureId,int stage )
    {
        if (textureId)
        {
            glActiveTexture(GL_TEXTURE0 + stage);
            glBindTexture(GL_TEXTURE_2D,textureId->_texture);
        }
        else
        {
            glActiveTexture(GL_TEXTURE0 + stage);
            glBindTexture(GL_TEXTURE_2D,0);
        }
    }

    void CELLOpenGL::enableState( unsigned state )
    {
        glEnable(state);
    }

    void CELLOpenGL::blendFunction( unsigned sfactor, unsigned dfactor )
    {
        glBlendFunc(sfactor,dfactor);
    }

}
