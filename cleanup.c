//   This code defines cleanup functions for freeing dynamically allocated memeory.  

#include <stdio.h>
#include <stdlib.h>
#include "header.h"

extern _global g;

//  Function to free hash table memory.
void Htable_cleanup(Htable *ht)
{
    if(ht==NULL)
        return;
    
    Htable *temp;
    int i=0;

    while(i<table_size)
    {
        if(ht->ht_entry[i]!=NULL)
            Htable_entry_cleanup(ht->ht_entry[i]);  //  Function to free hash table index enrty.
        i++;
    }
    free(ht);

    return;
}

//  Function to free hash table index enrty.
void Htable_entry_cleanup(Entry *ht_entry)
{
    Entry *temp;

    while(ht_entry!=NULL)
    {
        temp=ht_entry->ent_next;
        free(ht_entry->word);
        posting_cleanup(ht_entry->posting);  //  Function to free posting linked list associated with word in hash table.
        ht_entry=temp;
    }
    return;
}

//  Function to free posting linked list associated with word in hash table.
void posting_cleanup(Node *posting)
{
    Node *temp;

    while(posting!=NULL)
    {
        temp=posting->n_next;
        free(posting);
        posting=temp;
    }
    return;
}

//  Function to free linked list made out of words in command line arguments.
void cli_word_chain_cleanup(cli_word_chain *chain)
{
    cli_word_chain *temp;

    while(chain!=NULL)
    {
        temp=chain->next;

        free(chain->word);
        free(chain->buf);
        free(chain);

        chain=temp;
    }
    return;
}

//  Function to clear common word memory buffer.
void common_list_cleanup(common_list *list)
{
    common_list *temp;

    while(list!=NULL)
    {
        temp=list->next;
        free(list->word);
        free(list);
        list=temp;
    }
    return;
}

//  Function to clear delimiter memory buffer.
void delim_buf_cleanup(char *buf)
{
    if(buf!=NULL)
        free(buf);
    
    return;
}

//  Function to free memory allocated throughout the program.
void cleanup()
{
    cli_word_chain_cleanup( g.cli_chain);
    Htable_cleanup( g.ht);
    common_list_cleanup( g.common_link);
    delim_buf_cleanup( g.delim_buf);
}
