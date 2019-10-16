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
	/*int len = 1;
	OPERAND tem;
	tem.data_size = 16;
	len += modrm_rm(eip + 1, &tem);
	tem.addr = cpu.esp;
	operand_read(&tem);
	print_asm_1("ret", "", 1 + 2, &tem);
	cpu.eip = tem.val + len;
	cpu.esp += data_size / 8;
	return 0;*/
	
	OPERAND rel;
	rel.type = OPR_IMM;
	rel.data_size = 16;
	rel.addr = eip + 1;
	operand_read(&rel);
	int imm_val = sign_ext(rel.val, 16);

	OPERAND tem;
	tem.type = OPR_MEM;
	tem.data_size = data_size;
	tem.addr = cpu.esp;
	operand_read(&tem);
	print_asm_1("ret", "", 1 + 2, &tem);
	//print_asm_0("ret", "", 1);
	cpu.eip = tem.val;
	cpu.esp += data_size / 8;
	cpu.esp += imm_val;
	return 0;
}