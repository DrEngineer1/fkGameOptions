#include "fkUtils.h"
#include <string>
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
}