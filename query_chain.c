/*
 *  This code is for processing input query chain and performing subsequent AND/OR operations.
 *  'uint' and 'uchar' are typedefs defined in "typedef.h"
 *  malloc_chk function is used to check for dynamic memory allocation failure and subsequent proper exit from program.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "typedef.h"
#include "header.h"


/*
 *  Function: cli_chain_maker()
 *
 *  Purpose: To process command line arguments and form a linked list(chain) of words on which AND/OR operations are to be performed.
 *     
 *  Parameters: 
 *              argc        : command line arguments count
 *              argv        : command line arguments
 *              ht          : hash table
 *              chain       : linked list containing words on which AND/OR operations are to be performed
 *              nil         : no. of query terms not present in hash table
 *              and_n       : no. of 'and' operators present in query 
 *              or_n        : no. of 'or' operators present in query 
 *              delim_buf   : pointer to memory containing delimiters
 *              c_list      : linked list containing common words
 *
 *  Returns: no. of words/elements present in 'chain' linked list. 
 */

int cli_chain_maker( int argc, char *argv[], Htable *ht, cli_word_chain **chain, int *nil, int *and_n, int *or_n, char *delim_buf, common_list *c_list )
{
    char *word=NULL;
    
    //  Used to keep track whether command line argument in consideration is a word or operator.
    IN_type status=-1;

    /*
     *  i keeps the count of current command line argument in consideration
     *  j keeps count of words/elements in 'chain' linked list
     */
    int i=1, j=0;

    //  Empty query
    if(argc==1)
        return error_process(empty);

    //  When operators present at the start of query 
    if( ( strcmp(argv[1], "and")==0 ) || ( strcmp(argv[1], "or")==0 ) )
        return error_process(op_first);

    //  When operators present at the end of query 
    if( ( strcmp(argv[argc-1], "and")==0 ) || ( strcmp(argv[argc-1], "or")==0 ) )
        return error_process(op_last);

    while(i<argc)
    {   
        //  Copy of command line argument is made. 
        word=realloc( word, strlen(argv[i])+1 );
        strcpy(word, argv[i]);
        word=str_to_lower(word);    //  str_to_lower function converts string to lower case

        /*
         *  Checks performed to determine:
         *  1. Consecutive operators or words in input query
         *  2. Mixed operators present in query
         *  error_process function used to handle above mentioned errors.
         */
        if( strcmp(word, "and")==0 )
        {
            if(status==and_op)
                return error_process(and_again);
    
            if((*or_n)>0)
                return error_process(mixed_op);

            (*and_n)++;
            status=and_op;
            i++;
            continue;
        }
        if( strcmp(word, "or")==0 )
        {
            if(status==or_op)
                return error_process(or_again);
            if((*and_n)>0)
                return error_process(mixed_op);

            (*or_n)++;
            status=or_op;
            i++;
            continue;
        }
        if(status==_word)
            return error_process(word_again);

        /*
         *  Checks performed to determine if input word present in hash table.
         *  Checks performed using functions, 'in_word_processor' and 'cli_chain_insert'
         *  If word is present, it is added to the linked list.
         */
        if( in_word_processor( &word, delim_buf, c_list, argv[i] ) )
            status=_word;
        else if( cli_chain_insert(chain, word, ht)==0 )
        {
            printf("%s", argv[i]);
            error_process(word_absent);

            (*nil)++;
            status=_word;
        }
        else 
        {
            status=_word;
            j++;
        }
       
       
        i++;
        
    }
    free(word);

   return j;

}


/*
 *  Function to convert characters in an input string  to lower case.
 *  Returns modified string.
 */
char* str_to_lower(char *word)
{
    for(int i=0; i<strlen(word); i++)
        word[i]=lower_mod(word[i]);     //  lower_mod function is used to convert a character to lower case.  
    
    return word;
}


/*
 *  Function does the following:
 *  1. Strip the input word of delimiters(if present) at beginning and ending of the word.
 *  2. Check if word is a common word.
 *  3. Return 1 if word is a common word or a collection of delimiters, else 0.
 */
int in_word_processor(char **word, char *delim_buf, common_list *c_list, char *argv)
{
    char *temp=malloc(strlen(*word)+1);
    malloc_chk(temp);
    char *malloc_address=temp;

    strcpy(temp, *word);

    int i=0, temp_size=strlen(temp);

    //  Strip delimiters from the beginning of the word
    for(i=0; i<temp_size; i++)
    {
        if( delim_check(temp[i], delim_buf) )
            continue;
        else
            break;
    }
    temp=&temp[i];
    //  Strip delimiters from the ending of the word
    for(i=strlen(temp)-1; i>=0; i--)
    {
        if( delim_check(temp[i], delim_buf) )
        {   
            temp[i]='\0'; 
            continue;
        }
        else
            break;
    }

    //  If the word is a collection of delimiters, error_process function used to handle error.
    if(strlen(temp)==0)
    {
        printf("%s", argv);
        error_process(word_full_delim);
        free(malloc_address);
        return 1;
    }

    //  If the word is a common word, error_process function used to handle error.
    if( common_word(temp, c_list) )
    {
        printf("%s", argv);
        error_process(word_common);
        free(malloc_address);
        return 1;
    }

    *word=realloc(*word, strlen(temp)+1);
    strcpy(*word, temp);
    
    free(malloc_address);

    return 0;
}


/*
 *  Function does the following:
 *  1. Checks if input word present in hash table using 'find_word' function.
 *  2. If present, update the linked list containing words on which AND/OR operations are to be performed.
 *  3. Return 0 if word not present in hash table, else 1. 
 */
int cli_chain_insert(cli_word_chain **chain, char *word, Htable *ht)
{
    Entry *ht_entry=find_word(ht, word);

    if(ht_entry==NULL)
        return 0;
    
    cli_word_chain *link, *temp=*chain;

    link=malloc(sizeof(cli_word_chain));
    malloc_chk(link);

    link->word=malloc(sizeof(char)*(strlen(word)+1));
    malloc_chk(link->word);
    
    //  'posting_buffer' function outputs memory buffer containing doc_ids associated with the input word.
    link->buf_size=posting_buffer(ht_entry, &(link->buf));
    strcpy(link->word, word);

    //  Update the linked list at the head.
    if(temp==NULL)
    {  
        link->next=NULL;
        *chain=link;
    }
    else 
    {
        *chain=link;
        link->next=temp;
    }
    return 1;
}


/*
 *  Function to check whether input 'word' present in hash table.
 *  Returns address of hash table index if word is found, else NULL.
 */
Entry* find_word(Htable *ht, char *word)
{
    uint idx=hash(word)%table_size;

    if( ht->ht_entry[idx]==NULL )
        return NULL;

    Entry *word_entry=ht->ht_entry[idx];
    while(word_entry!=NULL)
    {
        if( strcmp(word_entry->word, word)==0 )
            break;

        word_entry=word_entry->ent_next;
    }
    if(word_entry==NULL)
        return NULL;

    return word_entry;
}


/*
 *  Function outputs memory buffer containing doc_ids associated with the input word.
 *  Returns the no. of doc_ids in the posting linked list associated with the input word.
 */
uint posting_buffer(Entry *word_entry, int **buf)
{
    uint i=0;   //  i counts the no. of doc_ids in the posting linked list associated with the input word.
    Node *temp=word_entry->posting;

    //  Loop to determine no. of doc_ids.
    while(temp!=NULL)
    {
        i++;
        temp=temp->n_next;
    }
    *buf=malloc( sizeof(int)*(i) );
    malloc_chk(*buf);

    //  Copying doc_ids to memory buffer.
    temp=word_entry->posting;
    i=0;
    while(temp!=NULL)
    {
        *(*buf+i)=temp->doc_id;
        i++;
        temp=temp->n_next;
    }

    return i;
}

/*
 *  Function does the following:
 *  1. Perform AND operations on doc_id memory buffers of each node in 'chain' linked list.
 *  2. At the beginning, AND operation performed on 1st and 2nd buffer, then between the result and 3rd buffer, and so on. 
 *  3. out_buf contains the final result.
 *  4. Returns the no. of doc_ids in the final result.
 */
uint loop_and_search(cli_word_chain *chain, int **out_buf, int size)
{
    int *in1=chain->buf, *in2;
    uint in1_size=chain->buf_size, in2_size;
    cli_word_chain *temp=chain->next;
    uint i=0;   //  i counts the no. of doc_ids in final result.

    if(chain!=NULL)
    {   
        while(temp!=NULL)
        {
            in2=temp->buf;
            in2_size=temp->buf_size;

            //  and_search function performs AND operation between 2 input buffers.
            i=and_search(in1, in1_size, in2, in2_size, out_buf);
            
            if(in1 != chain->buf)
                free(in1);

            if(*out_buf==NULL)
                break;
            
            in1=*out_buf;
            in1_size=i;
        
            temp=temp->next;
        }
    }

    return i;

}

/*
 *  Function does the following:
 *  1. Perform OR operations on doc_id memory buffers of each node in 'chain' linked list.
 *  2. At the beginning, OR operation performed on 1st and 2nd buffer, then between the result and 3rd buffer, and so on. 
 *  3. out_buf contains the final result.
 *  4. Returns the no. of doc_ids in the final result.
 */
uint loop_or_search(cli_word_chain *chain, int **out_buf, int size)
{
    int *in1=chain->buf, *in2;
    uint in1_size=chain->buf_size, in2_size;
    cli_word_chain *temp=chain->next;
    uint i=0;   //  i counts the no. of doc_ids in final result.

    if(chain!=NULL)
    {   
        while(temp!=NULL)
        {
            in2=temp->buf;
            in2_size=temp->buf_size;

            //  or_search function performs AND operation between 2 input buffers.
            i=or_search(in1, in1_size, in2, in2_size, out_buf);
            
            if(in1 != chain->buf)
                free(in1);

            if(*out_buf==NULL)
                break;
            
            in1=*out_buf;
            in1_size=i;
        
            temp=temp->next;
        }
    }

    return i;

}






/*
char* delim_strip(char *word, char *file_buf)
{
    char last=word[strlen(word)-1];
    
    if( delim_check(last, file_buf) )
    {
        printf("Delimiter '%c' is stripped from '%s', ", last, word);
        word[strlen(word)-1]='\0';
        printf("the searchable term will now be '%s'.\n", word);
        return word;
    }
    else
        return word;

}
*/


/*SORT ATTEMPT
void cli_list_sort(cli_word_chain **chain, int size)
{
    cli_word_chain *link=*chain, *temp;
    int i=0, j=0;

    for(i=0; i<size; i++)
    {
        for(j=0; j<size-i; j++)
        {
            if( (link->buf_size) > (link->next->buf_size) )
            {
                temp=link;
                link=link->next;
                link->next=temp;
            }
            link=link->next;
        }  
    }
}



BEfore internal body modification
uint cli_chain_maker( int argc, char *argv[], Htable *ht, cli_word_chain **chain, int *nil, int *and_n, int *or_n)
{
    IN_type status=-1;
    
    uint i=0;

    char *token=strtok(in_str, " ");
    while(token!=NULL)
    {
        if( strcmp(token, "AND")==0 ) 
        {
            (*and_n)++;
            token=strtok(NULL, " ");
            continue;
        }

        if(strcmp(token, "OR")==0) 
        {
            (*or_n)++;
            token=strtok(NULL, " ");
            continue;
        }
        
        if( cli_chain_insert(chain, token, ht)==0 )
        {
            printf("%s not present in index.\n", token);
            (*nil)++;
        }
        else 
            i++;

        token=strtok(NULL, " ");
        
    }
    return i;


}*/