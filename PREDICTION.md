# Prediction sheet — push by 0:20, before you compile

> Marked on **having predicted** and on reconciling it in S3.1 — **not on being
> right.** A confident wrong prediction you then explain is full marks. A blank
> page is none. A page timestamped after your first run is worse than none.
>
> Read `src/given.c` and `BRIEF.md`. Run nothing.

Cores:  4
Lab 0 spread:  27%

> **P1.** `./bar given` on **one** thread — does it come out right? Yes/no, one
> sentence why.

Yes, that ./bar given 1 .., runs properly since the with one thread it is always the last to arrive. So the counter can reset and move forward to the next generation without having to wait for anyone. 

> **P2.** On **8** threads, pick one and commit to it: right answer / wrong
> answer / it stops. If wrong, roughly how big is `bad`? If it stops, say at
> which of the two waits in a round.

It stops since threads are left waiting. Waits at the 2nd wait in a round. 

> **P3.** Three runs at 8 threads — **identical** numbers, or different? Think
> about this one before you write it; it is the most useful line on the page.

Yes, the number are identical, since no matter the order of the threads the number of threads executed is the same, therefore the numbers across runs are the same even if the threads being ran are different.

> **P4.** Seconds, before measuring. Orders of magnitude are what matter. `cpu`
> is process CPU time over all threads, so `cpu`/`time` is how many cores were
> busy — one number per box.

| | 1 thread: time | 8 threads: time | 8 threads: cpu/time |
|---|---|---|---|
| `given` | 0.05 | stops | NA |
| `fixed` | 0.05 | 0.001 | 0.0002 |
| `alt` | 0.05| 0.1 | 0.025 |

> **P5.** Fastest and slowest at 8 threads? Name anything you expect to get
> **slower** as threads are added, and anything you expect to stop altogether.

Fixed will be the fastest at 8 threads. alt is slower as more threadsa re added.
 
