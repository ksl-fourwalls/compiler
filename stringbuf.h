#ifndef _STRINGBUF_H_
#define _STRINGBUF_H_ 1
struct string {
	char* bufptr;
	size_t buflen;
	size_t size;
};

char* string_cstr(struct string* self)
{
	if (self->bufptr == NULL) return "";
	self->bufptr[self->buflen] = '\0';
	return self->bufptr;
}

void string_push(struct string* self, char pchar)
{
	if (self->size == self->buflen)
	{
		self->size += 20;
		self->bufptr = realloc(self->bufptr, self->size);
	}
	self->bufptr[self->buflen++] = pchar;
}

#endif
