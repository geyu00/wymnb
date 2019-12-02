#include "cpu/instr.h"

make_instr_func(lgdt)
{
	int len = 1;
	OPERAND rm;
	len += modrm_rm(eip + 1, &rm);
	rm.data_size = 16;
	operand_read(&rm);
	cpu.gdtr.limit = rm.val;


	rm.data_size = 32;
	rm.addr += 2;
	operand_read(&rm);
	cpu.gdtr.base = rm.val;

	return len;
}
