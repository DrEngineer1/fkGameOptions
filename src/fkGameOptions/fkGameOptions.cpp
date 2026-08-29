// *************************************************
//      Made with BCX BASIC To C/C++ Translator
//            Version 8.0.8 (12/27/2023)
// *************************************************
//    Translated for compiling with a C++ Compiler
// *************************************************

#ifndef __cplusplus
  #error A C++ compiler is required
#endif

#ifdef _MSC_VER
  #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
  #endif
#endif

#include <windows.h>    // WinApi
#include <windowsx.h>   // WinApi
#include <commctrl.h>   // WinApi
#include <commdlg.h>    // WinApi
#include <direct.h>     // WinApi
#include <mmsystem.h>   // WinApi
#include <oaidl.h>      // WinApi
#include <objbase.h>    // WinApi
#include <ocidl.h>      // WinApi
#include <ole2.h>       // WinApi
#include <oleauto.h>    // WinApi
#include <olectl.h>     // WinApi
#include <richedit.h>   // WinApi
#include <shellapi.h>   // WinApi
#include <shlobj.h>     // WinApi
#include <urlmon.h>     // WinApi
#include <wchar.h>      // WinApi
#include <wctype.h>     // WinApi
#include <tchar.h>      // WinApi
#include <unknwn.h>     // WinApi
#include <wingdi.h>     // WinApi
#include <wininet.h>    // WinApi
#include <winsock.h>    // WinApi
#include <winuser.h>    // WinApi
#include <stdbool.h>    // ISO StdLib
#include <ctype.h>      // ISO StdLib
#include <math.h>       // ISO StdLib
#include <setjmp.h>     // ISO StdLib
#include <stdarg.h>     // ISO StdLib
#include <stddef.h>     // ISO StdLib
#include <stdio.h>      // ISO StdLib
#include <stdlib.h>     // ISO StdLib
#include <string.h>     // ISO StdLib
#include <time.h>       // ISO StdLib
#include <process.h>    // ISO StdLib
#include <inttypes.h>   // ISO StdLib
#include <fcntl.h>      // POSIX
#include <io.h>         // WinNT POSIX subset
#include <conio.h>      // Primitive i/o


// *************************************************
//            System Defined Macros
// *************************************************

#define BCXSTRSIZE 2048
#include <filesystem>
#include "fkConfig.h"
#include "fkPatch.h"
#include "fkUtils.h"
#include "PEInfo.h"

// *************************************************
//                  Compiler Macros
// *************************************************

#if defined(__cplusplus)
  #define overloaded
  #define C_EXPORT EXTERN_C __declspec(dllexport)
  #define C_IMPORT EXTERN_C __declspec(dllimport)
#else
  #define C_EXPORT __declspec(dllexport)
  #define C_IMPORT __declspec(dllimport)
#endif

// *************************************************
//                   Microsoft VC++
// *************************************************

#ifndef DECLSPEC_UUID
  #if(_MSC_VER >= 1100) && defined(__cplusplus)
    #define DECLSPEC_UUID(x)  __declspec(uuid(x))
  #else
    #define DECLSPEC_UUID(x)
  #endif
#endif
#if (_MSC_VER >= 1900)            // earlier versions untested
   #include <intrin.h>
      #ifndef _rdtsc
         #define _rdtsc __rdtsc   // MSVC uses 2 underscores
      #endif
   #pragma warning(disable: 4018) // signed/unsigned mismatch warnings
   #pragma warning(disable: 4100) // unreferenced argument warnings
   #pragma warning(disable: 4244) // conversion from type1 to type2 warnings
   #pragma warning(disable: 4267) // conversion from type1 to type2 warnings
   #pragma warning(disable: 4305) // truncation from double to float warnings
   #pragma warning(disable: 4800) // forcing value to bool warnings
   #pragma warning(disable: 4838) // conversion from type1 to type2 warnings
#endif

// *************************************************
//                  GCC and CLANG
// *************************************************

#if defined(__GNUC__) || defined(__clang__)
   #ifndef __BCPLUSPLUS__
      #include <x86intrin.h>
   #endif
   #pragma GCC diagnostic ignored "-Wwrite-strings"
   #pragma GCC diagnostic ignored "-Wunused-parameter"
   #pragma GCC diagnostic ignored "-Wunknown-pragmas"
   #pragma GCC diagnostic ignored "-Wdangling-else"
   #pragma GCC diagnostic ignored "-Wdeprecated"
#endif

// *************************************************
//                  Embarcadero C++
// *************************************************

#if defined(__BCPLUSPLUS__)
      #if defined (_clang__)
          #include <mmintrin.h>
      #endif
      #define _kbhit kbhit
      #ifndef _rdtsc
         #define _rdtsc __rdtsc  // Uses 2 underscores
      #endif
#endif


// *************************************************
// Instruct Linker to Search Object/Import Libraries
// *************************************************

#if !defined(__GNUC__) && !defined(__TINYC__)
   #if !(defined(__BCPLUSPLUS__) && defined(_WIN64))
    #pragma comment(lib,"kernel32.lib")
    #pragma comment(lib,"user32.lib")
    #pragma comment(lib,"gdi32.lib")
    #pragma comment(lib,"comctl32.lib")
    #pragma comment(lib,"advapi32.lib")
    #pragma comment(lib,"winspool.lib")
    #pragma comment(lib,"shell32.lib")
    #pragma comment(lib,"msimg32.lib")
    #pragma comment(lib,"ole32.lib")
    #pragma comment(lib,"oleaut32.lib")
    #pragma comment(lib,"uuid.lib")
    #pragma comment(lib,"odbc32.lib")
    #pragma comment(lib,"odbccp32.lib")
    #pragma comment(lib,"winmm.lib")
    #pragma comment(lib,"comdlg32.lib")
    #pragma comment(lib,"imagehlp.lib")
    #pragma comment(lib,"version.lib")
    #pragma comment(lib,"wininet.lib")
    #pragma comment(lib,"urlmon.lib")
  #endif
#endif

// *************************************************
//               Standard Prototypes
// *************************************************

char*   BCX_TmpStr (size_t);
char*   join (int, ... );

// *************************************************
//          User Defined Types And Unions
// *************************************************
// @TODO Once Complete make sure to convert the DllMain C++ code into a switch case to make WinAPI happy.
// Mostly to be safe and to make sure MSVC doesn ' t get a bug up its bum.

// *************************************************
//                System Variables
// *************************************************

static char    CRLF[3]= {13,10,0}; // Carr Rtn & Line Feed


// *************************************************
//            User's Global Variables
// *************************************************

static BOOL    iniEnableW2SE;

// *************************************************
//               User's Prototypes
// *************************************************

void    Configure (void);
BOOL    VanillaGameStart (HWND);
void    patch (PEInfo &,int);
__declspec(dllexport) BOOL WINAPI DllMain (HINSTANCE,DWORD,LPVOID);

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
  config .get("Frontend","W2seEnabled",iniEnableW2SE,FALSE);
  // Then set its default setting when created.
  config .SET("Frontend","W2seEnabled",iniEnableW2SE);
}


BOOL VanillaGameStart (HWND HndlWnd)
{
  // This is a decompiled and translated version of the actual start game script called by the frontend as output by Ghidra.
  // With maybe an alteration here and there. Here for the purpose of the quick game buttons keeping their vanilla functionality.
  // Meanwhile the other start game buttons launch to W2SE. Hope and pray no one plays pure vanilla Worms 2 these days!
  BOOLEAN  GameStarted= {0};
  HANDLE   hHandle= {0};
  int      iVar1= {0};
  HINSTANCE  pHVar= {0};
  if(HndlWnd==NULL ){
      GameStarted=FALSE;
    }
  else
    {
      hHandle=CreateEvent(NULL,TRUE,FALSE,Worms2ExitEvent);
      // Unfortunately I have to do inline assembly using the disassembly here. Too bad!
      // I did decompilation for one function I ' m not doing it for another.
      // Especially as that function has functions within functions. Functionception.
#if defined (__POCC__) && !defined(__cplusplus)
  #pragma optimize(none)  // No Optimizations in ASM block
#elif !defined (__cplusplus)
  #pragma optimize(0)  // No Optimizations in ASM block
#endif
#if !defined(__POCC__) && !defined (__cplusplus)
_asm("push 00518064")	//load game.dat as a parameter.
#else
__asm{push 00518064}	//load game.dat as a parameter.
#endif
#if !defined(__POCC__) && !defined (__cplusplus)
_asm("mov ecx, [iVar1-08]")	//setup the original decompiled line of: iVar1 = thunk_FUN_00426e38(this, s_data\game.dat_00518064)
#else
__asm{mov ecx, [iVar1-08]}	//setup the original decompiled line of: iVar1 = thunk_FUN_00426e38(this, s_data\game.dat_00518064)
#endif
#if !defined(__POCC__) && !defined (__cplusplus)
_asm("call 00402BA3")	//just hope and pray you put iVar1 in the correct spot!
#else
__asm{call 00402BA3}	//just hope and pray you put iVar1 in the correct spot!
#endif
#if defined (__POCC__) && !defined(__cplusplus)
  #pragma optimize()  // Restoring Optimizer state
#elif !defined (__cplusplus)
  #pragma optimize(1)  // Restoring Optimizer state
#endif
      if(iVar1==1 ){
          // WE MADE IT FINALLY!
          pHVar=ShellExecute(hHandle,"open","worms2.exe","colin.dat",NULL,SW_SHOW);
          // Dunno why W2 needs to check if it ' s less than 32 for the instance. Maybe a null or 32-bit computing check?
          if(32<(int)pHVar ){
              WaitForSingleObject(hHandle,INFINITE);
            }
        }
    }
  return GameStarted;
}


void patch (PEInfo &  pe,int gameVersion)
{
  if(gameVersion==wk::GAMEID_W2_1_07_TRY ){
      if(iniEnableW2SE==TRUE ){
          // Make Sure that the quick game buttons and whatever else does an auto-generated game (i.e. the screensaver demo) doesn ' t get affected.
          // Mostly through a decompiled version of the Start game Function.
          fk::Patch::jump(pe.Offset(0x00009D53),5, &VanillaGameStart,fk::IJ_JUMP);
          fk::Patch::jump(pe.Offset(0x0000A06F),5, &VanillaGameStart,fk::IJ_JUMP);
          fk::Patch::jump(pe.Offset(0x0000A648),5, &VanillaGameStart,fk::IJ_JUMP);
          // Everything else gets the W2SE Patch.
          fk::Patch::Patch(pe.Offset(0x00118080),"SuperEdi.exe");
        }
    }
  else
    {
    }
}


__declspec(dllexport) BOOL WINAPI DllMain (HINSTANCE  hInst,DWORD Reason,LPVOID  Reserved)
{
  //**************************************************************
  if(Reason==DLL_PROCESS_ATTACH ){
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
      //**************************************************************
      goto L1000;
    }
  if(Reason==DLL_PROCESS_DETACH ){
    }
L1000:;
  return TRUE;
}



// *************************************************
//                  Main Program
// *************************************************

