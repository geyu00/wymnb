#include "cpu/instr.h"

make_instr_func(call_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
       	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);
        int offset = sign_ext(rel.val, data_size);
        cpu.esp -= data_size / 8;
	//*(int *)cpu.esp = cpu.eip;
	OPERAND tem;
	tem.type = OPR_MEM;
	tem.data_size = 32;
	tem.addr = cpu.esp;
	tem.val = cpu.eip + 1 + data_size / 8;
	operand_write(&tem);
	

	print_asm_1("call", "", 1 + data_size / 8, &rel);
	cpu.eip += offset;
        return 1 + data_size / 8;	
}

make_instr_func(call_near_indirect)
{
	int len = 1;
	OPERAND rel;
        rel.data_size = data_size;
	len += modrm_rm(eip + 1, &rel);
        operand_read(&rel);

	cpu.esp -= data_size / 8;
	OPERAND tem;
	tem.type = OPR_MEM;
	tem.data_size = 32;
	tem.addr = cpu.esp;
	tem.val = cpu.eip + 1 + data_size / 8;
	operand_write(&tem);

        print_asm_1("jmp", "", 1 + data_size / 8, &rel);
        cpu.eip = rel.val;
        return 0;
}
