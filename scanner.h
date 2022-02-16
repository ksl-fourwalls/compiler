#ifndef SCANNER_H
#define SCANNER_H 1

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

#define errExit(msg) do {     \
    perror(msg);              \
    exit(EXIT_FAILURE);       \
} while (0)


typedef struct {
    char ptr[BUFFER_SIZE];          // input buffer 

    unsigned start;                 // current index for 
                                    // circular buffer 

    unsigned end;                   // End of circular buffer 
} char_queue;


/*
 * manages data and
 * size and other
 * information reading
 * input
 */
struct manage_info
{
    int fild;                           /* file descriptor 
                                            for reading information */
    char_queue mQueue;                  /* manage queue */
};


/*
 * initialize manage_info
 * struct by opening compiler file
 * descriptor in read only mode
 */
void
init_manage_info(struct manage_info* info, const char *filename)
{
    info->fild = open(filename, O_RDONLY);
    if (info->fild < 0)
        errExit("open");

    // first index is skipped for
    // storing prev value
    info->mQueue.start = 1;
    info->mQueue.end = 1;
}

void
free_manage_info(struct manage_info* info)
{
    info->mQueue.start = 0;
    info->mQueue.end = 0;

    /* close open file descriptor */
    close(info->fild);
}


/* 
 * read bytes from file descriptor
 * of manage_info struct
 */
void
safe_read(struct manage_info *info)
{
    info->mQueue.start = 1;

    // store previous character
    info->mQueue.ptr[0] = info->mQueue.ptr[info->mQueue.end-1];

    info->mQueue.end = read(info->fild, &info->mQueue.ptr[1], BUFFER_SIZE-1);
}

/*
 * return next character
 * from the queue buffer
 */
char 
next_chr(struct manage_info* info)
{
    if (info->mQueue.start == info->mQueue.end)
    {
        // read from the file
        safe_read(info);

        // for a read doesn't fails
        if (info->mQueue.end == -1)
            return -1;
    }

    return info->mQueue.ptr[info->mQueue.start++];
}

/*
 * below definition 
 * should be called only
 * once with respect to
 * next_chr
 */
#define prev_chr(self) \
    ((struct manage_info *)self)->mQueue.ptr[--info->mQueue.start]


/*
 * token_fminfo function:
 * will check for three things
 * first is it cspace then
 * it will check for comment
 *
 * and returns token if
 * it is lexeme 
 */
struct token*
token_fminfo(struct manage_info *info)
{
    char chrp;

sanitycheck:
    while ((chrp = next_chr(info)) != -1)
    {
        if (isspace(chrp))
            continue;
        break;
    }

    if (chrp == '/')
    {
        chrp = next_chr(info);
        if (chrp == '/')
        {
            for (;; (chrp = next_chr(info)) != -1)
                if (chrp == '\n')
                    goto sanitycheck;
        }
        else if (chrp == '*')
        {
            char prev;
            for (;; prev = chrp, (chrp = next_chr(info)) != -1)
                if (prev == '*' && chrp == '/')
                    goto sanitycheck;
        }
        chrp = prev_chr(info);
    }
    return;
}

int 
main(int argc, char **argv)
{
    struct manage_info manage_struct;

    init_manage_info(&manage_struct, argv[1]);
    free_manage_info(&manage_struct);

    return 0;
}

#endif
