namespace HiddenMarkovModelLab
{
    using System;
    using System.Linq;

    public class DiscreteHiddenMarkovModel : HiddenMarkovModel<int>
    {
        private int numberOfOutcomes;
        private SafeNumber[,] outcomeLogLikelihoods;

        public DiscreteHiddenMarkovModel(int numberOfStates, int numberOfOutcomes) : base(numberOfStates)
        {
            this.numberOfOutcomes = numberOfOutcomes;
            this.outcomeLogLikelihoods = new SafeNumber[numberOfStates, numberOfOutcomes];
        }

        public override void Bootstrap(Random r)
        {
            base.Bootstrap(r);
            for (int s = 0; s < this.GetNumberOfStates(); s++)
            {
                // For outcome probabilities, we have to break the symmetry - without that all states look identical and it will stay that way
                var randomValues = Enumerable.Range(0, this.numberOfOutcomes).Select(_ => r.NextDouble());
                var sum = randomValues.Sum();
                var randomLogLikelihoods = randomValues.Select(v => v / sum).ToArray();

                for (int c = 0; c < this.numberOfOutcomes; c++)
                {
                    this.outcomeLogLikelihoods[s, c] = ArithmeticMethods.Log(randomLogLikelihoods[c]);
                }
            }
        }

        public override void Show()
        {
            base.Show();
            for (int i = 0; i < this.GetNumberOfStates(); i++)
            {
                for (int c = 0; c < this.numberOfOutcomes; c++)
                {
                    Console.Write(Math.Exp(this.outcomeLogLikelihoods[i, c].Value).ToString("0.0000"));
                    Console.Write(" ");
                }

                Console.WriteLine();
            }
        }

        protected override SafeNumber OutcomeLogLikelihood(int state, int observation)
        {
            return this.outcomeLogLikelihoods[state, observation];
        }

        protected override void TrainObservations(int[] sequence, SafeNumber[,] gamma)
        {
            for (int j = 0; j < this.GetNumberOfStates(); j++)
            {
                var time = Enumerable.Range(0, sequence.Length);
                SafeNumber d = ArithmeticMethods.LogSum(time.Select(t => gamma[j, t]));
                for (int c = 0; c < this.numberOfOutcomes; c++)
                {
                    SafeNumber n = ArithmeticMethods.LogSum(time.Where(t => sequence[t] == c).Select(t => gamma[j, t]));
                    this.outcomeLogLikelihoods[j, c] = n - d;
                }
            }
        }
    }
}
