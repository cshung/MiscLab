namespace HiddenMarkovModelLab
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using Newtonsoft.Json;

    public abstract class HiddenMarkovModel<T>
    {
        private int numberOfStates;
        private SafeNumber[] initialLogLikelihoods;
        private SafeNumber[,] transitionLogLikelihoods;

        public HiddenMarkovModel(int numberOfStates)
        {
            this.numberOfStates = numberOfStates;
            this.initialLogLikelihoods = new SafeNumber[numberOfStates];
            this.transitionLogLikelihoods = new SafeNumber[numberOfStates, numberOfStates];
        }

        protected interface IObservationParameters
        {
        }

        public virtual void Bootstrap(Random r)
        {
            double p = 1.0 / this.numberOfStates;
            SafeNumber q = ArithmeticMethods.Log(p);
            for (int s = 0; s < this.numberOfStates; s++)
            {
                this.initialLogLikelihoods[s] = q;
                for (int t = 0; t < this.numberOfStates; t++)
                {
                    this.transitionLogLikelihoods[s, t] = q;
                }
            }
        }

        public virtual void Train(T[][] sequences)
        {
            // Initialize the accumulators
            var initialNumerators = new List<SafeNumber>[this.numberOfStates];
            var transitionNumerators = new List<SafeNumber>[this.numberOfStates, this.numberOfStates];
            var transitionDenominators = new List<SafeNumber>[this.numberOfStates];
            SafeNumber log_likelihoods = new SafeNumber(0);

            for (int i = 0; i < this.numberOfStates; i++)
            {
                initialNumerators[i] = new List<SafeNumber>();
                transitionDenominators[i] = new List<SafeNumber>();
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    transitionNumerators[i, j] = new List<SafeNumber>();
                }
            }

            foreach (var sequence in sequences)
            {
                Console.WriteLine("Processing 1 sequence");
                SafeNumber[,] alpha = this.ComputeForwardLogLikelihoods(sequence);
                SafeNumber[,] beta = this.ComputeBackwardLogLikelihoods(sequence);
                SafeNumber[,] gamma = this.ComputePosteriorLogLikelihoods(alpha, beta, sequence.Length);
                SafeNumber[,,] xi = this.ComputeTransitionLogLikelihoods(alpha, beta, sequence);

                for (int i = 0; i < this.numberOfStates; i++)
                {
                    initialNumerators[i].Add(gamma[i, 0]);
                }

                for (int i = 0; i < this.numberOfStates; i++)
                {
                    var time = Enumerable.Range(0, sequence.Length - 1);
                    SafeNumber d = ArithmeticMethods.LogSum(time.Select(t => gamma[i, t]));
                    transitionDenominators[i].Add(d);
                    for (int j = 0; j < this.numberOfStates; j++)
                    {
                        SafeNumber n = ArithmeticMethods.LogSum(time.Select(t => xi[i, j, t]));
                        transitionNumerators[i, j].Add(n);
                    }
                }

                this.AccumulateObservation(sequence, gamma);

                SafeNumber log_likelihood = ArithmeticMethods.LogSum(Enumerable.Range(0, this.numberOfStates).Select(i => alpha[i, sequence.Length - 1]));
                log_likelihoods = log_likelihoods + log_likelihood;
            }

            for (int i = 0; i < this.numberOfStates; i++)
            {
                this.initialLogLikelihoods[i] = ArithmeticMethods.LogSum(initialNumerators[i]) - new SafeNumber(Math.Log(sequences.Length));
            }

            for (int i = 0; i < this.numberOfStates; i++)
            {
                SafeNumber d = ArithmeticMethods.LogSum(transitionDenominators[i]);
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    SafeNumber n = ArithmeticMethods.LogSum(transitionNumerators[i, j]);
                    this.transitionLogLikelihoods[i, j] = n - d;
                }
            }

            this.TrainObservations();

            Console.WriteLine(log_likelihoods.Value.ToString("0.0000"));
        }

        public int[] BestStateSequence(T[] sequence)
        {
            SafeNumber[,] delta = new SafeNumber[this.numberOfStates, sequence.Length];
            int[,] best_state = new int[this.numberOfStates, sequence.Length];
            for (int i = 0; i < this.numberOfStates; i++)
            {
                delta[i, 0] = this.initialLogLikelihoods[i] + this.OutcomeLogLikelihood(i, sequence[0]);
            }

            for (int t = 1; t < sequence.Length; t++)
            {
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    SafeNumber max_so_far = delta[0, t - 1] + this.transitionLogLikelihoods[0, j];
                    int max_so_far_index = 0;
                    for (int i = 1; i < this.numberOfStates; i++)
                    {
                        SafeNumber temp = delta[i, t - 1] + this.transitionLogLikelihoods[i, j];
                        if (temp > max_so_far)
                        {
                            max_so_far = temp;
                            max_so_far_index = i;
                        }
                    }

                    delta[j, t] = max_so_far + this.OutcomeLogLikelihood(j, sequence[t]);
                    best_state[j, t] = max_so_far_index;
                }
            }

            int[] best_state_sequence = new int[sequence.Length];
            SafeNumber best_log_likelihood = delta[0, sequence.Length - 1];
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

        public virtual void Show()
        {
            for (int i = 0; i < this.numberOfStates; i++)
            {
                Console.WriteLine(Math.Exp(this.initialLogLikelihoods[i].Value).ToString("0.0000"));
            }

            for (int i = 0; i < this.numberOfStates; i++)
            {
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    Console.Write(Math.Exp(this.transitionLogLikelihoods[i, j].Value).ToString("0.0000"));
                    Console.Write(" ");
                }

                Console.WriteLine();
            }
        }

        protected int GetNumberOfStates()
        {
            return this.numberOfStates;
        }

        protected string Save<U>(U observationParameters) where U : IObservationParameters
        {
            HmmData<U> data = new HmmData<U>();
            data.InitialLogLikelihoods = this.initialLogLikelihoods.Select(t => t.Value).ToArray();
            data.TransitionLogLikelihoods = new double[this.numberOfStates, this.numberOfStates];
            for (int i = 0; i < this.numberOfStates; i++)
            {
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    data.TransitionLogLikelihoods[i, j] = this.transitionLogLikelihoods[i, j].Value;
                }
            }

            data.ObservationParameters = observationParameters;
            return JsonConvert.SerializeObject(data);
        }

        protected U Load<U>(string saved) where U : IObservationParameters
        {
            HmmData<U> data = JsonConvert.DeserializeObject<HmmData<U>>(saved);
            this.initialLogLikelihoods = data.InitialLogLikelihoods.Select(t => new SafeNumber(t)).ToArray();
            this.numberOfStates = this.initialLogLikelihoods.Length;
            this.transitionLogLikelihoods = new SafeNumber[this.numberOfStates, this.numberOfStates];
            for (int i = 0; i < this.numberOfStates; i++)
            {
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    this.transitionLogLikelihoods[i, j] = new SafeNumber(data.TransitionLogLikelihoods[i, j]);
                }
            }

            return data.ObservationParameters;
        }

        protected abstract SafeNumber OutcomeLogLikelihood(int state, T observation);

        protected abstract void AccumulateObservation(T[] sequence, SafeNumber[,] gamma);

        protected abstract void TrainObservations();

        private SafeNumber[,] ComputeForwardLogLikelihoods(T[] sequence)
        {
            SafeNumber[,] alpha = new SafeNumber[this.numberOfStates, sequence.Length];

            // Initialization
            for (int s = 0; s < this.numberOfStates; s++)
            {
                alpha[s, 0] = this.initialLogLikelihoods[s] + this.OutcomeLogLikelihood(s, sequence[0]);
            }

            // Induction
            for (int t = 0; t < sequence.Length - 1; t++)
            {
                for (int j = 0; j < this.numberOfStates; j++)
                {
                    SafeNumber temp = ArithmeticMethods.LogSum(Enumerable.Range(0, this.numberOfStates).Select(i => alpha[i, t] + this.transitionLogLikelihoods[i, j]));
                    alpha[j, t + 1] = temp + this.OutcomeLogLikelihood(j, sequence[t + 1]);
                }
            }

            return alpha;
        }

        private SafeNumber[,] ComputeBackwardLogLikelihoods(T[] sequence)
        {
            SafeNumber[,] beta = new SafeNumber[this.numberOfStates, sequence.Length];

            // Initialization
            for (int s = 0; s < this.numberOfStates; s++)
            {
                beta[s, sequence.Length - 1] = new SafeNumber(0);
            }

            // Induction
            for (int t = sequence.Length - 2; t >= 0; t--)
            {
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    beta[i, t] = ArithmeticMethods.LogSum(Enumerable.Range(0, this.numberOfStates).Select(j => this.transitionLogLikelihoods[i, j] + this.OutcomeLogLikelihood(j, sequence[t + 1]) + beta[j, t + 1]));
                }
            }

            return beta;
        }

        private SafeNumber[,] ComputePosteriorLogLikelihoods(SafeNumber[,] alpha, SafeNumber[,] beta, int sequenceLength)
        {
            SafeNumber[,] gamma = new SafeNumber[this.numberOfStates, sequenceLength];
            for (int t = 0; t < sequenceLength; t++)
            {
                SafeNumber temp = ArithmeticMethods.LogSum(Enumerable.Range(0, this.numberOfStates).Select(i => alpha[i, t] + beta[i, t]));
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    gamma[i, t] = alpha[i, t] + beta[i, t] - temp;
                }
            }

            return gamma;
        }

        private SafeNumber[,,] ComputeTransitionLogLikelihoods(SafeNumber[,] alpha, SafeNumber[,] beta, T[] sequence)
        {
            SafeNumber[,,] xi = new SafeNumber[this.numberOfStates, this.numberOfStates, sequence.Length - 1];

            // This is really just a convoluted way to get to the set of all state pairs as Tuple<int, int>
            var statePairs = Enumerable.Range(0, this.numberOfStates).SelectMany(i => Enumerable.Range(0, this.numberOfStates).Select(j => Tuple.Create(i, j)));

            for (int t = 0; t < sequence.Length - 1; t++)
            {
                SafeNumber temp = ArithmeticMethods.LogSum(statePairs.Select(p => alpha[p.Item1, t] + this.transitionLogLikelihoods[p.Item1, p.Item2] + this.OutcomeLogLikelihood(p.Item2, sequence[t + 1]) + beta[p.Item2, t + 1]));
                for (int i = 0; i < this.numberOfStates; i++)
                {
                    for (int j = 0; j < this.numberOfStates; j++)
                    {
                        xi[i, j, t] = alpha[i, t] + this.transitionLogLikelihoods[i, j] + this.OutcomeLogLikelihood(j, sequence[t + 1]) + beta[j, t + 1] - temp;
                    }
                }
            }

            return xi;
        }

        private class HmmData<U> where U : IObservationParameters
        {
            public double[] InitialLogLikelihoods { get; set; }

            public U ObservationParameters { get; set; }

            public double[,] TransitionLogLikelihoods { get; set; }
        }
    }
}