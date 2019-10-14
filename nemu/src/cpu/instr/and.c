#include "cpu/instr.h"

static void instr_execute_2op() 
{	
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_and(opr_src.val, opr_dest.val, opr_dest.data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(and, r, rm, b)
make_instr_impl_2op(and, r, rm, v)
make_instr_impl_2op(and, rm, r, b)
make_instr_impl_2op(and, rm, r, v)
make_instr_impl_2op(and, i, a, b)
make_instr_impl_2op(and, i, a, v)
//make_instr_impl_2op(and, i, rm, bv)
make_instr_impl_2op(and, i, rm, v)

make_instr_func(and_i2rm_bv)
{
	int len = 1;
        OPERAND rm, i;
	
        i.data_size = 8;
	rm.data_size = data_size;
	len += modrm_rm(eip + 1, &rm);
        i.type = OPR_IMM;
	i.sreg = SREG_GS;
	i.addr = eip + len;
	rm.type = OPR_MEM;
	len += i.data_size / 8;
        operand_read(&i);
        operand_read(&rm);
        rm.val = alu_and(sign_ext(i.val, 8), rm.val, rm.data_size);
        operand_write(&rm);
	print_asm_2("and", "", len, &i, &rm);
        return len;
}