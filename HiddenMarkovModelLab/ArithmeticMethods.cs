namespace HiddenMarkovModelLab
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    public static class ArithmeticMethods
    {
        public static SafeNumber Add(SafeNumber x, SafeNumber y)
        {
            double xv = x.Value;
            double yv = y.Value;
            double rv = xv + yv;
            Check(rv);
            return new SafeNumber(rv);
        }

        public static SafeNumber Sub(SafeNumber x, SafeNumber y)
        {
            double xv = x.Value;
            double yv = y.Value;
            double rv = xv - yv;
            Check(rv);
            return new SafeNumber(rv);
        }

        public static SafeNumber Mul(SafeNumber x, SafeNumber y)
        {
            double xv = x.Value;
            double yv = y.Value;
            double rv = xv * yv;
            Check(rv);
            return new SafeNumber(rv);
        }

        public static bool Greater(SafeNumber x, SafeNumber y)
        {
            double xv = x.Value;
            double yv = y.Value;
            return xv > yv;
        }

        public static SafeNumber Div(SafeNumber x, SafeNumber y)
        {
            double xv = x.Value;
            double yv = y.Value;
            double rv = xv / yv;
            Check(rv);
            return new SafeNumber(rv);
        }

        public static bool Smaller(SafeNumber x, SafeNumber y)
        {
            double xv = x.Value;
            double yv = y.Value;
            return xv < yv;
        }

        public static SafeNumber Log(double p)
        {
            if (p == 0)
            {
                // exp(-23) is small enough to consider as negative infinity
                return new SafeNumber(-23);
            }
            else
            {
                double q = Math.Log(p);
                Check(q);
                return new SafeNumber(q);
            }
        }

        public static SafeNumber Log(SafeNumber p)
        {
            return ArithmeticMethods.Log(p.Value);
        }

        public static SafeNumber LogSum(IEnumerable<SafeNumber> input)
        {
            return input.Aggregate((x, y) => ArithmeticMethods.LogAdd(x, y));
        }

        public static SafeNumber Exp(SafeNumber p)
        {
            double result = Math.Exp(p.Value);
            Check(result);
            return new SafeNumber(result);
        }

        public static SafeNumber Floor(SafeNumber p, double v)
        {
            if (p.Value < v)
            {
                return new SafeNumber(v);
            }
            else
            {
                return p;
            }
        }

        public static SafeNumber Abs(SafeNumber p)
        {
            double result = Math.Abs(p.Value);
            Check(result);
            return new SafeNumber(result);
        }

        public static SafeNumber LogAdd(SafeNumber x, SafeNumber y)
        {
            double xv = x.Value;
            double yv = y.Value;

            double temp, diff, z;

            if (xv < yv)
            {
                temp = xv;
                xv = yv;
                yv = temp;
            }

            // Now xv >= yv

            // diff is log(Y) - log(X) = log(Y/X)
            diff = yv - xv;

            if (diff < -23)
            {
                // Y/X is really small, in the sense that log(Y/X) < -23 => Y/X < e^(-23)
                // If we were to run the else branch, it would lead to computing Log(1 + z) = Log(1 + e^diff) ~= Log(1) = 0
                // We might as well save our trouble and just return xv instead.
                return new SafeNumber(xv);
            }
            else
            {
                // z = Y/X
                z = Math.Exp(diff);
                Check(z);

                // result = log(X) + Log(1 + Y/X) = log(X(1 + Y/X)) + log(X + Y)
                double result = xv + Math.Log(1.0 + z);
                Check(result);
                return new SafeNumber(result);
            }
        }

        private static void Check(double rv)
        {
            if (double.IsNaN(rv))
            {
                throw new Exception("NaN");
            }

            if (double.IsPositiveInfinity(rv))
            {
                throw new Exception("IsPositiveInfinity");
            }

            if (double.IsNegativeInfinity(rv))
            {
                throw new Exception("IsNegativeInfinity");
            }
        }
    }
}
