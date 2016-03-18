#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include "semaphore.h"

sem_t *semInit (const char *name, int value)
{
	sem_t *result;
	result = sem_open (name, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, value);
	if (result == SEM_FAILED) {
		perror (name);
		exit (1);
	}
	return result;
}

sem_t *semAccess (const char *name)
{
	sem_t *result;
	result = sem_open (name, 0);
	if (result == SEM_FAILED) {
		perror (name);
		exit (1);
	}
	return result;
}

void semDestroy (const char *name, sem_t *sem)
{
	int ret;
	if ((ret = sem_close (sem)) == -1) {
		perror (name);
		exit (1);
	}
	if ((ret = sem_unlink (name)) == -1) {
		perror (name);
		exit (1);
	}
}

 void semUp (const char *name, sem_t *sem)
{
	int ret;
	if ((ret = sem_post (sem)) == -1) {
		perror (name);
		exit (1);
	}
}

void semDown (const char *name, sem_t *sem)
{
	int ret;
	do {
		ret = sem_wait (sem);
	} while (ret == -1 && errno == EINTR);
	if (ret == -1) {
		perror (name);
		exit (1);
	}
}
