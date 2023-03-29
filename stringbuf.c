#include "include/utils.h"
#include "stringbuf.h"

// return c_string from string struct
char* string_cstr(struct string* self)
{
	if (self->bufptr == NULL) return "";
	self->bufptr[self->buflen] = '\0';
	return self->bufptr;
}

// push character in string
void string_push(struct string* self, char pchar)
{
	if (self->size == self->buflen)
	{
		self->size += 20;
		self->bufptr = realloc(self->bufptr, self->size);
	}
	self->bufptr[self->buflen++] = pchar;
}

// concatenate given string
void string_cat(struct string* self, char* catstr)
{
	for (; *catstr != '\0'; catstr++)
		string_push(self, *catstr);
}

bool string_cmp(struct string *self, char *source)
{
	for (int idx = 0; idx < self->buflen; idx++)
		if (self->bufptr[idx] != source[idx])
			return false;
	// check for nulbyte
	if (source[self->buflen] != '\0')
		return false;

	return true;
}

// get input data from file until delimeter returns true on eof file
bool string_getdelim(struct string* self, int delim, FILE* filp)
{
	int byte;
	while ((byte = getc(filp)) != EOF)
	{
		if (delim != byte)
			string_push(self, byte);
		else return false;
	}
	return true;
}

// get input data from file until string returns true on eof file
bool string_getuntil(struct string* self, char* peekstr, FILE* filp)
{
	int byte, peekidx;
	for (peekidx = 0; (byte = getc(filp)) != EOF; peekidx++)
	{
		if (peekstr[peekidx] == '\0') 
			return false;
		if (peekstr[peekidx] != byte)
			peekidx = 0;
		string_push(self, byte);
	}
}
