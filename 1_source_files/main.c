#include <stdio.h>
#include <stdlib.h>
#include "header.h"
#include "typedef.h"


_global g={0};

int main(int argc, char *argv[])
{       
    //  Hash table initialization
    g.ht=calloc(1, sizeof(Htable));
    malloc_chk(g.ht);
    
    //  Loading delimiters to memory
    g.delim_buf=delim_file_load();
    if( g.delim_buf==NULL)
        goto L1;

    //   Loading common words to memory
    g.common_link=common_file_load();
    if( g.common_link==NULL)
        goto L1;
    
    //  Indexing files
    index_file( g.ht, "3_files_for_indexing/file_1.txt", 1,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_2.txt", 2,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_3.txt", 3,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_4.txt", 4,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_5.txt", 5,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_6.txt", 6,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_7.txt", 7,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_8.txt", 8,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_9.txt", 9,  g.delim_buf,  g.common_link);
    index_file( g.ht, "3_files_for_indexing/file_10.txt", 10,  g.delim_buf,  g.common_link);


    
    int nil=0, and_n=0, or_n=0;
    uint cli_chain_size;

    //  Input query parsing
    cli_chain_size=cli_chain_maker(argc, argv,  g.ht, &( g.cli_chain), &nil, &and_n, &or_n,  g.delim_buf,  g.common_link);
    //  Invalid query handling
    if(cli_chain_size==0)
        goto L1;
    
    //  Single word query
    if(cli_chain_size==1)
    {
        printf("Search results:\n");

        for( int i= ((g.cli_chain)->buf_size)-1 ; i>=0; i--)
            printf("file_%d \n", (g.cli_chain)->buf[i]);
        
        goto L1;
    }
    
    //  Sorting query chain
    g.cli_chain=merge_sort( g.cli_chain);

    int *out_buf;
    uint out_buf_size;

    //  Boolean chain operations
    if(and_n>0)
        out_buf_size=loop_and_search( g.cli_chain, &out_buf, cli_chain_size);
    else if(or_n>0)
        out_buf_size=loop_or_search( g.cli_chain, &out_buf, cli_chain_size);
    
    //  Display result
    if(out_buf!=NULL)
    {
        printf("Search results:\n");

        for( int i=out_buf_size-1; i>=0; i--)
            printf("file_%d \n", *(out_buf+i));
        
        free(out_buf);
    }
    else
        printf("Nothing common");
      
L1: cleanup();
      
    return 0;
}





/*
if( (and_n>0) && (and_n!=(cli_chain_size-1)) )
    and_n=cli_chain_size-1;
else if( (or_n>0) && (or_n!=(cli_chain_size-1)) )
    or_n=cli_chain_size-1;
*/
