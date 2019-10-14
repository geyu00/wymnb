#include "cpu/instr.h"
static void instr_execute_1op() 
{
	//operand_read(&opr_src);
	//cpu.esp -= opr_src.data_size/8;	
	//opr_src.addr = cpu.esp;
	//*(int *)cpu.esp = opr_src.val;
	operand_read(&opr_src);
	cpu.esp -= opr_src.data_size / 8;
	opr_dest.addr = cpu.esp;	
	opr_dest.type = OPR_MEM;
	opr_dest.data_size = data_size;
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, b)