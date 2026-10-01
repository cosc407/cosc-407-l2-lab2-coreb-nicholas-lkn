/* COSC 407/507 Lab 2, core B -- the barrier you were handed.
 *
 * *** DO NOT EDIT. *** It is hashed by the autograder, and your report has to
 * compare against the code you were handed. Work in fixed.c and alt.c.
 *
 * ---------------------------------------------------------------------------
 * What the author believed, in their own words:
 *
 *   "The counter cannot also be the wake-up condition, because it has to be
 *    reset for the next round and a thread that wakes up late would see the
 *    reset value and wait forever. So this barrier keeps a generation number
 *    as well: a waiting thread waits for the generation to move on, which it
 *    does exactly once per round, and the predicate is checked in a loop so a
 *    spurious wake-up costs nothing.
 *
 *    The last thread to arrive re-arms the counter, bumps the generation and
 *    signals the condition variable. Every thread waiting on that condition
 *    variable is waiting for the same thing and the generation has already
 *    changed before the signal goes out, so all of them are released. I ran it
 *    with one thread and with two; it is exactly right and it is reusable."
 *
 * Exactly one of the claims in that paragraph is false.
 * ---------------------------------------------------------------------------
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "barrier.h"

typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t  cv;
    int             n;        /* how many threads have to arrive */
    int             count;    /* how many have arrived this round */
    unsigned long   gen;      /* which round this barrier is on   */
} bar_t;

static void *create(int nthreads)
{
    bar_t *b = malloc(sizeof *b);
    if (b == NULL) {
        return NULL;
    }
    if (pthread_mutex_init(&b->lock, NULL) != 0 ||
        pthread_cond_init(&b->cv, NULL) != 0) {
        fprintf(stderr, "barrier init failed\n");
        free(b);
        return NULL;
    }
    b->n     = nthreads;
    b->count = 0;
    b->gen   = 0;
    return b;
}

static void wait_(void *p)
{
    bar_t *b = (bar_t *)p;

    pthread_mutex_lock(&b->lock);

    unsigned long mine = b->gen;      /* the round I am waiting to leave */

    b->count++;
    if (b->count == b->n) {
        b->count = 0;                 /* re-arm for the next round */
        b->gen++;                     /* this round is over        */
        pthread_cond_signal(&b->cv);
    } else {
        while (b->gen == mine) {
            pthread_cond_wait(&b->cv, &b->lock);
        }
    }

    pthread_mutex_unlock(&b->lock);
}

static void destroy(void *p)
{
    bar_t *b = (bar_t *)p;
    pthread_mutex_destroy(&b->lock);
    pthread_cond_destroy(&b->cv);
    free(b);
}

const bar_ops_t bar_given = { "given", create, wait_, destroy };
