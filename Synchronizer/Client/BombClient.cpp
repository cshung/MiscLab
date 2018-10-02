#include "BombClient.h"
#include <stdio.h>

void BombClient::Attach(DWORD pid, HANDLE bomb1, HANDLE bomb2)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
    if (hProcess == NULL)
    {
        wprintf(L"OpenProcess failed: %d", GetLastError());
    }
    else
    {
        DuplicateHandle(hProcess, bomb1, GetCurrentProcess(), &bomb1Dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
        DuplicateHandle(hProcess, bomb2, GetCurrentProcess(), &bomb2Dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
        SetEvent(bomb1Dup);
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
