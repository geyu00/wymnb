#include "cpu/instr.h"
static void instr_execute_1op() 
{
	operand_read(&opr_src);
	opr_src.addr -= 4;
	operand_write(&opr_src);
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)