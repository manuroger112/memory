#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <TlHelp32.h>
#include <cstdint>

namespace VARS {
    extern uintptr_t baseAddress;
    extern DWORD processId;
    extern HANDLE processHandle;

    DWORD GetProcess(const wchar_t* Target);
    uintptr_t GetModuleBaseAddress(DWORD processId, const wchar_t* ModuleTarget);
    bool Initialize();

    template <typename T>
    T memRead(uintptr_t address) {
        T value = { };
        if (processHandle && processHandle != INVALID_HANDLE_VALUE) {
            ReadProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), NULL);
        }
        return value;
    }

    template <typename T>
    bool memWrite(uintptr_t address, T value) {
        if (processHandle && processHandle != INVALID_HANDLE_VALUE) {
            return WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), NULL);
        }
        return false;
    }
}