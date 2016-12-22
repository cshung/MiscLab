namespace FastFourierTransform
{
    using System;
    using System.Linq;

    public class ConvolutionCalculator
    {
        private double[] signal;
        private double[] filter;

        // Buffer needed for performing Fast Fourier Transform
        private double[] bufferReal;
        private double[] bufferImag;

        // The filter (padded with N 0) in frequency domain
        private double[] filterFrequencyReal;
        private double[] filterFrequencyImag;

        // The window (padded with N 0) in the time domain
        private double[] windowReal;
        private double[] windowImag;

        // The window (padded with N 0) in the frequency domain
        private double[] windowFrequencyReal;
        private double[] windowFrequencyImag;

        // The product of the filter and the window in the frequency domain
        private double[] productReal;
        private double[] productImag;

        // The inverse Fourier transform of the product gives the convolution for the current window
        private double[] resultReal;
        private double[] resultImag;

        public ConvolutionCalculator(double[] signal, double[] filter)
        {
            this.signal = signal;
            this.filter = filter;

            // Allocating various buffer to use
            this.bufferReal = new double[filter.Length * 2];
            this.bufferImag = new double[filter.Length * 2];
            this.filterFrequencyReal = new double[filter.Length * 2];
            this.filterFrequencyImag = new double[filter.Length * 2];
            this.windowReal = new double[filter.Length * 2];
            this.windowImag = new double[filter.Length * 2];
            this.windowFrequencyReal = new double[filter.Length * 2];
            this.windowFrequencyImag = new double[filter.Length * 2];
            this.productReal = new double[filter.Length * 2];
            this.productImag = new double[filter.Length * 2];
            this.resultReal = new double[filter.Length * 2];
            this.resultImag = new double[filter.Length * 2];
        }

        public double[] Calculate()
        {
            double[] output = new double[this.signal.Length + this.filter.Length - 1];

            // Step 1: Compute the Fourier transform of the filter padded with 0
            double[] filterReal = this.filter.Concat(Enumerable.Range(0, this.filter.Length).Select(_ => 0.0)).ToArray();
            double[] filterImag = new double[this.filter.Length * 2];

            FastFourierTransform(this.filter.Length * 2, 1, 0, filterReal, filterImag, 0, this.filterFrequencyReal, this.filterFrequencyImag, 0, this.bufferReal, this.bufferImag);

            int i = 0;
            while (i < this.signal.Length)
            {
                for (int j = 0; j < this.filter.Length; j++)
                {
                    this.windowReal[j] = (i + j) < this.signal.Length ? this.signal[i + j] : 0;
                }

                FastFourierTransform(this.filter.Length * 2, 1, 0, this.windowReal, this.windowImag, 0, this.windowFrequencyReal, this.windowFrequencyImag, 0, this.bufferReal, this.bufferImag);

                for (int k = 0; k < this.filter.Length * 2; k++)
                {
                    this.productReal[k] = (this.filterFrequencyReal[k] * this.windowFrequencyReal[k]) - (this.filterFrequencyImag[k] * this.windowFrequencyImag[k]);
                    this.productImag[k] = (this.filterFrequencyReal[k] * this.windowFrequencyImag[k]) + (this.filterFrequencyImag[k] * this.windowFrequencyReal[k]);
                }

                FastFourierTransform(this.filter.Length * 2, -1, 0, this.productReal, this.productImag, 0, this.resultReal, this.resultImag, 0, this.bufferReal, this.bufferImag);

                for (int j = 0; j < (2 * this.filter.Length) - 1; j++)
                {
                    if (i + j == output.Length)
                    {
                        break;
                    }
                    else
                    {
                        output[i + j] += this.resultReal[j] / this.filter.Length / 2;
                    }
                }

                i += this.filter.Length;
            }

            return output;
        }

        private static void FastFourierTransform(int length, int sign, int input_start, double[] inputReal, double[] inputImag, int output_start, double[] outputReal, double[] outputImag, int temp_start, double[] tempReal, double[] tempImag)
        {
            // Step 1: Base case handling
            if (length == 1)
            {
                outputReal[output_start + 0] = inputReal[input_start + 0];
                outputImag[output_start + 0] = inputImag[input_start + 0];
                return;
            }

            // Step 1: Decimation in time
            int half = length / 2;
            for (int i = 0; i < half; i++)
            {
                tempReal[temp_start + i] = inputReal[input_start + (2 * i)];
                tempImag[temp_start + i] = inputImag[input_start + (2 * i)];
                tempReal[temp_start + i + half] = inputReal[input_start + (2 * i) + 1];
                tempImag[temp_start + i + half] = inputImag[input_start + (2 * i) + 1];
            }

            for (int i = 0; i < length; i++)
            {
                inputReal[input_start + i] = tempReal[temp_start + i];
                inputImag[input_start + i] = tempImag[temp_start + i];
            }

            // Step 2: Recursive calls
            FastFourierTransform(half, sign, input_start, inputReal, inputImag, temp_start, tempReal, tempImag, output_start, outputReal, outputImag);
            FastFourierTransform(half, sign, input_start + half, inputReal, inputImag, temp_start + half, tempReal, tempImag, output_start + half, outputReal, outputImag);

            // Step 3: Merge results
            for (int i = 0; i < half; i++)
            {
                double angle = sign * 2 * Math.PI / length * i;
                double cosine = Math.Cos(angle);
                double sine = Math.Sin(angle);
                outputReal[output_start + i] = tempReal[temp_start + i] + (tempReal[temp_start + i + half] * cosine) - (tempImag[temp_start + i + half] * sine);
                outputImag[output_start + i] = tempImag[temp_start + i] + (tempReal[temp_start + i + half] * sine) + (tempImag[temp_start + i + half] * cosine);
                outputReal[output_start + i + half] = tempReal[temp_start + i] - (tempReal[temp_start + i + half] * cosine) + (tempImag[temp_start + i + half] * sine);
                outputImag[output_start + i + half] = tempImag[temp_start + i] - (tempReal[temp_start + i + half] * sine) - (tempImag[temp_start + i + half] * cosine);
            }

            // Step 4: (Optional) restore input
            for (int i = 0; i < length; i++)
            {
                tempReal[temp_start + i] = inputReal[input_start + i];
                tempImag[temp_start + i] = inputImag[input_start + i];
            }

            for (int i = 0; i < half; i++)
            {
                inputReal[input_start + (2 * i)] = tempReal[temp_start + i];
                inputImag[input_start + (2 * i)] = tempImag[temp_start + i];
                inputReal[input_start + (2 * i) + 1] = tempReal[temp_start + i + half];
                inputImag[input_start + (2 * i) + 1] = tempImag[temp_start + i + half];
            }
        }
    }
}