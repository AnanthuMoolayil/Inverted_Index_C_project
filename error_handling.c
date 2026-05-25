#include <stdio.h>
#include <stdlib.h>
#include "header.h"

int error_process(error_handling i)
{
    switch(i)
    {
        case op_first:
        printf("Error: Query starts with an operator");
        break;

        case op_last:
        printf("Error: Query ends with an operator");
        break;

        case mixed_op:
        printf("Error: Mixed operators unsupported");
        break;

        case and_again:
        printf("Error: Query has consecutive AND operators");
        break;

        case or_again:
        printf("Error: Query has consecutive OR operators");
        break;

        case word_again:
        printf("Error: Query has consecutive words");
        break;

        case and_null:
        printf("Since query contains word/s not present in database, AND operation result is NULL");
        break;

        case empty:
        printf("Error: Empty query");
        break;

        case word_absent:
        printf(" not present in index. Hence, won't be considered as a searchable term.\n");
        break;

        case word_full_delim:
        printf(" only consists of delimiters. Hence, won't be considered a searchable term.\n ");
        break;

        case word_common:
        printf(" is a common word. Hence, won't be considered a searchable term.\n ");
        break;

        case file_big_word:
        printf(" exceeds 45 characters. Hence, won't be considered for indexing.\n ");

    }
    return 0;
}


int file_error(char *filename, FILE *ptr, file_open error)
{
    switch(error)
    {
        case indexing_file:
        if(ptr==NULL)
        {
            printf("Error: %s opening failed. File contenets won't be indexed.", filename);
            return 1;
        }
        break;
            
        case delim_file:
        if(ptr==NULL)
        {
            printf("Error: %s opening failed. Program execution aborted.", filename);
            return 1;
        }
        break;

        case common_file:
        if(ptr==NULL)
        {
            printf("Error: %s opening failed. Program execution aborted.", filename);
            return 1;
        }
            
    }
    return 0;
}

void malloc_chk(void *ptr)
{
    if(ptr==NULL)
    {
        printf("Error: Memory allocation failed.\n");
        
        cleanup();  
        
        exit(EXIT_FAILURE);
    }    
}
