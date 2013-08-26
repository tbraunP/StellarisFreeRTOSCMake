#include "syscalls/syscalls.h"
#include "FreeRTOS.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>

size_t __malloc_margin = 256;
char *__brkval;

void _exit(int code) {
	for (;;)
		;
}

ssize_t _read_r(struct _reent *r, int fd, void *ptr, size_t len) {
	//return uart_read_r(r, fd, ptr, len);
    return 0;
}

ssize_t _write_r(struct _reent *r, int fd, const void *ptr, size_t len) {
	//return uart_write_r(r, fd, ptr, len);
    return 0;
}

off_t _lseek_r(struct _reent *r, int fd, _off_t ptr, int dir) {
	fd = fd;
	ptr = ptr;
	dir = dir;
	r->_errno = ENOSYS;
	return -1;
}

int _close_r(struct _reent *r, int fd) {
	fd = fd;
	r->_errno = ENOSYS;
	return -1;
}

int _fstat_r(struct _reent *r, int fd, struct stat *st) {
	if ((fd >= STDIN_FILENO) && (fd <= STDERR_FILENO)) {
		st->st_mode = S_IFCHR;
		return 0;
	}

	r->_errno = ENOSYS;
	return -1;
}

int _isatty_r(struct _reent *r, int fd) {
	r = r;
	fd = fd;
	return 1;
}

void *_sbrk_r(struct _reent *r, ptrdiff_t incr) {
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL ) {
		// start address of heap
		heap = (unsigned char *) &__heap_start__;
	}
	prev_heap = heap;

	if ((heap + incr) >= (unsigned char *) &__heap_end__) {
		return 0;
	}
	heap += incr;
	return (void *) prev_heap;
}

int _kill_r(struct _reent *r, int pid, int signal) {
	r->_errno = ENOSYS;
	return -1;
}

int _getpid_r(struct _reent *r) {
	r->_errno = ENOSYS;
	return -1;
}

void __malloc_lock(struct _reent *r) {
	vPortEnterCritical();
}

void __malloc_unlock(struct _reent *r) {
	vPortExitCritical();
}
