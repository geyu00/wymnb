#include "cpu/instr.h"
make_instr_func(lea) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = 32;
	rm.data_size = data_size;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.addr;
	operand_write(&r);

	print_asm_2("lea", "", len, &rm, &r);
	return len;
}
/*static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val = opr_src.addr;
	operand_write(&opr_dest);
}

make_instr_impl_2op(lea, rm, r, v)*/