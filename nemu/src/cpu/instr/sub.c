#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val -= opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(sub, r, rm, b)
make_instr_impl_2op(sub, r, rm, v)
make_instr_impl_2op(sub, rm, r, b)
make_instr_impl_2op(sub, rm, r, v)
make_instr_impl_2op(sub, i, a, b)
make_instr_impl_2op(sub, i, a, v)