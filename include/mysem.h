/* COSC 407/507 Lab 2 -- a counting semaphore, built by hand. GIVEN; the
 * declarations are settled, the implementation in src/mysem.c is yours.
 *
 * Nobody ships a hand-rolled semaphore: <semaphore.h> has had one for thirty
 * years. You write one once so that for the rest of the course you know what
 * is underneath omp critical, cudaDeviceSynchronize and every queue you will
 * ever use -- a count, a lock, and a queue of sleeping threads.
 *
 * ---------------------------------------------------------------------------
 * WHAT A COUNTING SEMAPHORE IS
 *
 * An integer you are not allowed to read, with two operations:
 *
 *   wait (P, down, acquire)  if the value is greater than 0, take one and
 *                            carry on. Otherwise sleep until somebody posts.
 *   post (V, up, release)    give one back, and wake a sleeper if there is
 *                            one. NEVER blocks.
 *
 * THE INVARIANT, which your report and your oral both ask you to state:
 *
 *      value >= 0, always, and
 *      value == initial + (number of posts) - (number of completed waits)
 *
 * You cannot read the value, and that is deliberate: by the time you had
 * looked at it, it could have changed. A semaphore is observed by what it
 * permits, not by what it holds. Part A's tests are written that
 * way and so are the oral questions.
 *
 * WHAT IT CAN DO THAT A MUTEX CANNOT. A mutex has one thing to give and it
 * has to be given back by the thread that took it. A semaphore can hold n
 * permits, and a post can come from a thread that never waited -- which is how
 * one thread releases another, and that is what a barrier needs. Be ready for
 * this question out loud.
 *
 * TWO RULES you will be graded on in the lab, not here:
 *   * wait() must re-check its condition in a WHILE loop, never an if. A
 *     condition variable is allowed to wake a thread that nobody woke.
 *   * every path out of wait() and post() must release the lock exactly once.
 * ---------------------------------------------------------------------------
 */
#ifndef COSC407_MYSEM_H
#define COSC407_MYSEM_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t lock;   /* protects value                              */
    pthread_cond_t  cv;     /* threads asleep because value was 0          */
    int             value;  /* the permits available; never negative       */
} my_sem_t;

/* 0 on success, non-zero on failure. `initial` must be >= 0. */
int  my_sem_init(my_sem_t *s, int initial);

/* Take one permit, blocking until there is one to take. */
void my_sem_wait(my_sem_t *s);

/* Give one permit back. Never blocks, and may be called by any thread. */
void my_sem_post(my_sem_t *s);

/* Release what init took. No thread may be blocked on it. */
int  my_sem_destroy(my_sem_t *s);

#endif /* COSC407_MYSEM_H */
