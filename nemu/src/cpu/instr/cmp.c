#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(opr_src.val, opr_dest.val, data_size);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, a, b)
make_instr_impl_2op(mov, i, a, v)