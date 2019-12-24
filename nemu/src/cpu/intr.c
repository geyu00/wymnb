#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	//push eflags
	cpu.esp -= 4;
	OPERAND src1;
	src1.data_size = 32;
	src1.type = OPR_MEM;
	src1.sreg = SREG_SS;
	src1.addr = cpu.esp;
	src1.val = cpu.eflags.val;
	operand_write(&src1);

	//push cs
	cpu.esp -= 4;
	OPERAND src2;
	src2.data_size = 32;
	src2.type = OPR_MEM;
	src2.sreg = SREG_SS;
	src2.addr = cpu.esp;
	src2.val = cpu.cs.val;
	operand_write(&src2);

	//push eip
	cpu.esp -= 4;
	OPERAND src3;
	src3.data_size = 32;
	src3.type = OPR_MEM;
	src3.sreg = SREG_SS;
	src3.addr = cpu.esp;
	src3.val = cpu.eip;
	operand_write(&src3);

	GateDesc gatedesc;
	laddr_t addr = cpu.idtr.base + 64 * intr_no;
	gatedesc.val[0] = laddr_read(addr, 4);
	gatedesc.val[1] = laddr_read(addr + 4, 4);
	assert(gatedesc.present == 1);
	if (gatedesc.type == 0xe)
		cpu.eflags.IF = 0;
	cpu.cs.val = gatedesc.selector;
	load_sreg(SREG_CS);
	cpu.eip = (gatedesc.offset_31_16 << 16) | gatedesc.offset_15_0;
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
