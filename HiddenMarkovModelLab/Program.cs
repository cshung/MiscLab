using System;
using System.Collections.Generic;
using System.Linq;

namespace HiddenMarkovModelLab
{
    internal static class Program
    {
        // A fixed seed to make life easy when debugging
        private static Random r = new Random(0);

        private static void Main(string[] args)
        {
            // The goal of this project is to experiment with Hidden Markov Model.

            // The first part of the project is to build a simple hidden markov model data generator
            // That will generate some sample sequences, and then we learn another hidden markov model out 
            // of these sample sequence
            //
            // This data is relatively straightforward to generate
            //
            var sequence = GenerateSequence(2000).ToArray();

            // The hard part is to make sense of the data out of the generated sequence
            // Here is the code to train a hidden markov model
            // HiddenMarkovModel hmm = new DiscreteHiddenMarkovModel(/* numberOfStates = */2, /* numberOfOutcomes = */ 2);
            HiddenMarkovModel hmm = new ContinuousHiddenMarkovModel(/* numberOfStates = */2);
            hmm.Bootstrap();
            for (int i = 0; i < 200; i++)
            {
                hmm.Train(sequence);
                hmm.Show();
            }
            Console.WriteLine(string.Join("", hmm.BestStateSequence(sequence)));
        }

        // Generating a random sequence based on a hard coded hidden markov model
        private static IEnumerable<int> GenerateSequence(int length)
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

        // A model based on there are only a handful of observation values
        private class DicreteHiddenMarkovModel : HiddenMarkovModel
        {
            private int numberOfOutcomes;
            private double[,] outcomeProbabilities;

            public DicreteHiddenMarkovModel(int numberOfStates, int numberOfOutcomes) : base(numberOfStates)
            {
                this.numberOfOutcomes = numberOfOutcomes;
                this.outcomeProbabilities = new double[numberOfStates, numberOfOutcomes];
            }

            public override void Bootstrap()
            {
                base.Bootstrap();
                for (int s = 0; s < this.numberOfStates; s++)
                {
                    // For outcome probabilities, we have to break the symmetry - without that all states look identical and it will stay that way
                    var randomValues = Enumerable.Range(0, this.numberOfOutcomes).Select(_ => r.NextDouble());
                    var sum = randomValues.Sum();
                    var randomProbabilities = randomValues.Select(v => v / sum).ToArray();

                    for (int c = 0; c < this.numberOfOutcomes; c++)
                    {
                        this.outcomeProbabilities[s, c] = Math.Log(randomProbabilities[c]);
                    }
                }
            }

            protected override void TrainObservations(int[] sequence, double[,] gamma)
            {
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    var time = Enumerable.Range(0, sequence.Length);
                    double d = this.LogSum(time.Select(t => gamma[j, t]));
                    for (int c = 0; c < this.numberOfOutcomes; c++)
                    {
                        double n = this.LogSum(time.Where(t => sequence[t] == c).Select(t => gamma[j, t]));
                        this.outcomeProbabilities[j, c] = n - d;
                    }
                }
            }

            public override void Show()
            {
                base.Show();
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    for (int c = 0; c < this.numberOfOutcomes; c++)
                    {
                        Console.Write(Math.Exp(this.outcomeProbabilities[i, c]).ToString("0.0000"));
                        Console.Write(" ");
                    }
                    Console.WriteLine();
                }
            }

            protected override double OutcomeProbability(int state, int observation)
            {
                return this.outcomeProbabilities[state, observation];
            }
        }

        private class ContinuousHiddenMarkovModel : HiddenMarkovModel
        {
            private double[] means;
            private double[] variances;

            public ContinuousHiddenMarkovModel(int numberOfStates) : base(numberOfStates)
            {
                this.means = new double[numberOfStates];
                this.variances = new double[numberOfStates];
            }

            protected override double OutcomeProbability(int state, int observation)
            {
                return -Math.Log(2 * Math.PI) / 2 - SafeLog(variances[state]) - (observation - means[state]) * (observation - means[state]) / 2 / variances[state];
            }

            protected override void TrainObservations(int[] sequence, double[,] gamma)
            {
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    var time = Enumerable.Range(0, sequence.Length);
                    double d = this.LogSum(time.Select(t => gamma[j, t]));
                    double nm = this.LogSum(time.Select(t => this.SafeLog(sequence[t]) + gamma[j, t]));
                    double nv = this.LogSum(time.Select(t => 2 * this.SafeLog(Math.Abs(sequence[t] - means[j])) + gamma[j, t]));
                    means[j] = Math.Exp(nm - d);
                    variances[j] = Math.Exp(nv - d);
                    // Make sure the variance is not too small 
                    if (variances[j] < 1e-3)
                    {
                        variances[j] = 0.01;
                    }
                }
            }

            public override void Bootstrap()
            {
                base.Bootstrap();
                for (int s = 0; s < this.numberOfStates; s++)
                {
                    this.means[s] = r.NextDouble();
                    this.variances[s] = r.NextDouble();
                }
            }

            public override void Show()
            {
                base.Show();
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    Console.WriteLine(this.means[i].ToString("0.0000") + ", " + this.variances[i].ToString("0.0000"));
                }
                Console.WriteLine();
            }
        }

        private abstract class HiddenMarkovModel
        {
            protected int numberOfStates;
            private double[] initialProbabilities;
            private double[,] transitionProbabilities;

            public HiddenMarkovModel(int numberOfStates)
            {
                this.numberOfStates = numberOfStates;
                this.initialProbabilities = new double[numberOfStates];
                this.transitionProbabilities = new double[numberOfStates, numberOfStates];
            }

            public virtual void Bootstrap()
            {
                double p = Math.Log(1.0 / this.numberOfStates);
                for (int s = 0; s < this.numberOfStates; s++)
                {
                    this.initialProbabilities[s] = p;
                    for (int t = 0; t < this.numberOfStates; t++)
                    {
                        this.transitionProbabilities[s, t] = p;
                    }
                }
            }

            public virtual void Train(int[] sequence)
            {
                double[,] alpha = ComputeForwardProbabilities(sequence);
                double[,] beta = ComputeBackwardProbabilities(sequence);
                double[,] gamma = ComputePosteriorProbabilities(alpha, beta, sequence.Length);
                double[,,] xi = ComputeTransitionProbabilities(alpha, beta, sequence);

                for (int i = 0; i < this.numberOfStates; i++)
                {
                    this.initialProbabilities[i] = gamma[i, 0];
                }
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    for (int j = 0; j < this.numberOfStates; j++)
                    {
                        var time = Enumerable.Range(0, sequence.Length - 1);
                        double n = this.LogSum(time.Select(t => xi[i, j, t]));
                        double d = this.LogSum(time.Select(t => gamma[i, t]));
                        this.transitionProbabilities[i, j] = n - d;
                    }
                }

                this.TrainObservations(sequence, gamma);

                double log_likelihood = this.LogSum(Enumerable.Range(0, this.numberOfStates).Select(i => alpha[i, sequence.Length - 1]));
                Console.WriteLine(log_likelihood.ToString("0.0000"));
            }

            public int[] BestStateSequence(int[] sequence)
            {
                double[,] delta = new double[this.numberOfStates, sequence.Length];
                int[,] best_state = new int[this.numberOfStates, sequence.Length];
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    delta[i, 0] = this.initialProbabilities[i] + this.OutcomeProbability(i, sequence[0]);
                }

                for (int t = 1; t < sequence.Length; t++)
                {
                    for (int j = 0; j < this.numberOfStates; j++)
                    {
                        double max_so_far = delta[0, t - 1] + this.transitionProbabilities[0, j];
                        int max_so_far_index = 0;
                        for (int i = 1; i < this.numberOfStates; i++)
                        {
                            double temp = delta[i, t - 1] + this.transitionProbabilities[i, j];
                            if (temp > max_so_far)
                            {
                                max_so_far = temp;
                                max_so_far_index = i;
                            }
                        }
                        delta[j, t] = max_so_far + this.OutcomeProbability(j, sequence[t]);
                        best_state[j, t] = max_so_far_index;
                    }
                }
                int[] best_state_sequence = new int[sequence.Length];
                double best_log_likelihood = delta[0, sequence.Length - 1];
                int best_last_state = 0;
                for (int j = 1; j < this.numberOfStates; j++)
                {
                    if (delta[j, sequence.Length - 1] > best_log_likelihood)
                    {
                        best_log_likelihood = delta[j, sequence.Length - 1];
                        best_last_state = j;
                    }
                }
                best_state_sequence[sequence.Length - 1] = best_last_state;
                for (int t = sequence.Length - 2; t >= 0; t--)
                {
                    best_state_sequence[t] = best_state[best_state_sequence[t + 1], t + 1];
                }

                return best_state_sequence;
            }

            protected abstract void TrainObservations(int[] sequence, double[,] gamma);

            protected abstract double OutcomeProbability(int state, int observation);

            private double[,,] ComputeTransitionProbabilities(double[,] alpha, double[,] beta, int[] sequence)
            {
                double[,,] xi = new double[this.numberOfStates, this.numberOfStates, sequence.Length - 1];
                // A convoluted way to produce all the state pairs
                var statePairs = Enumerable.Range(0, this.numberOfStates).SelectMany(i => Enumerable.Range(0, this.numberOfStates).Select(j => Tuple.Create(i, j)));

                for (int t = 0; t < sequence.Length - 1; t++)
                {
                    double temp = this.LogSum(statePairs.Select(p => alpha[p.Item1, t] + this.transitionProbabilities[p.Item1, p.Item2] + this.OutcomeProbability(p.Item2, sequence[t + 1]) + beta[p.Item2, t + 1]));
                    for (int i = 0; i < this.numberOfStates; i++)
                    {
                        for (int j = 0; j < this.numberOfStates; j++)
                        {
                            xi[i, j, t] = alpha[i, t] + this.transitionProbabilities[i, j] + this.OutcomeProbability(j, sequence[t + 1]) + beta[j, t + 1] - temp;
                        }
                    }
                }

                return xi;
            }

            private double[,] ComputeForwardProbabilities(int[] sequence)
            {
                double[,] alpha = new double[this.numberOfStates, sequence.Length];

                // Initialization
                for (int s = 0; s < this.numberOfStates; s++)
                {
                    alpha[s, 0] = this.initialProbabilities[s] + this.OutcomeProbability(s, sequence[0]);
                }

                // Induction
                for (int t = 0; t < sequence.Length - 1; t++)
                {
                    for (int j = 0; j < this.numberOfStates; j++)
                    {
                        double temp = this.LogSum(Enumerable.Range(0, this.numberOfStates).Select(i => alpha[i, t] + this.transitionProbabilities[i, j]));
                        alpha[j, t + 1] = temp + this.OutcomeProbability(j, sequence[t + 1]);
                    }
                }
                return alpha;
            }

            private double[,] ComputeBackwardProbabilities(int[] sequence)
            {
                double[,] beta = new double[this.numberOfStates, sequence.Length];
                for (int s = 0; s < this.numberOfStates; s++)
                {
                    beta[s, sequence.Length - 1] = 1;
                }

                for (int t = sequence.Length - 2; t >= 0; t--)
                {
                    for (int i = 0; i < this.numberOfStates; i++)
                    {
                        beta[i, t] = this.LogSum(Enumerable.Range(0, this.numberOfStates).Select(j => this.transitionProbabilities[i, j] + this.OutcomeProbability(j, sequence[t + 1]) + beta[j, t + 1]));
                    }
                }

                return beta;
            }

            private double[,] ComputePosteriorProbabilities(double[,] alpha, double[,] beta, int sequenceLength)
            {
                double[,] gamma = new double[this.numberOfStates, sequenceLength];
                for (int t = 0; t < sequenceLength; t++)
                {
                    double temp = LogSum(Enumerable.Range(0, numberOfStates).Select(i => alpha[i, t] + beta[i, t]));
                    for (int i = 0; i < numberOfStates; i++)
                    {
                        gamma[i, t] = alpha[i, t] + beta[i, t] - temp;
                    }
                }
                return gamma;
            }

            protected double LogSum(IEnumerable<double> input)
            {
                return input.Aggregate((x, y) => this.LogAdd(x, y));
            }

            protected double SafeLog(double x)
            {
                if (x == 0)
                {
                    return -23;
                }
                else
                {
                    return Math.Log(x);
                }
            }

            private double LogAdd(double x, double y)
            {
                if (double.IsNaN(x))
                {
                    throw new Exception();
                }
                if (double.IsNaN(y))
                {
                    throw new Exception();
                }
                double temp, diff, z;

                if (x < y)
                {
                    temp = x; x = y; y = temp;
                }

                diff = y - x;

                if (diff < -23)
                {
                    return (x < -0.5e10) ? -1e10 : x;
                }
                else
                {
                    z = Math.Exp(diff);
                    return x + Math.Log(1.0 + z);
                }
            }

            public virtual void Show()
            {
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    Console.WriteLine(Math.Exp(this.initialProbabilities[i]).ToString("0.0000"));
                }
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    for (int j = 0; j < this.numberOfStates; j++)
                    {
                        Console.Write(Math.Exp(this.transitionProbabilities[i, j]).ToString("0.0000"));
                        Console.Write(" ");
                    }
                    Console.WriteLine();
                }
            }

        }
    }
}
