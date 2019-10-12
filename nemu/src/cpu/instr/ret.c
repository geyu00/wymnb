#include "cpu/instr.h"

make_instr_func(ret_near)
{
        /*OPERAND rel;
        rel.type = ;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;*/

        //operand_read(&rel);

        //int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_0("ret", "", 1);

        //cpu.eip -= offset;
	cpu.esp += 4;

        return 1 ;

	
}
