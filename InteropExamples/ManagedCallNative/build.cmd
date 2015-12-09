midl NativeInterface.idl
cl /LD NativeLibrary.cpp
tlbimp NativeInterface.tlb
csc /reference:NativeInterface.dll ManagedApplication.cs