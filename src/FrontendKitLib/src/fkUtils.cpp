#include "fkUtils.h"
#include <string>
#include <vector>
#include "fkPatch.h"

namespace fk
{
	int getGameVersion(DWORD timeDateStamp)
	{
		switch (timeDateStamp)
		{
			case 0x3528DAFA: return GAME_VERSION_BR;
			case 0x3528DCB1: return GAME_VERSION_EN;
			case 0x3528DB52: return GAME_VERSION_GE;
			case 0x3528DA98: return GAME_VERSION_NA;
			case 0x3528DBDA: return GAME_VERSION_SA;
			case 0x3587BE19: return GAME_VERSION_TRY;
		}
		return GAME_VERSION_NONE;
	}

	std::string getErrorMessage(int error)
	{
		if (error == ERROR_SUCCESS)
			return std::string();

		LPTSTR buffer = NULL;
		const DWORD cchMsg = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL,
			error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPTSTR>(&buffer), 0, NULL);
		if (cchMsg > 0)
		{
			std::string message(buffer);
			LocalFree(buffer);
			return message;
		}
		else
		{
			CHAR buffer[32];
			sprintf_s(buffer, "Error code 0x%08X.", error);
			return buffer;
		}
	}

	//I never used memcopy so i'm not sure if it will get freed once it's done with it. So there's potential for this to cause a memory leak.
	//Also is generated with Google's Gemini. Surprisingly works given that thing's track record. Looking at you for using glue in pizza sauce.
	std::vector<BYTE> GetResourceBytes(LPCWSTR hModule, UINT lpName, LPCTSTR lpType) {
    // 1. Find the resource
    HINSTANCE HandleDLL = GetModuleHandleW(hModule);
    HRSRC hResInfo = FindResource(HandleDLL, MAKEINTRESOURCE(lpName), lpType);
    if (!hResInfo) return {};

    // 2. Load the resource into memory
    HGLOBAL hResData = LoadResource(HandleDLL, hResInfo);
    if (!hResData) return {};

    // 3. Get the size of the resource
    DWORD dataSize = SizeofResource(HandleDLL, hResInfo);
    if (dataSize == 0) return {};

    // 4. Lock the resource to get a pointer to the raw bytes
    LPVOID pData = LockResource(hResData);
    if (!pData) return {};

    // 5. Copy bytes into a vector
    std::vector<BYTE> bytes(dataSize);
    //This is what the vector bytes actually returns. Wish i could free this in memory but I don't have a method of doing so here.
    memcpy(bytes.data(), pData, dataSize);

    return bytes;
	}
}