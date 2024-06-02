#pragma once
#include <wrl.h>
namespace RocketCore::Graphics
{
	///�ϴ�, �ƿ� �ٸ� ĭ�� �ִ� ������� ��������!
	///Texture ��ü ������ ������ �ʿ��ϴٸ�!
	//enum class eTextureType
	//{
	//	DEFAULT, //�⺻ ��� �ؽ��ĸ� ������ ��ȹ.
	//	DIFFUSE, //Diffuse/Albedo ���ο�
	//	NORMAL, //Normal Mapping ��
	//	SHADOW, //Shadow Mapping ��
	//	DISPLACEMENT, // Displacement Mapping��.
	//};
		
	//Shader Variables Ȱ���ϴ� ��.
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
		_USER_DEFINED_TYPE //������ ���Ƿ� ���� Ÿ��.
	};

}

