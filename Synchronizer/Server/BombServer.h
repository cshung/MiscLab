#pragma once

#define UNICODE
#include <windows.h>
#include <stdio.h>

class BombServer
{
public:
    BombServer();
    void Show();
    void WaitForAttach();
    void Break();
private:
    HANDLE bomb1;
    HANDLE bomb2;
};