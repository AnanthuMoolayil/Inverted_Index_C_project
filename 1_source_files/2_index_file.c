/*
 *  This code is for indexing words in a file to hash table.
 *  'uint' and 'uchar' are typedefs defined in "01_typedef.h"
 *  malloc_chk function is used to check for dynamic memory allocation failure and subsequent proper exit from program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "01_typedef.h"
#include "0_header.h"
#include <ctype.h>


/*
 *  index_file function is for indexing words in a file to hash table.
 *  The following steps are taken:
 *  1. Input file is opened.
 *  2. Characters are fetched one by one and stored in an array. Characters are converted to lowercase.
 *  3. It is checked whether the fetched character is a delimiter or not by comparing it characters present in 'delim_buf' buffer.
 *  3. When a delimiter is encountered, characters collected upto that point are made into string.
 *  4. It is checked whether this string ia a common word or not by comparing it with words present in a linked list of type'common_list'.
 *  5. If word is not a common word, it is inserted into the hash table.
 *  6. This process continues till EOF.
 */
void index_file(Htable *ht, char *filename, int doc_id, char *delim_buf, common_list *list)
{
    FILE *doc_fptr;
    char buf[word_size]={0};
    int file_char, i=0;
    
    //  file_error function for handling file opening error.
    doc_fptr=fopen(filename, "r");
    if( file_error(filename, doc_fptr, indexing_file) )
        return;

    file_char=fgetc(doc_fptr);

    while(file_char!= EOF)
    {
        /*
         *  Characters fetched from file are stored in 'buf'array.
         *  lower_mod function to convert character to lower case.  
         *  i is used to keep the count of stored characters.
         *  To avoid overflow, i should be less than 'buf' size.
         */
        while( (file_char!= EOF) && (i<(word_size-1)) )
        {
            //  delim_check function for handling delimiters.
            if( delim_check((char)file_char, delim_buf) )
                break;

            if(i==0)
            {
                buf[i]=lower_mod((char)file_char);
                i++;
                file_char=fgetc(doc_fptr);
                continue;
            }
            buf[i]=lower_mod((char)file_char);
            i++;    
    
            file_char=fgetc(doc_fptr);
        }
        
        /*
         *  Consider the case wherein a word exceeds the storage limit.
         *  error_process function displays the corresponding error message & storage array is cleared.
         *  Characters are fetched from file until delimiters or EOF is encountered.
         *  Then normal indexing operations are continued. 
         */
        if( i>=(word_size-1) )
        {
            printf("%s.... ", buf);
            error_process(file_big_word);

            while(file_char!=EOF)
            {
                if( (delim_check((char)file_char, delim_buf)) == 0)
                    file_char=fgetc(doc_fptr);
                else
                {
                    i=0;
                    break;
                }
                 
            }
            if(file_char== EOF)
                break;
            else
            {
                file_char=fgetc(doc_fptr);
                continue;
            }
        }
        if( (file_char== EOF) && (i==0) )
            break;
        
        if(i==0) 
        {
            file_char=fgetc(doc_fptr);
            continue;
        }
        
        buf[i]='\0';
    
        //  common_word function for handling common words.
        if(common_word(buf, list))
        {
            file_char=fgetc(doc_fptr);
            i=0;
            continue;
        }

        insert(ht, buf, doc_id);

        i=0;
        if(file_char!=EOF)
            file_char=fgetc(doc_fptr);
        else 
            break;
            
    }
    fclose(doc_fptr);
    return;
}

/*
 *  Function to convert character to lower case.
 *  Returs modified character.
 */
char lower_mod(char in_char)
{
    if( (in_char>='A') && (in_char<='Z') )
        return (char)tolower(in_char);
    else
        return in_char;
}


/*
 *  Function does the following:
 *  1. Open delimiters.txt file, which contain a collection of delimiters.
 *  2. Load all the characters in the file to memory in form of string 'buf'.
 *  3. Returns address of memory buffer.
 */
char* delim_file_load(void)
{
    //  file_error function for handling file opening error.
    FILE *delim_fptr=fopen("2_parsing_check_files/delimiters.txt", "r");
    if( file_error( "2_parsing_check_files/delimiters.txt", delim_fptr, delim_file) )
        return NULL;

    fseek(delim_fptr, 0, SEEK_END);
    uint delim_file_size=(uint)ftell(delim_fptr);
    char *buf=malloc(sizeof(char)*(delim_file_size+1));
    malloc_chk(buf);
    rewind(delim_fptr);

    fread(buf, delim_file_size, 1, delim_fptr);
    *(buf+delim_file_size)='\0';
    
    fclose(delim_fptr);
    return buf;
}


/*
 *  Function does the following:
 *  1. Check whether the input character is delimiter or not.
 *  2. Check if the character 'in_char' matches with any character from 'file_buf'.
 *  3. Returns 1 if character is a delimiter else 0.
 */
int delim_check(char in_char, char *file_buf)
{
    char *buf=file_buf;
    int i=0;
    while( *(buf+i)!='\0' )
    {
        if(in_char==*(buf+i))
            return 1;

        i++;
    }
    
    return 0;

}


/*
 *  'common words.txt' file contains a collection of common words separated by ','.
 *  common_file_load function loads words in 'common words.txt' into linked list of type 'common_list'.
 *  Returns address of first node of linked list containing common words.
 */
common_list* common_file_load(void)
{
    FILE *common_fptr=fopen("2_parsing_check_files/common_words.txt", "rb");
    if( file_error( "2_parsing_check_files/common_words.txt", common_fptr, common_file) )
        return NULL;
    
    fseek(common_fptr, 0, SEEK_END);
    uint common_file_size=(uint)ftell(common_fptr);
    char *buf=malloc(sizeof(char)*(common_file_size+1));
    malloc_chk(buf);
    rewind(common_fptr);

    fread(buf, common_file_size, 1, common_fptr);
    *(buf+common_file_size)='\0';
    fclose(common_fptr);

    common_list *list=NULL, *tail=NULL;

    char *common_word=strtok(buf, ",");
    while(common_word!=NULL)
    {
        common_list_insert(&list, &tail, common_word);  //  common_list_insert function is used to load words to linked list.
        common_word=strtok(NULL, ",");
    }
    free(buf);
    return list;
}

//  common_list_insert function is used to load words to linked list.
void common_list_insert(common_list **list, common_list **tail, char *word)
{
    if(*list==NULL)
    {
        *list=malloc(sizeof(common_list));
        malloc_chk(*list);

        (*list)->word=malloc( sizeof(char)*( strlen(word)+1 ) );
        malloc_chk((*list)->word);
        strcpy((*list)->word, word);

        *tail=*list;
        (*list)->next=NULL;

        return;
    }

    common_list *temp=malloc(sizeof(common_list));
    malloc_chk(temp);

    temp->word=malloc( sizeof(char)*( strlen(word)+1 ) );
    malloc_chk(temp->word);
    strcpy(temp->word, word);
    
    temp->next=NULL;
    (*tail)->next=temp;
    *tail=temp;
    return;

}


/*
 *  Function does the following:
 *  1. Checks whether the input word is a common word or not i.e. if it matches with any word from linked list.
 *  2. Returns 1 if word is a common word else 0.
 */
int common_word(char *word, common_list *list)
{
    common_list *temp=list;

    while(temp!=NULL)
    {
        if( strcmp(temp->word, word)==0 )
            return 1;
        temp=temp->next;
    }
    return 0;
}


