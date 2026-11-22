# WEBcaratheodory exercises

Here are the solutions to all problems in http://www.probability.net/WEBcaratheodory.pdf

## Exercise 1

$$
\begin{eqnarray*}
& & A \backslash (A \backslash B) \\
&=& A \backslash (A \cap B^c) \\
&=& A \cap (A \cap B^c)^c \\
&=& A \cap (A^c \cup B) \\
&=& (A \cap A^c) \cup (A \cap B) \\
&=& A \cap B
\end{eqnarray*}
$$

If $A \in \mathcal{R}$ and $B \in \mathcal{R}$, then $A \backslash B \in \mathcal{R}$ and $A \backslash (A \backslash B) \in \mathcal{R}$, therefore a ring is closed under pairwise intersection.

## Exercise 2
If $\mathcal{R}$ is a ring, then:
1. $\phi \in \mathcal{R}$, because it is a ring.
2. $A,B \in \mathcal{R} \implies A \cap B \in \mathcal{R}$, by exercise 1.
3. $A, B \in \mathcal{R} \implies A \backslash B \in \mathcal{R}$ because it is a ring. So $A \backslash B$ can be thought to be a pairwise disjoint collection of just 1 set.

Therefore we proved that $\mathcal{R}$ is a semi-ring.

## Exercise 3
The verification of $\mathcal{S}\_1$ and $\mathcal{S}\_2$ are semi-ring is routine and skipped. The key to this problem is that $\mathcal{S}\_1 \cap \mathcal{S}\_2 = \lbrace \phi. A\_1, \Omega\rbrace$ and therefore $\Omega \backslash A\_1 = A\_2 \uplus A\_3$ cannot be represented as a disjoint union of the elements of $\mathcal{S}\_1 \cap \mathcal{S}\_2$ .

## Exercise 4
1. $\phi \in \mathcal{R}_i$ for all $i \in I$,  therefore $\phi \in \mathcal{R}$.
2. If $A,B \in \mathcal{R}$, then $A, B \in \mathcal{R}\_i$ for all $i \in I$. So $A \cup B \in \mathcal{R}\_i$ for all $i \in I$, therefore $A \cup B \in \mathcal{R}$.
3. If $A,B \in \mathcal{R}$, then $A, B \in \mathcal{R}\_i$ for all $i \in I$. So $A \backslash B \in \mathcal{R}\_i$ for all $i \in I$, therefore $A \backslash B \in \mathcal{R}$.

So $\mathcal{R}$ is a ring.

## Exercise 5
$\mathcal{R}(\mathcal{A})$ is a ring because it is an intersection of a family of rings. If $\mathcal{R}$ is a ring containing $\mathcal{A}$, then by definition  $\mathcal{R} \in R(\mathcal{A})$ and therefore $\mathcal{R}(\mathcal{A}) \subseteq \mathcal{R}$.

## Exercise 6
1. This is simply the distribution law for set operations. The resulting union is for disjoint sets because either the index for $A$ or $B$ is different. Since any set in $\mathcal{R}$ can be expressed as a disjoint union of $A\_i$, therefore the intersection is also a disjoint union of $A\_i$ and so it is still in $\mathcal{R}$.

2. First, we prove $A \backslash \biguplus\limits\_{j=1}^n B\_j = \bigcap\limits\_{j=1}^{n}(A \cap B\_j^c)$.
This is obvious when $n = 1$. Assume this is true for $n = k$, when $n = k + 1$, we can write

$$
\begin{eqnarray*}
& & A \backslash \biguplus\limits_{j=1}^{k+1} B_j \\
&=& A \backslash (\biguplus\limits_{j=1}^{k} B_j \cup B_{k+1}) \\
&=& A \cap (\biguplus\limits_{j=1}^{k} B_j \cup B_{k+1})^c \\
&=& A \cap ((\biguplus\limits_{j=1}^{k} B_j)^c \cap B_{k+1}^c) \\
&=& (A \cap (\biguplus\limits_{j=1}^{k} B_j)^c) \cap B_{k+1}^c \\
&=& (A \backslash (\biguplus\limits_{j=1}^{k} B_j)) \cap B_{k+1}^c \\
&=& \bigcap\limits_{j=1}^{k}(A \cap B_j^c) \cap B_{k+1}^c \\
&=& \bigcap\limits_{j=1}^{k}(A \cap B_j^c) \cap A \cap B_{k+1}^c \\
&=& \bigcap\limits_{j=1}^{k+1}(A \cap B_j^c)
\end{eqnarray*}
$$

Next, we show that $(\biguplus\limits\_{i=1}^{n} A\_i) \cap B\_j^c = \biguplus\limits\_{i=1}^{n} A\_i \backslash B\_j$.

$$
\begin{eqnarray*}
& & (\biguplus\limits_{i=1}^{n} A_i) \cap B_j^c \\
&=& \biguplus\limits_{i=1}^{n} (A_i \cap B_j^c) \\
&=& \biguplus\limits_{i=1}^{n} (A_i \backslash B_j) \\
\end{eqnarray*}
$$

The result follows by combining two results.

3. When both $A$ and $B$ are disjoint unions of $A_i$, so is the pairwise set difference, so is the union of them, so is the intersection of them, so is the set difference of $A$ and $B$. Therefore it is in $\mathcal{R}$.

4. It is obvious that $(A \backslash B) \uplus B = A$. Since $\mathcal{R}$ contains all disjoint finite union. The union of any two disjoint finite unions can also be represented as a disjoint finite union.

Now We know $\mathcal{R}$ contains the empty set, all unions and all set differences. Therefore $\mathcal{R}$ is a ring.

5. It is clear that $\mathcal{R}$ is a ring. Any ring that contains $A$ must also contain any finite disjoint unions, therefore $\mathcal{R} = \mathcal{R}(A)$.

## Exercise 7
It is obvious that $\mathcal{R} \subseteq \mathcal{R}'$ since any finite disjoint union is a finite union. For an element $A = \biguplus\limits\_{i=1}^{n}A\_i$ of $\mathcal{R}'$ , now $A\_i \in \mathcal{S} \subseteq \mathcal{R}$. Since $\mathcal{R}$ is a ring, so $A \in \mathcal{R}$. Therefore $\mathcal{R}' \subseteq \mathcal{R}$, and thus $\mathcal{R}' = \mathcal{R}$.

## Exercise 8
It is because a sigma-algebra is closed under countable union.

## Exercise 9
1. We can define the sequence of sets ends with an infinite chain of $\phi$.
2. $\mu(B) = \mu(B \backslash A) + \mu(A) \ge \mu(A)$.

## Exercise 10
1. Since $\overline{\mu}$ is a measure, by exercise 9.1, $\overline{\mu}(A) = \sum\_{i=1}^{n} \overline\mu(A\_i)$, but $A\_i \in \mathcal{S}$, so the sum equals $\sum\_{i=1}^{n} \mu(A\_i)$.
2. Every element in $\mathcal{R}(\mathcal{S})$ has a disjoint union representation, so the measure value for every element has to be $\overline{\mu}$.

## Exercise 11
1. $A\_i = \biguplus\limits\_{j=1}^{}(A\_i \cap B\_j)$, then it follows from 9.1
2. $\sum\_{i=1}^{n} \mu(A\_i) = \sum\_{i=1}^{n} \sum\_{j=1}^{p} \mu(A\_i \cap B\_j) = \sum\_{j=1}^{p} \mu(B\_j)$.
3. Every element in $\mathcal{R}(\mathcal{S})$ can be written as a disjoint union of elements in $\mathcal{S}$, and the value is independent of the choice.
4. $\overline\mu(\phi)$ is defined to be 0 for it is an empty sum?

## Exercise 12
1. The formula split $B\_j$ by $A\_n$ into infinite pieces, and then split $A\_n$ into finite pieces. Since $A\_n^k, B\_j \in \mathcal{S}$, we have $C\_m = A\_n^k \cap B\_j \in \mathcal{S}$. $C\_m$ are pairwise disjoint because $A\_n^k$ are.
2. By 1, $B\_j$ is an countably infinite union of $C\_m$ in $\mathcal{S}$, so the result follows by the definition of $\mu$ is a measure on $\mathcal{S}$.
3. This is splitting $A\_n^k$ into $p$ disjoint pieces by $B\_j$.
4. Again, by definition $\mu$ is a measure on $\mathcal{S}$.
5. It is obvious that $\overline{\mu}(\phi) = 0$ by exercise 11.4.
The second property required for a measure is proved as follow:

$$
\begin{eqnarray*}
\overline{\mu}(A) &=& \sum_j{\mu(B_j)} \\
&=& \sum_{j=1}^{p}{\sum_{n=1}^{\infty}{\sum_{k=1}^{p_n}{\mu(A_n^k \cap B_j)}}}
\\
&=& \sum_{n=1}^{\infty}{\sum_{k=1}^{p_n}{\sum_{j=1}^{p}{\mu(A_n^k \cap B_j)}}} \\
&=& \sum_{n=1}^{\infty}{\sum_{k=1}^{p_n}{\mu(A_n^k)}} \\
&=& \sum_{n=1}^{\infty}{A_n} \\
\end{eqnarray*}
$$

## Exercise 13
Exercise 12.5 gives us a measure on $\mathcal{R}(\mathcal{S})$ and exercise 10.2 gives us uniqueness.

## Exercise 14
We can set $A\_1 = A$, $A\_2 = B$ and $A\_n = \phi$ for all $n > 2$, then by the third definition of outer-measure we got the inequality we wanted.

## Exercise 15
1. $\Omega \subseteq \Omega$, for all $T \subseteq \Omega$, we have

$$
\begin{eqnarray*}
& & \mu^{\ast}(T) \\
&=& \mu^{\ast}(T) + 0 \\
&=& \mu^{\ast}(T \cap \Omega) + \mu^{\ast}(\phi) \\
&=& \mu^{\ast}(T \cap \Omega) + \mu^{\ast}(T \cap \overline\Omega)
\end{eqnarray*}
$$

So $\Omega \in \Sigma(\mu^{\ast})$

$A^c \subseteq \Omega$, for all $T \subseteq \Omega$, we have:

$$
\begin{eqnarray*}
& & \mu^{\ast}(T) \\
&=& \mu^{\ast}(T \cap A) + \mu^{\ast}(T \cap A^c) \\
&=& \mu^{\ast}(T \cap A^c) + \mu^{\ast}(T \cap A) \\
\end{eqnarray*}
$$

Note that the first equality comes from the fact that $A \in \Sigma(\mu^{\ast})$, so $A^c \in \Sigma(\mu^{\ast})$.

2. Substitute $T = T \cap A$ and use $B \in \Sigma(\mu^{\ast})$.

3.

$$
\begin{eqnarray*}
& &T \cap A^c \cap (A^c \cup B^c)  \\
&=& (T \cap A^c \cap A^c) \cup (T \cap A^c \cap B^c)  \\
&=& (T \cap A^c) \cup (T \cap A^c \cap B^c)  \\
&=& (T \cap A^c)
\end{eqnarray*}
$$


4.

$$
\begin{eqnarray*}
& & T \cap A \cap (B \cap A)^c  \\
&=& T \cap A \cap (B^c \cup A^c)  \\
&= &T \cap A \cap B^c \cup T \cap A \cap A^c \\
&= &T \cap A \cap B^c
\end{eqnarray*}
$$

5. Substitute $T = T \cap (A \cap B)^c$ and use $A \in \Sigma(\mu^{\ast})$.

6. Add as required and use 2, that give:

$$
\begin{eqnarray*}
\mu^{\ast}(T \cap A) + \mu^{\ast}(T \cap A^c) &=& \mu^{\ast}(T \cap (A \cap B)^c) + \mu^{\ast}(T \cap (A \cap B)) \\
\mu^{\ast}(T)  &=& \mu^{\ast}(T \cap (A \cap B)^c) + \mu^{\ast}(T \cap (A \cap B))
\end{eqnarray*}
$$

So we get what we wanted.

7. $A \cup B = (A^c \cap B^c)^c$ and $A \backslash B = A \cap B^c$ are also belong to $\Sigma(\mu^{\ast})$.

## Exercise 16
Let $k < n$,

$$
\begin{eqnarray*}
& & B_n \cap B_k \\
&=& (A_{n} \backslash \bigcup_{i=1}^{n-1} A_i) \cap (A_{k} \backslash \bigcup_{i=1}^{k-1} A_i) \\
&=& (A_{n} \cap (\bigcup_{i=1}^{n-1} A_i)^c) \cap (A_{k} \cap (\bigcup_{i=1}^{k-1} A_k)^c) \\
&=& (A_{n} \cap (\bigcap_{i=1}^{n-1} A_i^c)) \cap (A_{k} \cap (\bigcap_{i=1}^{k-1} A_k^c)) \\
&=& \phi
\end{eqnarray*}
$$

The last equality follows because the second term contains $A_k^c$.

For any element $a \in A$, there must be a smallest index $p$ such that $a \in A\_p$, then $a \in B\_p = A\_p \backslash \bigcup\_{i=1}^{p-1} A\_i$ and therefore $a \in B$.

For any element $b \in B$, there must be a smallest index $q$ such that $b \in B\_q$, then $b \in B\_q = A\_q \backslash \bigcup\_{i=1}^{q-1} A\_i$ and therefore $b \in A\_q \subseteq A$.

Therefore $A = B$.

## Exercise 17
We have all these identities:

$$
\begin{eqnarray*}
\mu(T) &=& \mu(T \cap B) + \mu(T \cap B^c) \\
\mu(T \cap C) &=& \mu(T \cap C \cap B) + \mu(T \cap C \cap B^c) \\
\mu(T \cap C^c) &=& \mu(T \cap C^c \cap B) + \mu(T \cap C^c \cap B^c) \\
\mu(T) &=& \mu(T \cap C) + \mu(T \cap C^c) \\
\mu(T \cap B) &=& \mu(T \cap B \cap C) + \mu(T \cap B \cap C^c) \\
\mu(T \cap B^c) &=& \mu(T \cap B^c \cap C) + \mu(T \cap B^c \cap C^c) \\
\mu(T) &=& \mu(T \cap (B \cup C)) + \mu(T \cap B^c \cap C^c) \\
\end{eqnarray*}
$$

Now we have:

$$
\begin{eqnarray*}
& & \mu(T \cap (B \cup C)) \\
&=& \mu(T) - \mu(T \cap B^c \cap C^c) \\
&=& \mu(T \cap B) + \mu(T \cap B^c) - \mu(T \cap B^c \cap C^c) \\
&=& \mu(T \cap B) + \mu(T \cap B^c \cap C) + \mu(T \cap B^c \cap C^c) - \mu(T \cap B^c \cap C^c) \\
&=& \mu(T \cap B) + \mu(T \cap B^c \cap C) \\
&=& \mu(T \cap B) + \mu(T \cap C \cap B) + \mu(T \cap C \cap B^c) -  \mu(T \cap C \cap B) \\
&=& \mu(T \cap B) + \mu(T \cap C) - \mu(T \cap C \cap B) \\
&=& \mu(T \cap B) + \mu(T \cap C)
\end{eqnarray*}
$$

## Exercise 18
1. Union of two sets is in $\Sigma$, so induction gives us finite unions.
2. Exercise 17 gives us the sum rule for the union of two sets, so induction gives us finite union.
3.

$$
\begin{eqnarray*}
(\cup_{n=1}^{N}{B_n}) &\subseteq & (\cup_{i=1}^{\infty}B_n) \\
(\cup_{n=1}^{\infty}{B_n})^c &\subseteq &(\cup_{i=1}^{N}B_n)^c \\
T \cap B^c &\subseteq & T \cap (\cup_{i=1}^{N}B_n)^c
\end{eqnarray*}
$$

So the second rule for outer measure applies.

4. Part 2 and 3 gives:

$$
\begin{eqnarray*}
\mu^{\ast}(T\cap B^c) &\le& \mu^{\ast}(T\cap (\bigcup_{i=1}^{N} B_n)^c) \\
\sum_{n=1}^{N}\mu^{\ast}(T \cap B_n) &=& \mu^{\ast}(T \cap (\bigcup_{n=1}^{N} B_n))
\end{eqnarray*}
$$

Summing get,

$$
\begin{eqnarray*}
\mu^{\ast}(T\cap B^c) + \sum_{n=1}^{N}\mu^{\ast}(T \cap B_n) &\le& \mu^{\ast}(T\cap (\bigcup_{i=1}^{N} B_n)^c) + \mu^{\ast}(T \cap (\bigcup_{n=1}^{N} B_n)) \\
&=& \mu^{\ast}(T)
\end{eqnarray*}
$$

Since this works for arbitrary $N$, this shows the infinite sum of non-negative numbers is bounded above, and therefore it converges and satisfies the bound.

5.

$$
\begin{eqnarray*}
& & \mu^{\ast}(T) \\
&=& \mu^{\ast}((T \cap B) \cup (T \cap B^c)) \\
&\le& \mu^{\ast}(T \cap B) + \mu^{\ast}(T \cap B^c) \\
\end{eqnarray*}
$$

$$
\begin{eqnarray*}
& & \mu^{\ast}(T \cap B) \\
&=& \mu^{\ast}(T \cap (\biguplus_{n=1}^{\infty} B_n)) \\
&=& \mu^{\ast}(\biguplus_{n=1}^{\infty} T \cap B_n) \\
&\le& \sum_n \mu^{\ast}(T \cap B_n)
\end{eqnarray*}
$$

6. Part 4 and Part 5 can show $\mu^{\ast}(T) \le \mu^{\ast}(T \cap B) + \mu^{\ast}(T \cap B^c) \le \mu^{\ast}(T\cap B^c) + \sum\_{n=1}^{\infty}\mu^{\ast}(T \cap B\_n) \le \mu^{\ast}(T)$, so $B \in \Sigma(\mu^{\ast})$. Substitute $T = B$ to get the countably additivity we wanted.

7. Exercise 15.6, 15.7 and part 6 implies $\Sigma$ is a $\sigma$-algebra. Part 6 gives us countably additive property.

## Exercise 19
1. Set $A_n = \phi$, we reached the minimum.
2. If we had an R-cover of $B$, it certainly also covers $A$, so the infimum over a superset cannot be larger.
3. By definition, $\mu^{\ast}(A\_n)$ is the greatest lower bound, so $\mu^{\ast}(A\_n) + \frac{\epsilon}{2^n}$ is not a lower bound, therefore there exists an R-cover violating the bound.
4. The concatenation of the covers in part 3 will do the job. Do not remove duplicates.
5. Sum the equation on part 3 and use part 4.

$$
\begin{eqnarray*}
\sum_{n=1}^{\infty} \sum_{p=1}^{\infty} \mu(A_n^p) < \sum_{n=1}^{\infty} \mu^{\ast}(A_n) + \epsilon \\
\sum_{k=1}^{\infty} \mu(R_k) < \sum_{n=1}^{\infty} \mu^{\ast}(A_n) + \epsilon \\
\end{eqnarray*}
$$

and the result follows because the $\mu^{\ast}$ is the greatest lower bound.

6. Part 1 gives us the first condition to be an outer measure. Part 2 gives us the second condition. Part 5 gives us the last condition. Therefore $\mu^{\ast}$ is an outer measure.

## Exercise 20
1. $A$ is a $\mathcal{R}$-cover of $A$. Therefore, $\mu^{\ast}(A) \le \mu(A)$ as $\mu^{\ast}(A)$ is a lower bound of the measure of all $\mathcal{R}$-cover of $A$.
2. This is the same as exercise 16 except everything intersects with $A$. $B\_n \in \mathcal{R}$ because $A$ and $A\_n$ are. Note also that $B\_n \subseteq A\_n$.
3. Suppose $\mu^{\ast}(A) < \mu(A)$. Then there exists $A\_n$ a $\mathcal{R}$-cover of $A$ such that $\sum\_{n=1}^{\infty}\mu(A\_n) < \mu(A)$, but then $\mu(A) = \mu(\biguplus\_{n=1}^{\infty}B\_n) = \sum\_{n=1}^{\infty}\mu(B\_n) \le \sum\_{n=1}^{\infty}\mu(A\_n) < \mu(A)$, a contradiction.

## Exercise 21
1. $T = (T \cap A) \cup (T \cap A^c)$, therefore it holds because $\mu^{\ast}$ is an outer measure.
2. For any point in $T \cap A$, it must be in $T$ and therefore in at least one of the $T\_n$. Now that point is also in $T\_n \cap A$, therefore $T\_n \cap A$ covers $T \cap A$. The complement case is similar.
3. Suppose $\mu^{\ast}(T) < \mu^{\ast}(T \cap A) + \mu^{\ast}(T \cap A^c)$, then there exists $T\_n$ a $\mathcal{R}$-cover of $T$ such that $\sum\_{n=1}^{\infty}\mu(T\_n) < \mu^{\ast}(T \cap A) + \mu^{\ast}(T \cap A^c)$.
Now we can write
$\sum\_{n=1}^{\infty}\mu(T\_n \cap A) + \mu(T\_n \cap A^c) < \mu^{\ast}(T \cap A) + \mu^{\ast}(T \cap A^c)$, but $T\_n \cap A$ is a cover of $T \cap A$, leading to a contradiction.
4. Part 1 and 3 shows that $\mu^{\ast}(T) = \mu^{\ast}(T \cap A) + \mu^{\ast}(T \cap A^c)$ if $A \in \mathcal{R}$, therefore $\mathcal{R} \subseteq \Sigma(\mu^{\ast})$.
5. By exercise 18.7, $\Sigma(\mu^{\ast})$ is a sigma algebra and it contains $\mathcal{R}$, so it must also contains $\sigma(\mathcal{R})$.
## Exercise 22
$\mu^{\ast}$ is such a measure.
## Exercise 23
If we contain all countable unions, it certainly also contains all finite unions.
## Exercise 24
Extends $S$ to a ring and then use exercise 22.
