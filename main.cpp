#include <Windows.h>
#include <string>
#include "string.hpp"
#include <thread>
#include <TlHelp32.h>
#include "World/game.h"
#include "Includes/sdk.hpp"
#include <iostream>
#include "Xor/xor.hpp"

DWORD_PTR ProcessId;
#pragma warning(suppress : 4996).
int process_id;

DWORD FindProcessID(string name) {
    PROCESSENTRY32 ProcessEntry;
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &ProcessEntry) == (int)1) {
        while (Process32Next(snapshot, &ProcessEntry) == (int)1) {
            if (string(ProcessEntry.szExeFile) == name)
                return ProcessEntry.th32ProcessID;
        }
    }

    CloseHandle(snapshot);
    return NULL;
}

HWND windowid = NULL;

int main()
{
    ProcessId = FindProcessID("FortniteClient-Win64-Shipping.exe");
    if (!ProcessId) {
        std::cout << ("Failed to get games PID please launch your game!") << endl;
        Sleep(2000);
        return 0;
    }

    printf("Looking For Fortnite..", 50);
    system("cls");
    Sleep(500);
    printf("Initializing Cheat", 50);
    system("cls");
    Sleep(2000);
    printf("Looking For GeForce Experience Overlay...", 50);
    Sleep(2000);
    system("cls");
    std::cout << _xor_("\n");

    system("cls");
    Sleep(2000);
    printf("Found Overlay and hooking to fortnite!", 50);
    system("cls");
    Sleep(2000);
    printf("Hooked!", 50);
    system("cls");

    ProcessId = FindProcessID((string)("FortniteClient-Win64-Shipping.exe"));

    if (driver->Init(FALSE)) {
        driver->Attach(ProcessId);
        baseaddy = driver->GetModuleBase(L"FortniteClient-Win64-Shipping.exe");
    }

    
    //okay so we swapped our reading method and now its time for getting base and pid (process id)



    //okay so lets see if the driver will work, okay so we can see that the driver is working just like the cheat goodbye yall and dont
    //forget to join my discord for free sources and drivers




  

    cout << baseaddy;
    system("cls");
    // neura loaded
    system("Color 02");

    settings::width = GetSystemMetrics(SM_CXSCREEN);
    settings::height = GetSystemMetrics(SM_CYSCREEN);

    float ScreenCenterX = settings::width / 2;
    float  ScreenCenterY = settings::height / 2;

    create_overlay();
    directx_init();
    render_loop();

    return 0;
}