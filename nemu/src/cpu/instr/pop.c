#include "cpu/instr.h"
static void instr_execute_1op() 
{
	opr_dest.data_size=data_size;
	opr_dest.type=OPR_MEM;
	opr_dest.addr=cpu.esp;
	operand_read(&opr_dest);
	//opr_src.val=sign_ext(opr_dest.val,opr_dest.data_size);
	opr_src.val=opr_dest.val;
	operand_write(&opr_src);
	cpu.esp+=data_size/8;

	/*OPERAND rel;
	rel.data_size = data_size;
	rel.type = OPR_MEM;
	rel.addr = cpu.esp;
	operand_read(&rel);
	opr_src.val = rel.val;	
	operand_write(&opr_src);
	cpu.esp += data_size / 8;*/
}
make_instr_impl_1op(pop, r, v)
make_instr_impl_1op(pop, rm, v)
make_instr_impl_1op(pop, i, b)