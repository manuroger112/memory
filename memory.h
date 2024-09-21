#pragma once
#include <windows.h>
#include <TlHelp32.h>
#define WIN32_LEAN_AND_MEAN




namespace VARS {

	extern uintptr_t baseAddress;
	extern DWORD processId;
	extern HANDLE processHandle;

	DWORD GetProcess(const wchar_t* Target);
	extern uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* ModuleTarget);

	template <typename type>
	type memRead(uintptr_t pointerStatic) {
		type value = { };
		ReadProcessMemory(VARS::processHandle, (LPVOID)pointerStatic, &value, sizeof(type), NULL);
		return value;
	}
	template <typename type>
	bool memWrite(uintptr_t pointerStatic, type value) {
		return WriteProcessMemory(VARS::processHandle, (LPVOID)pointerStatic, &value, sizeof(type), NULL);
	}
	template <typename type>
	type add(type x, type y) {

		return x + y;
	}


}
