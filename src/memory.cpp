#include "../include/memory.h"
#include <cstring>

namespace VARS {
    DWORD GetProcess(const wchar_t* Target) {

        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32W);

        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            return 0;
        }

        if (Process32FirstW(snapshotHandle, &processEntry)) {
            do {
                if (!wcscmp(processEntry.szExeFile, Target)) {
                    CloseHandle(snapshotHandle);
                    return processEntry.th32ProcessID;
                }
            } while (Process32NextW(snapshotHandle, &processEntry));
        }

        CloseHandle(snapshotHandle);
        return 0;
    }
    uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* ModuleTarget) {
       
        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(MODULEENTRY32W);

        HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

        if (snapshotHandle == INVALID_HANDLE_VALUE) {
            return 0;
        }

        if (Module32FirstW(snapshotHandle, &moduleEntry)) {
            do {
                if (!wcscmp(moduleEntry.szModule, ModuleTarget)) {
                    CloseHandle(snapshotHandle);
                    return reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                }
            } while (Module32NextW(snapshotHandle, &moduleEntry));
        }

        CloseHandle(snapshotHandle);
        return 0;
    }

    DWORD processId = 0;
    uintptr_t baseAddress = 0;
    HANDLE processHandle = NULL;

    bool Initialize() {
        processId = GetProcess(L"cs2.exe");
        if (!processId) return false;

        baseAddress = GetModuleBaseAddress(processId, L"client.dll");
        if (!baseAddress) return false;

        processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
        return (processHandle != NULL && processHandle != INVALID_HANDLE_VALUE);
    }
} 