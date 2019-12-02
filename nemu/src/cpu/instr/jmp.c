#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_short)
{
	OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = 8;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, 8);
        
        print_asm_1("jmp", "", 1 + 1, &rel);

        cpu.eip += offset;

        return 1 + 1;
}

make_instr_func(jmp_near_indirect)
{
	int len = 1;
	OPERAND rel;
        rel.data_size = data_size;
	len += modrm_rm(eip + 1, &rel);
        operand_read(&rel);
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip = rel.val;

        return 0;
}

make_instr_func(jmp_far_imm)
{
	OPERAND rel1, rel2;
	rel1.type = OPR_IMM;
	rel1.sreg = SREG_CS;
	rel1.addr = eip + 1;
	rel1.data_size = data_size;
	operand_read(&rel1);
	cpu.eip = rel1.val;

	rel2.type = OPR_IMM;
	rel2.sreg = SREG_CS;
	rel2.addr = eip + 1;
	rel2.data_size = 16;
	operand_read(&rel2);
	cpu.cs.val = rel2.val;
	load_sreg(SREG_CS);
	return 0;
}