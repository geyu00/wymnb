#include "cpu/instr.h"

make_instr_func(ljmp)
{
	OPERAND res1, res2;
	res1.type = OPR.IMM;
	res1.sreg = SREG_CS;
	res1.addr = eip + 1;
	res1.data_size = data_size;
	operand_read(&res1);
	cpu.eip = res1.val;

	res2.type = OPR.IMM;
	res2.sreg = SREG_CS;
	res2.addr = eip + 1;
	res2.data_size = 16;
	operand_read(&res2);
	cpu.cs.val = res2.val;
	load_sreg(SREG_CS);
	return 0;
}