#include "cpu/instr.h"

make_instr_func(neg_rm_v)
{
	int len = 1;
	OPERAND rm;
	rm.data_size = data_size;
	len += modrm_rm(cpu.eip + 1, &rm);
	operand_read(&rm);
	if (rm.val == 0)
		cpu.eflags.CF = 0;
	else
		cpu.eflags.CF = 1;
	rm.val = -rm.val;
	operand_write(&rm);
	print_asm_1("neg", "", len, &rm);
	return len;
}