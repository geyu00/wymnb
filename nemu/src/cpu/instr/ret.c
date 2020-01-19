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

make_instr_func(iret)
{
	//pop eip
	OPERAND dest1;
	dest1.data_size = 32;
	dest1.type = OPR_MEM;
	dest1.sreg = SREG_SS;
	dest1.addr = cpu.esp;
	operand_read(&dest1);
	cpu.esp += 4;
	cpu.eip = dest1.val;

	//pop cs
	OPERAND dest2;
	dest2.data_size = 16;
	dest2.type = OPR_MEM;
	dest2.sreg = SREG_SS;
	dest2.addr = cpu.esp;
	operand_read(&dest2);
	cpu.esp += 4;
	cpu.cs.val = sign_ext(dest2.val, 32);
	//cpu.cs.val = dest2.val;

	//pop eflags
	OPERAND dest3;
	dest3.data_size = 32;
	dest3.type = OPR_MEM;
	dest3.sreg = SREG_SS;
	dest3.addr = cpu.esp;
	operand_read(&dest3);
	cpu.esp += 4;
	cpu.eflags.val = dest3.val;

	return 0;
}
























