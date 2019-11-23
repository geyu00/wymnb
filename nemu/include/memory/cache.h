#ifndef __CACHE_H__
#define __CACHE_H__

typedef struct {
	uint32_t valid:1;
	uint32_t tag:19;
	uint8_t data[64];
} CacheLine;

CacheLine L1_dcache[128][8];

void init_cache();
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine CacheLine cache[][8]);
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine cache[][8]);

#endif