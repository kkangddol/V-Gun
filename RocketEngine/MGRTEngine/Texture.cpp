#include "Texture.h"
#include "Define3D.h"
#include "RenderPipeline.h"
#include "LowLevelDX11.h"

namespace RocketCore::Graphics
{
	Texture::Texture()
	{
		//
	}
	///���߿��� CreateResourceView�� ���� Resource�� ��� ���� ������ �� �����ϰ� �ʹ�.
	void Texture::AssignSRVFromResource(ID3D11Resource* _resource)
	{
		HRESULT hr = S_OK;
		
		//�̶�, D3D11_BIND_SHADER_RESOURCE�� ������� Resource����!
		hr = LowLevelDX11::GetInstance()->m_d3dDevice.Get()->
			CreateShaderResourceView(_resource, NULL, &m_SRV);

		if (hr == S_OK)
		{
			m_isInit = true;
		}
	}

	ID3D11Resource* Texture::AssignSRVFromDDSPath(const std::wstring& _path)
	{
		HRESULT hr = S_OK;

		ID3D11Resource* tmpRes = nullptr;
		
		hr = CreateDDSTextureFromFile(
			LowLevelDX11::GetInstance()->m_d3dDevice.Get(),
			NULL, _path.c_str(), &tmpRes, &m_SRV);

		if (hr == S_OK)
		{
			m_isInit = true;
		}
		else
		{
			std::wstring tBasicError = L"AssignSRVFromDDSPath �� ���� - �ν� �õ� Path : ";
			tBasicError.append(_path);
			MessageBox(LowLevelDX11::GetInstance()->m_hWnd, tBasicError.c_str(), L"����...", MB_ICONERROR | MB_OK);
		}
		return tmpRes;
	}

	ID3D11Resource* Texture::AssignSRVFromWICPath(const std::wstring& _path)
	{
		ID3D11Resource* tmpRes = nullptr;

		HRESULT hr = S_OK;
		hr = CreateWICTextureFromFile(
			LowLevelDX11::GetInstance()->m_d3dDevice.Get(),
			LowLevelDX11::GetInstance()->m_d3dMainContext.Get(),
			_path.c_str(), &tmpRes, &m_SRV);

		if (hr == S_OK)
		{
			m_isInit = true;
		}
		else
		{
			std::wstring tBasicError = L"AssignSRVFromWICPath �� ���� - �ν� �õ� Path : ";
			tBasicError.append(_path);
			MessageBox(LowLevelDX11::GetInstance()->m_hWnd, tBasicError.c_str(), L"����...", MB_ICONERROR | MB_OK);
		}

		return tmpRes;

	}

	void Texture::SetSRV(ID3D11ShaderResourceView* _srv)
	{
		m_SRV = _srv;
	}

	ID3D11ShaderResourceView* Texture::GetSRV()
	{
		return m_SRV;
	}





}
