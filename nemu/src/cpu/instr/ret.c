#include "cpu/instr.h"

make_instr_func(ret_near)
{
	OPERAND tem;
	tem.type = OPR_MEM;
	tem.data_size = data_size;
	tem.addr = cpu.esp;
	operand_read(&tem);
	print_asm_0("ret", "", 1);
	cpu.eip = tem.val;
	
	
	
	cpu.esp += data_size / 8;
	return 0;
}

make_instr_func(ret_near_imm16)
{
	int len = 1;
	OPERAND tem;
	tem.data_size = data_size;
	len += modrm_rm(eip + 1, &rel);
	tem.addr = cpu.esp;
	operand_read(&tem);
	print_asm_1("ret", "", 3, &rel);
	cpu.eip = tem.val;
	return 0;
}