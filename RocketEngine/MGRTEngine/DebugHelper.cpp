#include "DebugHelper.h"
#include <windows.h>
namespace RocketCore::Graphics
{
	void DebugHelper::Trace(const std::string& _info)
	{
		std::wstring tStr;
		tStr.assign(_info.begin(), _info.end());
		OutputDebugString(tStr.c_str());
	}

}
