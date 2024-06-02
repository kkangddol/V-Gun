#include "ShaderMaterial.h"
#include "../RocketGraphicsInterface/ModelData.h"
#include <d3d11.h>
#include "Texture.h"
#include "Shader.h"
#include "ShaderUpdater.h"
#include "BaseRenderable.h"
#include "ResourceManager.h"

namespace RocketCore::Graphics
{
	ShaderMaterial::ShaderMaterial(Shader* _shader) : m_Shader(nullptr)
	{
		if (_shader == nullptr)
		{
			assert(false);
		}
		else
		{
			//Shader를 할당하는 부분.
			m_Shader = _shader;
		}
	}

	ShaderMaterial::~ShaderMaterial()
	{
		
	}

	RocketCore::Graphics::Shader* ShaderMaterial::GetShader()
	{
		return m_Shader;
	}

	
}
