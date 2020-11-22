# Quadratic Residue 

**Definition**: for any number $$ m $$, if there exists a solution for $$ m = x^2 \mod n $$, we call $$ m $$ a quadratic residue $$ \mod n$$. For brevity, $$ p $$ is always an odd prime.

- If $$ m $$ is a quadratic residue $$ \mod p $$, there are exactly two solutions to $$ m = x^2 \mod p $$, because $$ x^2 = y^2 \mod p $$ implies $$ (x + y)(x - y) $$ is a multiple of $$ p $$.

- There are exactly $$ \frac{p-1}{2} $$ quadratic residue $$ \mod p $$, because $$ x \to x^2 $$ is exactly $$ 2 \to 1 $$.

- **Euler's criterion**: $$ \left(\frac{a}{p}\right) = a^{\frac{p - 1}{2}} \mod p$$, To show it, we can compute $$ a^{\frac{p - 1}{2}} \mod p $$ two cases.
  - If $$ a = x^2 \mod p $$, then Fermat's theorem gives $$ a^{\frac{p - 1}{2}} = x^{p - 1} = 1 $$.
    - Fermat's: the multiplicative group has an order $$ p - 1 $$.
  - Otherwise, for each $$ x \in [1 \cdots p - 1] $$, $$ x $$ and $$ ax^{-1} $$ comes in pairs, so $$ a^{\frac{p - 1}{2}} = -1 \mod p $$ by Wilson's theorem.
    - Wilson's: for each $$ x \in [2 \cdots p - 2] $$, $$ x $$ and $$ x^{-1} $$ comes in pairs, so the product is -1.

- Since $$ \left(\frac{-1}{p}\right) = (-1)^{\frac{p - 1}{2}} \mod p$$, therefore if $$ p = 4m + 1 $$, then $$ \left(\frac{-1}{p}\right) = (-1)^{2m} = 1 $$, the $$ p = 4m + 3 $$ case is similar.

- **Gauss's Lemma**. If we take $$ \frac{p - 1}{2} $$ multiples of $$ a $$ and reduce them modulo $$ p $$. Denote the number of them greater than $$ \frac{p}{2} $$ by $$ s $$, then $$ \left(\frac{a}{p}\right) = (-1)^s $$.
  - To show that, let $$ S $$ be the set of residues less than $$ \frac{p}{2} $$, $$ B $$ be the set of residues greater than $$ \frac{p}{2} $$. $$ R $$ be the set $$ \{p - b | b \in B\} $$.
  - $$ S \cup R = [1 \cdots \frac{p-1}{2}] $$, this can be seen as there are $$ \frac{p-1}{2} $$ numbers and no two of them are the same (otherwise $$ p $$ factors)
  - Now we calculate the product of $$ S \cup B $$ two ways:
    - These are just all the multiples, so the product is $$ a^{\frac{p-1}{2}}\left(\frac{p-1}{2}\right)! $$
    - Since $$ S \cup R = [1 \cdots \frac{p-1}{2}] $$, so the product is $$ (-1)^s\left(\frac{p-1}{2}\right)! $$.
  - Therefore $$ \left(\frac{a}{p}\right) = a^{\frac{p - 1}{2}} = (-1)^s $$.

- **Eisenstein's lemma**. Suppose we alter the proof for Gauss's lemma and take all even multiple of $$ a $$ instead, we can do:
  - Let $$ S $$ be the set of even residues, $$ B $$ be the set of odd residues, $$ R $$ be the set $$ \{p - b | b \in B\} $$.
  - $$ S \cup R $$ is simply all the even integers in $$ [1 \cdots p] $$, this can be seen as there are $$ \frac{p-1}{2} $$ numbers and no two of them are the same (otherwise $$ p $$ factors)
  - The same calculate twice trick applies.
  - Now $$ na = \lfloor\frac{na}{p}\rfloor p + r $$. For even $$ n $$, the parity of $$ r $$ is the same as the parity of $$ \lfloor\frac{na}{p}\rfloor $$.
  - Therefore $$ \left(\frac{a}{p}\right) = a^{\frac{p - 1}{2}} = (-1)^{\sum_{n}\lfloor\frac{na}{p}\rfloor} $$.

- **Quadratic reciprocity**. $$ \left(\frac{p}{q}\right)\left(\frac{q}{p}\right) = (-1)^{\frac{(p-1)(q-1)}{4}}$$
  - https://en.wikipedia.org/wiki/Proofs_of_quadratic_reciprocity

- **Tonelli–Shanks algorithm**. To compute the square root of $$ n $$ modulo $$ p $$, we start with Euler's criterion $$ n^\frac{p-1}{2} = 1 $$.
  - **Algorithm**
    1. Find odd $$ q $$ and $$ s $$ such that $$ p = q2^s $$.
    2. Pick a quadratic nonresidue $$ z $$.
    3. Initialize $$ M = s $$, $$ C = z^q $$, $$ T = n^q $$, $$ R = n^\frac{q + 1}{2} $$.
    4. While $$ t \ne 1 $$
       - Find *least* $$ I $$ such that $$ T^{2^i} = 1$$
       - Set $$ B = C^{2^{M - I - 1}} $$
       - Set $$ M = I $$
       - Set $$ C = B^2 $$
       - Set $$ T = TB^2 $$
       - Set $$ R = RB $$
    5. return R
  - **Correctness**
    - Lemma:
      - $$ B^{2^I} = C^{2^{M - I - 1}}^{2^I} = C^{2^{M - 1}} = -1 $$
      - $$ T^{2^{I-1}} = -1 $$ because $$ T^{2^{I-1}} $$ is a square root of 1 and it is not 1.
    - Loop invariants:
      - $$ R^2 = Tn $$ because $$ R'^2 = (RB)^2 = R^2 B^2 = TnB^2 = T'n $$
      - $$ C^{2^{M-1}} = -1 $$ because $$ C'^{2^{M' - 1}} = (B^2)^{2^{I - 1}} = B^{2^I} = -1 $$
      - $$ T^{2^{M-1}} = 1 $$ because $$ T'^{2^{M'-1}} = (TB^2)^{2^{I-1}} = T^{2^{I-1}}B^{2^I}} = 1 $$
    - $$ M $$ always decrease and therefore the algorithm terminates.
          

**Composite moduli**: All of the above applies when $$ p $$ is an odd prime. The key idea is to solve it per prime power component and use the Chinese remainder theorem to find the answer. Key result is:
  - $$ x $$ is a quadratic residue $$ \pmod mn $$ if and only if $ x $ is a quadratic residue $$ \pmod m $$ and a quadratic residue $$ \pmod n $$ .
  - Let $$ c $$ be  a solution of the congruence $$x^2\equiv a\pmod{p}$$, and let $$ d $$ be a solution of $$ y^2 \equiv a \pmod{q}$$. By the Chinese Remainder Theorem, the system $$ z \equiv c\pmod{p}$$, $$ z \equiv d \pmod{q} $$ has a solution if p and q are coprime. If $$ z $$ is a solution of the system, then $$ z^2 \equiv c^2 \equiv a \pmod{p}$$ and $$z^2 \equiv d^2 = a \pmod{q} $$. It follows that $$ z^2 \equiv a \pmod{pq}$$.

# Recap - Chinese remainder theorem

Goal: Solve this system, assume $ p, q, r $ are pairwise coprime.

$$
x = x_p \mod p \\
x = x_q \mod q \\
x = x_r \mod r
$$

Consider $$ g_p = qr((qr)^{-1} \mod p) $$, we have

$$
g_p \pmod p = 1 \\
g_p \pmod q = 0 \\
g_p \pmod r = 0
$$

We can similarly define $$ g_q $$ and $$ g_r $$. Then we can get our answer as 

$$ x = x_p g_p + x_q g_q + x_r g_r $$
