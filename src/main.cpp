#include "../inclumemory.h"
#include <iostream>
#include <thread>

namespace offsets 
{
    constexpr auto localPlayer = 0x1BEAEB8;
    constexpr auto flag = 0x18;
    constexpr auto jump = 0x1BE4830;

}

int main() {
    if (!VARS::Initialize()) {
        std::cout << "The process not found!" << std::endl;
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "The process found." << std::endl;
    std::cout << "Process ID: " << VARS::processId << std::endl;
    std::cout << "Base Address: 0x" << std::hex << VARS::baseAddress << std::dec << std::endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        uintptr_t localPlayer = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::localPlayer);
        
        if (localPlayer)
        {
            int32_t onGround = VARS::memRead<int32_t>(localPlayer + offsets::flag);

            if (GetAsyncKeyState(VK_SPACE)) {
                VARS::memWrite(VARS::baseAddress + offsets::jump, 65537);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                VARS::memWrite(VARS::baseAddress + offsets::jump, 256);

            }
        }
    }

    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();
    return 0;
}