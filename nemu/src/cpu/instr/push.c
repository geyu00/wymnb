#include "cpu/instr.h"
static void instr_execute_1op() 
{
	cpu.esp -= data_size/8;	
	opr_des.type = OPR_MEN;
	opr_des.addr = cpu.esp;
	opr_des.data_size = data_size;
	operand_read(&opr_src);
	opr_des.value = opr_src.value;
	operand_write(&opr_des;
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)