#ifndef LEXER_GEN_H

#define LEXER_GEN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <assert.h>
#include <ctype.h>
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
		int32_t *bufptr, buflen;
	} type;

	struct {
		struct token *INC, *DEC, *OR, 
			*AND, *EQ, *NE, *LE, *GE,
			*CMUL, *CADD, *CSUB, *CDIV,
			*CXOR, *COR, *CAND;

	} Word;
};

enum Tag {
	Tag_Number,
	Tag_Unknown,
	Tag_String,
	Tag_Float,
	Tag_Identifier,
	Tag_Char,
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
