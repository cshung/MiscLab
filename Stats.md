# Notes:
If the population is normal, the sample mean is exactly normal.
- The mean of the sample mean is the $$ \mu $$
- The variance of the sample mean is the $$ \frac{\sigma^2}{n} $$

# Central limit theorem:
The sum of a large number of independent variables is approximately normal.

# Confidence interval of population mean
Assume population variance is known:
$$ \frac{\bar{X} - \mu}{\sqrt{\frac{\sigma^2}{n}}} $$ is standard normal.

So we can find $$ L $$ such that $$ P(-L \le \frac{\bar{X} - \mu}{\sqrt{\frac{\sigma^2}{n}}} \le L) =0.95 $$.

Flip inside also okay, since if $$ X $$ is standard normal, so is $$ -X $$

$$ P(-L \le \frac{\mu - \bar{x}}{\sqrt{\frac{\sigma^2}{n}}} \le L) =0.95 $$

$$ P(\bar{x} - \sqrt{\frac{\sigma^2}{n}} L \le \mu \le \bar{x} + \sqrt{\frac{\sigma^2}{n}} L) =0.95 $$

# Variance

- The sample variance of a sample coming from a **normal distribution** is a scaled chi-square variable.

We **beg** the answer - is there a central limit theorem like result for the sample variance?

$$ \frac{(n-1)S^2}{\sigma^2} $$ is a chi-square variable of $$ n - 1 $$ degree of freedom.

$$ \frac{a}{b} >c \implies \frac{1}{c} < \frac{b}{a} $$, now we can invert the formulas to get a confidence interval for variance.

# Unknown variance problem

$$ \frac{N}{\sqrt{\frac{\chi}{d}}} $$ is T distributed with $$ d $$ degree of freedom when $$ N $$ is standard normal, $$ \chi $$ is chi-square with $$ d $$ degree of freedom.

https://shoichimidorikawa.github.io/Lec/ProbDistr/t-e.pdf

Using the results above, we get

$$ \frac{\frac{\bar{X} - \mu}{\sqrt{\frac{\sigma^2}{n}}}}{\sqrt{\frac{\frac{(n-1)S^2}{\sigma^2}}{n-1}}} = \frac{\bar{X} - \mu}{\frac{S}{\sqrt{n}} $$  

The above assumed the population is normal in order to assume the chi-square.

# Proportion

The binomial distribution has a mean of $$ np $$ and a variance of $$ np(1-p) $$.

For large $$ n $$, the distribution is approximately normal.

To compare the sample proportion of **two different populations**, we can use the fact that the difference between the two normal variables is normal.

This **begs** the question, what about the paired-test?

## Difference

Comparing means is often bugged by the problem of variance - we have two key cases, the variance is equal or the variance is not equal.

We can test the variance equality by the F test. 

The distribution of two chi-square variables is an F distribution. 

When the population variances are equal, we can use a single $$ S $$ from all data to estimate the single unknown population distribution (i.e. pooling). In that case, the test statistic has a known T distribution.

Otherwise we use two S to estimate the two variances, but then we have a weird degree of freedom. 

https://stats.stackexchange.com/questions/10856/what-is-the-distribution-of-the-difference-of-two-t-distributions
https://en.wikipedia.org/wiki/Behrens%E2%80%93Fisher_distribution




