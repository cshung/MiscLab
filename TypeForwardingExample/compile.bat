REM Step 1: A standard master slave application - master is referencing slave1 for the Slave class
csc /target:library /out:Slave1.dll Slave.cs
csc /reference:Slave1.dll Master.cs

REM Step 2: Moved the Slave class implementation to Slave2.dll
csc /target:library /out:Slave2.dll Slave.cs
csc /target:library /reference:Slave2.dll /out:Slave1.dll Assembly.cs
