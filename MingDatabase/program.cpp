#include <iostream>
#include "constant.h"
#include "page_file.h"
#include <cstdint>

using namespace std;

// #define LOG

int main(int argc, char** argv)
{
    page_file file("hello.db");
    uint8_t buffer[PAGE_SIZE];
    bool red = true;
    if (red)
    {
        file.read_page(0, buffer);
        cout << (char*)buffer << endl;
    }
    else
    {
        strcpy((char*)buffer, "World");
        file.write_page(0, buffer);
    }
    return 0;
}
