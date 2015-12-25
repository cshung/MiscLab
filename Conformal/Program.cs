using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Numerics;

namespace Conformal
{
    class Program
    {
        static Complex Integrand(Complex input)
        {
            // [(w - 250)(w + 250)]^(-2/3)
            Complex inside = (input - 250) * (input + 250);
            return Complex.Pow(inside, -2.0 / 3);
        }

        static void Main(string[] args)
        {
            //for (int real = -100; real <= 100; real += 10)
            //{
            //    int real = 0;
            //    Complex result = new Complex(0, 0);
            //    for (int imag = 0; imag < 100; imag++)
            //    {
            //        Complex input = new Complex(real + 0.1, imag + 0.1);
            //        result += Integrand(input);
            //        Console.WriteLine("{0}, {1}", result.Real, result.Imaginary);
            //    }
            //}
            //for (int imag = 0; imag <= 100; imag += 10)
            {
                int imag = 10;
                Complex result = new Complex(0, 0);
                for (int real = -5000; real < 5000; real++)
                {
                    // Bugbug: The numerical integration must base itself from 0!
                    Complex input = new Complex(real/10.0 + 0.1, imag + 0.1);
                    result += Integrand(input) * 0.1;
                    Console.WriteLine("{0}, {1}", result.Real, result.Imaginary);
                }
            }
        }
    }
}
