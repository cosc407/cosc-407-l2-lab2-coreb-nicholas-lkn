# Lab 2 results — sealed core

Name:  Nicholas Lookkin
Student number:  40126518
Lab section:  L02
Core:  B
Machine:  Codespaces
Cores:  4

## Tools and sources

Tools and sources: Lecture Notes, textbook

Three or more runs of `./bar given`, including one thread:

```
./bar given 1 3000
mode=given threads=1 rounds=3000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0025 cpu=0.0025

./bar given 2 600
mode=given threads=2 rounds=600 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0155 cpu=0.0159

./bar given 4 600
mode=given threads=4 rounds=600 bad=-1 firstbad=-1 checksum=unknown correct=no deadlock=yes time=5.0092 cpu=0.0024
given: no progress after 5.0 s -- giving up. This is a result, not a crash: paste the line above.
```

**S2.1** Name the mechanism: which claim in `given.c`'s header is false, and
what is actually happening? State the barrier's invariant and say which half of
it this code does not keep.

The Claim "Every thread waiting on that condition variable is waiting for the same thing... so all of them are released." The first half is true, since they all wait for gen != mine, and the generation does change before the signal. The conclusion doesn't follow, because pthread_cond_signal wakes at least one waiter, not all of them.

**S2.2** Prove it, in the form your `BRIEF.md` requires. 

(1)
Works for n = 1 and 2. With two threads, the first thread waits and the second thread signals it, so both continue. With one thread, there is nobody waiting. Since signal is enough when there is only one waiter, these tests cannot show the bug.

(2)
Smallest failing size: n = 3. With 3 threads, two threads are waiting when the third arrives. signal only wakes one of them, so the other stays stuck forever. The test confirms this: deadlock=yes after 5 seconds with almost no CPU use. Therefore, the barrier fails at n = 3.

(3)
The threads are stuck, not spinning. For n = 3, the program ran for 5.0089 seconds but only used 0.0022 seconds of CPU time. This means the threads did their work and then waited. This is a lost wake-up, not a livelock. The fix is to use pthread_cond_broadcast instead of pthread_cond_signal.

**S2.3** Minimality: what breaks if you do less, what it costs if you do more.

Exchanging signal with broadcast is sufficient because it wakes all threads waiting for the current generation, while the while loop ensures they only leave after the generation changes; using signal is not enough for 3+ threads, while doing more than broadcast is unnecessary

## S3 — the measurement · 30 marks

`./bar all <t> <rounds>` at 1, 2, 4 and 8 threads. Pasted, not retyped. If a
mode stops, `all` stops with it — run the modes one at a time and paste those.

```
./bar all 1 2000
mode=given threads=1 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0016 cpu=0.0017
mode=fixed threads=1 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0016 cpu=0.0017
mode=alt threads=1 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0020 cpu=0.0020

./bar all 2 2000
mode=given threads=2 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0533 cpu=0.0541
mode=fixed threads=2 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0486 cpu=0.0440
mode=alt threads=2 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0930 cpu=0.0879

./bar all 4 2000
mode=given threads=4 rounds=2000 bad=-1 firstbad=-1 checksum=unknown correct=no deadlock=yes time=5.0085 cpu=0.0024
mode=fixed threads=4 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.0898 cpu=0.1453
mode=alt threads=4 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.2419 cpu=0.3426

/bar all 8 2000
mode=given threads=8 rounds=2000 bad=-1 firstbad=-1 checksum=unknown correct=no deadlock=yes time=5.0093 cpu=0.0026
mode=fixed threads=8 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.1905 cpu=0.3610
mode=alt threads=8 rounds=2000 bad=0 firstbad=-1 checksum=ok correct=yes deadlock=no time=0.6544 cpu=0.8652

```

| threads | given: correct? | given: time | given: cpu | fixed: time | fixed: cpu | alt: time | alt: cpu |
|---|---|---|---|---|---|---|---|
| 1 | yes | 0.0016 | 0.0017 | 0.0016 | 0.0017  | 0.0020 | 0.0020 |
| 2 | yes | 0.0533 | 0.0541 | 0.0486 | 0.0440 | 0.0930 | 0.0879 |
| 4 | no | 5.0085 | 0.0024 | 0.0898| 0.1453 | 0.2419 | 0.3426 |
| 8 | no | 5.0093 | 0.0026 | 0.1905 | 0.3610 | 0.6544 | 0.8652 |

**S3.1** Reconcile with `PREDICTION.md`: quote what you predicted, say what
happened, account for the difference. If you were right, say what would have
made you wrong.

P1: I predicted that given would work with one thread because the only thread is always the last to arrive. This happened as predicted, with correct=yes and no deadlock.

P2: I predicted that given would stop with 8 threads because some threads would be left waiting. This happened as predicted, with deadlock=yes after about 5 seconds.

P3: I predicted that three 8-thread runs would have identical numbers because the same number of threads run each round. This was not tested with the results shown, since only one 8-thread run is provided.

P4: I predicted that given would use very little CPU when it gets stuck. This happened: at 8 threads, it ran for 5.0093 seconds but only used 0.0026 seconds of CPU. fixed and alt both completed correctly, but their times increased as the number of threads increased.

P5: I predicted that fixed would be fastest at 8 threads and that alt would get slower as more threads were added. Both predictions were correct. At 8 threads, fixed took 0.1905 seconds, while alt took 0.6544 seconds. given stopped instead of completing.

**S3.2** Which would you ship on this machine, **and what measurement would
change your mind?**

I would ship fixed because it is correct and faster than alt on this machine. I would change my choice if alt became faster than fixed while still remaining correct.

## S4 — explain-back · 15 marks

> Two or three sentences, your own words: someone who has not seen this code
> asks *what was wrong with it, and what did fixing it cost?*

The barrier only woke up one waiting thread. So if there were 3 or more threads waiting, threads could get stuck forever. Fixing it with pthread_cond_broadcast wakes everyone who is waiting, but it adds a small amount of extra work because all waiting threads are woken up.

## Anything you got stuck on

Optional. One or two lines.
