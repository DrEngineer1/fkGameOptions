// *************************************************
//      Made with BCX BASIC To C/C++ Translator
//            Version 8.0.8 (12/27/2023)
// *************************************************
//    Translated for compiling with a C++ Compiler
// *************************************************
// You will not believe the amount of garbage that BCX throws on here. Thankfully this is what actually is necessary.
#include <windows.h>    // WinApi
#include <shellapi.h>   // WinApi
#include <stdarg.h>     // ISO StdLib
/*
stdio.h is here because you would not believe the amount of programs and code that would use this.
Is it necessary? Likely not. But for the hell of it is here as well I suppose.
*/
#include <stdio.h>      // ISO StdLib
#include <stdlib.h>     // ISO StdLib
#include <string.h>     // ISO StdLib
#include <libloaderapi.h>
#include <WinUser.h>
#include <wingdi.h>
#include <windef.h>
#include <winnt.h>
#include <basetsd.h>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <random>
#include <vector>

// *************************************************
//            System Defined Macros
// *************************************************

#define BCXSTRSIZE 2048
#include <filesystem>
#include "fkGameOptions.h"
#include "fkConfig.h"
#include "fkPatch.h"
#include "fkUtils.h"
#include "PEInfo.h"
#include "resource.h"

// *************************************************
//                  Compiler Macros
// *************************************************

// *************************************************
//                   Microsoft VC++
// *************************************************

// *************************************************
// Instruct Linker to Search Object/Import Libraries
// *************************************************

// *************************************************
//               Standard Prototypes
// *************************************************

char*   BCX_TmpStr (size_t);
char*   join (int, ... );

// *************************************************
//          User Defined Types And Unions
// *************************************************
typedef BOOL(__thiscall *FUN_00426E38_t)(void* thisPtr, LPCSTR GameDat);//Write into game.dat function pointer. Here for now i guess.

// *************************************************
//                System Variables
// *************************************************

static char    CRLF[3]= {13,10,0}; // Carr Rtn & Line Feed


// *************************************************
//            User's Global Variables
// *************************************************

BOOL    iniEnableW2SE;
BOOL    iniHalfImprQuickGame;
LPCWSTR DllSelf = L"fkGameOptions.dll";
//void    *W2Self;

// *************************************************
//               User's Prototypes
// *************************************************

// *************************************************
//            User's Global Initialized Arrays
// *************************************************


// *************************************************
//                 Runtime Functions
// *************************************************

char *BCX_TmpStr (size_t iBytes)
{
  static int   StrCnt;
  static char *StrFunc[2048];
  StrCnt = (StrCnt + 1) & (2047);
  if (StrFunc[StrCnt])
  {
    free (StrFunc[StrCnt]);
    StrFunc[StrCnt] = NULL;
  }
  StrFunc[StrCnt] = (char*)calloc(iBytes+1, sizeof(char));
  return StrFunc[StrCnt];
}


char *join(int n, ...)
{
  int ii = n, tmplen = 0;
  char *s_;
  char *strtmp;
  va_list marker;
  va_start(marker, n); // Initialize variable arguments
  while(ii-- > 0)
  {
    s_ = va_arg(marker, char *);
    if (s_) tmplen += (int)strlen(s_);
  }
  strtmp = BCX_TmpStr(tmplen);
  va_end(marker); // Reset variable arguments
  ii = n;
  va_start(marker, n); // Initialize variable arguments
  while(ii-- > 0)
  {
    s_ = va_arg(marker, char *);
    if (s_) strcat(strtmp, s_);
  }
  va_end(marker); // Reset variable arguments
  return strtmp;
}


// *************************************************
//            User's Subs and Functions
// *************************************************

void Configure ()
{
  fk::Config config("fkGameOptions.ini");
  // Load the ini settings.
  config.get("Frontend", "EnableW2SE", iniEnableW2SE, FALSE);
  config.get("Frontend", "HalfImprQuickGame", iniHalfImprQuickGame, FALSE);
  // Then set its default setting when created.
  config.set("Frontend", "EnableW2SE", iniEnableW2SE);
  config.set("Frontend", "HalfImprQuickGame", iniHalfImprQuickGame);
}

BOOL Van1pQuickGameStart (void* self, HWND HndlWnd)
{
  // This is a decompiled version of the actual start game script called by the frontend as output by Ghidra.
  // With maybe an alteration here and there. Here for the purpose of the quick game buttons keeping their vanilla functionality.
  // Meanwhile the other start game buttons launch to W2SE. Hope and pray no one plays pure vanilla Worms 2 these days!
  BOOL       GameStarted;
  HANDLE     hHandle;
  BOOL       LoadGameDat;
  HINSTANCE  pHVar;
  const auto GameDatCopyOpt = std::filesystem::copy_options::overwrite_existing;
  
  //took this snippet from a C++ forum. should work nicely. Credits to seeplus over there.
  const char* const Quick1PvCPUCoinFlip[]{"data\\QuickP1vCPUHeads.dat", "data\\QuickP1vCPUTails.dat"};//Heads player first, tails CPU first.
  std::mt19937 TheCoin(std::random_device{}());//The coin itself.
  std::uniform_int_distribution<size_t> Outcomes(0, std::size(Quick1PvCPUCoinFlip));//the two outcomes of the coin flip
  const auto TheFlip{Outcomes(TheCoin)};//The actual coinflip itself.

  //Begin the actual function.
  if(HndlWnd == (HWND)NULL){
      GameStarted=FALSE;
    }
  else
    {
      hHandle=CreateEventA(NULL, TRUE, FALSE, "Worms2ExitEvent");
      if (!iniHalfImprQuickGame)
      {
        //I think i figured it out but have to do this hacky and janky workaround for the time being.
          std::filesystem::copy(Quick1PvCPUCoinFlip[TheFlip], "data\\game.dat", GameDatCopyOpt);
          LoadGameDat = TRUE;
      }
      //Using XOR if for whatever reason these two are both true. Better safe than sorry!
      if((LoadGameDat^iniHalfImprQuickGame) == TRUE){
          // WE MADE IT FINALLY! Launch the game and pause the frontend. Dunno what colin.dat even is used for but it's there i guess.
          pHVar=ShellExecuteA(HndlWnd, "open", "worms2.exe", "colin.dat", NULL, SW_SHOW);
          // This actually is a check to see if the worms 2 game executable opened. Then waits until the match ends before doing the exit event.
          if(32 < (int)pHVar){
              WaitForSingleObject(hHandle, INFINITE);
            }
        }
      CloseHandle(hHandle);
      GameStarted=TRUE;
    }
  return GameStarted;
}
BOOL Van2pQuickGameStart (void* self, HWND HndlWnd)
{
  BOOL  GameStarted;
  HANDLE   hHandle;
  BOOL      LoadGameDat;
  HINSTANCE  pHVar;
  const auto GameDatCopyOpt = std::filesystem::copy_options::overwrite_existing;
  
  const char* const Quick1Pv2PCoinFlip[]{"data\\QuickP1vP2Heads.dat", "data\\QuickP1vP2Tails.dat"};//Heads player 1 first, tails player 2 first.
  std::mt19937 TheCoin(std::random_device{}());//The coin itself.
  std::uniform_int_distribution<size_t> Outcomes(0, std::size(Quick1Pv2PCoinFlip));//the two outcomes of the coin flip
  const auto TheFlip{Outcomes(TheCoin)};//The actual coinflip itself.

  //Begin the actual function.
  if(HndlWnd == (HWND)NULL){
      GameStarted=FALSE;
    }
  else
    {
      hHandle=CreateEventA(NULL, TRUE, FALSE, "Worms2ExitEvent");
      if (!iniHalfImprQuickGame)
      {
        //I think i figured it out but have to do this hacky and janky workaround for the time being.
          std::filesystem::copy(Quick1Pv2PCoinFlip[TheFlip], "data\\game.dat", GameDatCopyOpt);
          LoadGameDat = TRUE;
      }
      //Using XOR if for whatever reason these two are both true. Better safe than sorry!
      if((LoadGameDat^iniHalfImprQuickGame) == TRUE){
          // WE MADE IT FINALLY! Launch the game and pause the frontend. Dunno what colin.dat even is used for but it's there i guess.
          pHVar=ShellExecuteA(HndlWnd, "open", "worms2.exe", "colin.dat", NULL, SW_SHOW);
          // This actually is a check to see if the worms 2 game executable opened. Then waits until the match ends before doing the exit event.
          if(32 < (int)pHVar){
              WaitForSingleObject(hHandle, INFINITE);
            }
        }
      CloseHandle(hHandle);
      GameStarted=TRUE;
    }
  return GameStarted;
}

/*
Using this to disable single player missions for now. Reason being that the way the game handles the missions is much more complex than I thought.
What it does is when the mission begins it converts the Mission.dat file (which is plaintext btw.) into binary data to overide the Mission.opt file.
Then the rest of the files are then what I can only describe as being combined into the binary data necessary for the frontend to write into game.dat.
So I looked into Ghidra as one does and there's a lot of function pointers. Way more than the other Auto-generated game functions.
I don't think i'll exactly get it working unless there's a bloody miracle with the function pointer I got that makes it work as intented.
*/
BOOL MissionDisable (void*, HWND HndlWnd){
  BOOL  GameStarted;
  HANDLE   hHandle;
  HINSTANCE  pHVar;
  if(HndlWnd == (HWND)NULL){
      GameStarted=FALSE;
    }
  else{
    hHandle=CreateEventA(NULL, TRUE, FALSE, "Worms2ExitEvent");
    CloseHandle(hHandle);
    GameStarted = TRUE;
  }
  return GameStarted;
}

BOOL VanMissionStart (void* self, HWND HndlWnd){
  BOOL  GameStarted;
  HANDLE   hHandle;
  BOOL      LoadGameDat;
  HINSTANCE  pHVar;
  //Begin the actual function.
  if(HndlWnd == (HWND)NULL){
      GameStarted=FALSE;
    }
  else
    {
      hHandle=CreateEventA(NULL, TRUE, FALSE, "Worms2ExitEvent");
      if (!iniHalfImprQuickGame)
      {
        LoadGameDat = ((FUN_00426E38_t)0x00426E38)(HndlWnd, "data\\game.dat");
      }
      //Using XOR if for whatever reason these two are both true. Better safe than sorry!
      if((LoadGameDat^iniHalfImprQuickGame) == TRUE){
          // WE MADE IT FINALLY! Launch the game and pause the frontend. Dunno what colin.dat even is used for but it's there i guess.
          pHVar=ShellExecuteA(HndlWnd, "open", "worms2.exe", "colin.dat", NULL, SW_SHOW);
          // This actually is a check to see if the worms 2 game executable opened. Then waits until the match ends before doing the exit event.
          if(32 < (int)pHVar){
              WaitForSingleObject(hHandle, INFINITE);
            }
        }
      CloseHandle(hHandle);
      GameStarted=TRUE;
    }
  return GameStarted;
}

void DummyStart (){
  HWND FrontendHandle = (HWND)GetModuleHandleW(DllSelf);
  MissionDisable(NULL, FrontendHandle);
}

void patch (PEInfo& pe,int gameVersion)
{
  if(gameVersion == fk::GAME_VERSION_TRY){
      if(iniEnableW2SE == TRUE){
          // Make Sure that the quick game buttons and whatever else does an auto-generated game (i.e. the quick games) doesn ' t get affected.
          // Mostly through a decompiled version of the Start game Function.
          fk::Patch::jump(pe.Offset(0x00009D53), 5, &Van2pQuickGameStart, fk::IJ_CALL); // Quick 2 Player
          fk::Patch::jump(pe.Offset(0x0000A06F), 5, &Van1pQuickGameStart, fk::IJ_CALL); // Quick 1 Player
          fk::Patch::jump(pe.Offset(0x0000A648), 5, &Van2pQuickGameStart, fk::IJ_CALL); // Quick Game General. I have 0 idea why this is required for the previous 2 to work but it's here.
          fk::Patch::jump(pe.Offset(0x00074F62), 5, &MissionDisable, fk::IJ_CALL);// Single Player Missions. Entirely disabled for now.
          // Everything else gets the W2SE Patch.
          fk::Patch::WriteString(pe.Offset(0x00118080), 13, "SuperEdi.exe");
          //Have to add two more to the offset from the start point as to align it properly. Otherwise the worm will either stop at the W or he will speak chinese.
          //Not even joking about that latter part.
          fk::Patch::WriteUnicodeString(pe.Offset(0x00599028), 16, u"\nStart up W2SE.");
          // Lastly patch out resources for now. Particularly the Go buttons.
          fk::Patch::PatchResource(pe.Offset(0x002DA228), IDB_STARTUP, RT_BITMAP, DllSelf);
          fk::Patch::PatchResource(pe.Offset(0x002D6858), IDB_STARTDOWN, RT_BITMAP, DllSelf);
          fk::Patch::PatchResource(pe.Offset(0x002D8540), IDB_STARTDISABLED, RT_BITMAP, DllSelf);
        }
    }
  else
    {
    }
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,
    DWORD fdwReason,     
    LPVOID lpvReserved ) 
{
    switch( fdwReason ) 
    { 
        case DLL_PROCESS_ATTACH:
        {
        // Shameless copy+paste and conversion job from other FrontEndKit Modules.
        // But if it ' s what they use by default i guess it'll do.
        PEInfo   pe;
        int      tds = pe.FH->TimeDateStamp;
        int      version = fk::getGameVersion(tds);
        BOOLEAN  SuperEdiExists = std::filesystem::exists("SuperEdi.exe");
        // Check to see if W2SE Exists.
        if(SuperEdiExists){
          // Initialize the module by checking the game version first.
          if(version == fk::GameVersion::GAME_VERSION_NONE){
              char    MyMsg2[BCXSTRSIZE];
              strcpy(MyMsg2, join(3, "fkGameOptions is incompatible with whatever game version you got. ", CRLF, "Please use the v1.05 or TryMedia v1.07 release of Worms 2. Otherwise, you can remove this warning by moving the module out or deleting it."));
              MessageBoxA(GetActiveWindow(), MyMsg2, "fkGameOptions: Incompatible Game Version!", MB_OK|MB_ICONEXCLAMATION);
            }
          else
            {
              // If everything is up to code then complete initialization by configuring and patching.
              Configure();
              patch(pe, version);
            }
        }
        else
          {
            // If not bring up a Error message and detatch the module.
            char    MyMsg1[BCXSTRSIZE];
            strcpy(MyMsg1, join(6, "Worms 2 Super Editor was not found in your Worms 2 directory. ", CRLF, "Please check to see if it is installed in your root Worms 2 directory: e.g. C:\\GOG Games\\Worms 2", CRLF, CRLF, "Click OK to detach this module."));
            MessageBoxA(GetActiveWindow(), MyMsg1, "fkGameOptions: SuperEdi.exe not found!", MB_OK|MB_ICONHAND|MB_APPLMODAL);
            return FALSE;
          }
            break;
        }
        
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
