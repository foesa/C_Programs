#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "bitset.h"


// create a new, empty bit vector set with a universe of 'size' items
struct bitset * bitset_new(int size)
{
    struct bitset*result;
    int size_in_words;
    int i;
    int bits_in_word = (sizeof(unsigned int)*8);
    result = malloc(sizeof(struct bitset));
    size_in_words = size/ bits_in_word;
    if(size%bits_in_word!=0){
        size_in_words++;
    }
    result->data = malloc(sizeof(unsigned int)*size_in_words);
    for(i =0;i<size_in_words;i++){
        result->data[i]=0;
    }
    result ->size_in_bits = size;
    result->size_in_words = size_in_words;
    result->bits_in_word = bits_in_word;
    return result;    
}

// get the size of the universe of items that could be stored in the set
int bitset_size(struct bitset * this)
{
    if(this != NULL){
        return this->size_in_bits;
    }
    return 0;
}

// get the number of items that are stored in the set
int bitset_cardinality(struct bitset * this)
{
    int count =0;
    if(this != NULL){
        for(int wordc = 0;wordc<this->size_in_words;wordc++){
            unsigned checker = this->data[wordc];
            while(checker != 0){
                checker &= checker -1;
                count++;
            }
        }
    }
    printf("%d",count);
    return count;
}

// check to see if an item is in the set
int bitset_lookup(struct bitset * this, int item)
{
    if(this != NULL){
        int wordPos = item/(sizeof(unsigned int)*8);
        int bitToCheck = item % (sizeof(unsigned int)*8);
        unsigned int word = this->data[wordPos];
        unsigned int mask = 1<<bitToCheck;
        mask&=word;
        mask = mask>>bitToCheck;
        if(mask == 1){
            return 1;
        }
    }
    return 0;
}

// add an item, with number 'item' to the set
// has no effect if the item is already in the set
void bitset_add(struct bitset * this, int item){
    if(this != NULL) {
        int word_index = item/this->bits_in_word;
        int bit_index = item%this->bits_in_word;
        unsigned mask = 1;
        mask<<=bit_index;
        this->data[word_index] |= mask;
    }
}

// remove an item with number 'item' from the set
void bitset_remove(struct bitset * this, int item)
{
    if(this!=NULL && item<0){
        int word = item/this->bits_in_word;
        int bitPos = item%this->bits_in_word;
        unsigned int mask = ~(1<<bitPos);
        this->data[word] &= mask;
    }
}

// place the union of src1 and src2 into dest;
// all of src1, src2, and dest must have the same size universe
void bitset_union(struct bitset * dest, struct bitset * src1,
		  struct bitset * src2)
{
    int i;
    assert(dest != NULL && src1 != NULL && src2 != NULL);
    assert(src1->size_in_bits == src2->size_in_bits );
    for(i =0;i<src1->size_in_words;i++){
        dest->data[i] = src1->data[i]|src2->data[i];
    }
}

// place the intersection of src1 and src2 into dest
// all of src1, src2, and dest must have the same size universe
void bitset_intersect(struct bitset * dest, struct bitset * src1,
		      struct bitset * src2)
{
    int i;
    assert(dest != NULL && src1 != NULL && src2 != NULL);
    assert(src1->size_in_bits == src2->size_in_bits );
    for(i =0;i<src1->size_in_words;i++){
        dest->data[i] = src1->data[i]&src2->data[i];
    }
}


// print the contents of the bitset
void bitset_print(struct bitset * this)
{
  int i;
  int size = bitset_size(this);
  for ( i = 0; i < size; i++ ) {
    if ( bitset_lookup(this, i) == 1 ) {
      printf("%d ", i);
    }
  }
  printf("\n");
}
