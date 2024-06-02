#pragma once
#include <string>
#include "Util.h"
//Texture �� ���ҽ��� �ᱹ �ϳ��� Factory���� �����ϰ� �ʹ�.
//����, Cubemap / NormalMap / Shadow Map / Displacement Map �ϰ������� �����ϰ� ����.
//�ϴ�, ���̴����� �ƿ� �ٸ� ĭ�� ���ٰ� �����ϰ�, Enum ���� ��������.

struct ID3D11Texture2D;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace RocketCore::Graphics
{
	class Texture
	{
	public:
		Texture();
		//FilePath�� ���� Resource�� ����µ� ���̰�,
		//Resource�� AssignTextureFile���� ���δ�.
		//ID3D11Resource�� �ڽ� �߿� ID3D11Texture2D �ִ�! �Ű����� ȣȯ ����.
		void AssignSRVFromResource(ID3D11Resource* _resource);
		
		//FilePath�� ������, �̸� ������� SRV Assign!
		//������ ��������鼭 ���� ID3D11Resource ����. 
		//��ü������ SRV �Ҵ�.
		ID3D11Resource* AssignSRVFromDDSPath(const std::wstring& _path);
		ID3D11Resource* AssignSRVFromWICPath(const std::wstring& _path);

		//�׳� SRV �Ҵ�.
		void SetSRV(ID3D11ShaderResourceView* _srv);

		//Shader Resource View ��ȯ
		ID3D11ShaderResourceView* GetSRV();
		std::wstring m_FilePath;
	private:

		//���� �Ϸ� �Ǿ��� ���, true.
		bool m_isInit = false;

		ID3D11ShaderResourceView* m_SRV = nullptr;
		

	};
}


