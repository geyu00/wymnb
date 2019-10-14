#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_sub(opr_src.val, opr_dest.val, opr_dest.data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(sub, r, rm, b)
make_instr_impl_2op(sub, r, rm, v)
make_instr_impl_2op(sub, rm, r, b)
make_instr_impl_2op(sub, rm, r, v)
make_instr_impl_2op(sub, i, a, b)
make_instr_impl_2op(sub, i, a, v)
//make_instr_impl_2op(sub, i, rm, bv)
make_instr_impl_2op(sub, i, rm, v)

make_instr_func(sub_i2rm_bv)
{
	int len = 1;
        OPERAND rm, i;
	
        i.data_size = 8;
	rm.data_size = data_size;
	len += modrm_rm(eip + 1, &rm);
        i.type = OPR_IMM;
	i.sreg = SREG_GS;
	i.addr = eip + len;
	//rm.type = OPR_MEM;
	len += i.data_size / 8;
        operand_read(&i);
        operand_read(&rm);
        rm.val = alu_sub(sign_ext(i.val, 8), rm.val, rm.data_size);
        operand_write(&rm);
	print_asm_2("sub", "", len, &i, &rm);
        return len;
}