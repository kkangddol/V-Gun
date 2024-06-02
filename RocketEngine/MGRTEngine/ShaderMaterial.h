#pragma once
#include <DirectXMath.h>
#include <functional>

// #TODO: Shader Layer �����, Material Layer �����, Renderer ������!
// #TODO2 : ASSIMP ����, FBX �ε��� �� �ְ� ������!
/// ���̴�, Texture, ��ΰ� Unity Shader Graphó�� ���� �غ� �ϰ� �־��.

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
		//Shader ������ shader �Ҵ�, nullptr ������ Default Shader ������ִ� ����.
		ShaderMaterial(Shader* _shader);
		virtual ~ShaderMaterial();

		//���̴� ����.
		Shader* GetShader();
		
	private:
		Shader* m_Shader = nullptr;
		
	};
}


