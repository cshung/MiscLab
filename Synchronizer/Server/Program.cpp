#include "BombServer.h"

int main()
{
    BombServer bombServer;
    bombServer.Show();
    bombServer.WaitForAttach();

    for (int i = 0; i < 1000; i++)
    {
        if (i % 19 == 0)
        {
            bombServer.Break();
        }
    }

    return 0;
}

