$NODLLMAIN
$DLL
$CPP
$REMS
#include <filesystem>
#include "fkConfig.h"
#include "fkPatch.h"
#include "fkUtils.h"
#include "PEInfo.h"

DIM iniEnableW2SE AS BOOLEAN

SUB Configure()
    fk::Config config("fkGameOptions.ini")
    REM Load the ini settings.
    config.get("Frontend", "W2seEnabled", iniEnableW2SE, FALSE)
    REM Then set its default setting when created.
    config.set("Frontend", "W2seEnabled", iniEnableW2SE)
END SUB

SUB patch(pe AS PEInfo&, gameVersion AS INT)
    fk::Patch::jump()
END SUB

$COMMENT
@TODO Once Complete make sure to convert the DllMain C++ code into a switch case to make WinAPI happy.
Mostly to be safe and to make sure MSVC doesn ' t get a bug up its bum.
$COMMENT
FUNCTION DllMain (hInst AS HINSTANCE, _
    Reason AS DWORD, _
    Reserved AS LPVOID) EXPORT

    SELECT CASE Reason
        '**************************************************************
    CASE DLL_PROCESS_ATTACH
        REM Shameless copy+paste and conversion job from other FrontEndKit Modules.
        REM But if it ' s what they use by default i guess it'll do.
        PEInfo pe
        DIM AS INT tds = pe.FH->TimeDateStamp
        DIM AS INT version = fk::getGameVersion(tds)
        DIM AS BOOLEAN SuperEdiExists = std::filesystem::exists("SuperEdi.exe")
        REM Check to see if W2SE Exists.
        IF SuperEdiExists THEN
            REM Initialize the module by checking the game version first.
            IF version == fk::GAME_VERSION_NONE THEN
                DIM RAW MyMsg2$
                MyMsg2$ = "fkGameOptions is incompatible with whatever game version you got. " & CRLF$ & _
                "Please use the v1.05 or TryMedia v1.07 release of Worms 2. Otherwise, you can remove this warning by moving the module out or deleting it."
                MSGBOX MyMsg2$, "fkGameOptions: Incompatible Game Version!", MB_OK BOR MB_ICONEXCLAMATION
            ELSE
            REM If everything is up to code then complete initialization by configuring and patching.
            CALL Configure()
            CALL patch(pe, version)
            MSGBOX "fkGameOptions has been loaded! HUZZAH!" REM For Debugging purposes.
        END IF
    ELSE
        REM If not bring up a Error message and detatch the module.
        DIM RAW MyMsg1$
        MyMsg1$ = "Worms 2 Super Editor was not found in your Worms 2 directory. " & CRLF$ & _
        "Please check to see if it is installed in your root Worms 2 directory: e.g. C:\GOG Games\Worms 2" & CRLF$ & CRLF$ & _
        "Click OK to detach this module."
        MSGBOX MyMsg1$, "fkGameOptions: SuperEdi.exe not found!", MB_OK BOR MB_ICONHAND BOR MB_APPLMODAL
        FUNCTION = FALSE
    END IF

    '**************************************************************
CASE DLL_PROCESS_DETACH
END SELECT

FUNCTION = TRUE
END FUNCTION
