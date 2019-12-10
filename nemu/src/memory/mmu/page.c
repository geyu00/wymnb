#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	//printf("\nPlease implement page_translate()\n");
	//assert(0);
	//printf("23!!!!!");
	//BREAK_POINT;
	uint32_t dir = laddr >> 22;
	uint32_t page = (laddr >> 12) & 0x3ff;
	uint32_t offset = laddr & 0xfff;
	uint32_t page_addr = (dir << 2) + (cpu.cr3.pdbr << 12);
	PDE t_pde;
	t_pde.val = paddr_read(page_addr, 4);
	assert(t_pde.present == 1);
	uint32_t base = (t_pde.page_frame << 12) + (page << 2);
	PTE t_pte;
	t_pde.val = paddr_read(base, 4);
	assert(t_pte.present == 1);
	return ((t_pde.page_frame << 12) + offset);
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
	;
#endif
}
