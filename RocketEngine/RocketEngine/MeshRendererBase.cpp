#include "MeshRendererBase.h"
#include "EngineProcess.h"
#include "GameObject.h"

namespace RocketCore
{
	MeshRendererBase::MeshRendererBase(RocketEngine::GameObject* owner)
		: Component(owner)
	{

	}

	RocketCore::Graphics::ModelData& MeshRendererBase::GetModelData()
	{
		_modelData.objName = gameObject->objName;
		return _modelData;
	}

	RocketCore::Graphics::LegacyMaterialData& MeshRendererBase::GetLegacyMaterialData()
	{
		return _legacyMaterialData;
	}

	RocketCore::Graphics::PBRMaterialData& MeshRendererBase::GetPBRMaterialData()
	{
		return _PBRMaterialData;
	}

	void MeshRendererBase::SetMeshPath(std::string path)
	{
		_modelData.fbxPath = path;
	}

	void MeshRendererBase::SetShaderPath(std::string path)
	{
		_modelData.shaderPath = path;
	}

	void MeshRendererBase::SetDiffusePath(std::string path)
	{
		_modelData.diffusePath = path;
	}

	void MeshRendererBase::SetNormalPath(std::string path)
	{
		_modelData.normalPath = path;
	}

	void MeshRendererBase::SetRenderType(RocketCore::Graphics::eRenderType renderType)
	{
		_modelData.renderingType = renderType;
	}

	void MeshRendererBase::SetARMPath(std::string path)
	{
		_modelData.armPath = path;
	}

	std::string MeshRendererBase::GetMeshPath() const
	{
		return _modelData.fbxPath;
	}

	std::string MeshRendererBase::GetShaderPath() const
	{
		return _modelData.shaderPath;
	}

	std::string MeshRendererBase::GetDiffusePath() const
	{
		return _modelData.diffusePath;
	}

	std::string MeshRendererBase::GetNormalPath() const
	{
		return _modelData.normalPath;
	}

	RocketCore::Graphics::eRenderType MeshRendererBase::GetRenderType() const
	{
		return _modelData.renderingType;
	}

	std::string MeshRendererBase::GetARMPath() const
	{
		return _modelData.armPath;
	}

	void MeshRendererBase::SetLegacyAmbient(RocketEngine::RMFLOAT4 ambient)
	{
		_legacyMaterialData._ambient = ambient;
	}

	void MeshRendererBase::SetLegacyDiffuse(RocketEngine::RMFLOAT4 diffuse)
	{
		_legacyMaterialData._diffuse = diffuse;
	}

	void MeshRendererBase::SetLegacySpecular(RocketEngine::RMFLOAT4 specular)
	{
		_legacyMaterialData._specular = specular;
	}

	void MeshRendererBase::SetLegacyReflect(RocketEngine::RMFLOAT4 reflect)
	{
		_legacyMaterialData._reflect = reflect;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetLegacyAmbient() const
	{
		return _legacyMaterialData._ambient;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetLegacyDiffuse() const
	{
		return _legacyMaterialData._diffuse;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetLegacySpecular() const
	{
		return _legacyMaterialData._specular;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetLegacyReflect() const
	{
		return _legacyMaterialData._reflect;
	}

	void MeshRendererBase::SetPBRAlbedo(RocketEngine::RMFLOAT4 albedo)
	{
		_PBRMaterialData._albedo = albedo;
	}

	void MeshRendererBase::SetPBRMetallic(float metallic)
	{
		_PBRMaterialData._metallic = metallic;
	}

	void MeshRendererBase::SetPBRRoughness(float roughness)
	{
		_PBRMaterialData._roughness = roughness;
	}

	void MeshRendererBase::SetPBRSpecular(float specular)
	{
		_PBRMaterialData._specular = specular;
	}

	void MeshRendererBase::SetPBREmission(float emission)
	{
		_PBRMaterialData._emission = emission;
	}

	RocketEngine::RMFLOAT4 MeshRendererBase::GetPBRAlbedo() const
	{
		return _PBRMaterialData._albedo;
	}

	float MeshRendererBase::GetPBRMetallic() const
	{
		return _PBRMaterialData._metallic;
	}

	float MeshRendererBase::GetPBRRoughness() const
	{
		return _PBRMaterialData._roughness;
	}

	float MeshRendererBase::GetPBRSpecular() const
	{
		return _PBRMaterialData._specular;
	}

	float MeshRendererBase::GetPBREmission() const
	{
		return _PBRMaterialData._emission;
	}
}
