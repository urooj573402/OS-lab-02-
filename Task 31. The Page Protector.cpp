#include <iostream>
#include <windows.h>

using namespace std;

char* page;

LONG WINAPI MemoryProtectionHandler(EXCEPTION_POINTERS* ExceptionInfo)
{
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
    {
        DWORD oldProtect;

        VirtualProtect(page, 4096, PAGE_READWRITE, &oldProtect);

        cout << "Hardware memory protection trap caught!" << endl;

        return EXCEPTION_CONTINUE_EXECUTION;
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

int main()
{
    page = (char*)VirtualAlloc(
        nullptr,
        4096,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );

    if (page == nullptr)
    {
        cout << "Memory allocation failed!" << endl;
        return 1;
    }

    page[0] = 'A';

    DWORD oldProtect;

    VirtualProtect(
        page,
        4096,
        PAGE_READONLY,
        &oldProtect
    );

    AddVectoredExceptionHandler(
        1,
        MemoryProtectionHandler
    );

    page[0] = 'B';

    cout << "Program completed successfully!" << endl;

    VirtualFree(page, 0, MEM_RELEASE);

    return 0;
}