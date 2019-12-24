#include "cpu/instr.h"
static void instr_execute_1op() 
{
	//operand_read(&opr_src);
	//cpu.esp -= opr_src.data_size/8;	
	//opr_src.addr = cpu.esp;
	//*(int *)cpu.esp = opr_src.val;
	/*cpu.esp -= data_size / 8;
	opr_dest.addr = cpu.esp;	
	opr_dest.type = OPR_MEM;
	opr_dest.data_size = data_size;
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);*/
	

	operand_read(&opr_src);
	//cpu.esp -= opr_src.data_size / 8;
	cpu.esp -= data_size / 8;
	opr_src.data_size = data_size;
	opr_src.addr = cpu.esp;	
	opr_src.type = OPR_MEM;
	operand_write(&opr_src);

	/*cpu.esp -= data_size / 8;
	opr_dest.data_size = data_size;
	opr_dest.addr = cpu.esp;
	opr_dest.type = OPR_MEM;
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);*/
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)

make_instr_func(pusha)
{
	uint32_t tem_esp = cpu.esp;
	for (int i = 0; i < 8; i++)
	{
		cpu.esp -= 4;
		OPERAND src;
		src.data_size = data_size;
		src.type = OPR_MEM;
		src.sreg = SREG_SS;
		src.addr = cpu.esp;
		if (data_size == 16)
		{
			if (i != 4)
				src.val = cpu.gpr[i]._16;
			else
				src.val = tem_esp & 0xFFFF;
		}
		else if (data_size == 32)
		{
			if (i != 4)
				src.val = cpu.gpr[i]._32;
			else
				src.val = tem_esp;
		}
		operand_write(&src);
	}
	return 1;
}