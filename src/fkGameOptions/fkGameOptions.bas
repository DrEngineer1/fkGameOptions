$NODLLMAIN
$DLL STDCALL
$CPP
$REMS
$RESOURCE "C:\Users\aidan\Documents\MSVC-14.51.36231\MSVC\Windows Kits\10\bin\10.0.28000.0\x64\rc.exe" "PatchResources.rc"
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
$COMMENT
    @TODO Once complete get the ASM blocks in the C++ code to be one.
$COMMENT
FUNCTION VanillaGameStart(HndlWnd AS HWND) AS BOOLEAN
    REM This is a decompiled and translated version of the actual start game script called by the frontend as output by Ghidra.
    REM With maybe an alteration here and there. Here for the purpose of the quick game buttons keeping their vanilla functionality.
    REM Meanwhile the other start game buttons launch to W2SE. Hope and pray no one plays pure vanilla Worms 2 these days!
    DIM GameStarted AS BOOLEAN
    DIM hHandle AS HANDLE
    DIM iVar1 AS INT
    DIM pHVar AS HINSTANCE

    IF HndlWnd == NULL THEN
        GameStarted = FALSE
    ELSE
        hHandle = CreateEvent(NULL, TRUE, FALSE, "Worms2ExitEvent")
        REM Unfortunately I have to do inline assembly using the disassembly here. Too bad!
        REM I did decompilation for one function I ' m not doing it for another.
        REM Especially as that function has functions within functions. Functionception.
        REM Also the latter 2 asm lines were taken from asking copilot on making it work with MSVC
        $ASM
            push [0x00518064] ;load game.dat as a parameter.
            mov ecx, [iVar1-0x08] ;setup the original decompiled line of: iVar1 = thunk_FUN_00426e38(this, s_data\game.dat_00518064)
            mov eax, [0x00402BA3] ; have to do this otherwise MSVC throws a hissy fit. copies the call address into eax.
            call eax ;just hope and pray you put iVar1 in the correct spot!
        $ASM
        IF iVar1 == 1 THEN
            REM WE MADE IT FINALLY!
            pHVar = ShellExecute(hHandle, "open", "worms2.exe", "colin.dat", NULL, SW_SHOW)
            REM Dunno why W2 needs to check if it ' s less than 32 for the instance. Maybe a null or 32-bit computing check?
            IF 32 < (INT)pHVar THEN
                WaitForSingleObject(hHandle, INFINITE)
            END IF
        END IF
    END IF
    FUNCTION = GameStarted
END FUNCTION

REM This entire function will be added as a function to FrontendKitLib. So no one else will have to suffer what I had to make.
SUB PatchResource(FileStub AS LPCWSTR, _
                  ResType AS LPCWSTR, _
                  PatchResName AS CONST LPCWSTR, _
                  PatchResSize AS DWORD, _
                  PatchResDat AS LPVOID = NULL, _
                  LangID AS WORD = 0)
    REM The Wall of parameters. Why Microsoft... why...
    REM Let's break down all these parameters for the poor sap that looks upon this:
    REM FileStub: the OG file name.
    REM ResType: The resource type for both the OG and Patch.
    REM PatchResName: The name of our resource to overide.
    REM LangID: (Optional) The language of the resource. Defaulted to Language Neutral as it's a required parameter in UpdateResource.
    REM PatchResDat: (Optional) Binary data of the resource. Equivalent to lpData in UpdateResoure.
    DIM ResHandle AS HANDLE
    REM This really shouldn't be 3 whole functions. But Microsoft made it this way so no way around this other than this function.
    REM Also don't delete the files as this is just a overide. Not full replacement.
    ResHandle = BeginUpdateResourceW(FileStub, FALSE)
    UpdateResourceW(ResHandle, ResType, PatchResName, LangID, PatchResDat, PatchResSize)
    REM Lastly finish up everything. Have I already made it clear that this is stupid?
    EndUpdateResourceW(ResHandle, FALSE)
END SUB



REM Allocation Base Starts at 0x00400000. Whatever is added on is the offset.
SUB patch(pe AS PEInfo&, gameVersion AS INT)
    IF gameVersion ==  fk::GAME_VERSION_TRY THEN
        IF iniEnableW2SE = TRUE THEN
            REM Make Sure that the quick game buttons and whatever else does an auto-generated game (i.e. the screensaver demo) doesn ' t get affected.
            REM Mostly through a decompiled version of the Start game Function.
            fk::Patch::jump(pe.Offset(0x00009D53), 5, &VanillaGameStart, fk::IJ_JUMP)
            fk::Patch::jump(pe.Offset(0x0000A06F), 5, &VanillaGameStart, fk::IJ_JUMP)
            fk::Patch::jump(pe.Offset(0x0000A648), 5, &VanillaGameStart, fk::IJ_JUMP)
            REM Everything else gets the W2SE Patch.
            fk::Patch::Patch(pe.Offset(0x00118080), "SuperEdi.exe")
            REM Lastly patch out resources for now. Particularly the Go buttons. Hope and pray this works...
            CALL PatchResource("frontend.exe", "RT_BITMAP", MAKEINTRESOURCE(235), 7400, "StartDown")
            CALL PatchResource("frontend.exe", "RT_BITMAP", MAKEINTRESOURCE(237), 7400, "StartUp")
        END IF
    ELSE

    END IF
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
