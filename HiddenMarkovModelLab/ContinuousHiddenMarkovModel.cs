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

        private List<SafeNumber>[] positiveMeanNumerators;
        private List<SafeNumber>[] negativeMeanNumerators;
        private List<SafeNumber>[] varianceNumerators;
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

        public string Save()
        {
            ContinuousObservationParameters data = new ContinuousObservationParameters();
            data.Means = this.means.Select(t => t.Value).ToArray();
            data.Variances = this.variances.Select(t => t.Value).ToArray();
            return base.Save(data);
        }

        public void Load(string saved)
        {
            ContinuousObservationParameters observationParameters = base.Load<ContinuousObservationParameters>(saved);
            this.means = observationParameters.Means.Select(t => new SafeNumber(t)).ToArray();
            this.variances = observationParameters.Variances.Select(t => new SafeNumber(t)).ToArray();
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
            return this.gaussianConstant - (ArithmeticMethods.Log(this.variances[state]) / this.two) - (((safeObservation - this.means[state]) * (safeObservation - this.means[state]) / this.two) / this.variances[state]);
        }

        protected override void AccumulateObservation(double[] sequence, SafeNumber[,] gamma)
        {
            this.EnsureAccumulators();
            for (int j = 0; j < this.GetNumberOfStates(); j++)
            {
                var time = Enumerable.Range(0, sequence.Length);
                SafeNumber d = ArithmeticMethods.LogSum(time.Select(t => gamma[j, t]));

                if (time.Count(t => sequence[t] >= 0) > 0)
                {
                    SafeNumber nmp = ArithmeticMethods.LogSum(time.Where(t => sequence[t] >= 0).Select(t => ArithmeticMethods.Log(sequence[t]) + gamma[j, t]));
                    this.positiveMeanNumerators[j].Add(nmp);
                }

                if (time.Count(t => sequence[t] < 0) > 0)
                {
                    SafeNumber nmn = ArithmeticMethods.LogSum(time.Where(t => sequence[t] < 0).Select(t => ArithmeticMethods.Log(-sequence[t]) + gamma[j, t]));
                    this.negativeMeanNumerators[j].Add(nmn);
                }

                SafeNumber nv = ArithmeticMethods.LogSum(time.Select(t => (this.two * ArithmeticMethods.Log(ArithmeticMethods.Abs(new SafeNumber(sequence[t]) - this.means[j]))) + gamma[j, t]));
                this.varianceNumerators[j].Add(nv);
                this.denominators[j].Add(d);
            }
        }

        protected override void TrainObservations()
        {
            for (int i = 0; i < this.GetNumberOfStates(); i++)
            {
                if (this.positiveMeanNumerators[i].Count == 0)
                {
                    SafeNumber y = ArithmeticMethods.LogSum(this.negativeMeanNumerators[i]);
                    this.means[i] = new SafeNumber(0) - ArithmeticMethods.Exp(y + ArithmeticMethods.LogSum(this.denominators[i]));
                }
                else if (this.negativeMeanNumerators[i].Count == 0)
                {
                    SafeNumber x = ArithmeticMethods.LogSum(this.positiveMeanNumerators[i]);
                    this.means[i] = ArithmeticMethods.Exp(x - ArithmeticMethods.LogSum(this.denominators[i]));
                }
                else
                {
                    SafeNumber x = ArithmeticMethods.LogSum(this.positiveMeanNumerators[i]);
                    SafeNumber y = ArithmeticMethods.LogSum(this.negativeMeanNumerators[i]);
                    if (x.Value > y.Value)
                    {
                        SafeNumber z = ArithmeticMethods.Log(ArithmeticMethods.Exp(x) - ArithmeticMethods.Exp(y));
                        this.means[i] = ArithmeticMethods.Exp(z - ArithmeticMethods.LogSum(this.denominators[i]));
                    }
                    else
                    {
                        SafeNumber z = ArithmeticMethods.Log(ArithmeticMethods.Exp(y) - ArithmeticMethods.Exp(x));
                        this.means[i] = new SafeNumber(0) - ArithmeticMethods.Exp(z + ArithmeticMethods.LogSum(this.denominators[i]));
                    }
                }

                this.variances[i] = ArithmeticMethods.Exp(ArithmeticMethods.LogSum(this.varianceNumerators[i]) - ArithmeticMethods.LogSum(this.denominators[i]));

                // Make sure the variance is not too small 
                this.variances[i] = ArithmeticMethods.Floor(this.variances[i], 0.01);
            }

            this.positiveMeanNumerators = null;
            this.negativeMeanNumerators = null;
            this.varianceNumerators = null;
            this.denominators = null;
        }

        private void EnsureAccumulators()
        {
            if (this.positiveMeanNumerators == null)
            {
                this.positiveMeanNumerators = new List<SafeNumber>[this.GetNumberOfStates()];
                this.negativeMeanNumerators = new List<SafeNumber>[this.GetNumberOfStates()];
                this.varianceNumerators = new List<SafeNumber>[this.GetNumberOfStates()];
                this.denominators = new List<SafeNumber>[this.GetNumberOfStates()];
                for (int i = 0; i < this.GetNumberOfStates(); i++)
                {
                    this.positiveMeanNumerators[i] = new List<SafeNumber>();
                    this.negativeMeanNumerators[i] = new List<SafeNumber>();
                    this.varianceNumerators[i] = new List<SafeNumber>();
                    this.denominators[i] = new List<SafeNumber>();
                }
            }
        }

        private class ContinuousObservationParameters : IObservationParameters
        {
            public double[] Means { get; set; }

            public double[] Variances { get; set; }
        }
    }
}
