/* Lab 2, core B -- YOUR MINIMAL CORRECTION.
 *
 * wait_() must work every time it is called, at 1, 2, 4 and 8 threads, and at
 * more threads than this machine has cores. It is already correct at two, and
 * that tells you nothing.
 *
 * Your fix here is very small. That is not a reason to write less in S2.3:
 * "minimal" has to be argued, and the argument is the invariant -- state it,
 * then show your version keeps it for every thread and not just for one.
 *
 * Copy anything you like out of given.c. Do not edit it.
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "barrier.h"

/* TODO: the barrier's state. What has to be shared between the threads, and
 *       what does each thread have to remember for itself? */
typedef struct {
    pthread_mutex_t lock;
    pthread_cond_t  cv;
    int             n;
    int             count;
    unsigned long   gen;
} bar_t;
static void *create(int nthreads)
{
    /* TODO: allocate it, initialise everything, and return it. Anything a
     *       thread might lock or wait on has to be ready BEFORE the first
     *       thread can reach it. */
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

    b->n = nthreads;
    b->count = 0;
    b->gen = 0;

    return b;
}

static void wait_(void *p)
{
    /* TODO: the barrier. Write the invariant you are keeping in a comment
     *       above it, in one line, before you write the code -- your report
     *       and your oral both ask you to state it. */
        bar_t *b = (bar_t *)p;

    /* Invariant: a thread leaves only when the generation changes. */

    pthread_mutex_lock(&b->lock);

    unsigned long mine = b->gen;

    b->count++;

    if (b->count == b->n) {
        b->count = 0;
        b->gen++;

        /* Wake all threads waiting for this generation. */
        pthread_cond_broadcast(&b->cv);
    } else {
        while (b->gen == mine) {
            pthread_cond_wait(&b->cv, &b->lock);
        }
    }

    pthread_mutex_unlock(&b->lock);
}

static void destroy(void *p)
{
    /* TODO: release what create() took. Every thread has been joined by the
     *       time this is called. */
    bar_t *b = (bar_t *)p;

    pthread_mutex_destroy(&b->lock);
    pthread_cond_destroy(&b->cv);
    free(b);
}

const bar_ops_t bar_fixed = { "fixed", create, wait_, destroy };
