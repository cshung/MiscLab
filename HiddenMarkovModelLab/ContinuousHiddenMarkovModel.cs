namespace HiddenMarkovModelLab
{
    using System;
    using System.Linq;

    public class ContinuousHiddenMarkovModel : HiddenMarkovModel<double>
    {
        private SafeNumber[] means;
        private SafeNumber[] variances;

        private SafeNumber gaussianConstant = new SafeNumber(-Math.Log(2 * Math.PI) / 2);
        private SafeNumber two = new SafeNumber(2);

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

        protected override void TrainObservations(double[] sequence, SafeNumber[,] gamma)
        {
            for (int j = 0; j < this.GetNumberOfStates(); j++)
            {
                var time = Enumerable.Range(0, sequence.Length);
                SafeNumber d = ArithmeticMethods.LogSum(time.Select(t => gamma[j, t]));
                SafeNumber nm = ArithmeticMethods.LogSum(time.Select(t => ArithmeticMethods.Log(sequence[t]) + gamma[j, t]));
                SafeNumber nv = ArithmeticMethods.LogSum(time.Select(t => (this.two * ArithmeticMethods.Log(ArithmeticMethods.Abs(new SafeNumber(sequence[t]) - this.means[j]))) + gamma[j, t]));
                this.means[j] = ArithmeticMethods.Exp(nm - d);
                this.variances[j] = ArithmeticMethods.Exp(nv - d);

                // Make sure the variance is not too small 
                this.variances[j] = ArithmeticMethods.Floor(this.variances[j], 0.01);
            }
        }
    }
}
