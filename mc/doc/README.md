# Instructions

Use `npm install` to install the necessary dependencies.

Use `npm tsc` to compile the type script

Use `npm mocha ./dist/test` to run the unit tests

Use `npm webpack` to build the website.

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