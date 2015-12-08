csc /t:library /out:ManagedLibrary.dll ManagedComponent.cs
tlbexp ManagedLibrary.dll
cl /EHsc NativeApplication.cpp
link NativeApplication.obj
mt.exe -manifest ManagedLibrary.manifest -outputresource:ManagedLibrary.dll;2
mt.exe -manifest NativeApplication.exe.manifest -outputresource:NativeApplication.exe;1