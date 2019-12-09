#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
	uint32_t dir = laddr >> 22;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
	uint32_t mulu = (dir << 2) + (cpu.cr3.pdbr << 12);
	uint32_t base = (paddr_read(mulu, 4) & 0xfffff000) + (page << 2);
	return ((paddr_read(base, 4) & 0xfffff000) | offset);
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
	;
#endif
}
