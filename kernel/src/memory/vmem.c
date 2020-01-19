#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)
#define NR_PT ((SCR_SIZE + PT_SIZE - 1) / PT_SIZE) // number of page tables to cover the vmem

//PDE vpdir[NR_PDE] align_to_page;
PTE vptable[NR_PT] align_to_page; // kernel page tables

PDE *get_updir();

void create_video_mapping()
{

	/* TODO: create an identical mapping from virtual memory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memeory area
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
	 */
	//panic("please implement me");

	/*PDE *dir = get_updir();
	uint32_t* temp= va_to_pa(vptable);
	dir->val = make_pde(temp);
	for(int i = 0; i < (PAGE_SIZE + SCR_SIZE -1) / PAGE_SIZE ; i++)
	{
		vptable[VMEM_ADDR / PAGE_SIZE + i].val = make_pte(VMEM_ADDR + i* PAGE_SIZE);
	}*/
	PDE *pdir = get_updir();
	PTE *ptable = (PTE*)va_to_pa(vptable);
	uint32_t pdir_idx, ptable_idx, pframe_idx;
	pframe_idx = 0;
	for(pdir_idx = 0; pdir_idx < 2; pdir_idx ++){
		if(pframe_idx > 0xaf){break;}
		pdir[pdir_idx].val = make_pde(ptable);
		for(ptable_idx = 0; ptable_idx < NR_PTE; ptable_idx++){
			if(pframe_idx > 0xaf){
				break;
			}
			if(pframe_idx < 0xa0){
				ptable->val = 0;
			}
			else{
				ptable->val = make_pte(pframe_idx << 12);
			}
			pframe_idx ++;
			ptable ++;
		}
	}
	/*PDE *pdir = get_updir();
	PTE *ptable = (PTE*)va_to_pa(vptable);
	uint32_t pdir_idx, ptable_idx, pframe_idx;
	pdir_idx = 0;
	pframe_idx = 0;
	pdir[pdir_idx].val = make_pde(ptable);
	pdir[pdir_idx + KOFFSET / PT_SIZE].val = make_pde(ptable);
	for(ptable_idx = 0; ptable_idx < NR_PTE; ptable_idx++)
	{
		ptable->val = make_pte(pframe_idx << 12);
		pframe_idx++;
		ptable++;
	}*/
}

void video_mapping_write_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		buf[i] = i;
	}
}

void video_mapping_read_test()
{
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for (i = 0; i < SCR_SIZE / 4; i++)
	{
		assert(buf[i] == i);
	}
}

void video_mapping_clear()
{
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}
