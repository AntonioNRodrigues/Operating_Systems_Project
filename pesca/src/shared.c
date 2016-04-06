#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "shared.h"

void *sharedInit (const char *name, size_t size)
{
	void *result;
	int ret;
	int fd;
	fd = shm_open (name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror (name);
		exit (1);
	}
	ret = ftruncate (fd, size);
	if (ret == -1) {
		perror (name);
		exit (1);
	}
	result = mmap (0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (result == MAP_FAILED) {
		perror (name);
		exit (1);
	}
	return result;
}

void *sharedAttach (const char *name, size_t size)
{
	void *result;
	int ret;
	int fd;
	fd = shm_open (name, O_RDWR, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror (name);
		exit (1);
	}
	ret = ftruncate (fd, size);
	if (ret == -1) {
		perror (name);
		exit (1);
	}
	result = mmap (0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (result == MAP_FAILED) {
		perror (name);
		exit (1);
	}
	return result;
}

void sharedDestroy (const char *name, void *ptr, size_t size)
{
	int ret;
	ret = munmap (ptr, size);
	if (ret == -1) {
		perror (name);
		exit (1);
	}
	ret = shm_unlink (name);
	if (ret == -1) {
		perror (name);
		exit (1);
	}
}
