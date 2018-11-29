// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include "huff.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

// create a new huffcoder structure
struct huffcoder *  huffcoder_new()
{
  struct huffcoder * huffCoder1 = malloc(sizeof(huffCoder1));
  huffCoder1->tree = malloc(sizeof(huffCoder1->tree));
}

// count the frequency of characters in a file; set chars with zero
// frequency to one
void huffcoder_count(struct huffcoder * this, char * filename)
{
    FILE *file = fopen(filename,"r");
    char curChar;
    char *cur;
    cur = &curChar;
    for(int count = 0; count< 256;count++){
        this->freqs[count] = 1;
    }
    if(file) {
        while(*cur != EOF){
            *cur = fgetc(file);
            int charNum = *cur;
            this->freqs[charNum] = this->freqs[charNum] +1;
        }
    }

}
struct huffchar * new_huffchar(int freq, int seqNum, int isCompound){
    struct huffchar * newHuffchar = malloc(sizeof(struct huffchar));
    newHuffchar->freq = freq;
    newHuffchar->seqno = seqNum;
    newHuffchar->is_compound = isCompound;
}
void ArrSort(struct huffchar * sortArr[], int n){
    int i,j;
    struct huffchar * key = malloc(sizeof(struct huffchar));
    for(i =1;i<n;i++){
        *key = *sortArr[i];
        j = i-1;
        while(j>=0 && (sortArr[j]->freq > key->freq)){
            *sortArr[j+1] = *sortArr[j];
            j = j-1;
        }
        *sortArr[j+1] = *key;
    }
}
// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this)
{
    struct huffchar * huffArr[NUM_CHARS];
    for(int i =0;i<NUM_CHARS;i++){
        huffArr[i] = new_huffchar(this->freqs[i],i,0);
    }
    int n = sizeof(huffArr)/ sizeof(huffArr[0]);
    ArrSort(huffArr,n);
    int num = 0;
    int fir = 0;
    int sec = 1;
    int index = 1;
    int extindex = 1;
    while( huffArr[254] != NULL){
        struct huffchar * start1 = huffArr[fir];
        struct huffchar * start2 = huffArr[sec];
        struct huffchar * comp = new_huffchar(start1->freq+start2->freq,256-num,1);
        if(start1->freq < start2->freq){
            *comp->u.compound.left = *start1;
            *comp->u.compound.right = *start2;
        }
        else if(start1->freq > start2->freq){
            *comp->u.compound.left = *start2;
            *comp->u.compound.right = *start1;
        }
        else{
            if(start1->seqno < start2->seqno){
                *comp->u.compound.left = *start1;
                *comp->u.compound.right = *start2;
            }
            else{
                *comp->u.compound.left = *start2;
                *comp->u.compound.right = *start1;
            }
        }
        bool inserted = false;
        huffArr[fir] = NULL;
        while(!inserted) {
            if(index != 255){
                if (huffArr[index]->freq <= comp->freq) {
                    *huffArr[index] = *huffArr[index + 1];
                }
            }
            else {
                if (!inserted) {
                    *huffArr[index] = *comp;
                    inserted = true;
                }
            }
            index++;
        }
        num++;
        extindex++;
        index = extindex;
        fir++;
        sec++;
    }
    this->tree = huffArr[255];
}


int reverseBits(int value,int depth){
    unsigned int num = 0;
    unsigned int revNum;
    int noOfShift = depth;
    for(int i =0;i<noOfShift;i++){
        num = (value & (1<<i));
        if(num){
            revNum |= (1<<((noOfShift-1)-i));
        }
    }
    return revNum;
}
// recursive function to convert the Huffman tree into a table of
// Huffman codes
void tree2table_recursive(struct huffcoder * this, struct huffchar * node,
		 int * path, int depth){
    int value;
    value = *path;
    if(node -> is_compound == 0){
        this->code_lengths[node->seqno] =depth;
        value = reverseBits(value,depth);
        this->codes[node->seqno] = value;
    }
    else{
        value = value*2;
        *path = value;
        tree2table_recursive(this,node->u.compound.left,path,depth+1);
        value = value+1;
        *path = value;
        tree2table_recursive(this,node->u.compound.right,path,depth+1);
    }
}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder * this)
{
    int depth;
    int left = depth =0;
    int right =1;
    int *p1,*p2;
    p1,p2 = malloc(sizeof(int));
    *p1 = left;
    *p2 = right;
    tree2table_recursive(this,this->tree->u.compound.left,p1,depth+1);
    tree2table_recursive(this,this->tree->u.compound.right,p2,depth+1);
}


// print the Huffman codes for each character in order
void huffcoder_print_codes(struct huffcoder * this)
{
  int i, j;
  char buffer[NUM_CHARS];

  for ( i = 0; i < NUM_CHARS; i++ ) {
    // put the code into a string
    for ( j = this->code_lengths[i]-1; j >= 0; j--) {
      buffer[j] = (this->codes[i] >> j) & 1 + '0';
    }
    // don't forget to add a zero to end of string
    buffer[this->code_lengths[i]] = '\0';

    // print the code
    printf("char: %d, freq: %d, code: %s\n", i, this->freqs[i], buffer);;
  }
}
