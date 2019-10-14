#include "cpu/instr.h"
static void instr_execute_1op() 
{
	opr_src.addr = cpu.esp;
	operand_read(&rel);
	opr_dest.val = opr_src.val;
	cpu.esp += opr_src.data_size / 8;
	operand_write(&opr_dest);
}
make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_impl_1op(pop, i, b)