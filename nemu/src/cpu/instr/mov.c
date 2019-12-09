#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)

make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_c2r_l)
{
	int len = 1;
	OPERAND r, c;
	r.data_size = 32;
	c.data_size = 32;
	len += modrm_r_rm(eip + 1, &r, &c);
	r.type = OPR_CREG;
	operand_read(&r);
	//c.val = r.val;
	//operand_write(&c);
	if (r.addr == 3)
		cpu.gpr[3].val = cpu.cr3.val;
	else if (r.addr == 0)
		cpu.gpr[0].val = cpu.cr0.val;
	print_asm_2("mov", "", len, &c, &r);
	return len;
}

make_instr_func(mov_r2c_l)
{
	int len = 1;
	OPERAND c, r;
	c.data_size = 32;
	r.data_size = 32;
	len += modrm_r_rm(eip + 1, &c, &r);
	c.type = OPR_CREG;
	r.type = OPR_REG;
	operand_read(&c);
	operand_read(&r);
	//r.val = c.val;
	//operand_write(&r);
	if (r.addr == 3)
		cpu.cr3.val = r.val;
	else if (r.addr == 0)
		cpu.cr0.val = r.val;
	print_asm_2("mov", "", len, &r, &c);
	return len;
}

make_instr_func(mov_rm2s_w)
{
	int len = 1;
	OPERAND s, rm;
	s.data_size = 16;
	rm.data_size = 16;
	len += modrm_r_rm(eip + 1, &s, &rm);
	s.type = OPR_SREG;
	operand_read(&rm);
	s.val = rm.val;
	operand_write(&s);
	print_asm_2("mov", "w", len, &rm, &s);
	load_sreg(s.addr);
	return len;
}


























