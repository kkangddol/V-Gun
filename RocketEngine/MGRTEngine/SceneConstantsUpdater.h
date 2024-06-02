#pragma once
#include "../RocketGraphicsInterface/RenderConstantData.h"
#include <DirectXMath.h>
#include <string>
#include <vector>

///[TW] Shader.h를 쓰지 않고, 독자적으로 셰이더를 업데이트해야 한다.
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
		//SceneInformation.fx랑 연동되어야 한다.
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

		//씬이 시작될 때, Shader의 값을 정리해서 올려 놓는다.
		void PlaceShaderConstants(RenderConstantData* rcData);

		//오브젝트가 바뀔 때마다, SpotLight, PointLight을 위치에 따른 밝기 순서 정렬 필요.
		void AdjustShaderConstantsPerObject(DirectX::XMFLOAT3 objPosition);

		///템플릿 특수화될 코드에서 활용할 요소들. (만약 얘네들이 빛을 사용한다면)
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


