// code for a huffman coder


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include "huff.h"
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
    for(int count = 0; count< 256;count++){
        this->freqs[count] = 1;
    }
    while(curChar != EOF){
        curChar = fgetc(file);
        int charNum = curChar;
        this->freqs[charNum] = this->freqs[charNum] +1;
    }

}
struct huffchar * new_huffchar(int freq, int seqNum, int isCompound){
    struct huffchar * newHuffchar = malloc(sizeof(struct huffchar));
    newHuffchar->freq = freq;
    newHuffchar->seqno = seqNum;
    newHuffchar->is_compound = isCompound;
}
void ArrSort(struct huffchar * sortArr[]){
    
}
// using the character frequencies build the tree of compound
// and simple characters that are used to compute the Huffman codes
void huffcoder_build_tree(struct huffcoder * this)
{
    struct huffchar * huffArr[NUM_CHARS];
    for(int i =0;i<NUM_CHARS;i++){
        huffArr[i] = new_huffchar(this->freqs[i],i,0);
    }
    ArrSort(huffArr);
    int size = NELEMS(huffArr);
    int num = 0;
    while( huffArr[1] != NULL){
        struct huffchar * start1 = huffArr[0];
        struct huffchar * start2 = huffArr[1];
        struct huffchar * comp = new_huffchar(start1->freq+start2->freq,256-num,1);
        struct huffchar * arr2[NUM_CHARS];
        int start =2;
        bool inserted = false;
        for(int count =0;count<256-num;count++){
            if(huffArr[start]->freq <= comp->freq){
                huffArr[count] = huffArr[start];
                start++;
            }
            else if(huffArr[start]->freq > comp->freq ){
                if(!inserted){
                    huffArr[count]= comp;
                    inserted = true;
                }
                else{
                    huffArr[count] = huffArr[start];
                    start++;
                }
            }
        }
    }
}

// recursive function to convert the Huffman tree into a table of
// Huffman codes
void tree2table_recursive(struct huffcoder * this, struct huffchar * node,
		 int * path, int depth)
{

}

// using the Huffman tree, build a table of the Huffman codes
// with the huffcoder object
void huffcoder_tree2table(struct huffcoder * this)
{

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
