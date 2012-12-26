/**
 ******************************************************************************
 * @file      	LibraryHacks.cpp
 * @author    	Tecnologic86
 * @version   	V0.0.0
 * @date      	03.07.2012 15:08:09
 * @project	ThunderCryer
 ******************************************************************************
 */
#ifdef __cplusplus

#include <cstdlib>
#include <sys/types.h>
#include <stdlib.h>

/*
 * LibraryHacks.cpp  *
 *  Created on: 23 Jan 2011  *
 *  Author: Andy  */

/*
 * The default pulls in 70K of garbage
 */
namespace __gnu_cxx {
	void __verbose_terminate_handler() {
		for(;;);
	}
}

#ifdef __cplusplus
extern "C" {
#endif
	/*
	 * The default pulls in about 12K of garbage
	 */
	void __cxa_pure_virtual() {
		for(;;);
	}
#ifdef __cplusplus
}
#endif


	/*
	 * Implement C++ new/delete operators using the heap
	 */
	void *operator new(size_t size) {
		return malloc(size);
	}

	void *operator new[](size_t size) {
		return malloc(size);
	}

	void operator delete(void *p) {
		free(p);
	}

	void operator delete[](void *p) {
		free(p);
	}
#endif
