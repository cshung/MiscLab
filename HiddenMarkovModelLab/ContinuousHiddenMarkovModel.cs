namespace HiddenMarkovModelLab
{
    using System;
    using System.Collections.Generic;
    using System.Linq;

    public class ContinuousHiddenMarkovModel : HiddenMarkovModel<double>
    {
        private SafeNumber[] means;
        private SafeNumber[] variances;

        private SafeNumber gaussianConstant = new SafeNumber(-Math.Log(2 * Math.PI) / 2);
        private SafeNumber two = new SafeNumber(2);

        private List<SafeNumber>[] mean_numerators;
        private List<SafeNumber>[] variances_numerators;
        private List<SafeNumber>[] denominators;

        public ContinuousHiddenMarkovModel(int numberOfStates) : base(numberOfStates)
        {
            this.means = new SafeNumber[numberOfStates];
            this.variances = new SafeNumber[numberOfStates];
        }

        public override void Bootstrap(Random r)
        {
            base.Bootstrap(r);
            for (int s = 0; s < this.GetNumberOfStates(); s++)
            {
                this.means[s] = new SafeNumber(r.NextDouble());
                this.variances[s] = new SafeNumber(r.NextDouble());
            }
        }

        public override void Show()
        {
            base.Show();
            for (int i = 0; i < this.GetNumberOfStates(); i++)
            {
                Console.WriteLine(this.means[i].Value.ToString("0.0000") + ", " + this.variances[i].Value.ToString("0.0000"));
            }

            Console.WriteLine();
        }

        protected override SafeNumber OutcomeLogLikelihood(int state, double observation)
        {
            SafeNumber safeObservation = new SafeNumber(observation);            
            return this.gaussianConstant - ArithmeticMethods.Log(this.variances[state]) - (((safeObservation - this.means[state]) * (safeObservation - this.means[state]) / this.two) / this.variances[state]);
        }

        protected override void AccumulateObservation(double[] sequence, SafeNumber[,] gamma)
        {
            this.EnsureAccumulators();
            for (int j = 0; j < this.GetNumberOfStates(); j++)
            {
                var time = Enumerable.Range(0, sequence.Length);
                SafeNumber d = ArithmeticMethods.LogSum(time.Select(t => gamma[j, t]));
                SafeNumber nm = ArithmeticMethods.LogSum(time.Select(t => ArithmeticMethods.Log(sequence[t]) + gamma[j, t]));
                SafeNumber nv = ArithmeticMethods.LogSum(time.Select(t => (this.two * ArithmeticMethods.Log(ArithmeticMethods.Abs(new SafeNumber(sequence[t]) - this.means[j]))) + gamma[j, t]));
                this.mean_numerators[j].Add(nm);
                this.variances_numerators[j].Add(nv);
                this.denominators[j].Add(d);
            }
        }

        protected override void TrainObservations()
        {
            for (int i = 0; i < this.GetNumberOfStates(); i++)
            {
                this.means[i] = ArithmeticMethods.Exp(ArithmeticMethods.LogSum(mean_numerators[i]) - ArithmeticMethods.LogSum(denominators[i]));
                this.variances[i] = ArithmeticMethods.Exp(ArithmeticMethods.LogSum(variances_numerators[i]) - ArithmeticMethods.LogSum(denominators[i]));

                // Make sure the variance is not too small 
                this.variances[i] = ArithmeticMethods.Floor(this.variances[i], 0.01);
            }

            this.mean_numerators = null;
            this.variances_numerators = null;
            this.denominators = null;
        }

        private void EnsureAccumulators()
        {
            if (this.mean_numerators == null)
            {
                this.mean_numerators = new List<SafeNumber>[this.GetNumberOfStates()];
                this.variances_numerators = new List<SafeNumber>[this.GetNumberOfStates()];
                this.denominators = new List<SafeNumber>[this.GetNumberOfStates()];
                for (int i = 0; i < this.GetNumberOfStates(); i++)
                {
                    this.mean_numerators[i] = new List<SafeNumber>();
                    this.variances_numerators[i] = new List<SafeNumber>();
                    this.denominators[i] = new List<SafeNumber>();
                }
            }
        }
    }
}
