#pragma once
#include "TypedefGraphicHelper.h"
#include <string>


namespace RocketCore::Graphics
{
	class ShaderMaterial;

	//셰이더를 제한 없이 돌릴 수는 없이, (셰이더에 따른 커스텀 설정이 힘드니) => If/Else문에 쓰일 구문들을 동작시킨다!
	class CustomFXInstances
	{
	public:
		///Forward
		//개별적으로 Shader 위한 함수들을 만들기, 뭉친 코드 덩어리를 만들지 않기 위해서. 
		//MakeShaderInput_BasicTexFX(); // 이런 양상으로 갈 것.
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


