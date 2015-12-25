﻿namespace Conformal
{
    using System;
    using System.Collections.Generic;
    using System.Numerics;

    internal static class Program
    {
        private static Complex Integrand(Complex input)
        {
            // [(w - 50)(w + 50)]^(-2/3)
            Complex inside = (input - 50) * (input + 50);
            return Complex.Pow(inside, -2.0 / 3.0);
        }

        private static void Main(string[] args)
        {
            // This avoid evaluating the integrand at the poles
            double shift = 0.1;

            // The program implements the Schwarz-Christoffel Mapping
            // The function build a conformal mapping that maps the upper 
            // half plane to a triangle

            // The base for the integration is (-100, 0)
            // We create a grid with real = (-100, 100), imag = (0, 200)
            // Using delta = 0.1
            // The numerical integration is done using trapezoidal rule
            // The initial and final 0.5 is omitted for simplicity

            // There is still some bugs, but not sure what is that ...

            // First, build the X and Y axis
            // I am pretty sure the xMarks is correct as it does plot the initial triangle
            var xMarks = new List<Tuple<Complex, double>>();
            {
                Complex current = new Complex(0, 0);
                double imag = shift;
                for (int realStep = 0; realStep <= 2000; realStep++)
                {
                    double real = realStep / 10.0 - 100.0 + shift;
                    if (realStep % 200 == 0)
                    {
                        xMarks.Add(Tuple.Create(current, real));
                    }
                    var eval = Integrand(new Complex(real, imag));
                    current = current + eval * 0.1;
                    // Console.WriteLine("{0},{1}", current.Real, current.Imaginary);
                }
            }

            var yMarks = new List<Tuple<Complex, double>>();
            {
                Complex current = new Complex(0, 0);
                double real = -100 + shift;
                for (int imagStep = 0; imagStep <= 2000; imagStep++)
                {
                    double imag = imagStep / 10.0 + shift;
                    if (imagStep % 200 == 0)
                    {
                        yMarks.Add(Tuple.Create(current, imag));
                    }
                    var eval = Integrand(new Complex(real, imag));
                    current = current + eval * 0.1;
                    // Console.WriteLine("{0},{1}", current.Real, current.Imaginary);
                }
            }

            // Step 2: Build the grid lines
            // Stepping upwards from the previously built xMarks
            // This lines are really odd
            foreach (var xMark in xMarks)
            {
                double real = xMark.Item2;
                Complex current = xMark.Item1;
                for (int imagStep = 0; imagStep <= 2000; imagStep++)
                {
                    double imag = imagStep / 10.0 + shift;
                    var eval = Integrand(new Complex(real, imag));
                    current = current + eval * 0.1;
                    Console.WriteLine("{0},{1}", current.Real, current.Imaginary);
                }
            }

            // Stepping forward from the previously built yMarks
            // This lines does show a pattern, but I am not sure why they intersect themselves
            foreach (var yMark in yMarks)
            {
                double imag = yMark.Item2;
                Complex current = yMark.Item1;
                for (int realStep = 0; realStep <= 2000; realStep++)
                {
                    double real = realStep / 10.0 - 100.0 + shift;
                    var eval = Integrand(new Complex(real, imag));
                    current = current + eval * 0.1;
                    Console.WriteLine("{0},{1}", current.Real, current.Imaginary);
                }
            }
        }
    }
}
