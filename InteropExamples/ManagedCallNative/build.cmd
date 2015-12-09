midl NativeInterface.idl
cl /Zi /LD NativeLibrary.cpp
tlbimp NativeInterface.tlb
csc /reference:NativeInterface.dll ManagedApplication.cs