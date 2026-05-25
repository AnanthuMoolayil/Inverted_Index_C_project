/*
 *  This code is for sorting linked list made out of words from input query.
 *  Sorting based on size of buffers containing do_ids.
 *  Merge Sort algorithm is used.
 *  Sorting done in order to make performance of chained AND/OR operations efficient.
 */
#include <stdio.h>
#include "header.h"

 
/*
 *  Function to split linked list into two halves. 
 *  Fast and Slow pointer algorithm is used.
 */
cli_word_chain* split_half(cli_word_chain *head)
{
    if( (head==NULL) || (head->next==NULL) )
        return head;
    
    cli_word_chain *second, *fast=head->next, *slow=head;   //  'fast' starts from the second node so that second half starts from second middle. 

    while( (fast!=NULL) && (fast->next!=NULL) )
    {
        slow=slow->next;
        fast=fast->next->next;
    }
    second=slow->next;
    slow->next=NULL;

    return second;
}

//  Sorting & merging the split halves.
cli_word_chain* inner_merge_sort(cli_word_chain *first, cli_word_chain *second)
{
    if(first==NULL)
        return second;
    if(second==NULL)
        return first;

    //  Sorting based on ascending oreder of doc_id buffer size.
    if(first->buf_size<=second->buf_size)
    {
        first->next=inner_merge_sort(first->next, second);
        return first;
    }
    else 
    {
        second->next=inner_merge_sort(second->next, first);
        return second;
    }
}

//  Merge sort of input linked list.
cli_word_chain* merge_sort(cli_word_chain *head)
{
    if( (head==NULL) || (head->next==NULL) )
        return head;

    cli_word_chain *second=split_half(head);

    head=merge_sort(head);
    second=merge_sort(second);

    return inner_merge_sort(head, second);

}