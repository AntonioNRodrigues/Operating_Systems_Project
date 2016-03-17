/**
 * Encapsulates the POSIX shared object API.
 *
 * @author Pedro Mariano plmariano@ciencias.ulisboa.pt
 */

#ifndef __SHARED__
#define __SHARED__

/**
 * Creates a shared object with the given name and size.
 *
 * The function fails if the shared object already exists.
 *
 * If there is an error, it is printed to the standard error and the
 * process terminates.
 *
 * It is an error if there is already a shared object with the given name.
 * Other sources of error may occur when mapping the shared memory object
 * to the virtual address space of the process.
 *
 * @return a pointer to the shared object.
 */
void *sharedInit (const char *name, size_t size);

/**
 * Attach to the current process a shared object with the given name and
 * size.
 *
 * If there is an error, it is printed to the standard error and the
 * process terminates.
 *
 * It is an error if the shared object does not exist.
 *
 * @return a pointer to the shared object.
 */
void *sharedAttach (const char *name, size_t size);

/**
 * Destroys the given shared object.
 */
void sharedDestroy (const char *name, void *ptr, size_t size);

#endif
