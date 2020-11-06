namespace FastFourierTransform
{
    using System;
    using System.Linq;

    public class ConvolutionCalculator
    {
        private double[] signal;
        private double[] filter;

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

            FastFourierTransform(this.filter.Length * 2, 0, 1, filterReal, filterImag, 0, 1, this.filterFrequencyReal, this.filterFrequencyImag, 1);

            int i = 0;
            while (i < this.signal.Length)
            {
                for (int j = 0; j < this.filter.Length; j++)
                {
                    this.windowReal[j] = (i + j) < this.signal.Length ? this.signal[i + j] : 0;
                }

                FastFourierTransform(this.filter.Length * 2, 0, 1, this.windowReal, this.windowImag, 0, 1, this.windowFrequencyReal, this.windowFrequencyImag, 1);

                for (int k = 0; k < this.filter.Length * 2; k++)
                {
                    this.productReal[k] = (this.filterFrequencyReal[k] * this.windowFrequencyReal[k]) - (this.filterFrequencyImag[k] * this.windowFrequencyImag[k]);
                    this.productImag[k] = (this.filterFrequencyReal[k] * this.windowFrequencyImag[k]) + (this.filterFrequencyImag[k] * this.windowFrequencyReal[k]);
                }

                FastFourierTransform(this.filter.Length * 2, 0, 1, this.productReal, this.productImag, 0, 1, this.resultReal, this.resultImag, -1);

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

        void FastFourierTransform(int length, int inputOffset, int inputStride, double[] inputReal, double[] inputImag, int outputOffset, int outputStride, double[] outputReal, double[] outputImag, int sign)
        {
            // Step 1: Base case handling
            if (length == 1)
            {
                outputReal[outputOffset] = inputReal[inputOffset];
                outputImag[outputOffset] = inputImag[inputOffset];
                return;
            }

            // Step 2: Recursive calls
            int half = length / 2;
            FastFourierTransform(half, inputOffset, inputStride * 2, inputReal, inputImag, outputOffset, outputStride, outputReal, outputImag, sign);
            FastFourierTransform(half, inputOffset + inputStride, inputStride * 2, inputReal, inputImag, outputOffset + half, outputStride, outputReal, outputImag, sign);

            // Step 3: Merge results
            for (int i = 0; i < half; i++)
            {
                double angle = -2 * sign * Math.PI / length * i;
                double cosine = Math.Cos(angle);
                double sine = Math.Sin(angle);

                double aReal = outputReal[outputOffset + i * outputStride];
                double aImag = outputImag[outputOffset + i * outputStride];
                double bReal = outputReal[outputOffset + (i + half) * outputStride];
                double bImag = outputImag[outputOffset + (i + half) * outputStride];

                double cReal = aReal + bReal * cosine - bImag * sine;
                double cImag = aImag + bReal * sine + bImag * cosine;
                double dReal = aReal - bReal * cosine + bImag * sine;
                double dImag = aImag - bReal * sine - bImag * cosine;

                outputReal[outputOffset + i * outputStride] = cReal;
                outputImag[outputOffset + i * outputStride] = cImag;
                outputReal[outputOffset + (i + half) * outputStride] = dReal;
                outputImag[outputOffset + (i + half) * outputStride] = dImag;
            }
        }
    }
}