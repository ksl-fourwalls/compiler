#ifndef LEXER_GEN_H

#define LEXER_GEN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>
#include <ctype.h>
#include <search.h>
#include "stringbuf.h"


/*
 *                   Token
 *                  /  |  \
 *                 /   |   \
 *                /    |    \
 *               /     |     \
 *            string float integer
 */
struct LexicalScanner {
	int peek;
	int line;
	FILE* filp;

	struct {
		struct token *INC, *DEC, *OR, 
			*AND, *EQ, *NE, *LE, *GE,
			*CMUL, *CADD, *CSUB, *CDIV,
			*CXOR, *COR, *CAND, *WHILE,
			*BREAK, *DO, *IF, *ELSE;

	} Word;

	struct token *keywordTokens;
};



enum Tag {
	Tag_Number,
	Tag_Unknown,
	Tag_String,
	Tag_FloatNumber,
	Tag_Identifier,
	Tag_Byte,
	Tag_Comment,
	Tag_type,

	// complex operation
	Tag_CMUL,
	Tag_CADD,
	Tag_CSUB,
	Tag_CDIV,
	Tag_CXOR,
	Tag_COR,
	Tag_CAND,

	Tag_INC,
	Tag_DEC,
	Tag_OR,
	Tag_AND,
	Tag_EQ,
	Tag_NE,
	Tag_LE,
	Tag_GE,

	Tag_Float,
	Tag_Const,
	Tag_Volatile,
	Tag_Int,
	Tag_Char,
	Tag_Unsigned,
	Tag_Signed,
	Tag_Register,
	Tag_If,
	Tag_Else,
	Tag_Do,
	Tag_While,
	Tag_Break
};

struct token {
	union { 
		long long int integer; 
		double floatval;
		char* word; 
	} value;

	int tag;
	int attribute;

};

void initLexicalScanner(struct LexicalScanner* self, FILE *filp);
struct token* generateToken(struct LexicalScanner *self);
void printTokens(struct LexicalScanner* self);
void freeLexicalScanner(struct LexicalScanner *self);

#endif
