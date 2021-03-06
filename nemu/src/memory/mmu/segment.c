#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	return offset + cpu.segReg[sreg].base;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	uint32_t addr = cpu.gdtr.base + 8 * cpu.segReg[sreg].index;
	SegDesc seg;
	seg.val[0] = laddr_read(addr, 4);
	seg.val[1] = laddr_read(addr + 4, 4);
	cpu.segReg[sreg].base = seg.base_15_0 | (seg.base_23_16 << 16) | (seg.base_31_24 << 24);
	cpu.segReg[sreg].limit = seg.limit_15_0 | (seg.limit_19_16 << 16);
	cpu.segReg[sreg].type = seg.type;
	cpu.segReg[sreg].privilege_level = seg.privilege_level;
	cpu.segReg[sreg].soft_use = seg.soft_use;
}
