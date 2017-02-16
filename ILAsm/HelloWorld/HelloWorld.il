.assembly extern mscorlib {}
.assembly HelloWorld {}
.module HelloWorld.exe
.namespace Hello
{
  .class public auto ansi Program extends [mscorlib]System.Object
  {
    .method public static void Main() cil managed
    {
     .entrypoint
     ldstr "Hello World"
     call void [mscorlib]System.Console::WriteLine(string)
     ret
    }
  }
}