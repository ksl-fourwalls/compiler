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


struct manage_info
{
    int fild;                           /* file descriptor 
                                            for reading information */

    char *buf_ptr;                      /* file descriptor buffer */
    unsigned buf_len;                   /* buffer size */
    unsigned cur_index;                 /* pointing to the current
                                           index in the buf_ptr; */
};

void
init_manage_info(struct manage_info* info_struct, const char *filename)
{
    info_struct->fild = open(filename, O_RDONLY);
    if (info_struct->fild < 0)
        errExit("open");

    /* current index for buf_ptr */
    info_struct->cur_index = 0;
    info_struct->buf_len = 0;

    if((info_struct->buf_ptr = malloc(sizeof(char) * BUFFER_SIZE)) == NULL)
        errExit("malloc");
}

void
free_manage_info(struct manage_info* info_struct)
{
    /* Deallocate memory for manage info
     * struct */
    if (info_struct->buf_ptr != NULL)
    {
        free(info_struct->buf_ptr);
        info_struct->buf_ptr = NULL;

        info_struct->buf_len = 0;
        info_struct->cur_index = 0;

        /* close open file descriptor */
        close(info_struct->fild);
    }
}


/* 
 * read bytes from file descriptor
 * of manage_info struct
 */
bool
safe_read(struct manage_info *info_struct)
{
    if (++info_struct->cur_index < info_struct->buf_len)
        return true;

    if ((signed)(info_struct->buf_len = read(info_struct->fild, info_struct->buf_ptr, 
                    BUFFER_SIZE)) > (signed)0)
    {
        info_struct->cur_index = 0;
        return true;
    }

    return false;
}


/*
 * return string
 * token from the
 * given input and
 */
char *
token(struct manage_info *info_struct)
{
#ifndef BUFFERING
#define BUFFERING 60

    unsigned str_len;
    char *str;

    str_len = 0;

    str = malloc(BUFFERING * sizeof(char));
    if (str == NULL)
        errExit("malloc");

    do {
        str[str_len++] = info_struct->buf_ptr[info_struct->cur_index];

        /*
         * take input
         * string as buffer
         * bytes inorder to
         * decrease overhead
         * of malloc
         */
        if (str_len % BUFFERING == 0)
            str = realloc(str, (str_len + BUFFERING) * sizeof(char));

    } while (safe_read(info_struct) && !isspace(info_struct->buf_ptr[info_struct->cur_index]));

    str[str_len] = '\0';
    return str;

#undef BUFFERING
#endif
}

/*
 * gatherinfo function:
 * will check for three things
 * first is it cspace then
 * it will check for comment
 * 
 * lastly if string is clear
 * then we create copy of string
 * and return to the caller
 */
char *
gatherinfo(struct manage_info *info_struct)
{
    unsigned *index;
    bool cflag = false;         // comment flag
    char *data_ptr;

    index = &info_struct->cur_index;
    data_ptr = info_struct->buf_ptr;

    while (safe_read(info_struct))
    {
        do {
/* safety check
 * that is done
 * if comment 
 * string is
 * incomplete 
 */
check_again:
            if (isspace(data_ptr[*index]))
            {
                /* do nothing */
            }
            else if (data_ptr[*index] == '*')
            {
                if (safe_read(info_struct) && data_ptr[*index] == '/')
                    cflag = false;
                else 
                    goto check_again;
            }
            else if (data_ptr[*index] == '/')
            {
                if (safe_read(info_struct) && data_ptr[*index] == '*')
                    cflag = true;
                else
                    goto check_again;
            }
            else if (!cflag)
                // return pure string to the caller 
                return token(info_struct);

            *index = *index + 1;
        } while (*index < info_struct->buf_len);
    } 

    return NULL;
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
