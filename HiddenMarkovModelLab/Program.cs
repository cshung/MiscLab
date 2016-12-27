namespace HiddenMarkovModelLab
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

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

            var random = new Random(0);
            var sequences = GenerateSequences<int>(random);

            // The hard part is to make sense of the data out of the generated sequence
            // Here is the code to train a hidden markov model
            HiddenMarkovModel<int> hmm = new DiscreteHiddenMarkovModel(/* numberOfStates = */2, /* numberOfOutcomes = */ 2);
            // HiddenMarkovModel<double> hmm = new ContinuousHiddenMarkovModel(/* numberOfStates = */2);
            hmm.Bootstrap(random);
            for (int i = 0; i < 200; i++)
            {
                hmm.Train(sequences);
            }

            hmm.Show();

            foreach (var sequence in sequences)
            {
                Console.WriteLine("============================");
                Console.WriteLine(string.Join("", sequence));
                Console.WriteLine("============================");
            }

            Console.WriteLine(string.Join("", hmm.BestStateSequence(sequences[0])));
            Console.WriteLine("============================");
        }

        private static T[][] GenerateSequences<T>(Random random) where T : IConvertible
        {
            var sequencesList = new List<T[]>();
            for (int i = 0; i < 10; i++)
            {
                sequencesList.Add(GenerateSequence(random, 100).Select(t => (T)Convert.ChangeType(t, typeof(T))).ToArray());
            }

            var sequences = sequencesList.ToArray();
            return sequences;
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