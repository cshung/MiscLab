using System;
using System.Linq;

namespace MonteCarlos
{
    class Program
    {
        private const int trials = 100000;

        static void Main(string[] args)
        {
            // This is a simple MH algorithm!

            Random random = new Random(0);

            SampleNonUniformGrid(random);

            Console.WriteLine("Bayesian Learning");
            BayesianLearning(random);
        }

        private static void BayesianLearning(Random random)
        {
            const int numSamples = 1000;
            // Generating some Gaussian samples
            double[] samples = new double[numSamples];
            for (int i = 0; i < numSamples; i += 2)
            {
                double u1 = random.NextDouble();
                double u2 = random.NextDouble();
                double r = Math.Sqrt(-2 * Math.Log(u1));
                double theta = 2 * Math.PI * u2;
                double x = r * Math.Cos(theta);
                double y = r * Math.Sin(theta);

                // Mean 30 - Variance 400
                x *= 20;
                x += 30;
                y *= 20;
                y += 30;

                samples[i] = x;
                samples[i + 1] = y;
            }

            // Bayesian learning

            // Here is our state space of two real numbers (i.e. the plane)
            // Variance has to be positive, but log variance can take any value, that make the markov chain part easy.
            double mean = samples.Min();
            double logVariance = 0;

            // Here are the integral values to compute, 
            double meanSum = 0;
            double varianceSum = 0;

            for (int i = 0; i < trials; i++)
            {
                double logLikelihood = GetLogLikelihood(samples, mean, logVariance);
                double delta_mean = random.NextDouble() - 0.5;
                double delta_log_variance = random.NextDouble() - 0.5;
                double newLogLikelihood = GetLogLikelihood(samples, mean + delta_mean, logVariance + delta_log_variance);

                // Implicitly, we are assuming a flat, uninformative prior here.

                // P(theta'|D) = P(D|theta') * P(theta') / P(D)
                // P(theta|D) = P(D|theta) * P(theta) / P(D)
                // => P(theta'|D) / P(theta|D) = P(D|theta') / P(D|theta)

                double acceptanceLogLikelihood = newLogLikelihood - logLikelihood;

                if (Math.Log(random.NextDouble()) < acceptanceLogLikelihood)
                {
                    mean += delta_mean;
                    logVariance += delta_log_variance;
                }

                // Here we are processing a sample in the parameter space according to the posterior probability distribution
                // Computing expected value like this is no different from computing the expected value of a dice by averaging 
                // a lot of samples.
                meanSum += mean;
                varianceSum += Math.Exp(logVariance);
            }

            // We could have other statistics about the parameters too, if we wish.
            Console.WriteLine(meanSum / trials);
            Console.WriteLine(varianceSum / trials);
        }

        // The interesting thing about this example is that the probability model we wanted can be arbitrarily complex
        // As long as we know how to compute the log likelihood, we are good to go to estimate the parameters using MCMC.
        private static double GetLogLikelihood(double[] samples, double mean, double logVariance)
        {
            double logLikelihood = 0;
            double variance = Math.Exp(logVariance);
            double sigma = Math.Sqrt(variance);
            double logSigma = Math.Log(sigma);
            for (int j = 0; j < samples.Length; j++)
            {
                logLikelihood += LogNormDist(samples[j], mean, variance, logSigma, logVariance);
            }

            return logLikelihood;
        }

        private static double LogNormDist(double v, double mean, double variance, double logSigma, double logVariance)
        {
            double diff = v - mean;
            return -logSigma - diff * diff / 2 / variance;
        }


        private static void SampleNonUniformGrid(Random random)
        {
            // The goal of this program is to generate random sample such that the cell with weight 9 is 9 time more probable to be sampled 
            // This small problem would look like stupid because it is really easy to generate such a distribution without a Markov Chain,
            // but we will try Markov chain anyway.

            double[,] weight = new double[3, 3]
            {
                { 1, 2, 3 },
                { 4, 5, 6 },
                { 7, 8, 9 }
            };

            // The idea of Markov Chain Monte Carlos algorithm is that we can engineer a Markov Chain such that its stationary distribution is the 
            // distribution we wanted. Suppose we already entered the stationary state, the probability on ending in each cell is denoted by P, so
            // we need to have

            // P(a) P(a -> a) + P(b) P(b -> a) + P(c) P (c -> a) ... = P(a)
            //                                                       = P(a) (P(a -> a) + P(a -> b) + P(a -> c) + ... )

            // Such an equation is called the global balance equation, it is sufficient for the chain to have our desired stationary probability, but
            // we can't really compute the transition probabilities, so let's ask for more, we equate it term by term and get

            // p(b) p(b -> a) = p(a) p (a -> b)
            // ...

            // These are called the detailed balance equations, obviously, detailed balance implies global balance, and 
            // Suppose we define P(b -> a) = min(1, P(b)/P(a)), and vice verse P(a -> b) = min(1, P(a)/P(b)), now the detailed balance is automatically 
            // satisfied, the below is an example showing such a markov chain will actually generate the stationary distribution we wanted as follow:

            int[,] count = new int[3, 3];
            int x = 0;
            int y = 0;
            for (int t = 0; t < trials; t++)
            {
                int direction = (int)(random.NextDouble() * 4);
                int cx = x, cy = y;
                switch (direction)
                {
                    case 0:
                        cx++;
                        break;
                    case 1:
                        cy++;
                        break;
                    case 2:
                        cx--;
                        break;
                    case 3:
                        cy--;
                        break;
                }
                if (0 <= cx && cx <= 2 && 0 <= cy && cy <= 2)
                {
                    double acceptance = weight[cx, cy] / weight[x, y];

                    // Should be min(1, acceptance), but the 1 is actually implies because random.NextDouble() < 1 always
                    if (random.NextDouble() < acceptance)
                    {
                        x = cx;
                        y = cy;
                    }
                }
                count[x, y]++;
            }

            // Showing the weights
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    Console.Write(((0.0 + count[i, j]) / trials * 45).ToString("0.0000") + " ");
                }
                Console.WriteLine();
            }
        }
    }
}
