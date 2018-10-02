#pragma once

#include <windows.h>

class BombClient
{
public:
    void Attach(DWORD pid, HANDLE bomb1, HANDLE bomb2);
    void WaitForBomb();
    void ContinueBomb();
private:
    HANDLE bomb1Dup;
    HANDLE bomb2Dup;
};