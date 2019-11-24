#include "memory/cache.h"


void init_cache()
{
	for (int i = 0; i < 128; i++)
		for (int j = 0; j< 8; j++)
			L1_dcache[i][j].valid = 0;
}
uint32_t cache_read(paddr_t paddr, size_t len)
{
	uint32_t res = 0;
	for (int i = len - 1; i >= 0; i--)
	{
		res <<= 8;
		uint32_t tag = ((paddr + i) >> 13) & 0x7ffff;
		uint32_t cache_number = ((paddr + i) >> 6) & 0x7f;
		uint32_t block_addr = (paddr + i) & 0x3f;
		bool exist = false;
		for (int j = 0; j < 8; i++)
		{
			if (L1_dcache[cache_number][j].valid && L1_dcache[cache_number][j].tag == tag)
			{
				exist = true;
				res += L1_dcache[cache_number][j].data[block_addr];
				break;
			}
		}
		if (!exist)
		{
			int j;
			for (j = 0; j < 8; j++)
				if (!L1_dcache[cache_number][j].valid)
					break;
			if (j ==  8)
				j = rand() % 8;
			memcpy(L1_dcache[cache_number][j].data, hw_mem + ((paddr + i) & (~0x3f)), 64);
			L1_dcache[cache_number][j].valid = 1;
			L1_dcache[cache_number][j].tag = tag;
			res += L1_dcache[cache_number][j].data[block_addr];;
		}
	}
	return res;
}
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
	uint32_t tag = (paddr >> 13) & 0x7ffff;
	uint32_t cache_number = (paddr >> 6) & 0x7f;
	uint32_t block_addr = paddr & 0x3f;
	bool exist = false;
	for (int i = 0; i < 8; i++)
	{
		if (L1_dcache[cache_number][i].valid && L1_dcache[cache_number][i].tag == tag)
		{
			memcpy(hw_mem + paddr, &data, len);
			memcpy(&L1_dcache[cache_number][i].data[block_addr], &data, len);
			exist = true;
			break;
		}
	}
	if (!exist)
		memcpy(hw_mem + paddr, &data, len);
}

