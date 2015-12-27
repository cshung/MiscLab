namespace Trapezoid
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    internal static class Program
    {
        private static IEnumerable<double> Trapezoid(double integrateSrc, double integrateDst, int numIntervals, Func<double, double> f)
        {
            yield return 0;
            double N = (double)numIntervals;
            double range = integrateDst - integrateSrc;
            double delta = range / N;

            double current = f(integrateSrc) * 0.5 * delta;
            for (int n = 1; n <= numIntervals; n++)
            {
                double currentX = integrateSrc + (n / N) * range;
                double eval = f(currentX);
                current += eval * 0.5 * delta;
                yield return current;
                current += eval * 0.5 * delta;
            }
        }

        private static void Main(string[] args)
        {
            var result = Trapezoid(-10, 10, 200, (x) => x * x);
            var expect = Enumerable.Range(0, 201).Select(t => t / 10.0 - 10.0).Select(t => t * t * t / 3.0 - (-10) * (-10) * (-10) / 3.0);
            var differ = Enumerable.Zip(result, expect, (a, b) => a - b);
            Console.WriteLine(string.Join("\n", differ));
        }
    }
}
