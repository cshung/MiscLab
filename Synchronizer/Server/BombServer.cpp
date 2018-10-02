#include "BombServer.h"

BombServer::BombServer()
{
    bomb1 = CreateEvent(NULL, FALSE, FALSE, L"Bomb1");
    bomb2 = CreateEvent(NULL, FALSE, FALSE, L"Bomb2");
}

void BombServer::Show()
{
    wprintf(L"process id = %d\n", GetCurrentProcessId());
    wprintf(L"bomb1 = %d\n", (int)bomb1);
    wprintf(L"bomb2 = %d\n", (int)bomb2);
}

void BombServer::WaitForAttach()
{
    WaitForSingleObject(bomb1, INFINITE);
    ResetEvent(bomb1);
}

void BombServer::Break()
{
    SetEvent(bomb2);
    WaitForSingleObject(bomb1, INFINITE);
    ResetEvent(bomb1);
}
