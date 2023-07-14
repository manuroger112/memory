#pragma once
#include <windows.h>
#include <TlHelp32.h>

#define WIN32_LEAN_AND_MEAN



namespace VARS {

	DWORD GetProcess(const wchar_t* Target) {
		DWORD procId = 0;
		HANDLE snapShotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32W processEntry = {};
		processEntry.dwSize = sizeof(PROCESSENTRY32W);

		if (Process32FirstW(snapShotHandle, &processEntry)) {


			do {

				if (_wcsicmp(processEntry.szExeFile, Target) == 0) {
					procId = processEntry.th32ProcessID;

					break;
				}
			} while (Process32NextW(snapShotHandle, &processEntry));
		}
		//VARS::processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, VARS::processId);
		CloseHandle(snapShotHandle);
		return procId;
	}






	uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* moduleName) {
		uintptr_t modBaseAddr = 0;
		HANDLE snapshotModule = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
		if (snapshotModule == INVALID_HANDLE_VALUE) {
			return 0;
		}

		MODULEENTRY32W moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32W);

		if (Module32FirstW(snapshotModule, &moduleEntry)) {
			do {
				if (_wcsicmp(moduleEntry.szModule, moduleName) == 0) {
					modBaseAddr = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
					CloseHandle(snapshotModule);

				}
			} while (Module32NextW(snapshotModule, &moduleEntry));
		}



		CloseHandle(snapshotModule);
		return modBaseAddr;
	}



	
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, NULL, GetProcess(L"csgo.exe"));;
	uintptr_t baseAddress = GetModuleBaseAddress(GetProcess(L"csgo.exe"), L"client.dll");
	uintptr_t engineAddress = GetModuleBaseAddress(GetProcess(L"csgo.exe"), L"engine.dll");
	HDC hdc = GetDC(FindWindowA(NULL, "Counter-Strike: Global Offensive"));




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



	
	
}
