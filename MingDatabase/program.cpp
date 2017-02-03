#include <iostream>
#include <map>
#include <cstdlib>
#include <cassert>
#include "page.h"

using namespace std;

// #define LOG

int main(int argc, char** argv)
{
    page page;
    cout << page.get_page_number() << endl;
    buffer key;
    key.data = (uint8_t*)"hello";
    key.size = 5;
    page.append_key(key);
    key.data = (uint8_t*)"world";
    key.size = 5;
    page.append_key(key);
    page_iterator x = page.get_keys();
    while (x.has_next())
    {
        buffer r = x.next();
        cout << r.data[0] << endl;
    }
    return 0;
}
