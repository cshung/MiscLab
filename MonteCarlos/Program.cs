using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MonteCarlos
{
    class Program
    {
        private const int trials = 1000000;

        static void Main(string[] args)
        {
            // This is a simple MH algorithm!

            Random random = new Random(0);

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
