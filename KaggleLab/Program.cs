namespace KaggleLab
{
    using LINQtoCSV;
    using System;
    using System.Collections.Generic;
    using System.Linq;

    class Person
    {
        [CsvColumn(FieldIndex = 1, CanBeNull = false)]
        public int PassengerId { get; set; }

        [CsvColumn(FieldIndex = 2, CanBeNull = false)]
        public int Survived { get; set; }

        [CsvColumn(FieldIndex = 3, CanBeNull = false)]
        public int Pclass { get; set; }

        [CsvColumn(FieldIndex = 4, CanBeNull = false)]
        public string Name { get; set; }

        [CsvColumn(FieldIndex = 5, CanBeNull = false)]
        public string Sex { get; set; }

        [CsvColumn(FieldIndex = 6, CanBeNull = true)]
        public double? Age { get; set; }

        [CsvColumn(FieldIndex = 7, CanBeNull = false)]
        public int SibSp { get; set; }

        [CsvColumn(FieldIndex = 8, CanBeNull = false)]
        public int Parch { get; set; }

        [CsvColumn(FieldIndex = 9, CanBeNull = false)]
        public string Ticket { get; set; }

        [CsvColumn(FieldIndex = 10, CanBeNull = false)]
        public double Fare { get; set; }

        [CsvColumn(FieldIndex = 11, CanBeNull = true)]
        public string Cabin { get; set; }

        [CsvColumn(FieldIndex = 12, CanBeNull = true)]
        public string Embarked { get; set; }
    }

    class PersonDataSet : IDataSet
    {
        Person[] persons;

        public PersonDataSet(Person[] persons)
        {
            this.persons = persons;
        }

        public int GetNumClasses()
        {
            return 2;
        }

        public int GetNumAttributes()
        {
            return 3;
        }

        public int GetNumDataVectors()
        {
            return this.persons.Length;
        }

        public IDataVector GetDataVector(int i)
        {
            return new PersonDataVector(persons[i]);
        }

        public List<string> GetAttributeDomain(int attribute)
        {
            if (attribute == 0)
            {
                return new List<string> { "male", "female" };
            }
            else if (attribute == 1)
            {
                return new List<string> { "1", "2", "3" };
            }
            else if (attribute == 2)
            {
                return new List<string> { "0", "1", "2", "3", "4", "5", "6", "7", "8", "Null" };
            }
            else
            {
                throw new Exception("No such attribute");
            }
        }
    }

    class PersonDataVector : IDataVector
    {
        private Person person;

        public PersonDataVector(Person person)
        {
            this.person = person;
        }

        public string GetAttributeValue(int i)
        {
            if (i == 0)
            {
                return person.Sex;
            }
            else if (i == 1)
            {
                return person.Pclass.ToString();
            }
            else if (i == 2)
            {
                if (person.Age.HasValue)
                {
                    return ((int)(person.Age / 10)).ToString();
                }
                else
                {
                    return "Null";
                }
            }
            else
            {
                throw new Exception("No such attribute");
            }
        }

        public int GetClass()
        {
            return person.Survived;
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            var persons = new CsvContext().Read<Person>(@"C:\Users\andrewau\Desktop\DesktopFiles\train.csv", new CsvFileDescription { SeparatorChar = ',', FirstLineHasColumnNames = true }).ToArray();

            // Exploratory data analysis
            // We already knew this

            //double survivedFemaleCount = persons.Where(p => p.Sex.Equals("female") && p.Survived == 1).Count();
            //double femaleCount = persons.Where(p => p.Sex.Equals("female")).Count();

            //double survivedMaleCount = persons.Where(p => p.Sex.Equals("male") && p.Survived == 1).Count();
            //double maleCount = persons.Where(p => p.Sex.Equals("male")).Count();

            //Console.WriteLine(survivedFemaleCount / femaleCount);
            //Console.WriteLine(survivedMaleCount / maleCount);

            // Let's look at the data from a pclass perspective
            // Upper class  has 62% survival rate
            // Middle class has 47% 
            // Lower class  has 24% 
            // Console.WriteLine(ComputeConditionalProbability(persons, p => p.Pclass == 1));

            // Let's look at the data from a sex perspective
            // Female       has 75% 
            // Male         has 19% 
            // Console.WriteLine(ComputeConditionalProbability(persons, p => p.Sex.Equals("male")));

            // Let's look at the data from an age perspective
            // Average survived age = 24
            // Average not dead age = 23
            // Nothing real special here
            // But if we bucket the ages
            // Age 0 - 9   has 61% survival rate 
            // Age 10 - 19 has 40%
            // Age 20 - 29 has 35%
            // Age 30 - 39 has 43%
            // Age 40 - 49 has 38%
            // Age 50 - 59 has 41%
            // Age 60 - 69 has 31%
            // Age 70 - 79 has 0%
            // Console.WriteLine(ComputeConditionalProbability(persons, p => p.Age != null && ((int)(p.Age / 10)) == 5));

            // Look like these are good features, let's try to do a simple Naive Bayes classifier on it?
            Person[] train = persons.Take(800).ToArray();
            Person[] test = persons.Skip(800).ToArray();
            Model model = NaiveBayes.Train(new PersonDataSet(train));
            Console.WriteLine(NaiveBayes.Test(model, new PersonDataSet(test)));
        }

        private static double ComputeConditionalProbability(Person[] persons, Predicate<Person> isPersonSpecial)
        {
            double survivedSpecialCount = persons.Where(p => isPersonSpecial(p) && p.Survived == 1).Count();
            double specialCount = persons.Where(p => isPersonSpecial(p)).Count();
            double conditionalProbability = survivedSpecialCount / specialCount;
            return conditionalProbability;
        }
    }
}
