#pragma once

#include    "class/CELLOpenGL.hpp"
#include    "class/CELLProgramLibrary.hpp"
#include    <string>
#include    <map>


namespace   CELL
{
    class CELLResource 
    {
    public:
        typedef std::map<std::string,Texture2dId>   TextureMap;
    public:
        CELLOpenGL*         _device;
        PROGRAM_P2_C4       _PROGRAM_P2_C4;
        PROGRAM_P2_UV2      _PROGRAM_P2_UV2;
        TextureMap          _textureMap;
    public:

        CELLResource()
        {
            _device =   0;
        }
        void    initialize(CELLOpenGL& device)
        {
            _device =   &device;
            _PROGRAM_P2_C4.initialize(_device);
            _PROGRAM_P2_UV2.initialize(_device);
        }

        Texture2dId getTexture(const char* texture)
        {
            TextureMap::iterator itr = _textureMap.find(texture);
            if (itr == _textureMap.end())
            {
                Texture2dId texId   =   _device->createTexture2DFromFile(texture);
                _textureMap[texture]=   texId;
                return      texId;
            }
            else
            {
                return  itr->second;
            }
        }

        ~CELLResource(void)
        {
        }
    };
}
