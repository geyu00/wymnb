#include "cpu/cpu.h"
void set_ZF(uint32_t result, size_t data_size)
{
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.ZF = (result == 0);
}
void set_SF(uint32_t result, size_t data_size)
{
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(result);
}
void set_PF(uint32_t result)
{
	uint32_t res = 1;
	for(int i = 0; i < 8; i++)
	{
		res ^= ((result >> i) & 0x1);
	}
	cpu.eflags.PF = res;
}
void set_CF_add(uint32_t result, uint32_t src, size_t data_size)
{
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result < src;
}
void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
	switch(data_size){
		case 8:
			result = sign_ext(result & 0xFF, 8);
			src = sign_ext(src & 0xFF, 8);
			dest = sign_ext(dest & 0xFF, 8);
			break;
		case 16:
			result = sign_ext(result & 0xFFFF, 16);
			src = sign_ext(src & 0xFFFF, 16);
			dest = sign_ext(dest & 0xFFFF, 16);
			break;
		default:break;
	}
	if(sign(src) == sign(dest)){
		if(sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}
	else
		cpu.eflags.OF = 0;
}
uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src;
	set_CF_add(res, src, data_size);
	set_OF_add(res, src, dest, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}
void set_CF_adc(uint32_t result, uint32_t src, size_t data_size)
{
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF == 0)
		cpu.eflags.CF = result < src;
	else
		cpu.eflags.CF = result <= src;
}
uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src + cpu.eflags.CF;
	set_CF_adc(res, src, data_size);
	set_OF_add(res, src, dest, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}
void set_CF_sub(uint32_t dest, uint32_t src, size_t data_size)
{
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = dest < src;
}
uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res = 0;
	uint32_t src_tra = src ^ (0xFFFFFFFF >> (32 - data_size));
	res = dest + src_tra + 0x1;
	set_CF_sub(dest, src, data_size);
	set_OF_add(res, src_tra, dest, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}
void set_CF_sbb(uint32_t dest, uint32_t src, size_t data_size)
{
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(src != (0xFFFFFFFF))
		cpu.eflags.CF = dest < (src + cpu.eflags.CF);
	else
		cpu.eflags.CF = (dest != 0xFFFFFFFF) || (cpu.eflags.CF == 1);	
}
uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res = 0;
	uint32_t src_tra = (src ^ (0xFFFFFFFF >> (32 - data_size)));
	res = dest + src_tra +0x1 - cpu.eflags.CF;
	set_CF_sbb(dest, src, data_size);
	set_OF_add(res, src_tra, dest, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}
uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	src = sign_ext_64(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	dest = sign_ext_64(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	int64_t res = 0;
	res = src * dest;
	return res & (0xFFFFFFFFFFFFFFFF >> (64 - 2 * data_size));
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	assert(0);
	return 0;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res = 0;
	res = src & dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res = 0;
	res = src ^ dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res = 0;
	res = src | dest;
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}
void set_CF_shl(uint32_t src, uint32_t dest, size_t data_size)
{
	dest &= (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.CF = 0x1 & (dest >> (data_size - src));
}
uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest << src;
	set_CF_shl(src, dest, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}
void set_CF_shr(uint32_t src, uint32_t dest)
{
	cpu.eflags.CF = 0x1 & (dest >> (src - 1));
}
uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	uint32_t res = 0;
	dest &= (0xFFFFFFFF >> (32 - data_size));
	res = dest >> src;
	set_CF_shr(src, dest);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	uint32_t res = 0;
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	uint32_t tem = dest >> 31;
	res = dest >> src;
	for(int i = 0; i < src; i++)
		res |= (tem << (31 - i));
	set_CF_shr(src, dest);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest << src;
	set_CF_shl(src, dest, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}
