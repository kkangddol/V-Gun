#include "ScreenBuffer.h"
#include "Shader.h"

namespace RocketCore::Graphics
{
	
	ScreenBuffer::ScreenBuffer(Shader* belongShader)
	{
		this->_belongShader = belongShader;
	}

}