#include "cpu/instr.h"

make_instr_func(ret_near)
{
	//opr_dest.data_size = data_size
	print_asm_0("ret", "", 1);

	cpu.esp += 4;

	return 1 ;

	
}
