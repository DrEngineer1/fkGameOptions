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

// *************************************************
//                System Variables
// *************************************************

static char    CRLF[3]= {13,10,0}; // Carr Rtn & Line Feed


// *************************************************
//            User's Global Variables
// *************************************************

BOOL    iniEnableW2SE;
const char*  SuperEdiCharSize = "SuperEdi.exe";

// *************************************************
//               User's Prototypes
// *************************************************
//commented out but not in the header file just in case it does need them.
//void    Configure (void);
//void    patch (PEInfo &,int);

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
  config.get("Frontend","W2seEnabled",iniEnableW2SE,FALSE);
  // Then set its default setting when created.
  config.set("Frontend","W2seEnabled",iniEnableW2SE);
}


BOOL VanillaGameStart (HWND HndlWnd)
{
  // This is a decompiled and translated version of the actual start game script called by the frontend as output by Ghidra.
  // With maybe an alteration here and there. Here for the purpose of the quick game buttons keeping their vanilla functionality.
  // Meanwhile the other start game buttons launch to W2SE. Hope and pray no one plays pure vanilla Worms 2 these days!
  BOOLEAN  GameStarted;
  HANDLE   hHandle;
  int      iVar1;
  HINSTANCE  pHVar;
  if(HndlWnd==NULL ){
      GameStarted=FALSE;
    }
  else
    {
      hHandle=CreateEvent(NULL,TRUE,FALSE,"Worms2ExitEvent");
      // Unfortunately I have to do inline assembly using the disassembly here. Too bad!
      // I did decompilation for one function I ' m not doing it for another.
      // Especially as that function has functions within functions. Functionception.
      // Also the latter 2 asm lines were taken from asking copilot on making it work with MSVC.
    __asm{
      push [0x00518064]	;load game.dat as a parameter.
      mov ecx, [iVar1-0x08]	;setup the original decompiled line of: iVar1 = thunk_FUN_00426e38(this, s_data\game.dat_00518064)
      mov eax, [0x00402BA3]	;have to do this otherwise MSVC throws a hissy fit. copies the call address into eax.
      call eax ;just hope and pray you put iVar1 in the correct spot!
      }
      if(iVar1==1 ){
          // WE MADE IT FINALLY!
          pHVar=ShellExecute(HndlWnd,"open","worms2.exe","colin.dat",NULL,SW_SHOW);
          // Dunno why W2 needs to check if it ' s less than 32 for the instance. Maybe a null or 32-bit computing check?
          if(32<(int)pHVar ){
              WaitForSingleObject(hHandle,INFINITE);
            }
        }
      CloseHandle(hHandle);
      GameStarted=TRUE;
    }
  return GameStarted;
}


void PatchResource (LPCSTR FileStub, LPCSTR ResType, const LPCSTR PatchResName, DWORD PatchResSize, LPVOID PatchResDat, WORD LangID)
{
  // The Wall of parameters. Why Microsoft... why...
  // Let's break down all these parameters for the poor sap (that being you likely) that looks upon this:
  //--------------------------------------------------------------------------------------------------------------------------------
  //| FileStub: the executable name.
  //| ResType: The resource type for both the OG and Patch.
  //| PatchResName: The name of our resource to overide.
  //| PatchResSize: The size of our patch. Must be the same as the OG.
  //| PatchResDat: (Optional) Binary data of the resource. Equivalent to lpData in UpdateResoure.
  //| LangID: (Optional) The language of the resource. Defaulted to Language Neutral as it's a required parameter in UpdateResource.
  //--------------------------------------------------------------------------------------------------------------------------------
  HANDLE   ResHandle;
  // This really shouldn't be 3 whole functions. But Microsoft made it this way so no way around this other than this function.
  // Also don't delete the files as this is just a overide. Not full replacement.
  ResHandle=BeginUpdateResource(FileStub,FALSE);
  UpdateResource(ResHandle,ResType,PatchResName,LangID,PatchResDat,PatchResSize);
  // Lastly finish up everything. Have I already made it clear that this is stupid?
  EndUpdateResource(ResHandle,FALSE);
}


void patch (PEInfo& pe,int gameVersion)
{
  if(gameVersion==fk::GAME_VERSION_TRY ){
      if(iniEnableW2SE==TRUE ){
          // Make Sure that the quick game buttons and whatever else does an auto-generated game (i.e. the screensaver demo) doesn ' t get affected.
          // Mostly through a decompiled version of the Start game Function.
          fk::Patch::jump(pe.Offset(0x00009D53),5, &VanillaGameStart,fk::IJ_JUMP);
          fk::Patch::jump(pe.Offset(0x0000A06F),5, &VanillaGameStart,fk::IJ_JUMP);
          fk::Patch::jump(pe.Offset(0x0000A648),5, &VanillaGameStart,fk::IJ_JUMP);
          // Everything else gets the W2SE Patch.
          fk::Patch::Patch(pe.Offset(0x00118080),SuperEdiCharSize);
          // Lastly patch out resources for now. Particularly the Go buttons. Hope and pray this works...
          PatchResource("frontend.exe","RT_BITMAP",MAKEINTRESOURCE(235),7400,"StartDown");
          PatchResource("frontend.exe","RT_BITMAP",MAKEINTRESOURCE(237),7400,"StartUp");
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
        // Shameless copy+paste and conversion job from other FrontEndKit Modules.
        // But if it ' s what they use by default i guess it'll do.
        PEInfo pe;
        int      tds=pe.FH->TimeDateStamp;
        int      version=fk::getGameVersion(tds);
        BOOLEAN  SuperEdiExists=std::filesystem::exists("SuperEdi.exe");
        // Check to see if W2SE Exists.
        if(SuperEdiExists ){
          // Initialize the module by checking the game version first.
          if(version==fk::GAME_VERSION_NONE ){
              char    MyMsg2[BCXSTRSIZE];
              strcpy(MyMsg2, join(3,"fkGameOptions is incompatible with whatever game version you got. ",CRLF,"Please use the v1.05 or TryMedia v1.07 release of Worms 2. Otherwise, you can remove this warning by moving the module out or deleting it."));
              MessageBox (GetActiveWindow(),MyMsg2,"fkGameOptions: Incompatible Game Version!",MB_OK|MB_ICONEXCLAMATION );
            }
          else
            {
              // If everything is up to code then complete initialization by configuring and patching.
              Configure();
              patch(pe,version);
              MessageBox (GetActiveWindow(),"fkGameOptions has been loaded! HUZZAH!","",0 );
            }
        }
        else
          {
            // If not bring up a Error message and detatch the module.
            char    MyMsg1[BCXSTRSIZE];
            strcpy(MyMsg1, join(6,"Worms 2 Super Editor was not found in your Worms 2 directory. ",CRLF,"Please check to see if it is installed in your root Worms 2 directory: e.g. C:\\GOG Games\\Worms 2",CRLF,CRLF,"Click OK to detach this module."));
            MessageBox (GetActiveWindow(),MyMsg1,"fkGameOptions: SuperEdi.exe not found!",MB_OK|MB_ICONHAND|MB_APPLMODAL );
            return FALSE;
          }
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}