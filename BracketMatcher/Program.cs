namespace BracketMatcher
{
    using System;
    using System.Collections.Generic;

    internal static class Program
    {
        private class Pair
        {
            public List<Pair> children = new List<Pair>();
            public int oIndex;
            public int cIndex;
            public int level;
        }

        private static void Main(string[] args)
        {
            if (args.Length != 1)
            {
                Console.WriteLine("Invalid command");
                Console.WriteLine("  Usage: BracketMatcher input");
                return;
            }

            var input = args[0];

            // Step 1: Build the parse tree with a simple stack
            var stack = new Stack<Pair>();
            var root = new Pair { oIndex = -1, cIndex = input.Length, level = 0 };
            stack.Push(root);
            int maxLevel = 0;
            for (int i = 0; i < input.Length; i++)
            {
                if (input[i] == '(')
                {
                    Pair newPair = new Pair();
                    newPair.oIndex = i;
                    Pair parent = stack.Peek();
                    parent.children.Add(newPair);
                    newPair.level = parent.level + 1;
                    maxLevel = Math.Max(newPair.level, maxLevel);
                    stack.Push(newPair);
                }
                else if (input[i] == ')')
                {
                    Pair match = stack.Pop();
                    match.cIndex = i;
                }
            }

            // Step 2: build the output
            char[,] output = new char[maxLevel + 1, input.Length];
            for (int i = 0; i < maxLevel + 1; i++)
            {
                for (int j = 0; j < input.Length; j++)
                {
                    output[i, j] = ' ';
                }
            }

            var bfsQueue = new Queue<Pair>();
            bfsQueue.Enqueue(root);
            while (bfsQueue.Count > 0)
            {
                var currentPair = bfsQueue.Dequeue();
                if (currentPair.level != 0)
                {
                    for (int i = currentPair.oIndex; i <= currentPair.cIndex; i++)
                    {
                        output[currentPair.level - 1, i] = '-';
                    }
                    for (int i = currentPair.level - 1; i < maxLevel; i++)
                    {
                        output[i, currentPair.oIndex] = '|';
                        output[i, currentPair.cIndex] = '|';
                    }
                    output[currentPair.level - 1, currentPair.oIndex] = '+';
                    output[currentPair.level - 1, currentPair.cIndex] = '+';
                }
                foreach (var child in currentPair.children)
                {
                    bfsQueue.Enqueue(child);
                }
            }
            for (int i = 0; i < input.Length; i++)
            {
                output[maxLevel, i] = input[i];
            }

            // Step 3: Output
            for (int i = 0; i < maxLevel + 1; i++)
            {
                for (int j = 0; j < input.Length; j++)
                {
                    Console.Write(output[i, j]);
                }
                Console.WriteLine();
            }
        }
    }
}
