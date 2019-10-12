#include "cpu/instr.h"

make_instr_func(ret_near)
{
	cpu.eip = *(int *)cpu.esp;
	print_asm_0("ret", "", 1);

	cpu.esp += 4;

	return 1 + 4;

	
}
