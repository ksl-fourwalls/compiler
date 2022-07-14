
#include "scanner.h"

/*
 * initialize scanner_info
 * struct by opening compiler file
 * object in read only mode
 */
void init_scanner_info(struct scanner_info *info, const char *fname)
{
	info->filp = fopen(fname, "rb");
	if (info->filp == NULL)
		errExit("Unable to open file %s\n", fname);

	setvbuf(info->filp, NULL, _IONBF, 0);
}


void free_scanner_info(struct scanner_info* info) 
{
	fclose(info->filp);
}

static uint64_t safe_read(struct scanner_info* info)
{
	return fread(info->buffer, sizeof(char), BUFFER_SIZE, info->filp);
}

struct token* get_token(struct scanner_info* info)
{
	uint64_t endptr = (uint64_t)info->buffer + safe_read(info);
	bool comment_flag;
	char prevchar = '\0';

	for (char* bufptr = info->buffer; (uint64_t)bufptr != endptr; bufptr++) 
	{
		if (comment_flag == true) 
		{
			if (prevchar == '*' && *bufptr == '/')
				comment_flag = false;
			goto end;
		}

		if (ispunct(*bufptr))
		{
			if (prevchar == '/' && *bufptr == '*')
			{
				comment_flag = true;
				goto end;
			}
		}

		printf("%c", *bufptr);
end:
		prevchar = *bufptr;
	}
}
