#pragma once
#include "DLLExporter.h"
#include "Component.h"
#include "..\\RocketGraphicsInterface\\ModelData.h"
#include "..\\RocketGraphicsInterface\\MaterialData.h"
#include "..\\RocketGraphicsInterface\\RenderType.h" //[TW]

#include "..\\RocketMath\\RocketMath.h"

#ifdef _DEBUG
#pragma comment(lib,"..\\x64\\Debug\\RocketMath.lib")
#else
#pragma comment(lib,"..\\x64\\Release\\RocketMath.lib")
#endif // _DEBUG


namespace RocketEngine
{
	class GameObject;
}

namespace RocketCore
{

	/// <summary>
	/// Renderer 컴포넌트들이 상속받을 인터페이스.
	/// 근데 얘는 abstract인데 Component 베이스클래스를 상속받아도 되는건가?
	/// 
	/// 23.06.29 강석원 인재원.
	/// </summary>
	class ROCKET_API MeshRendererBase : public RocketEngine::Component
	{
	public:
		MeshRendererBase(RocketEngine::GameObject* owner);

	public:
		Graphics::ModelData& GetModelData();
		Graphics::LegacyMaterialData& GetLegacyMaterialData();
		Graphics::PBRMaterialData& GetPBRMaterialData();

	public:
		void SetMeshPath(std::string path);
		void SetShaderPath(std::string path);
		void SetDiffusePath(std::string path);
		void SetNormalPath(std::string path);
		void SetRenderType(RocketCore::Graphics::eRenderType renderType);  //[TW] 
		void SetARMPath(std::string path);

		

		std::string GetMeshPath() const;
		std::string GetShaderPath() const;
		std::string GetDiffusePath() const;
		std::string GetNormalPath() const;
		RocketCore::Graphics::eRenderType GetRenderType() const; //[TW] 
		std::string GetARMPath() const;

	private:
		Graphics::ModelData _modelData;

	public:
		void SetLegacyAmbient(RocketEngine::RMFLOAT4 ambient);
		void SetLegacyDiffuse(RocketEngine::RMFLOAT4 diffuse);
		void SetLegacySpecular(RocketEngine::RMFLOAT4 specular);
		void SetLegacyReflect(RocketEngine::RMFLOAT4 reflect);

		RocketEngine::RMFLOAT4 GetLegacyAmbient() const;
		RocketEngine::RMFLOAT4 GetLegacyDiffuse() const;
		RocketEngine::RMFLOAT4 GetLegacySpecular() const;
		RocketEngine::RMFLOAT4 GetLegacyReflect() const;

	private:
		Graphics::LegacyMaterialData _legacyMaterialData;

	public:
		void SetPBRAlbedo(RocketEngine::RMFLOAT4 albedo);
		void SetPBRMetallic(float metallic);
		void SetPBRRoughness(float roughness);
		void SetPBRSpecular(float specular);
		void SetPBREmission(float emission);

		RocketEngine::RMFLOAT4 GetPBRAlbedo() const;
		float GetPBRMetallic() const;
		float GetPBRRoughness() const;
		float GetPBRSpecular() const;
		float GetPBREmission() const;

	private:
		Graphics::PBRMaterialData _PBRMaterialData;
	};
}
