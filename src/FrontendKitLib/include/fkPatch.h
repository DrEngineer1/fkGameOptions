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
	};

	struct Patch
	{
	public:
		ULONG_PTR position;

		Patch(ULONG_PTR address, SIZE_T size);
		~Patch();

		void close() const;
		template <class T> void write(const T& value);

		static void nops(ULONG_PTR address, SIZE_T size);
		static void jump(ULONG_PTR address, SIZE_T size, PVOID callee, DWORD jumpType);

	private:
		LPBYTE _address;
		SIZE_T _size;
		DWORD _oldProtect;
	};
}

#include "fkPatch.inl"
