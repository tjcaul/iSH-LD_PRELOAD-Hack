/* qemu-hack.c
 * Library used with LD_PRELOAD to stub certain syscalls
 * in qemu-system-i386 on iSH.
 * The stubs are mostly non-functional and return -1 (failure),
 * so qemu might not work properly when using this library.
 *
 * Compile with
 *     gcc -O2 -shared -o /usr/local/lib/qemu-hack.so qemu-hack.c
 */

#include <dlfcn.h>
#include <stdio.h>
#include <sys/syscall.h>

#ifndef DEBUG //Compile with -DDEBUG to get flooded with messages from every stubbed syscall
#define printf(fmt, ...) printf("") //This could be improved to avoid *any* I/O
#endif

long syscall (long number, long a, long b, long c, long d, long e, long f) {
	long (*orig)(long number, ...);
	orig = dlsym(RTLD_NEXT, "syscall");
	if (!orig)
		printf("dlsym(\"syscall\") failed\n");
	printf("syscall(%ld, ...)", number);

	if (number == SYS_seccomp) {
		printf(" [INTERCEPTED]\n");
		return -1;
	} else if (number == SYS_io_uring_setup) {
		printf(" [INTERCEPTED]\n");
		printf("  io_uring_setup(%u, %p)\n", (unsigned int)a, (void *)b);
		return -1;
	} else {
		printf("\n");
		/* For a syscall with e.g. 3 arguments, d, e, and f will be garbage,
		 * but the syscall will ignore them anyways.
		 */
		return orig(number, a, b, c, d, e, f);
	}
}

long get_mempolicy (int *mode, unsigned long *nodemask, unsigned long maxnode, void *addr, unsigned long flags) {
	printf("get_mempolicy(%p, %p, %p, %lu)\n", mode, nodemask, addr, flags);
	if (flags != 0)
		printf("get_mempolicy called with flags != 0\n");
	//Weak attempt at simulating the output of a real get_mempolicy
	*mode = 0;
	*nodemask = 0;
	return -1;
}

long set_mempolicy (int mode, const unsigned long *nodemask, unsigned long maxnode) {
	printf("set_mempolicy(%d, *%p = %lu, %lu)\n", mode, nodemask, *nodemask, maxnode);
	return -1;
}

int memfd_create (const char *name, unsigned int flags) {
	printf("memfd_create(%s, %u)\n", name, flags);
	return -1;
}

int signalfd (int fd, const void *mask, int flags) {
	printf("signalfd(%d, %p, %d)\n", fd, mask, flags);
	return -1;
}
