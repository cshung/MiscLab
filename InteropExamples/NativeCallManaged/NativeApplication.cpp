#include <iostream>

#define _WIN32_DCOM
#import "ManagedLibrary.tlb" no_namespace, raw_interfaces_only

/*
copy /y ManagedLibrary\bin\Debug\ManagedLibrary.dll Debug\
mt.exe -manifest ManagedLibrary\App.manifest -outputresource:Debug\ManagedLibrary.dll;2
mt.exe -manifest NativeApplication\NativeApplication.exe.manifest -outputresource:Debug\NativeApplication.exe;1
 */

using namespace std;

int main(int argc, char** argv)
{
    CoInitializeEx(0, COINIT_MULTITHREADED);

    IManagedComponentPtr  ptr;
    HRESULT hr = ptr.CreateInstance(__uuidof(ManagedComponent));

    long s = 7;
    long t;
    ptr->ManagedMethod(s, &t);
    cout << t << endl;

    CoUninitialize();
    return 0;
}