(** * Induction: Proof by Induction *)

(* ################################################################# *)
(** * Separate Compilation *)

(** Before getting started on this chapter, we need to import
    all of our definitions from the previous chapter: *)

From LF Require Export Basics.

(** For this [Require Export] command to work, Coq needs to be
    able to find a compiled version of [Basics.v], called [Basics.vo],
    in a directory associated with the prefix [LF].  This file is
    analogous to the [.class] files compiled from [.java] source files
    and the [.o] files compiled from [.c] files.

    First create a file named [_CoqProject] containing the following
    line (if you obtained the whole volume "Logical Foundations" as a
    single archive, a [_CoqProject] should already exist and you can
    skip this step):

      -Q . LF

    This maps the current directory ("[.]", which contains [Basics.v],
    [Induction.v], etc.) to the prefix (or "logical directory")
    "[LF]".  Proof General and CoqIDE read [_CoqProject]
    automatically, so they know to where to look for the file
    [Basics.vo] corresponding to the library [LF.Basics].

    Once [_CoqProject] is thus created, there are various ways to
    build [Basics.vo]:

     - In Proof General or CoqIDE, the compilation should happen
       automatically when you submit the [Require] line above to PG.

     - If you want to compile from the command line, generate a
       [Makefile] using the [coq_makefile] utility, which comes
       installed with Coq (if you obtained the whole volume as a
       single archive, a [Makefile] should already exist and you can
       skip this step):

         coq_makefile -f _CoqProject *.v -o Makefile

       Note: You should rerun that command whenever you add or remove
       Coq files to the directory.

       Now you can compile [Basics.v] by running [make] with the
       corresponding [.vo] file as a target:

         make Basics.vo

       All files in the directory can be compiled by giving no
       arguments:

         make

       Under the hood, [make] uses the Coq compiler, [coqc].  You can
       also run [coqc] directly:

         coqc -Q . LF Basics.v

       But [make] also calculates dependencies between source files to
       compile them in the right order, so [make] should generally be
       preferred over explicit [coqc].

    If you have trouble (e.g., if you get complaints about missing
    identifiers later in the file), it may be because the "load path"
    for Coq is not set up correctly.  The [Print LoadPath.] command
    may be helpful in sorting out such issues.

    In particular, if you see a message like

        Compiled library Foo makes inconsistent assumptions over
        library Bar

    check whether you have multiple installations of Coq on your
    machine.  It may be that commands (like [coqc]) that you execute
    in a terminal window are getting a different version of Coq than
    commands executed by Proof General or CoqIDE.

    - Another common reason is that the library [Bar] was modified and
      recompiled without also recompiling [Foo] which depends on it.
      Recompile [Foo], or everything if too many files are
      affected.  (Using the third solution above: [make clean; make].)

    One more tip for CoqIDE users: If you see messages like [Error:
    Unable to locate library Basics], a likely reason is
    inconsistencies between compiling things _within CoqIDE_ vs _using
    [coqc] from the command line_.  This typically happens when there
    are two incompatible versions of [coqc] installed on your
    system (one associated with CoqIDE, and one associated with [coqc]
    from the terminal).  The workaround for this situation is
    compiling using CoqIDE only (i.e. choosing "make" from the menu),
    and avoiding using [coqc] directly at all. *)

(* ################################################################# *)
(** * Proof by Induction *)

(** We can prove that [0] is a neutral element for [+] on the left
    using just [reflexivity].  But the proof that it is also a neutral
    element on the _right_ ... *)

Theorem add_0_r_firsttry : forall n:nat,
  n + 0 = n.

(** ... can't be done in the same simple way.  Just applying
  [reflexivity] doesn't work, since the [n] in [n + 0] is an arbitrary
  unknown number, so the [match] in the definition of [+] can't be
  simplified.  *)

Proof.
  intros n.
  simpl. (* Does nothing! *)
Abort.

(** And reasoning by cases using [destruct n] doesn't get us much
    further: the branch of the case analysis where we assume [n = 0]
    goes through fine, but in the branch where [n = S n'] for some [n'] we
    get stuck in exactly the same way. *)

Theorem add_0_r_secondtry : forall n:nat,
  n + 0 = n.
Proof.
  intros n. destruct n as [| n'] eqn:E.
  - (* n = 0 *)
    reflexivity. (* so far so good... *)
  - (* n = S n' *)
    simpl.       (* ...but here we are stuck again *)
Abort.

(** We could use [destruct n'] to get one step further, but,
    since [n] can be arbitrarily large, we'll never get all the there
    if we just go on like this. *)

(** To prove interesting facts about numbers, lists, and other
    inductively defined sets, we often need a more powerful reasoning
    principle: _induction_.

    Recall (from high school, a discrete math course, etc.) the
    _principle of induction over natural numbers_: If [P(n)] is some
    proposition involving a natural number [n] and we want to show
    that [P] holds for all numbers [n], we can reason like this:
         - show that [P(O)] holds;
         - show that, for any [n'], if [P(n')] holds, then so does
           [P(S n')];
         - conclude that [P(n)] holds for all [n].

    In Coq, the steps are the same: we begin with the goal of proving
    [P(n)] for all [n] and break it down (by applying the [induction]
    tactic) into two separate subgoals: one where we must show [P(O)]
    and another where we must show [P(n') -> P(S n')].  Here's how
    this works for the theorem at hand: *)

Theorem add_0_r : forall n:nat, n + 0 = n.
Proof.
  intros n. induction n as [| n' IHn'].
  - (* n = 0 *)    reflexivity.
  - (* n = S n' *) simpl. rewrite -> IHn'. reflexivity.  Qed.

(** Like [destruct], the [induction] tactic takes an [as...]
    clause that specifies the names of the variables to be introduced
    in the subgoals.  Since there are two subgoals, the [as...] clause
    has two parts, separated by [|].  (Strictly speaking, we can omit
    the [as...] clause and Coq will choose names for us.  In practice,
    this is a bad idea, as Coq's automatic choices tend to be
    confusing.)

    In the first subgoal, [n] is replaced by [0].  No new variables
    are introduced (so the first part of the [as...] is empty), and
    the goal becomes [0 = 0 + 0], which follows by simplification.

    In the second subgoal, [n] is replaced by [S n'], and the
    assumption [n' + 0 = n'] is added to the context with the name
    [IHn'] (i.e., the Induction Hypothesis for [n']).  These two names
    are specified in the second part of the [as...] clause.  The goal
    in this case becomes [S n' = (S n') + 0], which simplifies to
    [S n' = S (n' + 0)], which in turn follows from [IHn']. *)

Theorem minus_n_n : forall n,
  minus n n = 0.
Proof.
  (* WORKED IN CLASS *)
  intros n. induction n as [| n' IHn'].
  - (* n = 0 *)
    simpl. reflexivity.
  - (* n = S n' *)
    simpl. rewrite -> IHn'. reflexivity.  Qed.

(** (The use of the [intros] tactic in these proofs is actually
    redundant.  When applied to a goal that contains quantified
    variables, the [induction] tactic will automatically move them
    into the context as needed.) *)

(** **** Exercise: 2 stars, standard, especially useful (basic_induction)

    Prove the following using induction. You might need previously
    proven results. *)

Theorem mul_0_r : forall n:nat,
  n * 0 = 0.
Proof.
  intros n.
  induction n as [| n IHn].
  -              (* 0 * 0 = 0       *)
    reflexivity.
  -              (* IHn : n * 0 = 0 *)
                 (* S n * 0 = 0     *)
    simpl.       (* n * 0 = 0       *) (* Check the definition of mult and plus to see why this is a simplification *)
    apply IHn.                         (* when the goal is identical with the hypothesis, we can use apply *)
    Qed.

Theorem plus_n_Sm : forall n m : nat,
  S (n + m) = n + (S m).
Proof.
  intros n m.
  induction n as [|n IHn].
  -                 (* S (0 + m) = 0 + S m         *)
    reflexivity.
  -                 (* IHn : S (n + m) = n + S m   *)
                    (* S (S n + m) = S n + S m     *)
    simpl.          (* S (S (n + m)) = S (n + S m) *) (* Check the definition of plus to see why this is a simplification *)
    rewrite -> IHn. (* S (n + S m) = S (n + S m)   *)
    reflexivity.
  Qed.

Theorem add_comm : forall n m : nat,
  n + m = m + n.
Proof.
  intros n m.
  induction m as [|m IHm].
  -                        (* n + 0 = 0 + n         *)
    simpl.                 (* n + 0 = n             *)
    apply add_0_r.
  -                        (* IHm : n + m = m + n   *)
                           (* n + S m = S m + n     *)
    simpl.                 (* n + S m = S (m + n)   *)
    rewrite <- IHm.        (* n + S m = S (n + m)   *)
    rewrite <- plus_n_Sm.  (* S (n + m) = S (n + m) *)
    reflexivity.
  Qed.

Theorem add_assoc : forall n m p : nat,
  n + (m + p) = (n + m) + p.
Proof.
  intros n m p.
  induction m as [|m IHm].
  -                         (* n + (0 + p) = n + 0 + p         *)
      rewrite -> add_0_r.   (* n + (0 + p) = n + p             *)
      reflexivity.
  -                         (* IHm : n + (m + p) = n + m + p   *)
                            (* n + (S m + p) = n + S m + p     *)
      simpl.                (* n + S (m + p) = n + S m + p     *)
      rewrite <- plus_n_Sm. (* S (n + (m + p)) = n + S m + p   *)
      rewrite <- plus_n_Sm. (* S (n + (m + p)) = S (n + m) + p *)
      simpl.                (* S (n + (m + p)) = S (n + m + p) *)
      rewrite -> IHm.       (* S (n + m + p) = S (n + m + p)   *)
      reflexivity.
  Qed.
(** [] *)

(** **** Exercise: 2 stars, standard (double_plus)

    Consider the following function, which doubles its argument: *)

Fixpoint double (n:nat) :=
  match n with
  | O => O
  | S n' => S (S (double n'))
  end.

(** Use induction to prove this simple fact about [double]: *)

Lemma double_plus : forall n, double n = n + n .
Proof.
  intros n.
  induction n as [|n IHn].
  -                       (* double 0 = 0 + 0                 *)
    reflexivity.
  -                       (* IHn : double n = n + n           *)
                          (* double (S n) = S n + S n         *)
    simpl.                (* S (S (double n)) = S (n + S n)   *)
    rewrite <- plus_n_Sm. (* S (S (double n)) = S (S (n + n)) *)
    rewrite -> IHn.       (* S (S (n + n)) = S (S (n + n))    *)
    reflexivity.
  Qed.
(** [] *)

(** **** Exercise: 2 stars, standard, optional (even_S)

    One inconvenient aspect of our definition of [even n] is the
    recursive call on [n - 2]. This makes proofs about [even n]
    harder when done by induction on [n], since we may need an
    induction hypothesis about [n - 2]. The following lemma gives an
    alternative characterization of [even (S n)] that works better
    with induction: *)

Theorem even_S : forall n : nat,
  even (S n) = negb (even n).
Proof.
  induction n as [|n IHn].
  -                             (* even 1 = negb (even 0)                *)
    reflexivity.
  -                             (* IHn : even (S n) = negb (even n)      *)
                                (* even (S (S n)) = negb (even (S n))    *)
    rewrite -> IHn.             (* even (S (S n)) = negb (negb (even n)) *)
    simpl.                      (* even n = negb (negb (even n))         *)
    rewrite -> negb_involutive. (* even n = even n                       *)
    reflexivity.
  Qed.
(** [] *)

(** **** Exercise: 1 star, standard, optional (destruct_induction)

    Briefly explain the difference between the tactics [destruct]
    and [induction].

(* FILL IN HERE *)
*)

(* Do not modify the following line: *)
Definition manual_grade_for_destruct_induction : option (nat*string) := None.
(** [] *)

(* ################################################################# *)
(** * Proofs Within Proofs *)

(** In Coq, as in informal mathematics, large proofs are often
    broken into a sequence of theorems, with later proofs referring to
    earlier theorems.  But sometimes a proof will require some
    miscellaneous fact that is too trivial and of too little general
    interest to bother giving it its own top-level name.  In such
    cases, it is convenient to be able to simply state and prove the
    needed "sub-theorem" right at the point where it is used.  The
    [assert] tactic allows us to do this. *)

Theorem mult_0_plus' : forall n m : nat,
  (0 + n) * m = n * m.
Proof.
  intros n m.
  assert (H: 0 + n = n). { reflexivity. }
  rewrite -> H.
  reflexivity.  Qed.

(** The [assert] tactic introduces two sub-goals.  The first is
    the assertion itself; by prefixing it with [H:] we name the
    assertion [H].  (We can also name the assertion with [as] just as
    we did above with [destruct] and [induction], i.e., [assert (0 + n
    = n) as H].)  Note that we surround the proof of this assertion
    with curly braces [{ ... }], both for readability and so that,
    when using Coq interactively, we can see more easily when we have
    finished this sub-proof.  The second goal is the same as the one
    at the point where we invoke [assert] except that, in the context,
    we now have the assumption [H] that [0 + n = n].  That is,
    [assert] generates one subgoal where we must prove the asserted
    fact and a second subgoal where we can use the asserted fact to
    make progress on whatever we were trying to prove in the first
    place. *)

(** For example, suppose we want to prove that [(n + m) + (p + q)
    = (m + n) + (p + q)]. The only difference between the two sides of
    the [=] is that the arguments [m] and [n] to the first inner [+]
    are swapped, so it seems we should be able to use the
    commutativity of addition ([add_comm]) to rewrite one into the
    other.  However, the [rewrite] tactic is not very smart about
    _where_ it applies the rewrite.  There are three uses of [+] here,
    and it turns out that doing [rewrite -> add_comm] will affect
    only the _outer_ one... *)

Theorem plus_rearrange_firsttry : forall n m p q : nat,
  (n + m) + (p + q) = (m + n) + (p + q).
Proof.
  intros n m p q.
  (* We just need to swap (n + m) for (m + n)... seems
     like add_comm should do the trick! *)
  rewrite add_comm.
  (* Doesn't work... Coq rewrites the wrong plus! :-( *)
Abort.

(** To use [add_comm] at the point where we need it, we can introduce
    a local lemma stating that [n + m = m + n] (for the _particular_ [m]
    and [n] that we are talking about here), prove this lemma using
    [add_comm], and then use it to do the desired rewrite. *)

Theorem plus_rearrange : forall n m p q : nat,
  (n + m) + (p + q) = (m + n) + (p + q).
Proof.
  intros n m p q.
  assert (H: n + m = m + n).
  { rewrite add_comm. reflexivity. }
  rewrite H. reflexivity.  Qed.

(* ################################################################# *)
(** * Formal vs. Informal Proof *)

(** "_Informal proofs are algorithms; formal proofs are code_." *)

(** What constitutes a successful proof of a mathematical claim?
    The question has challenged philosophers for millennia, but a
    rough and ready definition could be this: A proof of a
    mathematical proposition [P] is a written (or spoken) text that
    instills in the reader or hearer the certainty that [P] is true --
    an unassailable argument for the truth of [P].  That is, a proof
    is an act of communication.

    Acts of communication may involve different sorts of readers.  On
    one hand, the "reader" can be a program like Coq, in which case
    the "belief" that is instilled is that [P] can be mechanically
    derived from a certain set of formal logical rules, and the proof
    is a recipe that guides the program in checking this fact.  Such
    recipes are _formal_ proofs.

    Alternatively, the reader can be a human being, in which case the
    proof will be written in English or some other natural language,
    and will thus necessarily be _informal_.  Here, the criteria for
    success are less clearly specified.  A "valid" proof is one that
    makes the reader believe [P].  But the same proof may be read by
    many different readers, some of whom may be convinced by a
    particular way of phrasing the argument, while others may not be.
    Some readers may be particularly pedantic, inexperienced, or just
    plain thick-headed; the only way to convince them will be to make
    the argument in painstaking detail.  But other readers, more
    familiar in the area, may find all this detail so overwhelming
    that they lose the overall thread; all they want is to be told the
    main ideas, since it is easier for them to fill in the details for
    themselves than to wade through a written presentation of them.
    Ultimately, there is no universal standard, because there is no
    single way of writing an informal proof that is guaranteed to
    convince every conceivable reader.

    In practice, however, mathematicians have developed a rich set of
    conventions and idioms for writing about complex mathematical
    objects that -- at least within a certain community -- make
    communication fairly reliable.  The conventions of this stylized
    form of communication give a fairly clear standard for judging
    proofs good or bad.

    Because we are using Coq in this course, we will be working
    heavily with formal proofs.  But this doesn't mean we can
    completely forget about informal ones!  Formal proofs are useful
    in many ways, but they are _not_ very efficient ways of
    communicating ideas between human beings. *)

(** For example, here is a proof that addition is associative: *)

Theorem add_assoc' : forall n m p : nat,
  n + (m + p) = (n + m) + p.
Proof. intros n m p. induction n as [| n' IHn']. reflexivity.
  simpl. rewrite IHn'. reflexivity.  Qed.

(** Coq is perfectly happy with this.  For a human, however, it
    is difficult to make much sense of it.  We can use comments and
    bullets to show the structure a little more clearly... *)

Theorem add_assoc'' : forall n m p : nat,
  n + (m + p) = (n + m) + p.
Proof.
  intros n m p. induction n as [| n' IHn'].
  - (* n = 0 *)
    reflexivity.
  - (* n = S n' *)
    simpl. rewrite IHn'. reflexivity.   Qed.

(** ... and if you're used to Coq you may be able to step
    through the tactics one after the other in your mind and imagine
    the state of the context and goal stack at each point, but if the
    proof were even a little bit more complicated this would be next
    to impossible.

    A (pedantic) mathematician might write the proof something like
    this: *)

(** - _Theorem_: For any [n], [m] and [p],

      n + (m + p) = (n + m) + p.

    _Proof_: By induction on [n].

    - First, suppose [n = 0].  We must show that

        0 + (m + p) = (0 + m) + p.

      This follows directly from the definition of [+].

    - Next, suppose [n = S n'], where

        n' + (m + p) = (n' + m) + p.

      We must now show that

        (S n') + (m + p) = ((S n') + m) + p.

      By the definition of [+], this follows from

        S (n' + (m + p)) = S ((n' + m) + p),

      which is immediate from the induction hypothesis.  _Qed_. *)

(** The overall form of the proof is basically similar, and of
    course this is no accident: Coq has been designed so that its
    [induction] tactic generates the same sub-goals, in the same
    order, as the bullet points that a mathematician would write.  But
    there are significant differences of detail: the formal proof is
    much more explicit in some ways (e.g., the use of [reflexivity])
    but much less explicit in others (in particular, the "proof state"
    at any given point in the Coq proof is completely implicit,
    whereas the informal proof reminds the reader several times where
    things stand). *)

(** **** Exercise: 2 stars, advanced, especially useful (add_comm_informal)

    Translate your solution for [add_comm] into an informal proof:

    Theorem: Addition is commutative.

    Proof: (* FILL IN HERE *)
*)

(* Do not modify the following line: *)
Definition manual_grade_for_add_comm_informal : option (nat*string) := None.
(** [] *)

(** **** Exercise: 2 stars, standard, optional (eqb_refl_informal)

    Write an informal proof of the following theorem, using the
    informal proof of [add_assoc] as a model.  Don't just
    paraphrase the Coq tactics into English!

    Theorem: [(n =? n) = true] for any [n].

    Proof: (* FILL IN HERE *)
*)
(** [] *)

(* ################################################################# *)
(** * More Exercises *)

(** **** Exercise: 3 stars, standard, especially useful (mul_comm)

    Use [assert] to help prove [add_shuffle3].  You don't need to
    use induction yet. *)

Theorem add_shuffle3 : forall n m p : nat,
  n + (m + p) = m + (n + p).
Proof.
  intros n m p.                  (* n + (m + p) = m + (n + p) *)
  rewrite -> add_assoc.          (* n + m + p = m + (n + p)   *)
  rewrite -> add_assoc.          (* n + m + p = m + (n + p)   *)
  assert ( n + m = m + n ) as H.
  {
    apply add_comm.
  }
  rewrite -> H.                  (* m + n + p = m + n + p     *)
  reflexivity.
  Qed.

(** Now prove commutativity of multiplication.  You will probably
    want to define and prove a "helper" theorem to be used
    in the proof of this one. Hint: what is [n * (1 + k)]? *)

Theorem mul_comm : forall m n : nat,
  m * n = n * m.
Proof.
  intros n m.
  induction m as [|m IHm].
  -                       (* n * 0 = 0 * n         *)
    simpl.                (* n * 0 = 0             *)
    apply mul_0_r.
  -                       (* IHm : n * m = m * n   *)
                          (* n * S m = S m * n     *)
    simpl.                (* n * S m = n + m * n   *)
    rewrite <- IHm.       (* n * S m = n + n * m   *)
    rewrite <- mult_n_Sm. (* n * m + n = n + n * m *)
    rewrite <- add_comm.  (* n + n * m = n + n * m *)
    reflexivity.
  Qed.
(** [] *)

(** **** Exercise: 3 stars, standard, optional (more_exercises)

    Take a piece of paper.  For each of the following theorems, first
    _think_ about whether (a) it can be proved using only
    simplification and rewriting, (b) it also requires case
    analysis ([destruct]), or (c) it also requires induction.  Write
    down your prediction.  Then fill in the proof.  (There is no need
    to turn in your piece of paper; this is just to encourage you to
    reflect before you hack!) *)

Check leb.

Theorem leb_refl : forall n:nat,
  (n <=? n) = true.
Proof.
  intros n.
  induction n as [|n IHn].
  -              (* (0 <=? 0) = true       *)
    reflexivity.
  -              (* IHn : (n <=? n) = true *)
                 (* (S n <=? S n) = true   *)
    simpl.       (* (n <=? n) = true       *)
    apply IHn.
  Qed.

Theorem zero_neqb_S : forall n:nat,
  0 =? (S n) = false.
Proof.
  intros n.    (* (0 =? S n) = false *)
  reflexivity.
  Qed.

Theorem andb_false_r : forall b : bool,
  andb b false = false.
Proof.
  intros b.
  destruct b.
  -              (* true && false = false  *)
    reflexivity.
  -              (* false && false = false *)
    reflexivity.
  Qed.

Theorem plus_leb_compat_l : forall n m p : nat,
  n <=? m = true -> (p + n) <=? (p + m) = true.
Proof.
  intros n m p.
  induction p as [|p IHp].
  -            (* (n <=? m) = true -> (0 + n <=? 0 + m) = true     *)
    simpl.     (* (n <=? m) = true -> (n <=? m) = true             *)
    intros H.  (* H : (n <=? m) = true                             *)
               (* (n <=? m) = true                                 *)
    apply H.
  -            (* (n <=? m) = true -> (S p + n <=? S p + m) = true *)
    simpl.     (* (n <=? m) = true -> (p + n <=? p + m) = true     *)
    apply IHp.
  Qed.

Theorem S_neqb_0 : forall n:nat,
  (S n) =? 0 = false.
Proof.
  intros n.    (* (S n =? 0) = false *)
  reflexivity.
  Qed.

Theorem mult_1_l : forall n:nat, 1 * n = n.
Proof.
  intros n.
  destruct n.
  -                     (* 1 * 0 = 0       *)
    reflexivity.
  -                     (* 1 * S n = S n   *)
    simpl.              (* S (n + 0) = S n *)
    rewrite -> add_0_r. (* S n = S n       *)
    reflexivity.
  Qed.

Theorem all3_spec : forall b c : bool,
  orb
    (andb b c)
    (orb (negb b)
         (negb c))
  = true.
Proof.
  intros b c.
  destruct b,c.
  -              (* true && true || (negb true || negb true) = true     *)
    reflexivity.
  -              (* true && false || (negb true || negb false) = true   *)
    reflexivity.
  -              (* false && true || (negb false || negb true) = true   *)
    reflexivity.
  -              (* false && false || (negb false || negb false) = true *)
    reflexivity.
  Qed.

Theorem mult_plus_distr_r : forall n m p : nat,
  (n + m) * p = (n * p) + (m * p).
Proof.
  intros n m p.
  induction p as [|p IHp].
  -                       (* (n + m) * 0 = n * 0 + m * 0                         *)
    rewrite -> mul_0_r.   (* 0 = n * 0 + m * 0                                   *)
    rewrite -> mul_0_r.   (* 0 = 0 + m * 0                                       *)
    rewrite -> mul_0_r.   (* 0 = 0 + 0                                           *)
    reflexivity.
  -                       (* IHp : (n + m) * p = n * p + m * p                   *)
                          (* (n + m) * S p = n * S p + m * S p                   *)
    rewrite <- mult_n_Sm. (* (n + m) * p + (n + m) = n * S p + m * S p           *)
    rewrite <- mult_n_Sm. (* (n + m) * p + (n + m) = n * p + n + m * S p         *)
    rewrite <- mult_n_Sm. (* (n + m) * p + (n + m) = n * p + n + (m * p + m)     *)
    rewrite -> IHp.       (* n * p + m * p + (n + m) = n * p + n + (m * p + m)   *)
    assert (n * p + (m * p + (n + m)) = n * p + m * p + (n + m)) as assoc1.
    {
      apply add_assoc.
    }
    rewrite <- assoc1.    (* n * p + (m * p + (n + m)) = n * p + n + (m * p + m) *)
    assert (m * p + (n + m) = n + (m * p + m)) as shuffle. {
      apply add_shuffle3.
    }
    rewrite -> shuffle.   (* n * p + (n + (m * p + m)) = n * p + n + (m * p + m) *)
    rewrite -> add_assoc. (* n * p + n + (m * p + m) = n * p + n + (m * p + m)   *)
    reflexivity.
  Qed.

Theorem mult_assoc : forall n m p : nat,
  n * (m * p) = (n * m) * p.
Proof.
  intros n m p.
  induction m as [|m IHm].
  -                                               (* n * (0 * p) = n * 0 * p                 *)
    rewrite -> mult_0_l.                          (* n * 0 = n * 0 * p                       *)
    rewrite -> mul_0_r.                           (* 0 = 0 * p                               *)
    rewrite -> mult_0_l.                          (* 0 = 0                                   *)
    reflexivity.
  -                                               (* IHm : n * (m * p) = n * m * p           *)
                                                  (* n * (S m * p) = n * S m * p             *)
    replace (S m * p) with (p * S m).             (* n * (p * S m) = n * S m * p             *)
    replace (n * (p * S m)) with ((p * S m) * n). (* p * S m * n = n * S m * p               *)
    rewrite <- mult_n_Sm.                         (* (p * m + p) * n = n * S m * p           *)
    rewrite <- mult_n_Sm.                         (* (p * m + p) * n = (n * m + n) * p       *)
    rewrite -> mult_plus_distr_r.                 (* p * m * n + p * n = (n * m + n) * p     *)
    rewrite -> mult_plus_distr_r.                 (* p * m * n + p * n = n * m * p + n * p   *)
    rewrite <- IHm.                               (* p * m * n + p * n = n * (m * p) + n * p *)
    replace (m * p) with (p * m).                 (* p * m * n + p * n = n * (p * m) + n * p *)
    replace (n * p) with (p * n).                 (* p * m * n + p * n = n * (p * m) + p * n *)
    replace (n * (p * m)) with (p * m * n).       (* p * m * n + p * n = p * m * n + p * n   *)
    reflexivity.
    apply mul_comm.
    apply mul_comm.
    apply mul_comm.
    apply mul_comm.
    apply mul_comm.
   Qed.

(** [] *)

(** **** Exercise: 2 stars, standard, optional (eqb_refl) *)

Theorem eqb_refl : forall n : nat,
  (n =? n) = true.
Proof.
  intros n.
  induction n as [|n IHn].
  -              (* (0 =? 0) = true       *)
    reflexivity.
  -              (* IHn : (n =? n) = true *)
                 (* (S n =? S n) = true   *)
    simpl.       (* (n =? n) = true       *)
    apply IHn.
  Qed.
(** [] *)

(** **** Exercise: 2 stars, standard, optional (add_shuffle3')

    The [replace] tactic allows you to specify a particular subterm to
   rewrite and what you want it rewritten to: [replace (t) with (u)]
   replaces (all copies of) expression [t] in the goal by expression
   [u], and generates [t = u] as an additional subgoal. This is often
   useful when a plain [rewrite] acts on the wrong part of the goal.

   Use the [replace] tactic to do a proof of [add_shuffle3'], just like
   [add_shuffle3] but without needing [assert]. *)

Theorem add_shuffle3' : forall n m p : nat,
  n + (m + p) = m + (n + p).
Proof.
  intros n m p.                 (* n + (m + p) = m + (n + p) *)
  rewrite -> add_assoc.         (* n + m + p = m + (n + p)   *)
  rewrite -> add_assoc.         (* n + m + p = m + n + p     *)
  replace (n + m) with (m + n). (* m + n + p = m + n + p     *)
  reflexivity.
  apply add_comm.
  Qed.
(** [] *)

(** **** Exercise: 3 stars, standard, especially useful (binary_commute)

    Recall the [incr] and [bin_to_nat] functions that you
    wrote for the [binary] exercise in the [Basics] chapter.  Prove
    that the following diagram commutes:

                            incr
              bin ----------------------> bin
               |                           |
    bin_to_nat |                           |  bin_to_nat
               |                           |
               v                           v
              nat ----------------------> nat
                             S

    That is, incrementing a binary number and then converting it to
    a (unary) natural number yields the same result as first converting
    it to a natural number and then incrementing.
    Name your theorem [bin_to_nat_pres_incr] ("pres" for "preserves").

    Before you start working on this exercise, copy the definitions of
    [incr] and [bin_to_nat] from your solution to the [binary]
    exercise here so that this file can be graded on its own.  If you
    want to change your original definitions to make the property
    easier to prove, feel free to do so! *)

(* FILL IN HERE *)

(* Do not modify the following line: *)
Definition manual_grade_for_binary_commute : option (nat*string) := None.
(** [] *)

(** **** Exercise: 5 stars, advanced (binary_inverse)

    This is a further continuation of the previous exercises about
    binary numbers.  You may find you need to go back and change your
    earlier definitions to get things to work here.

    (a) First, write a function to convert natural numbers to binary
        numbers. *)

Fixpoint nat_to_bin (n:nat) : bin
  := match n with
  | O => Z
  | S n => incr (nat_to_bin(n))
  end.

Example test_nat_to_bin_1 : nat_to_bin(0) = Z.
Proof. reflexivity. Qed.

Example test_nat_to_bin_2 : nat_to_bin(1) = B1 Z.
Proof. reflexivity. Qed.

Example test_nat_to_bin_3 : nat_to_bin(2) = B0 (B1 Z).
Proof. reflexivity. Qed.

Example test_nat_to_bin_4 : nat_to_bin(3) = B1 (B1 Z).
Proof. reflexivity. Qed.

(** Prove that, if we start with any [nat], convert it to binary, and
    convert it back, we get the same [nat] we started with.  (Hint: If
    your definition of [nat_to_bin] involved any extra functions, you
    may need to prove a subsidiary lemma showing how such functions
    relate to [nat_to_bin].) *)

Theorem nat_bin_nat : forall n, bin_to_nat (nat_to_bin n) = n.
Proof.
  intros n.
  induction n as [|n IHn].
  -                          (* bin_to_nat (nat_to_bin 0) = 0                                                               *)
    reflexivity.
  -                          (* IHn : bin_to_nat (nat_to_bin n) = n                                                         *)
                             (* bin_to_nat (nat_to_bin (S n)) = S n                                                         *)
    simpl.                   (* bin_to_nat (incr (nat_to_bin n)) = S n                                                      *)
    assert (P: forall b, bin_to_nat(incr (b)) = S (bin_to_nat(b))).
    {
      intros b.
      induction b as [|p|q].
      *                      (* bin_to_nat (incr Z) = S (bin_to_nat Z)                                                      *)
        reflexivity.
      *                      (* IHp : bin_to_nat (incr p) = S (bin_to_nat p)                                                *)
                             (* bin_to_nat (incr (B0 p)) = S (bin_to_nat (B0 p))                                            *)
        simpl.               (* S (bin_to_nat p + (bin_to_nat p + 0)) = S (bin_to_nat p + (bin_to_nat p + 0))               *)
        reflexivity.
      *                      (* IHq : bin_to_nat (incr q) = S (bin_to_nat q)                                                *)
                             (* bin_to_nat (incr (B1 q)) = S (bin_to_nat (B1 q))                                            *)
        simpl.               (* bin_to_nat (incr q) + (bin_to_nat (incr q) + 0) = S (S (bin_to_nat q + (bin_to_nat q + 0))) *)
        rewrite -> IHq.      (* S (bin_to_nat q) + (S (bin_to_nat q) + 0) = S (S (bin_to_nat q + (bin_to_nat q + 0)))       *)
        rewrite -> add_0_r.  (* S (bin_to_nat q) + S (bin_to_nat q) = S (S (bin_to_nat q + (bin_to_nat q + 0)))             *)
        rewrite -> add_0_r.  (* S (bin_to_nat q) + S (bin_to_nat q) = S (S (bin_to_nat q + bin_to_nat q))                   *)
        simpl.               (* S (bin_to_nat q + S (bin_to_nat q)) = S (S (bin_to_nat q + bin_to_nat q))                   *)
        rewrite -> add_comm. (* S (S (bin_to_nat q) + bin_to_nat q) = S (S (bin_to_nat q + bin_to_nat q))                   *)
        simpl.               (* S (S (bin_to_nat q + bin_to_nat q)) = S (S (bin_to_nat q + bin_to_nat q))                   *)
        reflexivity.
    }
    rewrite -> P.            (* S (bin_to_nat (nat_to_bin n)) = S n                                                         *)
    rewrite -> IHn.          (* S n = S n                                                                                   *)
    reflexivity.
  Qed.

(* Do not modify the following line: *)
Definition manual_grade_for_binary_inverse_a : option (nat*string) := None.

(** (b) One might naturally expect that we could also prove the
        opposite direction -- that starting with a binary number,
        converting to a natural, and then back to binary should yield
        the same number we started with.  However, this is not the
        case!  Explain (in a comment) what the problem is. *)

(* FILL IN HERE *)

(* Do not modify the following line: *)
Definition manual_grade_for_binary_inverse_b : option (nat*string) := None.

(** (c) Define a normalization function -- i.e., a function
        [normalize] going directly from [bin] to [bin] (i.e., _not_ by
        converting to [nat] and back) such that, for any binary number
        [b], converting [b] to a natural and then back to binary yields
        [(normalize b)].  Prove it.  (Warning: This part is a bit
        tricky -- you may end up defining several auxiliary lemmas.
        One good way to find out what you need is to start by trying
        to prove the main statement, see where you get stuck, and see
        if you can find a lemma -- perhaps requiring its own inductive
        proof -- that will allow the main proof to make progress.) Don't
        define this using [nat_to_bin] and [bin_to_nat]! *)

Fixpoint normalize (n:bin) : bin :=
  match n with
  | Z => Z
  | B0 b => match normalize(b) with
            | Z => Z
            | B0 c => B0 (B0 c)
            | B1 d => B0 (B1 d)
            end
  | B1 e => B1 (normalize e)
  end.

Example test_normalize_1: normalize(Z) = Z.
Proof. reflexivity. Qed.

Example test_normalize_2: normalize(B0 Z) = Z.
Proof. reflexivity. Qed.

Example test_normalize_3: normalize(B0 (B0 Z)) = Z.
Proof. reflexivity. Qed.

Example test_normalize_4: normalize(B1 Z) = (B1 Z).
Proof. reflexivity. Qed.

Example test_normalize_5: normalize(B0 (B1 Z)) = (B0 (B1 Z)).
Proof. reflexivity. Qed.

Example test_normalize_6: normalize(B1 (B0 Z)) = (B1 Z).
Proof. reflexivity. Qed.

(* These definitions are used to describe property only *)
Definition isNotZ (n:bin) : bool  :=
  match n with
  | Z => false
  | B0 _ => true
  | B1 _ => true
  end.

Fixpoint normalized (n:bin) :bool :=
  match n with
  | Z => true
  | B0 b => (normalized b) && (isNotZ b)
  | B1 e => normalized e
  end.

Theorem normalize_keep_value: forall b, bin_to_nat(b) = bin_to_nat(normalize(b)).
Proof.
  induction b as [|b IHb|b IHb].
  - reflexivity.
  - simpl.
    destruct (normalize b).
    * rewrite -> IHb. reflexivity.
    * rewrite -> IHb. rewrite -> add_0_r. simpl. rewrite -> add_0_r. rewrite -> add_0_r. rewrite -> add_assoc. reflexivity.
    * rewrite -> IHb. rewrite -> add_0_r. simpl. rewrite -> add_0_r. rewrite -> add_0_r. reflexivity.
  - simpl. rewrite -> add_0_r. rewrite -> add_0_r. rewrite <- IHb. reflexivity.
Qed.

Theorem normalize_is_normalized: forall b, normalized(normalize b) = true.
Proof.
  intros b.
  induction b as [|b IHb|b IHb].
  - reflexivity.
  - destruct (normalize b) eqn:D.
    + simpl. rewrite -> D. reflexivity.
    + simpl. rewrite -> D.
      assert (imm: normalized (B0 (B0 b0)) = (normalized (B0 b0) && (isNotZ (B0 b0)))).
      {
        reflexivity.
      }
      rewrite -> imm.
      rewrite -> IHb.
      reflexivity.
    + simpl. rewrite -> D.
      assert (imm: normalized (B0 (B1 b0)) = (normalized (B1 b0) && (isNotZ (B1 b0)))).
      {
        reflexivity.
      }
      rewrite -> imm.
      rewrite -> IHb.
      reflexivity.
  - simpl. apply IHb.
  Qed.

Theorem normalized_normalize: forall b, (normalized b = true) -> b = normalize b.
Proof.
  intros b.
  induction b as [|b IHb|b IHb].
  - reflexivity.
  -
    intros H.
    simpl in H.
    assert (D:normalized b = true).
    {
      destruct (normalized b) eqn:D.
      + reflexivity.
      + simpl in H. inversion H.
    }
    apply IHb in D.
    simpl.
    rewrite <- D.
    destruct b.
    + assert (F:isNotZ Z = true).
      {
        destruct (isNotZ Z) eqn:E.
        * reflexivity.
        * simpl in H. inversion H.
      }
      simpl in F.
      inversion F.
    + reflexivity.
    + reflexivity.
  - simpl. intros H. apply IHb in H. rewrite <- H. reflexivity.
  Qed.

Theorem normalize_is_idempotent: forall b, (normalize b) = normalize(normalize b).
Proof.
  intros b.
  symmetry.
  rewrite <- normalized_normalize.
  reflexivity.
  apply normalize_is_normalized.
  Qed.


Theorem nat_to_bin_b0: forall x, nat_to_bin(S x + S x) = B0 (nat_to_bin (S x)).
Proof.
  intros x.
  induction x as [|x IHx].
  -
    reflexivity.
  -
    replace (S (S x) + S (S x)) with (S(S(S x + S x))).
    replace (nat_to_bin (S (S (S x + S x)))) with (incr (incr (nat_to_bin (S x + S x)))).
    rewrite -> IHx.
    simpl.
    reflexivity.
    reflexivity.
    simpl.
    rewrite -> plus_n_Sm.
    reflexivity.
  Qed.

Theorem normal_b0: forall b, B0 b = normalize(B0 b) -> b = normalize b.
Proof.
  intros b H.
  inversion H.
  destruct (normalize b) eqn:E.
  + discriminate H1.
  + inversion H1. reflexivity.
  + inversion H1. reflexivity.
  Qed.

Theorem normal_b1: forall b, B1 b = normalize(B1 b) -> b = normalize b.
Proof.
  intros b H.
  inversion H.
  rewrite <- H1.
  apply H1.
  Qed.

Theorem zero: forall b, (b = normalize b) -> bin_to_nat(b) = 0 -> b = Z.
Proof.
  intros b H1 H2.
  induction b.
  + reflexivity.
  + pose proof (H1) as H3. apply normal_b0 in H3. simpl in H2. rewrite -> add_0_r in H2. 
    assert (H4: forall x, x + x = 0 -> x = 0).
    {
      destruct x.
      * reflexivity.
      * simpl. intros H. inversion H.
    }
    apply H4 in H2.
    rewrite IHb in H1.
    simpl in H1.
    inversion H1.
    apply H3.
    apply H2.
  + simpl in H2. inversion H2.
  Qed.

Theorem main_normalized: forall b, (b = normalize b) -> nat_to_bin(bin_to_nat(b)) = b.
Proof.
  intros b H.
  (* This hypothesis is specifically designed for the induction later *)
  destruct ((bin_to_nat b) =? 0) eqn:D1.
  + destruct (bin_to_nat b) eqn:D2.
    - apply zero in H. rewrite -> H. reflexivity. apply D2.
    - simpl in D1. inversion D1.
    (* 
       Whenever we do an induction, all the current hypothesis are captured as the induction hypothesis
       If the destruct earlier created an hypothesis that bin_to_nat b = S n, then it will be a required
       precondition of the hypothesis, which is not what we wanted, all we need is non zero.
    *)
  + induction b.
    - simpl in D1. inversion D1.
    - simpl in D1. rewrite -> add_0_r in D1. apply normal_b0 in H.
      assert ((bin_to_nat b =? 0) = false).
      {
        destruct (bin_to_nat b) eqn:D3.
        + simpl in D1. inversion D1.
        + reflexivity.
      }
      simpl. rewrite -> add_0_r. destruct (bin_to_nat b) eqn:D4.
      * simpl in D1. inversion D1.
      * rewrite -> nat_to_bin_b0. rewrite -> IHb. reflexivity. apply H. reflexivity.
    - apply normal_b1 in H. simpl. rewrite -> add_0_r. destruct (bin_to_nat b) eqn:D3.
      * apply zero in D3. rewrite -> D3. reflexivity. apply H.
      * rewrite -> nat_to_bin_b0. rewrite -> IHb. reflexivity. apply H. reflexivity.
  Qed.

(*
   Here is the main theorem.
   The idea of this proof is that we always use normalized representation.
   Therefore, we argue, if the input is not normalized, it wouldn't hurt to first
   normalize it.
*)
Theorem main: forall b, nat_to_bin(bin_to_nat(b)) = normalize(b).
Proof.
  intros b.
  rewrite -> normalize_keep_value.
  apply main_normalized.
  apply normalize_is_idempotent.
  Qed.

(* Do not modify the following line: *)
Definition manual_grade_for_binary_inverse_c : option (nat*string) := None.
(** [] *)

(* 2021-08-11 15:08 *)
