#pragma once
#include "TypedefGraphicHelper.h"
#include <string>


namespace RocketCore::Graphics
{
	class ShaderMaterial;

	//���̴��� ���� ���� ���� ���� ����, (���̴��� ���� Ŀ���� ������ �����) => If/Else���� ���� �������� ���۽�Ų��!
	class CustomFXInstances
	{
	public:
		///Forward
		//���������� Shader ���� �Լ����� �����, ��ģ �ڵ� ����� ������ �ʱ� ���ؼ�. 
		//MakeShaderInput_BasicTexFX(); // �̷� ������� �� ��.
		static std::pair<ShaderMaterial*, AssetInputStruct> MakeShaderInput_BasicTexFX(const std::string& fullFilePath, const std::string& shaderName);
		static std::pair<ShaderMaterial*, AssetInputStruct> MakeShaderInput_SkinnedMeshTex(const std::string& fullFilePath, const std::string& shaderName);
		static std::pair<ShaderMaterial*, AssetInputStruct> MakeShaderInput_PosNormalTex(const std::string& fullFilePath, const std::string& shaderName);
	
		///Deferred
		static std::pair<ShaderMaterial*, AssetInputStruct> MakeShaderInput_DeferredPhongShader(const std::string& fullFilePath, const std::string& shaderName);
		static std::pair<ShaderMaterial*, AssetInputStruct> MakeShaderInput_DeferredPBRShader(const std::string& fullFilePath, const std::string& shaderName);
		static std::pair<ShaderMaterial*, AssetInputStruct> MakeShaderInput_DeferredSkinnedPBRShader(const std::string& fullFilePath, const std::string& shaderName);

		static std::pair<ShaderMaterial*, AssetInputStruct> MakeShaderInput_FinalCombinationShader(const std::string& fullFilePath, const std::string& shaderName);
	};
}


