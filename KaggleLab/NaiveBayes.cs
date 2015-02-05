namespace KaggleLab
{
    using System;
    using System.Collections.Generic;

    interface IDataSet
    {
        int GetNumClasses();
        int GetNumAttributes();
        int GetNumDataVectors();
        IDataVector GetDataVector(int i);
        List<string> GetAttributeDomain(int attribute);
    }

    interface IDataVector
    {
        string GetAttributeValue(int i);
        int GetClass();
    }

    class Model
    {
        public double[] prior;
        public Dictionary<string, double>[,] likeihood;
    }

    internal static class NaiveBayes
    {
        internal static double Test(Model model, IDataSet testDataSet)
        {
            int successCount = 0;
            double[] posteriors = new double[testDataSet.GetNumClasses()];
            for (int i = 0; i < testDataSet.GetNumDataVectors(); i++)
            {
                IDataVector dataVector = testDataSet.GetDataVector(i);
                for (int c = 0; c < testDataSet.GetNumClasses(); c++)
                {
                    posteriors[c] = model.prior[c];
                    for (int j = 0; j < testDataSet.GetNumAttributes(); j++)
                    {
                        posteriors[c] *= model.likeihood[j, c][dataVector.GetAttributeValue(j)];
                    }
                }

                int maxPosteriorClass = -1;
                double maxPosterior = -1;
                for (int c = 0; c < testDataSet.GetNumClasses(); c++)
                {
                    if (posteriors[c] > maxPosterior)
                    {
                        maxPosterior = posteriors[c];
                        maxPosteriorClass = c;
                    }
                }
                if (maxPosteriorClass == dataVector.GetClass())
                {
                    successCount++;
                }
            }
            return ((double)successCount) / testDataSet.GetNumDataVectors();
        }

        internal static Model Train(IDataSet dataSet)
        {
            // Step 1: Estimate all the prior probabilities
            double[] prior = new double[dataSet.GetNumClasses()];
            for (int i = 0; i < dataSet.GetNumClasses(); i++)
            {
                prior[i] = 0;
            }
            for (int i = 0; i < dataSet.GetNumDataVectors(); i++)
            {
                IDataVector dataVector = dataSet.GetDataVector(i);
                prior[dataVector.GetClass()]++;
            }

            // Step 2: Estimate all the likelihood values
            // Probability(attribute_i = domain_i | class = x);

            Dictionary<string, double>[,] likeihoods = new Dictionary<string, double>[dataSet.GetNumAttributes(), dataSet.GetNumClasses()];
            for (int c = 0; c < dataSet.GetNumClasses(); c++)
            {
                for (int j = 0; j < dataSet.GetNumAttributes(); j++)
                {
                    likeihoods[j, c] = new Dictionary<string, double>();
                    foreach (var attributeValue in dataSet.GetAttributeDomain(j))
                    {
                        likeihoods[j, c].Add(attributeValue, 0);
                    }
                }
            }
            for (int i = 0; i < dataSet.GetNumDataVectors(); i++)
            {
                IDataVector dataVector = dataSet.GetDataVector(i);
                for (int j = 0; j < dataSet.GetNumAttributes(); j++)
                {
                    likeihoods[j, dataVector.GetClass()][dataVector.GetAttributeValue(j)]++;
                }
            }

            // Step 3: Finalize probability computation by normalizing appropriately   
            //         This is count driven maximum likelihood estimates - so it is possible for unobserved scenario to have 0 probability.
            for (int c = 0; c < dataSet.GetNumClasses(); c++)
            {
                for (int j = 0; j < dataSet.GetNumAttributes(); j++)
                {
                    foreach (var attributeValue in dataSet.GetAttributeDomain(j))
                    {
                        likeihoods[j, c][attributeValue] /= prior[c];
                    }
                }
            }
            for (int i = 0; i < dataSet.GetNumClasses(); i++)
            {
                prior[i] /= dataSet.GetNumDataVectors();
            }

            return new Model { prior = prior, likeihood = likeihoods };
        }
    }
}
