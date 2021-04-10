#pragma comment(lib, "XInput.lib") 
#include <windows.h>
#include <iostream>
#include <Xinput.h>

int main()
{
    DWORD dwResult;

    XINPUT_STATE state, prevstate;
    SecureZeroMemory(&state, sizeof(XINPUT_STATE));
    SecureZeroMemory(&prevstate, sizeof(XINPUT_STATE));
    dwResult = XInputGetState(0, &prevstate);

    DWORD wButtonPressing = 0;
    DWORD Lstick = 64;

    //key down R key and key up R key
    INPUT inputs[2];
    ZeroMemory(inputs, sizeof(inputs));
    //key down
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = 0x52;

    //key up (same as inputs[0] object but with extra attr)
    CopyMemory(&inputs[1], &inputs[0], sizeof(INPUT));
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    if (dwResult == ERROR_SUCCESS)
    {
        std::cout << "controller found. program running." << std::endl;
        // Controller is connected
        while (XInputGetState(0, &state) == ERROR_SUCCESS) {
            //dwpacketnumber diff?
            if (state.dwPacketNumber != prevstate.dwPacketNumber) {
                //different input
                wButtonPressing = state.Gamepad.wButtons;
            }
            if (wButtonPressing & Lstick) {
                //std::cout << "hold" << std::endl;
                SendInput(1, &inputs[0], sizeof(INPUT));
            }
            if (prevstate.Gamepad.wButtons & Lstick
                && (wButtonPressing & Lstick) == 0) {
                //relese
                //std::cout << "release" << std::endl;
                SendInput(1, &inputs[1], sizeof(INPUT));
            }
            MoveMemory(&prevstate, &state, sizeof(XINPUT_STATE));

            Sleep(10);
        }
        //std::cout << state.Gamepad.wButtons << std::endl;
    }
    else
    {
        std::cout << "controller not found" << std::endl;
    }
}