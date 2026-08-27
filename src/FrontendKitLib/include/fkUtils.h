#pragma once
#include <stdexcept>
#include <Windows.h>

namespace fk
{
	enum GameVersion
	{
		GAME_VERSION_NONE = -1,
		GAME_VERSION_BR, // 1.05 Br
		GAME_VERSION_EN, // 1.05 Du, En, Fr, It, Po, Sp, Sw
		GAME_VERSION_GE, // 1.05
		GAME_VERSION_NA, // 1.05
		GAME_VERSION_SA, // 1.05
		GAME_VERSION_TRY // 1.07 Trymedia
	};

	int getGameVersion(DWORD timeDateStamp);
	std::string getErrorMessage(int error);
}
