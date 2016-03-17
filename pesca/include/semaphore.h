/**
 * Encapsulates the POSIX semaphore API.
 *
 * Pedro Mariano plmariano@ciencias.ulisboa.pt
 */
#ifndef __SEMAPHORE__
#define __SEMAPHORE__

#include <semaphore.h>

/**
 * Creates a semaphore with the given name and initial value.
 * @return a pointer to the shared semaphore.
 */
sem_t *semInit (const char *name, int value);

/**
 * Access an existent semaphore.  Aborts if no semaphore exists.
 * @return a pointer to the shared semaphore.
 */
sem_t *semAccess (const char *name);

/**
 * Destroys a semaphore with the given name.
 */
void semDestroy (const char *name, sem_t *sem);

/**
 * Increments the given semaphore.  Calls function semPost().
 */
void semUp (const char *name, sem_t *sem);

/**
 * Decrements the given semaphore. Calls function semWait().
 */
void semDown (const char *name, sem_t *sem);

#endif
