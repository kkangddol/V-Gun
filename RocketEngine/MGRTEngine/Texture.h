#pragma once
#include <string>
#include "Util.h"
//Texture 및 리소스를 결국 하나의 Factory에서 관리하고 싶다.
//또한, Cubemap / NormalMap / Shadow Map / Displacement Map 일괄적으로 관리하고 싶음.
//일단, 셰이더에서 아예 다른 칸에 들어간다고 생각하고, Enum 없이 진행하자.

struct ID3D11Texture2D;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;

namespace RocketCore::Graphics
{
	class Texture
	{
	public:
		Texture();
		//FilePath는 먼저 Resource를 만드는데 쓰이고,
		//Resource는 AssignTextureFile에서 쓰인다.
		//ID3D11Resource의 자식 중에 ID3D11Texture2D 있다! 매개변수 호환 가능.
		void AssignSRVFromResource(ID3D11Resource* _resource);
		
		//FilePath가 들어오면, 이를 기반으로 SRV Assign!
		//별개로 만들어지면서 나온 ID3D11Resource 리턴. 
		//자체적으로 SRV 할당.
		ID3D11Resource* AssignSRVFromDDSPath(const std::wstring& _path);
		ID3D11Resource* AssignSRVFromWICPath(const std::wstring& _path);

		//그냥 SRV 할당.
		void SetSRV(ID3D11ShaderResourceView* _srv);

		//Shader Resource View 반환
		ID3D11ShaderResourceView* GetSRV();
		std::wstring m_FilePath;
	private:

		//세팅 완료 되었을 경우, true.
		bool m_isInit = false;

		ID3D11ShaderResourceView* m_SRV = nullptr;
		

	};
}


