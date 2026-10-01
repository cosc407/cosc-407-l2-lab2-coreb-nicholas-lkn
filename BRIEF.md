# Sealed core **B**

Read `lab2-core.md` first. Other sections were given a different core.

```sh
make
./bar given 1 2000         # always start with one thread
./bar given 2 2000         # then two. Do this one before you read further.
./bar given 8 2000
```

**The alternative** (`src/alt.c`): the same barrier built out of **counting
semaphores**, as two turnstiles — the shape class 6 derives and Downey's *Little
Book of Semaphores* §3.6–3.7 calls a reusable barrier. Arrive, count, and when
the last one arrives post the first turnstile `n` times; then leave, count down,
and when the last one leaves post the second turnstile `n` times. It is correct.

`src/mysem_ref.c` is a **working counting semaphore, given** — the Part A
exercise, handed to you because the comparison here is between two *barriers*
and the semaphore underneath is not what is being marked. If your own Part A
version works, copying it over that file at the end is a good ten minutes: it is
worth no marks and it will tell you something.

The question it answers: **releasing threads one at a time is not the bug.** It
is a design, it is what your `given` was reaching for, and it can be made
correct. What it costs is a measurement, not an opinion.

**Your four questions, for this core**

- **S2.1 mechanism.** Which claim in the header is false? Not "it deadlocks" —
  say how many threads are released by the call the author chose, how many
  needed to be, and what the ones that were not released are waiting for. The
  phrase you need is in class 6 and in Pacheco §4.7.
- **S2.2 proof.** An argument, not a timing table, and it has three parts. (1)
  It is **correct at two threads** — say why, exactly, and why that makes "I
  tested it" worthless here. (2) The smallest number of threads at which it
  stops, predicted before you run it and then run. (3) `cpu` on the run that
  stops: state what the threads are doing and how you know from that one
  number.
- **S2.3 minimality.** Your fix is one word. Prove that it is *sufficient*:
  state the barrier's invariant and show your version keeps it. Then say what a
  barrier that really does release one thread at a time has to do instead —
  your `alt` is one of the two ways, name the other.
- **S3.2 ship it.** `fixed` and `alt` are both correct and will come out close
  on wall-clock time. Look at `cpu` as well as `time`, then choose, **and say
  what measurement would change your mind.** No second half, half the marks.

**One thing that will not help you.** ThreadSanitizer, from class 5, reports
data races. Nothing here is a data race. Knowing why a sanitizer is silent on
this defect is worth a sentence in S2.2.

**One question you may get in the oral.** *This barrier is correct with two
threads and stops with four. What does the third thread change?*
