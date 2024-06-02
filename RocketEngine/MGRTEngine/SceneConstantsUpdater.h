#pragma once
#include "../RocketGraphicsInterface/RenderConstantData.h"
#include <DirectXMath.h>
#include <string>
#include <vector>

///[TW] Shader.h�� ���� �ʰ�, ���������� ���̴��� ������Ʈ�ؾ� �Ѵ�.
struct ID3DX11Effect;
struct ID3D11Buffer;
struct ID3D11UnorderedAccessView;

struct ID3DX11EffectVariable;
struct ID3DX11EffectScalarVariable;
struct ID3DX11EffectUnorderedAccessViewVariable;

namespace RocketCore::Graphics
{
	class RenderConstantData;

	class SceneConstantsUpdater
	{
	private:
		//SceneInformation.fx�� �����Ǿ�� �Ѵ�.
		enum { MAXIMUM_LIGHT_CALL_COUNT = 25 };

	public:
		static SceneConstantsUpdater* GetInstance()
		{
			if (instance == nullptr)
			{
				instance = new SceneConstantsUpdater();
				//atexit(DestroyD3D);
			}
			return instance;
		}

		//Scene Constants Variable : Shader Binding.
		void Initialize();

		//���� ���۵� ��, Shader�� ���� �����ؼ� �÷� ���´�.
		void PlaceShaderConstants(RenderConstantData* rcData);

		//������Ʈ�� �ٲ� ������, SpotLight, PointLight�� ��ġ�� ���� ��� ���� ���� �ʿ�.
		void AdjustShaderConstantsPerObject(DirectX::XMFLOAT3 objPosition);

		///���ø� Ư��ȭ�� �ڵ忡�� Ȱ���� ��ҵ�. (���� ��׵��� ���� ����Ѵٸ�)
		RenderConstantData* GetRenderConstantData();

		unsigned int GetDirLightCount();
		unsigned int GetPointLightCount();
		unsigned int GetSpotLightCount();

		unsigned int GetDirLightBrightIndex(unsigned int numIndex);
		unsigned int GetPointLightBrightIndex(unsigned int numIndex);
		unsigned int GetSpotLightBrightIndex(unsigned int numIndex);


	private:
		static SceneConstantsUpdater* instance;

		RenderConstantData* _renderConstantData = nullptr;

		SceneConstantsUpdater() {}
		SceneConstantsUpdater(const SceneConstantsUpdater& ref) {}
		SceneConstantsUpdater& operator=(const SceneConstantsUpdater& ref) {}
		~SceneConstantsUpdater() {}
		
		int _tempDirLightCount = 0;
		int _tempPointLightCount = 0;
		int _tempSpotLightCount = 0;
		
		std::vector<unsigned int> _tempPointIndexList;
		std::vector<unsigned int> _tempSpotIndexList;

	};
}


