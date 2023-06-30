#pragma once
#include <windows.h>
#include <TlHelp32.h>

#define WIN32_LEAN_AND_MEAN



namespace VARS {
	const wchar_t* Target = L"csgo.exe";
	const wchar_t* moduleName = L"client.dll";
	const wchar_t* moduleNameE = L"engine.dll";
	DWORD processId = NULL;
	uintptr_t baseAddress = NULL;
	uintptr_t engineAddress = NULL;
	HANDLE processHandle = NULL;

}





void GetProcess(const wchar_t* Target) {
	HANDLE snapShotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32W processEntry = {};
	processEntry.dwSize = sizeof(PROCESSENTRY32W);

	while (Process32NextW(snapShotHandle, &processEntry)) {

		if (wcscmp(processEntry.szExeFile, Target) == 0) {
			VARS::processId = processEntry.th32ProcessID;
			CloseHandle(snapShotHandle);
			break;
		}
	}
	VARS::processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, VARS::processId);
}






uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName) {
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (snapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	MODULEENTRY32W moduleEntry;
	moduleEntry.dwSize = sizeof(MODULEENTRY32W);

	if (!Module32FirstW(snapshot, &moduleEntry)) {
		CloseHandle(snapshot);
		return 0;
	}

	do {
		if (wcscmp(moduleEntry.szModule, moduleName) == 0) {
			CloseHandle(snapshot);
			return reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
		}
	} while (Module32NextW(snapshot, &moduleEntry));

	CloseHandle(snapshot);
	return 0;
}





template <typename type>
type memRead(uintptr_t pointerStatic) {
	type value = NULL;
	ReadProcessMemory(VARS::processHandle, (LPVOID)pointerStatic, &value, sizeof(type), NULL);
	return value;
}

template <typename type>
bool memWrite(uintptr_t pointerStatic, type value) {

	return WriteProcessMemory(VARS::processHandle, (LPVOID)pointerStatic, &value, sizeof(type), NULL);

}