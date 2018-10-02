#include "BombClient.h"
#include <stdio.h>

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
    scanf_s("%d", (int*)&bomb1);
    wprintf(L"bomb2: ");
    scanf_s("%d", (int*)&bomb2);

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

