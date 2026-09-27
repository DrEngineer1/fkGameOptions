#pragma once
#include <Windows.h>

namespace fk
{
	enum InsertJump
	{
		IJ_JUMP, // Insert a jump (0xE9) with patchJump
		IJ_CALL, // Insert a call (0xE8) with patchJump
		IJ_FARJUMP, // Insert a farjump (0xEA) with patchJump
		IJ_FARCALL, // Insert a farcall (0x9A) with patchJump
		IJ_PUSHRET, // Insert a pushret with patchJump
		IJ_PUSH, // Insert a push with patchJump
	};

	struct Patch
	{
	public:
		ULONG_PTR position;

		Patch(ULONG_PTR address, SIZE_T size);//Semi-Internal function. DO NOT USE DIRECTLY BY ITSELF!
		~Patch();

		void close() const;
		template <class T> void write(const T& value);

		static void WriteString(ULONG_PTR address,SIZE_T size, const char* PatchStr);//Write a null-padded string at the address. Unicode/Pascal Strings should use WriteUnicodeString Instead.
		static void WriteUnicodeString(ULONG_PTR address,SIZE_T size, const char16_t* PatchStr);//Write a null-padded unicode string at the address. Good for string table objects and whatever else uses this type.
		static void PatchResource(ULONG_PTR address, UINT PatchResourceId, LPCTSTR ResourceType, LPCWSTR DllName);//Patches a compiled resource from a executable file through a compiled resource in your DLL file. And to repeat myself, you are required to have a compiled resource as obviously the executable already has compiled resources. Especially in the case of bitmaps.
		static void nops(ULONG_PTR address, SIZE_T size);//Insert a one or more nop instructions at the specified address
		static void jump(ULONG_PTR address, SIZE_T size, PVOID callee, DWORD jumpType);//Insert an jump-type instruction (e.g. PUSHRET) with the specified address

	private:
		LPBYTE _address;
		SIZE_T _size;
		DWORD _oldProtect;
	};
}

#include "fkPatch.inl"
