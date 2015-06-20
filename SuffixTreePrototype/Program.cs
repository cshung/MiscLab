namespace SuffixTreePrototype
{
    using System;

    internal static class Program
    {
        private static void Main(string[] args)
        {
            SuffixTree result = SuffixTree.Build("Hello world to suffix tree, I finally made it, this is non-trivial, challenging, but rewarding!");
            Console.WriteLine(result);
        }
    }
}
