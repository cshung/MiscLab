#include "BombClient.h"
#include <stdio.h>
#include <stdint.h>

int main()
{
    BombClient bombClient;
    DWORD pid;
    HANDLE bomb1;
    HANDLE bomb2;
    int dummy;

    wprintf(L"process id: ");
    scanf_s("%d", &pid);
    wprintf(L"bomb1: ");
#if _WIN64
    scanf_s("%lld", (int64_t*)&bomb1);
#else
    scanf_s("%d", (int*)&bomb1);
#endif
    wprintf(L"bomb2: ");
#if _WIN64
    scanf_s("%lld", (int64_t*)&bomb2);
#else
    scanf_s("%d", (int*)&bomb2);
#endif

    bombClient.Attach(pid, bomb1, bomb2);

    while (true)
    {
        bombClient.WaitForBomb();
        printf("Look\r\n");
        scanf_s("%d", &dummy);
        bombClient.ContinueBomb();
    }

    return 0;
}

