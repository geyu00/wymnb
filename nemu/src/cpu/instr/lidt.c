#include "cpu/instr.h"

make_instr_func(lidt)
{
	int len = 1;
	OPERAND rm;
	len += modrm_rm(eip + 1, &rm);
	rm.data_size = 16;
	operand_read(&rm);
	cpu.idtr.limit = rm.val;


	rm.data_size = 32;
	rm.addr += 2;
	operand_read(&rm);
	cpu.idtr.base = rm.val;

	return len;
}