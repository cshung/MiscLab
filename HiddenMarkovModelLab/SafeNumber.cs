namespace HiddenMarkovModelLab
{
    public struct SafeNumber
    {
        private double v;

        public SafeNumber(double v)
        {
            this.v = v;
        }

        public double Value
        {
            get { return this.v; }
        }

        public static SafeNumber operator +(SafeNumber a, SafeNumber b)
        {
            return ArithmeticMethods.Add(a, b);
        }

        public static SafeNumber operator -(SafeNumber a, SafeNumber b)
        {
            return ArithmeticMethods.Sub(a, b);
        }

        public static SafeNumber operator *(SafeNumber a, SafeNumber b)
        {
            return ArithmeticMethods.Mul(a, b);
        }

        public static SafeNumber operator /(SafeNumber a, SafeNumber b)
        {
            return ArithmeticMethods.Div(a, b);
        }

        public static bool operator >(SafeNumber a, SafeNumber b)
        {
            return ArithmeticMethods.Greater(a, b);
        }

        public static bool operator <(SafeNumber a, SafeNumber b)
        {
            return ArithmeticMethods.Smaller(a, b);
        }
    }
}
