/*
 *  This code is for performing AND and OR operations on memory buffers associated with input words.
 *  'uint' and 'uchar' are typedefs defined in "typedef.h"
 *  malloc_chk function is used to check for dynamic memory allocation failure and subsequent proper exit from program.
 */

#include <stdio.h>
#include <stdlib.h>
#include "typedef.h"
#include "header.h"


/*
 *  Function logic requires input buffers(in1 & in2) to be sorted.
 *  'buf' contains the result.
 *  Returns the no. of doc_ids present in the result.
 */
uint and_search(int *in1, uint in1_size, int *in2, uint in2_size, int **buf)
{
    uint  i1=0, i2=0, j=0;  //  j counts the no. of doc_ids in result.

    if(in1_size>=in2_size)
        *buf=malloc(sizeof(int)*(in2_size));
    else
        *buf=malloc(sizeof(int)*(in1_size));
         
    malloc_chk(*buf);
    
    while( (i1<in1_size) && (i2<in2_size) )
    {       
        //  Check and update result buffer only if matching doc_ids present.
        if( *(in1+i1)==*(in2+i2) )
        {
            *(*buf+j)=*(in1+i1);
            j++;
            i1++;
            i2++;       
        }
        else if( *(in1+i1)>*(in2+i2) )
            i1++;
        else
            i2++;          
    }
    //When no matching doc_ids are found.
    if(j==0)
    {
        free(*buf);
        *buf=NULL;
    }    
    else
        *buf=realloc(*buf, sizeof(int)*j);
    
    return j;    
}


/*
 *  Function logic requires input buffers(in1 & in2) to be sorted.
 *  'buf' contains the result.
 *  Returns the no. of doc_ids present in the result.
 */
uint or_search(int *in1, uint in1_size, int *in2, uint in2_size, int **buf)
{
    uint  i1=0, i2=0, j=0;  //  j counts the no. of doc_ids in result.

    *buf=malloc(sizeof(int)*(in1_size+in2_size));
    malloc_chk(*buf);  
   
    while( (i1<in1_size) && (i2<in2_size) )
    {       
        /*
         *  Update doc_ids to the result buffer.
         *  If matching doc_ids found, update result buffer once.
         */
        if( *(in1+i1)==*(in2+i2) )
        {
            *(*buf+j)=*(in1+i1);
            j++;
            i1++;
            i2++;       
        }
        else if( *(in1+i1)>*(in2+i2) )
        {
            *(*buf+j)=*(in1+i1);
            i1++;
            j++;
        }    
        else
        {
            *(*buf+j)=*(in2+i2);
            i2++;
            j++;
        }             
    }
    while(i1<in1_size)
    {
        *(*buf+j)=*(in1+i1);
        i1++;
        j++;
    }
    while(i2<in2_size)
    {
        *(*buf+j)=*(in2+i2);
        i2++;
        j++;
    }
    if( j<(in1_size+in2_size) )
        *buf=realloc(*buf, sizeof(int)*j);
    
    return j;

    
}



/*2nd ATTEMPT
uint and_search(int *in1, uint in1_size, int *in2, uint in2_size, int **buf)
{
    uint temp_size, i1=0, i2=0, j=0;
    int *temp;
    
    if(in1_size>=in2_size)
    {
        *buf=malloc(sizeof(int)*(in2_size));  
    }
    else
    {
        *buf=malloc(sizeof(int)*(in1_size));
        
        temp_size=in1_size;
        in1_size=in2_size;
        in2_size=temp_size;

        temp=in1;
        in1=in2;
        in2=temp;
    }
    
    
    while(i2<in2_size)
    {
        if(i1<in1_size)
        {
            if( *(in1+i1)==*(in2+i2) )
            {
                *(*buf+j)=*(in1+i1);
                j++;
                i1++;
                i2++;
                continue;
            }
            if( *(in1+i1)>*(in2+i2) )
            {
                i1++;
                continue;
            }
            i2++;
            continue;
           
        }
        i2++;
    }
    if(j==0)
    {
        free(*buf);
        *buf=NULL;
    }    
    else
        *buf=realloc(*buf, sizeof(int)*j);
    
    return j;

    
}

*/


/*1st ATTEMPT
uint and_search(int *in1, uint in1_size, int *in2, uint in2_size, int **buf)
{
    uint limit, i1=0, i2=0, j=0;
    
    if(in1_size>=in2_size)
        *buf=malloc(sizeof(int)*(in2_size));
    else
        *buf=malloc(sizeof(int)*(in1_size));
    
    while( (i1<in1_size) || (i2<in2_size) )
    {
        if( *(in1+i1)==*(in2+i2) )
        {
            *(*buf+j)=*(in1+i1);
            j++;
            
            if(i1<(in1_size-1))
                i1++;
            
            if(i2<(in2_size-1))
                i2++;

            continue;
        }
        if( *(in1+i1)>*(in2+i2) )
        {
            if(i1<(in1_size-1))
                i1++;
            continue;
        }
        if(i2<(in2_size-1))
            i2++;

        if( (i1==in1_size-1) && (i2==in2_size-1) )
            break;
    }
    buf=realloc(buf, sizeof(int)*j);
    return j;

    
}

*/
