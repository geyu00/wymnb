#include "memory/cache.h"


void init_cache()
{
	for (int i = 0; i < 128; i++)
		for (int j = 0; j< 8; j++)
			L1_dcache[i][j].valid = 0;
}
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine cache[][8]);
{
	
}
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine cache[][8])
{
	
}

