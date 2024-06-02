#pragma once
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include "Util.h"
#include "TypedefGraphicHelper.h"

struct ID3DX11Effect;
struct ID3DX11EffectVariable;
struct ID3DX11EffectTechnique;
class ByteStorage;
class ByteBuffer;

namespace RocketCore::Graphics
{
	class Shader
	{
	public:
		//����Ʈ ���̴� ����
		Shader();

		//Custom Shader
		Shader(const std::string& _shaderName, //���̴� �̸�.
			const std::string& _fileRoute, //���� ��Ʈ.
			const std::vector<std::string>& _techMap, //��ũ�� �̸����� �� ��� ����.
			const std::map<std::pair<std::string, eShaderVarType>, std::pair<int, int>>& _varMap); //<VariableName, std::pair<IndividualSize, IndividualCnt> 
		
		void SetShaderFileName(std::string _shaderName);
		std::string GetShaderFileName();

		//���� ��ũ���� ������ �� �ִ�.
		void SetMainTechnique(const std::string& _techStr);
		ID3DX11EffectTechnique* GetMainTechnique();

		//Self : �ڱ� �ڽ��� ������Ʈ��Ű�� �Լ��̴�.
		void UpdateShader();

		//�ڱ� �ڽ��� ������ �� -> ��ü������ EffectVariable���� ���ο� ä����.
		//�̸� ��������, ���� Update�ϴ� �Լ�!
		void UpdateSingleVariable(const std::string& _varName, void* _dataMem);

		//Deferred �������� Effect�� �����ͼ� Ư�� Variable���� �ߵ����Ѿ� �� ���� �ִ�.
		//�̸� ����.
		ID3DX11Effect* GetEffect();

	private:
		//���̴� �̸�
		std::string m_ShaderName;
		//FX File Path.
		std::string m_FXFilePath;
		//������� Effect ����.
		ID3DX11Effect* m_Effect = nullptr;

		//String�� �ް�, �̿� �����ϴ� Ÿ���� ������ �۵�.
		//������ Ÿ�� ��ü�� �����ϱ⺸�ٴ�, �ش� Ÿ���� SetRawValue�� ���ϴ� size�� �޾Ƶ� ���� ���� ��.
		// NameString //	 FXVariable /  EffectVarType				// Single Size(Byte) / Size ����.	//�Ŀ� ������Ʈ�� ���� ������.
	  //std::map<std::string, std::tuple<ID3DX11EffectVariable*, eShaderVarType, std::pair<int, int>, ByteBufferMGRT*>> m_FXVarSizeMap;
		std::map<std::string, EffectsTuple> m_FXVarSizeMap;
		
		//		TechVarName / FXTechnique
		std::map<std::string, ID3DX11EffectTechnique*> m_FXTechMap;
		//�������� �������� Ȱ��� ���.
		ID3DX11EffectTechnique* m_MainTech;

		//�����Լ�.
		//Effect�� ����� ��ȯ�Ѵ�.
		ID3DX11Effect* CreateEffect(const std::string& _fileRoute);

		//������ ���̴��� �� �����Ͱ� �����ϴ����� Check�Ѵ�.
		//(�Ź� üũ���ָ� ��������, ���������� ������ ������ �ʾƵ� �� �� ����.
		bool CheckShaderDataExist();
	
	};
}


