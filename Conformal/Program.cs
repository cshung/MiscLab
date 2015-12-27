namespace Conformal
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Numerics;

    // http://math.aalto.fi/~mnummeli/tutkimus/esitelma071015.pdf
    internal static class Program
    {
        private static Complex Integrand(Complex input)
        {
            // (w - 50)^(-2/3) w^(-2/3) (w + 50)^(-2/3)
            return Complex.Pow((input + 50), -2.0 / 3.0) * Complex.Pow(input, -2.0 / 3.0) * Complex.Pow((input - 50), -2.0 / 3.0);
        }

        private static IEnumerable<Tuple<Complex, Complex>> TrapezoidReal(Complex integrateSrc, Complex integrateDst, int numIntervals, Func<Complex, Complex> f)
        {
            yield return Tuple.Create(integrateSrc, (Complex)0);
            double N = (double)numIntervals;
            double range = integrateDst.Real - integrateSrc.Real;
            double delta = range / N;

            Complex sum = f(integrateSrc) * 0.5 * delta;
            for (int n = 1; n <= numIntervals; n++)
            {
                Complex current = new Complex(integrateSrc.Real + (n / N) * range, integrateSrc.Imaginary);
                Complex eval = f(current);
                sum += eval * 0.5 * delta;
                yield return Tuple.Create(current, sum);
                sum += eval * 0.5 * delta;
            }
        }

        private static IEnumerable<Tuple<Complex, Complex>> TrapezoidImag(Complex integrateSrc, Complex integrateDst, int numIntervals, Func<Complex, Complex> f)
        {
            yield return Tuple.Create(integrateSrc, (Complex)0);
            double N = (double)numIntervals;
            double range = integrateDst.Imaginary - integrateSrc.Imaginary;
            double delta = range / N;

            Complex sum = f(integrateSrc) * new Complex(0, 0.5 * delta);
            for (int n = 1; n <= numIntervals; n++)
            {
                Complex current = new Complex(integrateSrc.Real, integrateSrc.Imaginary + (n / N) * range);
                Complex eval = f(current);
                sum += eval * new Complex(0, 0.5 * delta);
                yield return Tuple.Create(current, sum);
                sum += eval * new Complex(0, 0.5 * delta);
            }
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
            // The grid is divided into 10 lines for simplicity
            // The numerical integration is done using trapezoidal rule

            Complex point_o = new Complex(-100 + shift, 0 + shift);
            Complex point_x = new Complex(100 + shift, 0 + shift);
            Complex point_y = new Complex(-100 + shift, 200 + shift);

            // First, build the X and Y axis
            var xAxis = TrapezoidReal(point_o, point_x, 10000, Integrand);
            var yAxis = TrapezoidImag(point_o, point_y, 10000, Integrand);

            // Sample every 1,000 elements for the marks
            var xMarks = Sample(xAxis, 1000);
            var yMarks = Sample(yAxis, 1000);

            // Build the horizontal grid lines
            foreach (var yMark in yMarks)
            {
                var imag = yMark.Item1.Imaginary;
                var gridPoints = Sample(TrapezoidReal(new Complex(-100 + shift, imag), new Complex(100 + shift, imag), 10000, Integrand).Select(t => Tuple.Create(t.Item1, t.Item2 + yMark.Item2)), 100);
                PrintPoints(gridPoints);
            }

            // Build the vertical grid lines
            foreach (var xMark in xMarks)
            {
                var real = xMark.Item1.Real;
                var gridPoints = Sample(TrapezoidImag(new Complex(real, 0 + shift), new Complex(real, 200 + shift), 10000, Integrand).Select(t => Tuple.Create(t.Item1, t.Item2 + xMark.Item2)), 100);
                PrintPoints(gridPoints);
            }
        }

        private static IEnumerable<Tuple<Complex, Complex>> Sample(IEnumerable<Tuple<Complex, Complex>> xAxis, int every)
        {
            return xAxis.Where((t, i) => i % every == 0);
        }

        private static void PrintPoints(IEnumerable<Tuple<Complex, Complex>> gridPoints)
        {
            foreach (var gridPoint in gridPoints)
            {
                Console.WriteLine("{0},{1},{2},{3}", gridPoint.Item1.Real, gridPoint.Item1.Imaginary, gridPoint.Item2.Real, gridPoint.Item2.Imaginary);
            }
        }
    }
}
