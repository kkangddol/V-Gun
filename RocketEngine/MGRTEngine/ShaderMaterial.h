#pragma once
#include <DirectXMath.h>
#include <functional>

// #TODO: Shader Layer 만들기, Material Layer 만들기, Renderer 돌리기!
// #TODO2 : ASSIMP 연결, FBX 로드할 수 있게 만들어보자!
/// 셰이더, Texture, 모두가 Unity Shader Graph처럼 받을 준비를 하고 있어야.

namespace RocketCore::Graphics
{
	class Texture;
	class Shader;
	class ShaderUpdater;
	class ModelData;
	class BaseRenderable;

	class ShaderMaterial
	{
		friend class ResourceManager;
	public:
		//Shader 들어오면 shader 할당, nullptr 들어오면 Default Shader 만들어주는 구조.
		ShaderMaterial(Shader* _shader);
		virtual ~ShaderMaterial();

		//셰이더 리턴.
		Shader* GetShader();
		
	private:
		Shader* m_Shader = nullptr;
		
	};
}


