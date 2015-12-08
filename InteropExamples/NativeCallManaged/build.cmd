csc /t:library /out:ManagedLibrary.dll ManagedComponent.cs
tlbexp ManagedLibrary.dll
cl /Zi /EHsc NativeApplication.cpp
link /DEBUG NativeApplication.obj
mt.exe -manifest ManagedLibrary.manifest -outputresource:ManagedLibrary.dll;2
mt.exe -manifest NativeApplication.exe.manifest -outputresource:NativeApplication.exe;1