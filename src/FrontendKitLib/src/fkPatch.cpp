#include "fkPatch.h"
#include "fkUtils.h"
#include <vector>
#include <stdexcept>

namespace fk
{
	Patch::Patch(ULONG_PTR address, SIZE_T size)
		: _address(reinterpret_cast<LPBYTE>(address))
		, _size(size)
		, position(0)
	{
		if (!_address || !_size)
			throw std::invalid_argument("Address and size must not be 0.");
		if (!VirtualProtect(_address, _size, PAGE_EXECUTE_READWRITE, &_oldProtect))
			throw std::exception("VirtualProtect failed, call GetLastError for more info.");
	}

	Patch::~Patch()
	{
		close();
	}

	void Patch::close() const
	{
		DWORD oldProtect;
		if (!VirtualProtect(_address, _size, _oldProtect, &oldProtect))
			throw std::exception("VirtualProtect failed, call GetLastError for more info.");
	}

	void Patch::nops(ULONG_PTR address, SIZE_T size)
	{
		fk::Patch patch(address, size);
		while (size--)
			patch.write<BYTE>(0x90);
	}
	
	void Patch::WriteString(ULONG_PTR address,SIZE_T size, const char* PatchStr)
	{
		fk::Patch patch(address, size);
		for (SIZE_T i = 0; i < size; i++) {
		if (i < strlen(PatchStr))
			patch.write<char>(PatchStr[i]);
		else
			patch.write<char>('\0');  // pad with nulls
		}
	}
	
	void Patch::WriteUnicodeString(ULONG_PTR address,SIZE_T size, const char16_t* PatchStr)
	{
		fk::Patch patch(address, size);
		for (SIZE_T i = 0; i < size; i++) {
		if (i < std::char_traits<char16_t>::length(PatchStr)) //using char_traits::length here as C++ doesn't exactly have a good method beyond this for unicode string length.
			patch.write<char16_t>(PatchStr[i]);
		 else
		 	patch.write<char16_t>('\0');  // pad with nulls
		}
	}

	void Patch::PatchResource(ULONG_PTR address, UINT PatchResourceId, LPCTSTR ResourceType, LPCWSTR DllName){
    	//Grab the bytes of our resource.
    	std::vector<BYTE> bitmapData = GetResourceBytes(DllName, PatchResourceId, ResourceType);
    	//Then write the bitmap data to the resource
    	fk::Patch patch(address, bitmapData.size());
    	for (SIZE_T i = 0; i < bitmapData.size(); i++) {
        	patch.write<BYTE>(bitmapData[i]);
    	}
    	//Lastly when we're done with it free it as to not cause a potential memory leak.   
    	free(&bitmapData);
	}
	
	void Patch::jump(ULONG_PTR address, SIZE_T size, PVOID callee, DWORD jumpType)
	{
		fk::Patch patch(address, size);

		if (size >= 5 && address)
		{
			BYTE opSize, opCode;
			switch (jumpType)
			{
				case IJ_PUSHRET: opSize = 6; opCode = 0x68; break;
				case IJ_PUSH: opSize = 5; opCode = 0x68; break;
				case IJ_FARJUMP: opSize = 7; opCode = 0xEA; break;
				case IJ_FARCALL: opSize = 7; opCode = 0x9A; break;
				case IJ_CALL: opSize = 5; opCode = 0xE8; break;
				default: opSize = 5; opCode = 0xE9; break;
			}

			if (size < opSize)
				throw std::exception("Not enough space to patch opcode.");

			patch.write(opCode);
			switch (opSize)
			{
				case 7:
					patch.write((ULONG)callee);
					patch.write<WORD>(0x23);
					break;
				case 6:
					patch.write((ULONG)callee);
					patch.write<BYTE>(0xC3);
					break;
				default:
					patch.write((ULONG)callee - address - 5);
					break;
			}
			for (DWORD i = opSize; i < size; i++)
				patch.write<BYTE>(0x90);
		}
	}
};
