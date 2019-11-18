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
bool check_parentheses(int p, int q);
int dominant_operator(int p, int q);
int priority(int my_nr_token);
uint32_t hex_to_dec(char *m_s);
void bad_expression();
uint32_t look_up_symtab(char *sym, bool *success);

enum
{
	NOTYPE = 256,
	EQ,
	NEQ,
	NUM,
	REG,
	SYMB,
	/*'+',
	'-',
	'*',
	'/',
	'%',
	'(',
	')',*/
	HEX,
	DEREF,
	NEG

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
	{"==", EQ},
	{"!=", NEQ},
	{"[0-9]+", NUM},
	{"$e[(ax)(bx)(cx)(dx)(si)(sp)(di)(dp)]", REG},
	{"[a-zA-Z][a-zA-Z0-9]*", SYMB},
	{"\\+", '+'},
	{"-", '-'},
	{"\\*", '*'},
	{"/", '/'},
	{"%", '%'},
	{"\\(", '('},
	{"\\)", ')'},
	{"0[Xx][0-9a-fA-F]{1,8}", HEX}
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

				printf("match regex[%d] at position %d with len %d: %.*s", i, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. 
				 * Add codes to perform some actions with this token.
				 */

				switch (rules[i].token_type)
				{
				case NOTYPE: break;
				case NUM: case HEX: case SYMB:
					if (substr_len >= 32)
					{
						printf("Buffer Overflow!\n");
						assert(0);
					}
					else
					{
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

	//printf("\nPlease implement expr at expr.c\n");
	//assert(0);

	*success = true;
	for (int i = 0; i < nr_token; i++)
	{
		if (tokens[i].type == '-' && (i == 0 || (tokens[i - 1].type != NUM && tokens[i - 1].type != HEX && tokens[i - 1].type != SYMB && tokens[i - 1].type != ')')))
		{
			tokens[i].type = NEG;
		}
		else if (tokens[i].type == '*' && (i == 0 || (tokens[i - 1].type != NUM && tokens[i - 1].type != HEX && tokens[i - 1].type != SYMB && tokens[i - 1].type != ')')))
		{
			tokens[i].type = DEREF;
		}
	}

	return eval(0, nr_token - 1);
}

uint32_t eval(int p, int q)
{
	if (p > q)
	{
		bad_expression();
		return 0;
	}
	else if (p == q)
	{
		bool success;
		uint32_t res;
		switch (tokens[p].type)
		{
		case NUM: return (uint32_t)atoi(tokens[p].str);
		case HEX: return hex_to_dec(tokens[p].str);
		case SYMB:
			res = look_up_symtab(tokens[p].str, &success);
			if(success) return res;
			else
			{
				bad_expression();
				return 0;
			}
		default:
			bad_expression();
			return 0;
		}
	}
	else if (check_parentheses(p, q) == true)
		return eval(p + 1, q - 1);
	else
	{
		int op = dominant_operator(p, q);
		int val1 = eval(p, op - 1);
		int val2 = eval(op + 1, q);
		switch (tokens[op].type)
		{
		case '+': return val1 + val2;
		case '-': return val1 - val2;
		case '*': return val1 * val2;
		case '/': return val1 / val2;
		case '%': return val1 % val2;
		case EQ: return val1 == val2;
		case NEQ: return val1 != val2;
		case DEREF: return vaddr_read((uint32_t)val2, SREG_CS, 4);
		case NEG: return -val2;
		default: bad_expression(); return 0;
		}
	}
}

bool check_parentheses(int p, int q)
{
	if (tokens[p].type!='(' || tokens[q].type!=')')
		return false;
	int left_bracket = 0, right_bracket = 0;
	for (int i = p; i <= q; i++)
	{
		if (tokens[i].type == '(')
			left_bracket++;
		else if (tokens[i].type == ')')
			right_bracket++;
		if(left_bracket < right_bracket)
			return false;
	}
	return left_bracket==right_bracket;
}

int dominant_operator(int p, int q)
{
	int left_bracket = 0, right_bracket = 0;
	int result = -1;
	for (int i = p; i <= q; i++)
	{
		if (tokens[i].type == '(')
			left_bracket++;
		else if (tokens[i].type == ')')
			right_bracket++;
		else if (tokens[i].type == NUM || tokens[i].type == HEX || tokens[i].type == SYMB);
		else
		{
			if (left_bracket > right_bracket);
			else if (result == -1 || priority(i) <= priority(result))
				result = i;
		}
	}
	return result;
}

int priority(int my_nr_token)
{
	int res = 0;
	switch (tokens[my_nr_token].type)
	{
	case EQ: case NEQ:
		res = 0;
		break;
	case '+': case '-':
		res = 1;
		break;
	case '*': case '/': case '%':
		res = 2;
		break;
	case DEREF:
		res = 3;
		break;
	case NUM: case HEX: case SYMB:
		res = 4;
		break;
	case NEG:
		res = 5;
		break;
	default:
		res = -1;
		break;
	}
	return res;
}

uint32_t hex_to_dec(char *m_s)
{
	uint32_t res = 0;
	for (int i = 2; i < strlen(m_s); i++)
	{
		res *= 16;
		if (m_s[i] >= '0' && m_s[i] <= '9')
			res += m_s[i] -'0';
		else if(m_s[i] >= 'a' && m_s[i] <= 'f')
			res += m_s[i] -'a' + 10;
		else if(m_s[i] >= 'A' && m_s[i] <= 'F')
			res += m_s[i] -'A' + 10;
	}
	return res;
}

void bad_expression()
{
	printf("Bad Expression!\n");
	assert(0);
}























