#include "TypedefGraphicHelper.h"
#include "ResourceManager.h"
#include <cassert>

namespace RocketCore::Graphics
{
	RocketCore::Graphics::Texture* TextureStorage::GetDiffuseTexture(int index)
	{
		assert(index < MAX_EACH_TEXTURE_COUNT);
		return m_DiffuseMapTextures[index];
	}

	RocketCore::Graphics::Texture* TextureStorage::GetNormalTexture(int index)
	{
		assert(index < MAX_EACH_TEXTURE_COUNT);
		return m_NormalMapTextures[index];
	}

	RocketCore::Graphics::Texture* TextureStorage::GetArmTexture(int index)
	{
		assert(index < MAX_EACH_TEXTURE_COUNT);
		return m_ArmMapTextures[index];
	}

	void TextureStorage::AssignTextures(ModelData model)
	{
		//적절한 범위 안에 값이 들어오는지를 나중에는 받아야 할 것.

		if (model.diffusePath != "")
		{
			std::string tPath = model.diffusePath;
			Texture* res = ResourceManager::GetInstance().FindTextureByFilePath(tPath);
			if (res == nullptr)
			{
				this->m_DiffuseMapTextures[0] = ResourceManager::GetInstance().MakeTextureFromFilePath(tPath);
			}
			else
			{
				this->m_DiffuseMapTextures[0] = res;
			}
		}
		else
		{
			this->m_DiffuseMapTextures[0] = ResourceManager::GetInstance().GetDefaultDiffuseTexture();
		}

		if (model.normalPath != "")
		{
			std::string tPath = model.normalPath;
			Texture* res = ResourceManager::GetInstance().FindTextureByFilePath(tPath);
			if (res == nullptr)
			{
				this->m_NormalMapTextures[0] = ResourceManager::GetInstance().MakeTextureFromFilePath(tPath);
			}
			else
			{
				this->m_NormalMapTextures[0] = res;
			}
		}
		else
		{
			this->m_NormalMapTextures[0] = ResourceManager::GetInstance().GetDefaultNormalTexture();
		}

		if (model.armPath != "")
		{
			std::string tPath = model.armPath;
			Texture* res = ResourceManager::GetInstance().FindTextureByFilePath(tPath);
			if (res == nullptr)
			{
				this->m_ArmMapTextures[0] = ResourceManager::GetInstance().MakeTextureFromFilePath(tPath);
			}
			else
			{
				this->m_ArmMapTextures[0] = res;
			}
		}
		else
		{
			this->m_ArmMapTextures[0] = ResourceManager::GetInstance().GetDefaultArmTexture();
		}

		//다 끝났으면, 할당할 만큼 할당했으니, 이제는 쓸 수 있다가 할 수 있다.
		this->isEmpty = false;
	}

	

}