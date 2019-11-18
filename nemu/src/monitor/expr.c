#include "nemu.h"
#include "cpu/reg.h"
#include "memory/memory.h"

#include <stdlib.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
uint32_t eval(int p, int q);
uint32_t inv_e();
uint32_t convert_hex(char *c);
int dominant(int p, int q);
bool check_parentheses(int p, int q);
uint32_t look_up_symtab(char *sym, bool *success);
enum
{
	NOTYPE = 256,
	EQ,
	NEQ,
	NUM,
	SYMB,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	LP,
	RP,
	HEX,
	DEREF,
	NEGATIVE

	/* TODO: Add more token types */

};

static struct rule
{
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE}, // white space
	{"\\+", ADD},
	{"\\(", LP},
	{"\\)", RP},
	{"-", SUB},
	{"\\*", MUL},
	{"/", DIV},
	{"==", EQ},
	{"!=", NEQ},
	{"%", MOD},
	{"0[Xx][0-9a-fA-F]{1,8}", HEX},
	{"[A-Za-z_][A-Za-z0-9_]*", SYMB},
	{"[0-9]+", NUM}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

static regex_t re[NR_REGEX];

/* Rules are used for more times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
	int i;
	char error_msg[128];
	int ret;

	for (i = 0; i < NR_REGEX; i++)
	{
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if (ret != 0)
		{
			regerror(ret, &re[i], error_msg, 128);
			assert(ret != 0);
		}
	}
}

typedef struct token
{
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

int level(int e){
	switch (tokens[e].type){
		case EQ: return 0;break;
		case NEQ: return 0;break;
		case ADD: return 1;break;
		case SUB: return 1;break;
		case MUL: return 2;break;
		case DIV: return 2;break;
		case MOD: return 2;break;
		case DEREF: return 3;break;
		case NUM: return 4;break;
		case SYMB: return 4;break;
		case NEGATIVE: return 5;break;
		default: return -1;
	}
}
static bool make_token(char *e)
{
	int position = 0;
	int i;
	regmatch_t pmatch;

	nr_token = 0;

	while (e[position] != '\0')
	{
		/* Try all rules one by one. */
		for (i = 0; i < NR_REGEX; i++)
		{
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
			{
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				printf("match regex[%d] at position %d with len %d: %.*s\n", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type)
				{
				case NOTYPE: break;
				case NUM:
				case HEX:
				case SYMB:
					if(substr_len>=32){
						printf("buffer overflow!size is too long.\n");
						assert(0);
					}
					else{
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = '\0';
					}
				default:
					tokens[nr_token].type = rules[i].token_type;
					nr_token++;
				}

				break;
			}
		}

		if (i == NR_REGEX)
		{
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

uint32_t expr(char *e, bool *success)
{
	if (!make_token(e))
	{
		*success = false;
		return 0;
	}
	*success = true;

	for(int i=0;i<nr_token;i++)
	{
		if(tokens[i].type == SUB && (i == 0 || (tokens[i - 1].type != NUM && 
											  tokens[i - 1].type != HEX &&
											  tokens[i - 1].type != RP &&
											  tokens[i - 1] .type != SYMB)))
		{
			tokens[i].type = NEGATIVE;
		}
	}
	for (int i = 0; i < nr_token; i++)
	{
		if(tokens[i].type == MUL && (i == 0 || (tokens[i - 1].type != NUM && 
											  tokens[i - 1].type != HEX &&
											  tokens[i - 1].type != RP &&
											  tokens[i - 1] .type != SYMB)))
		{
			tokens[i].type = DEREF;
		}
	}
	return eval(0,nr_token - 1);
}
bool check_parentheses(int p,int q){
	int cnt = 0;
	if(tokens[p].type != LP || tokens[q].type != RP)
		return false;
	for(int i=p; i<=q;i++){
		if(tokens[i].type == LP)
			cnt++;
		else if(tokens[i].type==RP)
			cnt--;
		if(cnt < 0)
			return false;		
	}
	return cnt == 0;
}
int dominant(int p,int q){
	int cnt = 0;
	int result = -1;
	for(int i=p;i<=q;i++){
		if(tokens[i].type == LP)
		{
			cnt++;
			continue;
		}
		else if(tokens[i].type == RP)
		{
			cnt--;
			continue;
		}
		else if(tokens[i].type == NUM)
			continue;
		if(cnt != 0)
			continue;
		if (result == -1)
		{
			result = i;
			continue;
		}
		else if(level(i) <= level(result))
			result = i;
	}
	return result;
}
uint32_t convert_hex(char *c){
	uint32_t result = 0;
	int len = strlen(c);
	for(int i = 2;i<len;i++)
	{
		result = result * 16;
		if(c[i] >= '0' && c[i] <= '9')
			result += c[i] - '0';
		else if (c[i] >= 'A' && c[i] <= 'F')
			result += c[i] - 'A' + 10;
		else if (c[i] >= 'a' && c[i] <= 'f')
			result += c[i] - 'a' + 10;
	}
	return result;
}
uint32_t eval(int p, int q)
{	bool success;
	uint32_t result;
	if (p > q)
		return inv_e();
	else if (p == q)
		switch (tokens[p].type)
		{
		case NUM:
			return (uint32_t)atoi(tokens[p].str);
		case HEX:
			return (uint32_t)convert_hex(tokens[p].str);
		case SYMB:
			result = look_up_symtab(tokens[p].str, &success);
			if (success)
				return result;
			else
				return inv_e();
		default:
			return inv_e();
		}
	else if(p + 1 == q && tokens[p].type == NEGATIVE)
		return -eval(q,q);
	else if (p + 1 == q && tokens[p].type == DEREF)
		return vaddr_read((uint32_t)eval(q,q), SREG_CS, 4);
	else if (check_parentheses(p, q))
		return eval(p + 1, q - 1);
	else
	{
		int op = dominant(p, q);
		int val1 = eval(p, op - 1);
		int val2 = eval(op + 1, q);
		//printf("p = %d,op - 1 = %d,val1 = %d, val2 = %d\n", p,op-1,val1, val2);
		switch (tokens[op].type)
		{

		case ADD:

			return val1 + val2;
		case SUB:
			return val1 - val2;
		case MUL:
			return val1 * val2;
		case DIV:
			return val1 / val2;
		case MOD:
			return val1 % val2;
		case EQ:
			return val1 == val2;
		case NEQ:
			return val1 != val2;
		case DEREF:
			return vaddr_read((uint32_t)val2, SREG_CS, 4);
		case NEGATIVE:
			return -val2;
		default:
			return inv_e();
		}
	}
}
uint32_t inv_e()
{
	printf("invalid expression!\n");
	assert(0);
	return 0;
}