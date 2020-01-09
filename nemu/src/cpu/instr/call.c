#include "cpu/instr.h"

make_instr_func(call_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
       	rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);
        int offset = sign_ext(rel.val, data_size);
        cpu.esp -= data_size / 8;
	//*(int *)cpu.esp = cpu.eip;
	OPERAND tem;
	tem.type = OPR_MEM;
	//tem.data_size = 32;
	tem.data_size = data_size;
	tem.addr = cpu.esp;
	//tem.val = cpu.eip + 1 + data_size / 8;
	tem.val = eip + 1 + data_size / 8;
	operand_write(&tem);
	

	print_asm_1("call", "", 1 + data_size / 8, &rel);
	cpu.eip += offset;
        return 1 + data_size / 8;	
}

/*make_instr_func(call_near_indirect)
{
	int len = 1;
	OPERAND rel;
	//rel.type = OPR_MEM;
        rel.data_size = data_size;
	len += modrm_rm(eip + 1, &rel);
        operand_read(&rel);

	cpu.esp -= 4;
	OPERAND tem;
	tem.type = OPR_MEM;
	//tem.sreg = SREG_SS;
	//tem.data_size = 32;
	tem.data_size = data_size;
	tem.addr = cpu.esp;
	//tem.val = eip + 1 + data_size / 8;
	tem.val = eip + len;
	operand_write(&tem);

        print_asm_1("call", "", 1 + data_size / 8, &rel);
	if (data_size == 32)
	        cpu.eip = rel.val;
	else if (data_size == 16)
	{
		cpu.eip = rel.val & 0x0000FFFF;
	}
	return 0;
	
}*/
make_instr_func(call_near_indirect)

{

	OPERAND mem;

	mem.type = OPR_MEM;

	//printf("%d\n",520);

	//mem.sreg = SREG_CS;

	mem.data_size = data_size;

	int len = 1;

	len += modrm_rm(eip + 1,&mem);

	operand_read(&mem);

	//push

	OPERAND src;

	src.data_size = data_size;

	src.type = OPR_MEM;

	src.sreg = SREG_SS;

	cpu.esp = cpu.esp - 4;

	src.addr = cpu.esp;

	src.val = cpu.eip + len;

	operand_write(&src);

	//printf("%x\n",mem.val);

	if(data_size==16)

	{

		cpu.eip = cpu.eip>>16;

		cpu.eip = cpu.eip<<16;

		cpu.eip += mem.val;

		cpu.eip = cpu.eip &0x0000FFFF;

	}

	else

	{

		cpu.eip = mem.val;

	}

	return 0;

	

}
