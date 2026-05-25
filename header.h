#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include "typedef.h"

#define table_size 1000 
#define word_size 46

//  Indexing words in a file into a hash table

/*
 *  Hash table has value of Entry struct type at each index.
 *  
 *  Entry struct contains the following:
 *  1. Word assigned to that particular index.
 *  2. List of doc_ids containing the word(Using linked list of Node struct type).
 *  3. Address of next Entry struct type data containing a different word assigned to same index.    
 *  4. Direct chaining collision handling is used.    
 */
typedef struct Node
{
    int doc_id;
    struct Node *n_next;

}Node;


typedef struct Entry
{
    char *word;
    Node *posting;
    struct Entry *ent_next;

}Entry;

typedef struct 
{
    Entry *ht_entry[table_size];
}Htable; 



//  struct common_list  is for storing words within "common words.txt" to memory.
typedef struct common_list
{
    char *word;
    struct common_list *next;

}common_list;


/*
 *  struct cli_word_chain contains:
 *  1. input searchable term.
 *  2. pointer to address of contigous memory locations containing doc_ids.
 *  3. count of doc_ids.    
 *  4. address of next cli_word_chain type data.         
 */
typedef struct cli_word_chain
{
    char *word;
    uint buf_size;
    int *buf;
    struct cli_word_chain *next;

}cli_word_chain;


//stuct _global contains core data structures of inverted index; intended for global use.
typedef struct _global
{
    Htable *ht;
    cli_word_chain *cli_chain;
    char *delim_buf;
    common_list *common_link;

}_global;


//Markers to inidicate the type of input command line arguments
typedef enum 
{
    and_op, or_op, _word
}IN_type;


//Markers to indicate invalid input queries and for following error handling
typedef enum
{
    op_first,
    op_last,
    mixed_op,
    and_again,
    or_again,
    word_again,
    and_null,
    empty,
    word_absent,
    word_full_delim,
    word_common,
    file_big_word
}error_handling;


//Markers to indicate file opening errors and for following error handling
typedef enum
{
    indexing_file,
    delim_file,
    common_file
}file_open;



//Functions for inserting a word into hash table
void Htable_initialize(Htable **ht);

uint hash(char *word);

void insert(Htable *ht, char *word, int doc_id);


//Functions for indexing words in a file into hash table 
void index_file(Htable *ht, char *filename, int doc_id, char *delim_buf, common_list *list);

char lower_mod(char in_char);

char* delim_file_load();

int delim_check(char in_char, char *file_buf);

void common_list_insert(common_list **list, common_list **tail, char *word);

common_list* common_file_load();

int common_word(char *word, common_list *list);


//Functions for performing single AND/OR operations
Entry* find_word(Htable *ht, char *word);

uint posting_buffer(Entry *word_entry, int **buf);

uint and_search(int *in1, uint in1_size, int *in2, uint in2_size, int **buf);

uint or_search(int *in1, uint in1_size, int *in2, uint in2_size, int **buf);


//Functions for processing input query into a word chain 
int cli_chain_maker( int argc, char *argv[], Htable *ht, cli_word_chain **chain, int *nil, int *and_n, int *or_n, char *delim_buf, common_list *c_list );

int cli_chain_insert(cli_word_chain **chain, char *word, Htable *ht);

uint loop_and_search(cli_word_chain *chain, int **out_buf,int size);

uint loop_or_search(cli_word_chain *chain, int **out_buf, int size);

char* str_to_lower(char *word);

char* delim_strip(char *word, char *file_buf);

int in_word_processor(char **word, char *delim_buf, common_list *c_list, char *argv);


//Functions for sorting word chain for effiecient operations
cli_word_chain* split_half(cli_word_chain *head);

cli_word_chain* inner_merge_sort(cli_word_chain *first, cli_word_chain *second);

cli_word_chain* merge_sort(cli_word_chain *head);


//Allocated memory cleanup functions 
void posting_cleanup(Node *posting);

void Htable_entry_cleanup(Entry *ht_entry);

void Htable_cleanup(Htable *ht);

void common_list_cleanup(common_list *list);

void cli_word_chain_cleanup(cli_word_chain *chain);

void delim_buf_cleanup(char *buf);

void cleanup();



//Functions to display/handle errors
int error_process(error_handling i);

int file_error(char *filename, FILE *ptr, file_open error);

void malloc_chk(void *ptr);



#endif
