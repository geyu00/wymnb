#ifndef __CACHE_H__
#define __CACHE_H__

typedef struct {
	
} CacheLine;

void init_cache();
uint32_t cache_read(paddr_t paddr, size_t len, CacheLine * cache);
void cache_write(paddr_t paddr, size_t len, uint32_t data, CacheLine * cache);