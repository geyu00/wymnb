#include "cpu/instr.h"
static void instr_execute_1op() 
{
	OPERAND rel;
	rel.addr = cpu.esp;
	rel.type = OPR_MEM;
	rel.data_size = data_size;
	operand_read(&rel);
	opr_src.val = rel.val;	
	operand_write(&opr_src);
	cpu.esp += opr_src.data_size / 8;	
}
make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_impl_1op(pop, i, b)