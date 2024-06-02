#pragma once
#include <wrl.h>
namespace RocketCore::Graphics
{
	///일단, 아예 다른 칸에 넣는 방식으로 구성하자!
	///Texture 자체 딴에서 구별은 필요하다면!
	//enum class eTextureType
	//{
	//	DEFAULT, //기본 흰색 텍스쳐를 매핑할 계획.
	//	DIFFUSE, //Diffuse/Albedo 매핑용
	//	NORMAL, //Normal Mapping 용
	//	SHADOW, //Shadow Mapping 용
	//	DISPLACEMENT, // Displacement Mapping용.
	//};
		
	//Shader Variables 활용하는 법.
	enum class eShaderVarType
	{
		_BLEND,
		_CLASS_INSTANCE,
		_CONSTANT_BUFFER,
		_DEPTH_STENCIL,
		_DEPTH_STENCIL_VIEW,
		_INTERFACE,
		_MATRIX,
		_RASTERIZER,
		_RENDER_TARGET_VIEW,
		_SAMPLER,
		_SCALAR_BOOL,
		_SCALAR_INT,
		_SCALAR_FLOAT,
		_SHADER,
		_SHADER_RESOURCE,
		_STRING,
		_UNORDERED_ACCESS_VIEW,
		_VECTOR,
		_USER_DEFINED_TYPE //유저가 임의로 만든 타입.
	};

}

