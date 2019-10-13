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
	tem.type = OPR_IMM;
	tem.data_size = data_size;
	tem.addr = cpu.esp;
	
	operand_read(&tem);
	tem.val = cpu.eip;

	print_asm_1("call", "", 1 + data_size / 8, &rel);
	cpu.eip += offset;
        return 1 + data_size / 8;

	
}
