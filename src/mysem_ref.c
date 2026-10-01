/* Lab 2, core B -- a counting semaphore. GIVEN and complete; nothing to
 * change, and no marks in this file.
 *
 * This is the reference version of the Part A exercise, handed to you because
 * your alternative in BRIEF.md is built out of counting semaphores and the
 * point of that comparison is the BARRIER, not the semaphore underneath it.
 * Part A carried no marks and it is behind you.
 *
 * If your own Part A semaphore works, swapping it in is a good ten minutes at
 * the end -- copy it over this file, rebuild, and say in RESULTS.md whether
 * any number moved. It is worth no marks and it will tell you something.
 */
#include <pthread.h>

#include "mysem.h"

int my_sem_init(my_sem_t *s, int initial)
{
    if (initial < 0) {
        return -1;
    }
    s->value = initial;
    if (pthread_mutex_init(&s->lock, NULL) != 0) {
        return -1;
    }
    if (pthread_cond_init(&s->cv, NULL) != 0) {
        pthread_mutex_destroy(&s->lock);
        return -1;
    }
    return 0;
}

void my_sem_wait(my_sem_t *s)
{
    pthread_mutex_lock(&s->lock);
    /* while, not if: a condition variable may wake a thread nobody woke, and
     * another thread may have taken the permit between the wake-up and the
     * re-acquisition of the lock. */
    while (s->value == 0) {
        pthread_cond_wait(&s->cv, &s->lock);
    }
    s->value--;
    pthread_mutex_unlock(&s->lock);
}

void my_sem_post(my_sem_t *s)
{
    pthread_mutex_lock(&s->lock);
    s->value++;
    /* One permit given back releases at most one waiter, so signal is right
     * here -- and unlike the barrier in given.c, that is not a bug: every
     * waiter wants the same one thing, and the next post wakes the next one. */
    pthread_cond_signal(&s->cv);
    pthread_mutex_unlock(&s->lock);
}

int my_sem_destroy(my_sem_t *s)
{
    int a = pthread_cond_destroy(&s->cv);
    int b = pthread_mutex_destroy(&s->lock);
    return (a != 0) ? a : b;
}
