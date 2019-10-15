#include "cpu/instr.h"

make_instr_func(leave)
{
	cpu.esp = cpu.ebp;
	OPERAND rel;
	rel.type = OPR_MEM;
	rel.data_size = data_size;
	rel.addr = cpu.esp;
	operand_read(&rel);
	cpu.ebp = rel.val;
	cpu.esp += data_size / 8;
	print_asm_0("leave", "",1);
	return 1;
}