class Program
{
    static void Main()
    {
        string tape = @"        string a = @""class Program
{
    static void Main()
    {
        string tape = @"""""" + tape.Replace(""\"""", ""\""\"""") + @"""""";
"";
        System.Console.WriteLine(a + tape);
    }
}";
        string a = @"class Program
{
    static void Main()
    {
        string tape = @""" + tape.Replace("\"", "\"\"") + @""";
";
        System.Console.WriteLine(a + tape);
    }
}