#ifndef	_MM_H
#define	_MM_H

#include "peripherals/base.h"

#define PAGE_SHIFT	 	    12
#define TABLE_SHIFT 		9
#define SECTION_SHIFT		(PAGE_SHIFT + TABLE_SHIFT)

#define PAGE_SIZE   		(1 << PAGE_SHIFT)	
#define SECTION_SIZE		(1 << SECTION_SHIFT)	
#define STACK_SPACE         (4 * PAGE_SIZE)

#define LOW_MEMORY          (2 * SECTION_SIZE)
#define HIGH_MEMORY         PBASE

#define PAGING_MEMORY       (HIGH_MEMORY - LOW_MEMORY)
#define PAGING_PAGES        (PAGING_MEMORY/PAGE_SIZE)

#ifndef __ASSEMBLER__

unsigned long get_free_page();
void free_page(unsigned long p);
void memzero(unsigned long src, unsigned long n);
void memcpy(unsigned long src, unsigned long dst, unsigned long n);

#endif

#endif  /*_MM_H */