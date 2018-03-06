#pragma once
#include "CELLOpenGL.hpp"

namespace   CELL
{
    /**
    *   float2 + rgba
    */
    class   PROGRAM_P2_C4 :public Program   //顶点2个属性   颜色4个属性
    {
    public:
        atrribute   _position;
        uniform     _color;
        uniform     _MVP;
    public:
        PROGRAM_P2_C4()
        {
            _position   =   -1;
            _color      =   -1;
            _MVP        =   -1;
        }
        ~PROGRAM_P2_C4()
        {
        }
        atrribute   getPositionAttribute() const
        {
            return  _position;
        }
        uniform     getColorAttribute() const
        {
            return  _color;
        }
        uniform     getColorUniform()
        {
            return  _color;
        }
        uniform     getMVP() const 
        {
            return  _MVP;
        }
        /**
        *   舒适化函数，做基本的OpenGL和应用程序直接的接口
        */
        virtual void    initialize(CELLOpenGL* device)
        {
            _device =   device;
            const char* vs  =   
            {
                "precision lowp float; "
                "uniform   mat4 _MVP;"
                "attribute vec2 _position;"
                
                "void main()"
                "{"
                "   vec4    pos =   vec4(_position,0,1);"
                "   gl_Position =   _MVP * pos;"
                "}"
            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform    vec4 _color;"
                "void main()"
                "{"
                "   gl_FragColor   =   _color;"
                "}"
            };
            ProgramId&  programId   =   *this;
            programId   =   device->createProgram(vs,ps);
            _position   =   device->getAttribLocation(programId, "_position");
            _color      =   device->getUniformLocation(programId, "_color");
            _MVP        =   device->getUniformLocation(programId,"_MVP");
        }
        /**
        *   使用程序
        */
        virtual void    begin()
        {
            _device->useProgram(this);

            if (_position != -1)
            {
                _device->enableVertexAttributeArray(_position);
            }
        }
        /**
        *   使用完成
        */
        virtual void    end()
        {
            if (_position != -1)
            {
                _device->disableVertexAttributeArray(_position);
            }
            _device->useProgram(this);
        }

    };

    /**
    *   位置属性 float2 + uv2
    */
    class   PROGRAM_P2_UV2 :public Program
    {
    public:
        atrribute   _position;
        uniform     _uv;
        /**
        *   uniform
        */
        uniform     _MVP;
        uniform     _texture;
    public:
        PROGRAM_P2_UV2()
        {
            _position   =   -1;
            _uv         =   -1;
            _MVP        =   -1;
            _texture    =   -1;
        }

        virtual ~PROGRAM_P2_UV2()
        {}

        atrribute    getPositionAttribute() const
        {
            return  _position;
        }
        atrribute    getUVAttribute() const
        {
            return  _uv;
        }
        uniform     getMVPUniform() const
        {
            return  _MVP;
        }
        uniform     getTextureUniform() const
        {
            return  _texture;
        }
        /**
        *   舒适化函数，做基本的OpenGL和应用程序直接的接口
        */
        virtual void    initialize(CELLOpenGL* device)
        {
            _device =   device;
            const char* vs  =   
            {
                "precision  lowp float; "
                "uniform    mat4 _MVP;\n"
                "attribute  vec2 _position;\n"
                "attribute  vec2 _uv;\n"
                "varying    vec2 _outUV;\n"
                "void main()\n"
                "{"
                "   vec4    pos =   vec4(_position,0,1);\n"
                "   _outUV      =   _uv;"
                "   gl_Position =   _MVP * pos;\n"
                "}"
            };
            const char* ps  =   
            {
                "precision  lowp float; "
                "uniform    sampler2D   _texture;\n"
                "varying    vec2        _outUV;\n"
                "void main()\n"
                "{\n"
                "   vec4   color   =   texture2D(_texture,_outUV);\n"
                "   gl_FragColor   =   color;\n"
                "}\n"
            };
            ProgramId&  programId   =   *this;
            programId   =   device->createProgram(vs,ps);
            _position   =   device->getAttribLocation(programId,    "_position");
            _uv         =   device->getAttribLocation(programId,    "_uv");

            _texture    =   device->getUniformLocation(programId,   "_texture");
            _MVP        =   device->getUniformLocation(programId,   "_MVP");
        }
        /**
        *   使用程序
        */
        virtual void    begin()
        {
            _device->useProgram(this);
            if (_position != -1)
            {
                _device->enableVertexAttributeArray(_position);
            }
            if (_uv != -1)
            {
                _device->enableVertexAttributeArray(_uv);
            }
        }
        /**
        *   使用完成
        */
        virtual void    end()
        {
            if (_position != -1)
            {
                _device->disableVertexAttributeArray(_position);
            }
            if (_uv != -1)
            {
                _device->disableVertexAttributeArray(_uv);
            }
            _device->useProgram(0);
        }
    };

	class PROGRAME_P2_UV_C4 : public Program{
	public:
		atrribute _position;
		atrribute _uv;
		atrribute _color;
		uniform _MVP;
		uniform _texture;
	public:
		PROGRAME_P2_UV_C4()
		{
			_position = -1;
			_color = -1;
			_MVP = -1;
			_uv = -1;
			_texture = -1;
		}

		void initialize(CELLOpenGL* device)
		{
			_device = device;

			const char* vs = {
				"precision lowp float; "
				"uniform mat4 _MVP;"
				"attribute vec2 _position;"
				"attribute vec4 _color;"
				"attribute vec2 _uv;"
				"varying vec2 _outUV;"
				"varying vec4 _outColor;"

				"void main()"
				"{"
				"	vec4 pos = vec4(_position, 0, 1);"
				"	_outUV = _uv;"
				"	_outColor = _color;"
				"	gl_Position = _MVP * pos;"
				"}"
			};

			const char* ps = {
				"precision lowp float;"
				"uniform sampler2D _texture;"
				"varying vec2 _outUV;"
				"varying vec4 _outColor;"

				"void main()"
				"{"
				"	vec4 color = texture2D(_texture, _outUV);"
				"	color += _outColor;"
				"	gl_FragColor = color;"
				"}"

			};
			ProgramId& programId = *this;
			programId = _device->createProgram(vs, ps);
			_position = _device->getAttribLocation(programId, "_position");
			_color = _device->getAttribLocation(programId, "_color");
			_uv = _device->getAttribLocation(programId, "_uv");
			_texture = _device->getUniformLocation(programId, "_texture");
			_MVP = _device->getUniformLocation(programId, "_MVP");
		}

		void begin()
		{
			_device->useProgram(this);

			if (_position != -1){
				_device->enableVertexAttributeArray(_position);
			}

			if (_color != -1){
				_device->enableVertexAttributeArray(_color);
			}

			if (_uv != -1){
				_device->enableVertexAttributeArray(_uv);
			}
		}

		void end()
		{
			if (_position != -1){
				_device->disableVertexAttributeArray(_position);
			}

			if (_color != -1){
				_device->disableVertexAttributeArray(_color);
			}

			if (_uv != -1){
				_device->disableVertexAttributeArray(_uv);
			}

			_device->useProgram(0);
		}

	};

}