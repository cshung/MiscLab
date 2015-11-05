namespace ConsoleApplication2
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using System.Threading.Tasks;

    class Program
    {
        static List<double> Solve(List<double> coefficients)
        {
            List<double> result = new List<double>();
            if (coefficients.Count == 3)
            {
                // Quadratic, we can just solve with simple quadratic formula
                double a = coefficients[0];
                double b = coefficients[1];
                double c = coefficients[2];

                double delta = b * b - 4 * a * c;
                if (delta > 0)
                {
                    double t = Math.Sqrt(delta);
                    result.Add((-b - t) / 2 / a);
                    result.Add((-b + t) / 2 / a);
                }
            }
            else
            {
                List<double> derivative = new List<double>();
                int power = coefficients.Count - 1;
                foreach (var coefficient in coefficients)
                {
                    if (power != 0)
                    {
                        double derivativeCoefficient = power * coefficient;
                        derivative.Add(derivativeCoefficient);
                    }
                    power--;
                }

                List<double> extremaX = Solve(derivative);

                int expectedSignOnPositiveInfinity;
                int expectedSignOnNegativeInfinity;

                if (coefficients[0] > 0)
                {
                    expectedSignOnPositiveInfinity = 1;
                    if (coefficients.Count % 2 == 0)
                    {
                        expectedSignOnNegativeInfinity = -1;
                    }
                    else
                    {
                        expectedSignOnNegativeInfinity = +1;
                    }
                }
                else
                {
                    expectedSignOnPositiveInfinity = -1;
                    if (coefficients.Count % 2 == 0)
                    {
                        expectedSignOnNegativeInfinity = 1;
                    }
                    else
                    {
                        expectedSignOnNegativeInfinity = -1;
                    }
                }

                if (coefficients[0] > 0)
                {
                    // For sufficiently large x, the polynomial should grow to expected sign
                    double probe = Math.Max(1, extremaX.Count > 0 ? extremaX[extremaX.Count - 1] : 1);
                    while (Eval(coefficients, probe) * expectedSignOnPositiveInfinity < 0)
                    {
                        probe *= 2;
                    }
                    extremaX.Add(probe);
                    probe = Math.Min(-1, extremaX.Count > 0 ? extremaX[0] : -1);
                    while (Eval(coefficients, probe) * expectedSignOnNegativeInfinity < 0)
                    {
                        probe *= 2;
                    }
                    extremaX.Insert(0, probe);
                }

                for (int i = 0; i < extremaX.Count - 1; i++)
                {
                    // Could save evals, skipped
                    double left = Eval(coefficients, extremaX[i]);
                    double right = Eval(coefficients, extremaX[i + 1]);
                    if (left * right < 0)
                    {
                        if (left > 0)
                        {
                            result.Add(Bisect(coefficients, extremaX[i + 1], extremaX[i]));
                        }
                        else
                        {
                            result.Add(Bisect(coefficients, extremaX[i], extremaX[i + 1]));
                        }
                    }
                }

            }
            return result;
        }

        private static double Bisect(List<double> coefficients, double evalToNeg, double evalToPos)
        {
            if (Math.Abs(evalToNeg - evalToPos) < 1e-6)
            {
                return evalToNeg;
            }
            else
            {
                double mid = (evalToNeg + evalToPos) / 2;
                double midEval = Eval(coefficients, mid);
                if (midEval > 0)
                {
                    return Bisect(coefficients, evalToNeg, mid);
                }
                else
                {
                    return Bisect(coefficients, mid, evalToPos);
                }
            }
        }

        private static double Eval(List<double> coefficients, double probe)
        {
            return EvalImpl(coefficients, coefficients.Count, probe);
        }

        private static double EvalImpl(List<double> coefficients, int count, double probe)
        {
            if (count == 1)
            {
                return coefficients[0];
            }
            else
            {
                double last = coefficients[count - 1];
                return EvalImpl(coefficients, count - 1, probe) * probe + last;
            }
        }

        static void Main(string[] args)
        {
            List<double> coeff = new double[] { 0.999999999999998, 8.0, 28.0, 56.0, 70.0, 56.0, 28.0, 8.0, 1.0 }.ToList();
            List<double> roots = Solve(coeff);
            foreach (var root in roots)
            {
                Console.WriteLine("f({0}) = {1}", root, Eval(coeff, root));
            }
        }
    }
}