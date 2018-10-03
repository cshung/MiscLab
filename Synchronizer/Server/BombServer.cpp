#include "BombServer.h"
#include <stdint.h>

BombServer::BombServer()
{
    bomb1 = CreateEvent(NULL, FALSE, FALSE, nullptr);
    bomb2 = CreateEvent(NULL, FALSE, FALSE, nullptr);
}

void BombServer::Show()
{
    wprintf(L"process id = %d\n", GetCurrentProcessId());
    wprintf(L"bomb1 = %lld\n", (int64_t)bomb1);
    wprintf(L"bomb2 = %lld\n", (int64_t)bomb2);
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
