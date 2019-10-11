#include "cpu/instr.h"
static void instr_execute_1op() 
{
	
	//opr_dest.type = OPR_REG;
	
	//opr_dest.data_size = data_size;
	operand_read(&opr_src);
	cpu.esp -= opr_src.data_size/8;	
	//opr_dest.val = opr_src.val;
	opr_src.addr = cpu.esp;
	operand_write(&opr_src);
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)