#include "cpu/instr.h"

make_instr_func(ret_near)
{
	//cpu.eip = *(int *)cpu.esp;
	OPERAND tem;
	tem.type = OPR_MEM;
	tem.data_size = data_size;
	tem.addr = cpu.esp;
	operand_read(&tem);
	cpu.eip = tem.val;
	
	
	print_asm_0("ret", "", 1);
	cpu.esp += data_size / 8;
	return 0;
}