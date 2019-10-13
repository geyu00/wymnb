#include "cpu/instr.h"

make_instr_func(ret_near)
{
	//cpu.eip = *(int *)cpu.esp;
	OPERAND res;
	OPERAND tem;
	tem.type = OPR_MEM;
	tem.data_size = 32;
	tem.addr = cpu.eip;
	
	res.type = OPR_MEM;
	res.data_size = 32;
	res.addr = cpu.esp;
	operand_read(&res);
	
	
	tem.val = res.val;
	operand_write(&tem);
	
	print_asm_0("ret", "", 1);
	cpu.esp += 4;
	return 1 + data_size / 8;
}