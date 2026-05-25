 /*
 *  Code for inserting a word into hash table
 *  'uint' and 'uchar' are typedefs defined in "typedef.h"
 *  malloc_chk function is used to check for dynamic memory allocation failure and subsequent proper exit from program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedef.h"
#include "header.h"


/*
 *  Function for generating hash key.
 *  DJB2 algorithm is used.
 *  Returns the hash key.
 */
uint hash(char *word)
{
    uint hash=5381;
    uint c;

    while((c=(uchar)*word++))
        hash=((hash<<5)+hash)+c;

    return hash;
}



//  function for inserting word into index
void insert(Htable *ht, char *word, int doc_id)
{
    uint idx = hash(word)%table_size;
    
    /*
     *  Check if any data already present at index 'idx'.     
     *  If no data present, update the table.
     */
    if(ht->ht_entry[idx]==NULL)
    {
        ht->ht_entry[idx]=malloc(sizeof(Entry));
        malloc_chk(ht->ht_entry[idx]);
        
        ht->ht_entry[idx]->word=malloc(sizeof(char)*( strlen(word) + 1 ));
        malloc_chk(ht->ht_entry[idx]->word);
        strcpy(ht->ht_entry[idx]->word, word);

        ht->ht_entry[idx]->ent_next=NULL;

        ht->ht_entry[idx]->posting=malloc(sizeof(Node));
        malloc_chk(ht->ht_entry[idx]->posting);

        ht->ht_entry[idx]->posting->doc_id=doc_id;

        ht->ht_entry[idx]->posting->n_next=NULL;

        return;
    }
    
    /*
     *  Check if word present at idx matches the input word.
     *  Traverse linked list of Entry data type to check if word already present
     */
    Entry *temp1=ht->ht_entry[idx];
    while(temp1 != NULL) 
    {
        if( strcmp(temp1->word, word)==0 )
            break;

        temp1=temp1->ent_next;
    }
    
    /*
     *  If word not present at index 'idx';
     *  update the linked list at the head. 
     */    
    if(temp1 == NULL)
    {
        Entry *temp4=ht->ht_entry[idx];

        ht->ht_entry[idx]=malloc(sizeof(Entry));
        malloc_chk(ht->ht_entry[idx]);

        ht->ht_entry[idx]->word=malloc(sizeof(char)*( strlen(word) + 1 ));
        malloc_chk(ht->ht_entry[idx]->word);
        strcpy(ht->ht_entry[idx]->word, word);

        ht->ht_entry[idx]->ent_next=temp4;

        ht->ht_entry[idx]->posting=malloc(sizeof(Node));
        malloc_chk(ht->ht_entry[idx]->posting);

        ht->ht_entry[idx]->posting->doc_id=doc_id;

        ht->ht_entry[idx]->posting->n_next=NULL;
        
        return;
    }

    /*
     *  If word present;
     *  traverse the posting type linked list to check if input doc_id alraedy present. 
     */
    Node *temp2=temp1->posting;
    while(temp2 != NULL)
    {
        if( temp2->doc_id == doc_id )
            return;
                    
        temp2=temp2->n_next;
    }

    /*
     *  If doc_id not present;
     *  upadte the posting linked list at the head.
     */
    Node *temp3=temp1->posting;

    temp1->posting=malloc(sizeof(Node));
    malloc_chk(temp1->posting);

    temp1->posting->doc_id=doc_id;

    temp1->posting->n_next=temp3;

    return;

}















/*inside common_word
    uint file_buf_size=0, i=0;
    do
    {
        file_buf_size++;
    }while(*(file_buf+file_buf_size)!='\0');

    char *buf=malloc(sizeof(char)*(file_buf_size));
    strcpy(buf, file_buf);
*/


//INSERT ATTEMPT 2
/* void insert(Htable *ht, char *word, int doc_id)
{
    uint idx = hash(word)%table_size;
    
    ht->ht_entry[idx]=malloc(sizeof(Entry));
    ht->ht_entry[idx]->posting=malloc(sizeof(Node));

    Htable *temp1=malloc(sizeof(Htable));
    temp1->ht_entry[idx]=malloc(sizeof(Entry));
    temp1->ht_entry[idx]->posting=malloc(sizeof(Node));
    
    Entry *t_entry=malloc(sizeof(Entry));
    Node *t_posting=malloc(sizeof(Node));

    if(ht->ht_entry[idx]==NULL)
    {
        ht->ht_entry[idx]->word=word;
        ht->ht_entry[idx]->posting->doc_id=doc_id;
        ht->ht_entry[idx]->posting->n_next=NULL;

        return;
    }

    temp1=ht;
    while( (temp1->ht_entry[idx]!=NULL) && ( strcmp(temp1->ht_entry[idx]->word, word)!=0 ) )
    {
        temp1->ht_entry[idx]=temp1->ht_entry[idx]->ent_next;
    }
    
    if(temp1->ht_entry[idx]==NULL)
    {
        t_entry=ht->ht_entry[idx];
        ht->ht_entry[idx]->ent_next=t_entry;
        ht->ht_entry[idx]->word=word;
        ht->ht_entry[idx]->posting->doc_id=doc_id;
        ht->ht_entry[idx]->posting->n_next=NULL;

        return;
    }

    while(temp1->ht_entry[idx]->posting != NULL)
    {
        if( temp1->ht_entry[idx]->posting->doc_id == doc_id )
            return;
        
        temp1->ht_entry[idx]->posting = temp1->ht_entry[idx]->posting->n_next;
    }

    t_posting=ht->ht_entry[idx]->posting;
    ht->ht_entry[idx]->posting->doc_id=doc_id;
    ht->ht_entry[idx]->posting->n_next=t_posting;
    return;

} */


//INSERT ATTEMPT 1
/*void insert(Htable* ht, char* word, int doc_id)
{
    uint idx=hash(word);
    Htable *temp1=ht, *temp2=ht;
    
    
    if( strcmp( (ht->ht_entry[idx]->word) , word) == 0 )
    {
        if( (ht->ht_entry[idx]->posting->doc_id) == doc_id )
            return;
        else
        {
            while( (temp1->ht_entry[idx]->posting) != NULL )
            {
                if( (temp1->ht_entry[idx]->posting->doc_id) == doc_id )
                    return;

                temp1->ht_entry[idx]->posting = temp1->ht_entry[idx]->posting->n_next;
            }

            temp1=ht;
            ht->ht_entry[idx]->posting->doc_id=doc_id;
            ht->ht_entry[idx]->posting->n_next=temp1->ht_entry[idx]->posting;

            return;

        }
    }
    else 
    {
        temp1=ht;
        while( (temp1->ht_entry[idx]) != NULL  )
        {
            if( strcmp( (temp1->ht_entry[idx]->word) , word) == 0 )
            {
                if( (temp1->ht_entry[idx]->posting->doc_id) == doc_id )
                    return;
                else
                {
                    while( (temp1->ht_entry[idx]->posting) != NULL )
                    {
                        if( (temp1->ht_entry[idx]->posting->doc_id) == doc_id )
                            return;

                        temp1->ht_entry[idx]->posting = temp1->ht_entry[idx]->posting->n_next;
                    }

                    temp2=temp1;
                    temp1->ht_entry[idx]->posting->doc_id=doc_id;
                    temp1->ht_entry[idx]->posting->n_next=temp2->ht_entry[idx]->posting;

                    return;

                }
                
            }
            temp1->ht_entry[idx]=temp1->ht_entry[idx]->ent_next;

        }
        
        ht->ht_entry[idx]->word=word;
        ht->ht_entry[idx]->posting->doc_id=doc_id;
        ht->ht_entry[idx]->posting->n_next=NULL;
        ht->ht_entry[idx]->ent_next=NULL;

        return;
    }
}*/