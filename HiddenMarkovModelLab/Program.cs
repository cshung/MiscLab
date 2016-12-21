using System;
using System.Collections.Generic;
using System.Linq;

namespace HiddenMarkovModelLab
{
    internal static class Program
    {
        private static void Main(string[] args)
        {
            // The goal of this project is to experiment with Hidden Markov Model.

            // The first part of the project is to build a simple hidden markov model data generator
            // That will generate some sample sequences, and then we learn another hidden markov model out 
            // of these sample sequence
            //
            // This data is relatively straightforward to generate
            //

            Random random = new Random(0);
            // var sequence = GenerateSequence(random, 2000).ToArray();
            var sequence = GenerateSequence(random, 2000).Select(t => (double)t).ToArray();

            // The hard part is to make sense of the data out of the generated sequence
            // Here is the code to train a hidden markov model
            // HiddenMarkovModel<int> hmm = new DiscreteHiddenMarkovModel(/* numberOfStates = */2, /* numberOfOutcomes = */ 2);
            HiddenMarkovModel<double> hmm = new ContinuousHiddenMarkovModel(/* numberOfStates = */2);
            hmm.Bootstrap(random);
            for (int i = 0; i < 200; i++)
            {
                hmm.Train(sequence);
            }
            hmm.Show();
            Console.WriteLine(string.Join("", hmm.BestStateSequence(sequence)));
        }

        // Generating a random sequence based on a hard coded hidden markov model
        private static IEnumerable<int> GenerateSequence(Random r, int length)
        {
            // There are two states corresponding to two biased coins
            int state = r.NextDouble() > 0.7 ? 1 : 0;
            for (int i = 0; i < length; i++)
            {
                int observation;
                if (state == 0)
                {
                    // The first coin biased towards head
                    observation = r.NextDouble() > 0.8 ? 1 : 0;
                    // The state tends to stay
                    state = r.NextDouble() > 0.8 ? 1 : 0;
                }
                else
                {
                    // The first coin biased towards tail
                    observation = r.NextDouble() > 0.2 ? 1 : 0;
                    // The state tends to stay even longer
                    state = r.NextDouble() > 0.9 ? 0 : 1;
                }
                yield return observation;
            }
        }
    }
}