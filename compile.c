#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "stringbuf.h"
#include "token.h"

void printTokens(struct ProgramScanner* self)
{
	int prevline = 1;
	struct token* m_Token;

	// print all tokens
	printf("# line %-10d ", prevline);
	while ((m_Token = generateToken(self)) != NULL)
	{
		if (prevline != self->line)
		{
			prevline = self->line;
			printf("\n# line %-10d ", prevline);
		}

		switch (m_Token->tag)
		{
		case Tag_Number:
			printf(" %lld ", m_Token->value.integer);
			break;

		case Tag_Float:
			printf(" %lf ", m_Token->value.floatval);
			break;

		case Tag_Char:
			 printf(" %c ", (char)m_Token->value.integer);

		case Tag_Unknown:
			printf(" %c ", (char)m_Token->value.integer);
			break;

		case Tag_INC: case Tag_DEC: case Tag_OR: case Tag_AND:
		case Tag_EQ: case Tag_NE: case Tag_LE: case Tag_GE:
		case Tag_String: case Tag_Identifier: 
			printf(" %s ", m_Token->value.word);
			break;
		case Tag_Comment:
			printf("%s", m_Token->value.word);
			break;
		}
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	struct ProgramScanner pscanner;
	FILE* filp;

	assert(argc >= 2);

	filp = fopen(argv[1], "r");
	assert(filp != NULL);

	initProgramScanner(&pscanner, filp);
	printTokens(&pscanner);

	fclose(filp);
	return 0;
}