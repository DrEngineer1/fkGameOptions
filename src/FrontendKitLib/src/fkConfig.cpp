#include "fkConfig.h"
#include <stdio.h>

namespace fk
{
	Config::Config(LPCSTR fileName)
	{
		GetModuleFileName(NULL, _filePath, MAX_PATH);
		char* sepIdx = strrchr(_filePath, '\\') + 1;
		strcpy_s(sepIdx, MAX_PATH - (int)(sepIdx - _filePath), fileName);
	}

	void Config::get(LPCSTR category, LPCSTR key, BOOL& result, UINT fallback) const
	{
		result = GetPrivateProfileInt(category, key, fallback, _filePath);
	}

	void Config::get(LPCSTR category, LPCSTR key, UINT& result, UINT fallback) const
	{
		result = GetPrivateProfileInt(category, key, fallback, _filePath);
	}

	void Config::get(LPCSTR category, LPCSTR key, LPSTR result, INT resultLength, LPCSTR fallback) const
	{
		GetPrivateProfileString(category, key, fallback, result, resultLength, _filePath);
	}

	void Config::set(LPCSTR category, LPCSTR key, UINT value) const
	{
		CHAR buffer[32];
		sprintf_s(buffer, "%d", value);
		WritePrivateProfileString(category, key, buffer, _filePath);
	}

	void Config::set(LPCSTR category, LPCSTR key, LPCSTR value) const
	{
		WritePrivateProfileString(category, key, value, _filePath);
	}
}