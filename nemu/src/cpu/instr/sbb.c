#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val -= (opr_src.val + cpu.eflags.CF);
	operand_write(&opr_dest);
}

make_instr_impl_2op(sbb, r, rm, b)
make_instr_impl_2op(sbb, r, rm, v)
make_instr_impl_2op(sbb, rm, r, b)
make_instr_impl_2op(sbb, rm, r, v)
make_instr_impl_2op(sbb, i, a, b)
make_instr_impl_2op(sbb, i, a, v)