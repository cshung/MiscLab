#include "BombClient.h"
#include <stdio.h>

void BombClient::Attach(DWORD pid, HANDLE bomb1, HANDLE bomb2)
{
    HANDLE hProcess = OpenProcess(PROCESS_DUP_HANDLE, false, pid);
    if (hProcess == NULL)
    {
        wprintf(L"OpenProcess failed: %d", GetLastError());
    }
    else
    {
        BOOL result = DuplicateHandle(hProcess, bomb1, GetCurrentProcess(), &bomb1Dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
        if (result == 0)
        {
            wprintf(L"DuplicateHandle1: %d\n", GetLastError());
        }
        else
        {
            result = DuplicateHandle(hProcess, bomb2, GetCurrentProcess(), &bomb2Dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
            if (result == 0)
            {
                wprintf(L"DuplicateHandle2: %d\n", GetLastError());
            }
            else
            {
                SetEvent(bomb1Dup);
            }
        }
    }
}

void BombClient::WaitForBomb()
{
    WaitForSingleObject(bomb2Dup, INFINITE);
    ResetEvent(bomb2Dup);
}

void BombClient::ContinueBomb()
{
    SetEvent(bomb1Dup);
}
