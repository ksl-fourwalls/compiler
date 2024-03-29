#include "include/utils.h"
#include "lexer.h"

#include <stdint.h>

static int32_t fnv_1a(const char* str)
{
	const int32_t FNV_offset_basis = 0x811c9dc5;
	const int32_t FNV_prime = 0x01000193;

	int32_t hash = FNV_offset_basis;

	for (; *str != '\0'; str++)
	{
		hash ^= (int32_t)*str;
		hash *= FNV_prime;
	}
	return hash;

}

void initLexicalScanner(struct LexicalScanner* self, FILE *filp)
{
	struct {
		const char* str;
		enum Tag tag;
	} keywords[] = {
		{ "float", Tag_Float },
		{ "const", Tag_Const },
		{ "volatile", Tag_Volatile },
		{ "int", Tag_Int },
		{ "char", Tag_Char },
		{ "unsigned", Tag_Unsigned },
		{ "signed", Tag_Signed },
		{ "register", Tag_Register },
		{ "if", Tag_If },
		{ "else", Tag_Else },
		{ "do", Tag_Do },
		{ "while", Tag_While },
		{ "break", Tag_Break },
	};



	struct token* Word(char* string, enum Tag tag);
	ENTRY e, *ep;

	self->keywordTokens = (struct token *)malloc(ARRAYSIZE(keywords)*sizeof(struct token));

	// create static hash table 
	hcreate(ARRAYSIZE(keywords));

	for (int idx = 0; idx < ARRAYSIZE(keywords); idx++)
	{
		e.key = (char *)keywords[idx].str;
		e.data = (void *)((size_t)idx);
		self->keywordTokens[idx].tag = keywords[idx].tag;
		self->keywordTokens[idx].value.word = (char *)keywords[idx].str;
		ep = hsearch(e, ENTER);
	}

	self->peek = ' ';
	self->line = 1;
	self->filp = filp;


	self->Word.INC = Word("++", Tag_INC);
	self->Word.DEC = Word("--", Tag_DEC);

	self->Word.CMUL = Word("*=", Tag_CMUL);
	self->Word.CADD = Word("+=", Tag_CADD);
	self->Word.CSUB = Word("-=", Tag_CSUB);
	self->Word.CDIV = Word("/=", Tag_CDIV);
	self->Word.CXOR = Word("^=", Tag_CXOR);
	self->Word.CAND = Word("&=", Tag_CAND);
	self->Word.COR = Word("|=", Tag_COR);

	self->Word.AND = Word("&&", Tag_AND);
	self->Word.OR = Word("||", Tag_OR);
	self->Word.EQ = Word("==", Tag_EQ);
	self->Word.NE = Word("!=", Tag_NE);
	self->Word.LE = Word(">=", Tag_LE);
	self->Word.GE = Word("<=", Tag_GE);
}

struct token* Word(char* string, enum Tag tag)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = tag;
	m_Token->value.word = string;
}

struct token* FloatNumber(double value)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = Tag_FloatNumber;
	m_Token->value.floatval = value;
}

struct token* Integer(long long int value)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = Tag_Number;
	m_Token->value.integer = value;
}

struct token* Token(char peek, enum Tag tag)
{
	struct token* m_Token = malloc(sizeof(struct token));
	assert(m_Token != NULL);

	m_Token->tag = tag;
	m_Token->value.integer = (long long int)peek;
}

 
struct token* generateToken(struct LexicalScanner *self)
{
	struct token *m_Token;

	for (;; self->peek = getc(self->filp))
	{
		if (self->peek == ' ' || self->peek == '\t');
		else if (self->peek == '\n') ++self->line;
		else if (self->peek == EOF) return NULL;
		else break;
	}

	switch (self->peek)
	{
		case '/':
		{
			self->peek = getc(self->filp);
			// single line comments
			if (self->peek == '/')
			{
				struct string m_String = { 0 };
				string_cat(&m_String, "//");
				self->peek = string_getdelim(&m_String, '\n', self->filp) ? EOF : ' ';
				return Word(string_cstr(&m_String), Tag_Comment);
			}
			// "/*" multiline comments
			else if (self->peek == '*')
			{
				struct string m_String = { 0 };
				string_cat(&m_String, "/*");
				self->peek = string_getuntil(&m_String, "*/", self->filp) ? EOF : ' ';
				return Word(string_cstr(&m_String), Tag_Comment);
			}
			// "/=" equalto operator
			else if (self->peek == '=')
			{
				self->peek = ' ';
				return self->Word.CDIV;
			}
			return Token('/', Tag_Unknown);
		}

		case '&':
		{
			self->peek = getc(self->filp);
			if (self->peek == '&') 
			{
				self->peek = ' ';
				return self->Word.AND;
			}
			return Token('&', Tag_Unknown);
		}

		case '|':
		{
			self->peek = getc(self->filp);
			if (self->peek == '|') 
			{
				self->peek = ' ';
				return self->Word.OR;
			}
			return Token('|', Tag_Unknown);
		}
		case '+':
		{
			self->peek = getc(self->filp);
			if (self->peek == '+') 
			{
				self->peek = ' ';
				return self->Word.INC;
			}

			else if (self->peek == '=')
			{
				self->peek = ' ';
				return self->Word.CADD;
			}
			return Token('-', Tag_Unknown);
		}

		case '-':
		{
			self->peek = getc(self->filp);
			if (self->peek == '-') 
			{
				self->peek = ' ';
				return self->Word.DEC;
			}
			else if (self->peek == '=')
			{
				self->peek = ' ';
				return self->Word.CSUB;
			}
			return Token('-', Tag_Unknown);
		}

		case '*':
		{
			self->peek = getc(self->filp);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.CMUL;
			}
			return Token('*', Tag_Unknown);
		}

		case '!':
		{
			self->peek = getc(self->filp);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.NE;
			}
			return Token('!', Tag_Unknown);
		}

		case '>':
		{
			self->peek = getc(self->filp);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.GE;
			}
			return Token('>', Tag_Unknown);
		}

		case '<':
		{
			self->peek = getc(self->filp);
			if (self->peek == '=') 
			{
				self->peek = ' ';
				return self->Word.LE;
			}
			return Token('<', Tag_Unknown);
		}
		case '=':
		{
			struct token *m_Token;
			self->peek = getc(self->filp);
			m_Token = self->peek == '=' ? self->Word.EQ : Token('=', Tag_Unknown);
			self->peek = ' ';
			return m_Token;
		}


		case '"': 
		{
			struct string m_String = { 0 };
			while (1) 
			{
				string_push(&m_String, self->peek);
				self->peek = getc(self->filp);
				if (self->peek == EOF) 
					return Word(string_cstr(&m_String), Tag_String);

				if ( self->peek == '"' && m_String.bufptr[m_String.buflen-1] != '\\')
				{
					string_push(&m_String, self->peek);
					self->peek = ' ';

					return Word(string_cstr(&m_String), Tag_String);
				}
			}
		}
	}

	if (isdigit(self->peek))
	{
		int value = 0;
		float x, d;

		do {
			value = 10 * value + (self->peek - 0x30);
			self->peek = getc(self->filp);
		} while (isdigit(self->peek));
		if (self->peek != '.') return Integer(value);

		
		for (x = value, d = 10.0;; d *= 10) {
			self->peek = getc(self->filp);
			if (!isdigit(self->peek)) 
				break;
			x = x + (float)(self->peek-0x30) / d;
		}
		return FloatNumber(x);
	}

	else if (isalpha(self->peek) || self->peek == '_')
	{
		struct string m_String = { 0 };
		char* c_str;
		int32_t strhash;
		ENTRY e, *ep;

		do {
			string_push(&m_String, self->peek);
			self->peek = getc(self->filp);
		} while (isalnum(self->peek) || self->peek == '_');

		c_str = string_cstr(&m_String);
		e.key = c_str;

		ep = hsearch(e, FIND);
		if (ep != NULL)
			return &self->keywordTokens[(long)ep->data];

		return Word(c_str, Tag_Identifier);
	}

	m_Token = Token(self->peek, Tag_Unknown); 
	self->peek = ' ';
	return m_Token;
}

void printTokens(struct LexicalScanner* self)
{
	struct token* m_Token;
	int prevline = 1;

	// print all tokens
	printf("# line %-10d ", prevline);
	while ((m_Token = generateToken(self)) != NULL)
	{
		if (prevline != self->line) {
			prevline = self->line;
			printf("\n# line %-10d ", prevline);
		}

		switch (m_Token->tag)
		{
		case Tag_Number: printf(" %lld ", m_Token->value.integer); 	break;
		case Tag_FloatNumber: printf(" %lf ", m_Token->value.floatval); 	break; 
		case Tag_Byte: printf(" %c ", (char)m_Token->value.integer); 	break;
		case Tag_Unknown: printf(" %c ", (char)m_Token->value.integer); break;
		case Tag_Comment: printf("%s", m_Token->value.word); 		break;

		case Tag_INC: case Tag_DEC: case Tag_OR: case Tag_AND:
		case Tag_EQ: case Tag_NE: case Tag_LE: case Tag_GE:
		case Tag_String: case Tag_Identifier: 
			printf(" %s ", m_Token->value.word);
			break;

		default:
			ENTRY e = { .key = m_Token->value.word }, *ep;
			ep = hsearch(e, FIND);
			if (ep != NULL)
				printf(" %s ", m_Token->value.word);
			break;
		}

	}
	printf("\n");
	fseek(self->filp, 0, SEEK_SET);
}

void freeLexicalScanner(struct LexicalScanner *self)
{
	free(self->keywordTokens);
	hdestroy();
}
