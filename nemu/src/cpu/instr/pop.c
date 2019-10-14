#include "cpu/instr.h"
static void instr_execute_1op() 
{
	//operand_read(&opr_src);
	cpu.esp += opr_src.data_size / 8;
	opr_src.addr = cpu.esp;	
	//opr_src.type = OPR_MEM;
	//operand_write(&opr_src);
}
make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_impl_1op(pop, i, b)