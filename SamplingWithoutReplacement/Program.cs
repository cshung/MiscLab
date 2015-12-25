using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SamplingWithoutReplacement
{
    class Program
    {
        static void Main(string[] args)
        {
            int numSamples = 5;
            int numProblem = 17;

            var problem = Enumerable.Range(1, numProblem).ToList();
            var samples = new List<int>();
            Random random = new Random();
            for (int i = 0; i < numSamples; i++)
            {
                int id = random.Next(problem.Count());
                samples.Add(problem[id]);
                problem.RemoveAt(id);
            }
            samples.Sort();
            Console.WriteLine(string.Join(",", samples));
        }
    }
}
