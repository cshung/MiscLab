The grammar:
```
Document := EMPTY
Document := Element Document
Element  := TEXT
Element  := Cell
Cell     := {Name}
Cell     := {Name:Value}
Name     := ID
Value    := TEXT
```
To be continued