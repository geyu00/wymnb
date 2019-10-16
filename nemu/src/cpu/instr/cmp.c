#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(opr_src.val, opr_dest.val, opr_dest.data_size);
}

make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, i, a, b)
make_instr_impl_2op(cmp, i, a, v)
//make_instr_impl_2op(cmp, i, rm, bv)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, i, rm, b)

make_instr_func(cmp_i2rm_bv)
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
        alu_sub(sign_ext(i.val , 8), rm.val, rm.data_size);
        //operand_write(&rm);
	print_asm_2("cmp", "", len, &i, &rm);
        return len;
}