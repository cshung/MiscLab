# Reed Solomon Code

## General block code
- Let $$ k $$ be a finite field.
- Block code maps $$ k^m $$ to $$ k^n $$ with $$ n > m $$.

## Linear Code
- Linear block code means the range of the mapping is a $$ k $$ dimension vector subspace. Therefore the mapping can be described as a linear transformation, $$ y = wG $$.
- The minimal Hamming distance between any two codewords in a linear code is the same as the minimum Hamming weight of a codeword because $$ w_1 \in C, w_2 \in C \implies w_1 - w_2 \in C $$
- By Gaussian elimination, we can reduce G to the reduced echelon form, so that $$ G = [I|B] $$. Note that it is no longer the same encoding function, but the set of codewords is the same.
- $$ H = [-B^T|I] $$ is called the parity check matrix, the key property is that $$ GH^T = -IB + BI = 0 $$. 
- $$ H $$ generates the dual code, every codeword of $$ G $$ is orthogonal to a codeword of $$ H $$ because $$ w_1G(w_2H)^T  = w_1GH^Tw_2 = 0 $$.

## Cyclic code
- A linear code that contains all cyclic permutation of a codeword is called a cyclic code.
- A cyclic code is an ideal of a polynomial $$ g(x) $$ that is a factor of $$ x^n - 1 $$.
  - An ideal contains all linear combinations and cyclic shifts, therefore, an ideal is a code.
  - A code is an ideal because it is linear. Because polynomials over a finite field is a principal ideal domain, any ideal is principal. Let the ideal be $$ (g(x)) $$. Because "cyclic shift", therefore $$ g(x) $$ is a factor of $$ x^n - 1 $$. 

TODO - flush out the details for the "cyclic shift" argument.

- Because codewords are basically multiples of $$ g(x) $$, therefore the encoding function works simply by multiplying polynomials. The matrix multiplication given in the book can be interpreted as so.

## Primitive roots
- Let $$ q $$ be the number of elements in the finite field, therefore $$ q = p^x $$ for some $$ x $$.
- If $$ gcd(q, n) = 1 $$, then we have a primitive nth root of 1, and therefore $$ g $$ has no repeated root.

## BCH
- If $$ g(x) $$ has $$ l + 1 $$ consecutive primitive roots as its roots, then any code word has at least $$ l + 2 $$ non zero. This can be proved by the Vandermonde determinant on page 418 of Abstract Algebra.

## Reed Solomon is a special BCH
- Set $$ g(x) = (x-p)(x-p^2)...(x-p^{2t}) $$ for a primitive root $$ p $$ and $$ 2t < q $$. That has the consecutive primitive roots and thus a BCH.
- The code has at least $$ 2t $$ primitive roots, so it has at least $$ 2t + 1 $$ non zeros, and thus it can correct up to $$ t $$ errors.

## Decoding

### Syndrome
- A codeword is a multiple of $$ g $$, therefore, a codeword (as a polynomial) evaluates to 0 at the roots of $$ g $$.
- If we apply all the primitive roots to the received polynomial $$ y = c + e $$, we yield the syndrome $$ Y $$. 
- At the roots of $$ g $$, $$ Y $$ should be zero, but it might not be due to errors.

### Transform
- Applying a single root to the received message and be interpreted as matrix multiplication.
- Applying all the primitive roots to a polynomial can be interpreted as a Fourier transform using a primitive root as a root of unity.

Let $$ c $$ and $$ C $$ be a Fourier transform pair, both of them can be interpreted both as a vector and a polynomial.
- If $$ c(\alpha^k) = 0 $$, then $$ C_k = 0 $$.
- If $$ C(\alpha^{-k}) = 0 $$, then $$ c_k = 0 $$.

### Solving
Consider the error locator polynomial $$ \Lambda(x) = (\alpha^{e_1}x - 1)...(\alpha^{e_v}x - 1) $$ such that $$ \alpha^{-e_k} $$ is a root if any only if $$ e_k $$ is an error location. 

Interpret $$ \Lambda $$ as the Fourier transform of $$ \lambda $$. Using property 2, we know $$ \lambda_{e_k} $$ must be 0. Therefore $$ \lambda_k e_k = 0 $$ for all $$ k $$. 

Back in the frequency domain, the circular convolution of $$ E $$ and $$ \Lambda $$ will be 0. This is called the key equation and we will solve it for both $$ \Lambda $$ and $$ E $$.

We do not know $$ \Lambda $$, but we know it has at most $$ t $$ non-zero and the constant term has to be 1.

We do not know $$ E $$, but we know the first $$ 2t $$ terms, they are simply $$ Y $$. The constant term is unknown.

Let's give an example with $$ t = 3 $$.

    1 u u u 0 0 0 0 0 0 0 0 0
    ? k k k k k k ? ? ? ? ? ?

The first row corresponds to the unknown error locator polynomial coefficients in the frequency domain. The second row corresponds to the Fourier transform of the error. If we slide the error locator polynomial, we can see that there are exactly 3 spots where the error locator polynomial overlaps on only the known portion of the error. Note that the circular convolution is 0, which yields 3 linear equations and therefore we can solve for the unknown coefficients in a unique way.

### Advanced solving
Solving a linear equation system is complicated. We can simplify it. By considering the convolution as polynomial multiplication. In particular, we have $$ \Lambda \times E = \Omega $$. We only know $$ E $$ up to $$ 2t $$ terms, so we simply $$ \pmod{x^{2t}} $$. We do not really care what is $$ \Omega $$. The extended Euclidean algorithm applied on $$ Y $$ and $$ x^{2t} $$ will give us $$ \Lambda $$. 

TODO - flush out the details.

### Decoding
Once we know all the values of $$ u $$, we can slide the error locator polynomial one at a time to determine all the unknown question marks in the second row one at a time. Once we know all the errors in the frequency domain, inverse Fourier transform and polynomial division will give us the decoding.

