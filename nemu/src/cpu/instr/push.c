#include "cpu/instr.h"
static void instr_execute_1op() 
{
	operand_read(&opr_src);
	cpu.esp -= opr_src.data_size/8;	
	opr_src.addr = cpu.esp;
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)