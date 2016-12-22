namespace FastFourierTransform
{
    using System;
    using System.Diagnostics;
    using System.Linq;

    internal static class Program
    {
        private static void Main(string[] args)
        {
            Random r = new Random(0);

            int filter_size = 2048;

            double[] signal = Enumerable.Range(0, filter_size * 100).Select(t => r.NextDouble()).ToArray();
            double[] filter = Enumerable.Range(0, filter_size).Select(_ => 1 / 2048.0).ToArray();

            Stopwatch sw = Stopwatch.StartNew();
            double[] fast_answer = new ConvolutionCalculator(signal, filter).Calculate();
            Console.WriteLine(sw.Elapsed);

            sw = Stopwatch.StartNew();
            double[] dump_answer = new double[signal.Length + filter.Length - 1];
            for (int j = 0; j < dump_answer.Length; j++)
            {
                double temp = 0;
                for (int i = 0; i < filter.Length; i++)
                {
                    int signalIndex = i + j - filter.Length + 1;
                    int filterIndex = filter.Length - i - 1;

                    double signal_value;
                    if (signalIndex < 0)
                    {
                        signal_value = 0;
                    }
                    else if (signalIndex >= signal.Length)
                    {
                        signal_value = 0;
                    }
                    else
                    {
                        signal_value = signal[signalIndex];
                    }

                    temp += signal_value * filter[filterIndex];
                }

                dump_answer[j] = temp;
            }

            double squared_error = 0;
            for (int i = 0; i < signal.Length + filter.Length - 1; i++)
            {
                double error = fast_answer[i] - dump_answer[i];
                squared_error += error * error;
            }

            Console.WriteLine(sw.Elapsed);
            Console.Write(squared_error);
        }
    }
}
