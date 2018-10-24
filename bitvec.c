#include <stdio.h>
# include <assert.h>
#include <stdlib.h>
// create a new, empty bit vector set with a universe of 'size' items

struct bitset {
    unsigned int *bits;
    int size_in_bits;
    int size_in_words;
    int bits_per_word;
};
struct bitset * bitset_new(int size){
    struct bitset*result;
    int size_in_words;
    int i;
    int bits_per_word = (sizeof(unsigned int)*8);
    result = malloc(sizeof(struct bitset));
    size_in_words = size/ bits_per_word;
    if(size%bits_per_word!=0){
        size_in_words++;
    }
    result->bits = malloc(sizeof(unsigned int)*size_in_words);
    for(i =0;i<size_in_words;i++){
        result->bits[i]=0;
    }
    result ->size_in_bits = size;
    result->size_in_words = size_in_words;
    result->bits_per_word = bits_per_word;
    return result;
}

// get the size of the universe of items that could be stored in the set
int bitset_size(struct bitset * this){
    if(this != NULL){
        return this->size_in_bits;
    }
    return 0;
}

// get the number of items that are stored in the set
int bitset_cardinality(struct bitset * this){
    int count =0;
    if(this != NULL){
        for(int wordc = 0;wordc<this->size_in_words;wordc++){
            unsigned checker = this->bits[wordc];
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
int bitset_lookup(struct bitset * this, int item){
    if(this != NULL){
        int wordPos = item/(sizeof(unsigned int)*8);
        int bitToCheck = item % (sizeof(unsigned int)*8);
        unsigned int word = this->bits[wordPos];
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
int bitset_add(struct bitset * this, int item){
    if(this != NULL) {
        int word_index = item/this->bits_per_word;
        int bit_index = item%this->bits_per_word;
        unsigned mask = 1;
        mask<<=bit_index;
        this->bits[word_index] |= mask;
        return 1;
    }
}

// remove an item with number 'item' from the set
int bitset_remove(struct bitset * this, int item){
    if(this!=NULL && item<0){
        int word = item/this->bits_per_word;
        int bitPos = item%this->bits_per_word;
        unsigned int mask = ~(1<<bitPos);
        this->bits[word] &= mask;
        return 1;
    }
}

// place the union of src1 and src2 into dest
void bitset_union(struct bitset * dest, struct bitset * src1,struct bitset * src2){
   int i;
   assert(dest != NULL && src1 != NULL && src2 != NULL);
   assert(src1->size_in_bits == src2->size_in_bits );
   for(i =0;i<src1->size_in_words;i++){
       dest->bits[i] = src1->bits[i]|src2->bits[i];
   }
}

// place the intersection of src1 and src2 into dest
void bitset_intersect(struct bitset * dest, struct bitset * src1,struct bitset * src2){
    int i;
    assert(dest != NULL && src1 != NULL && src2 != NULL);
    assert(src1->size_in_bits == src2->size_in_bits );
    for(i =0;i<src1->size_in_words;i++){
        dest->bits[i] = src1->bits[i]&src2->bits[i];
    }
}


//print the contents of a bitset ------------- this is a testing function
void bitset_print(struct bitset *this){
    int size = bitset_size(this);
    for(int i = 0; i < size; i++){
        // printf("%d = %d ", i, bitset_lookup(this, i));
        if(bitset_lookup(this, i) == 1){
            printf("%d ", i);
            printf("foundAThing ");
        }
    }
    printf("\n");
}

//add the characters from a string to the bitset ------------- this is a testing function
//charArray must be null terminated
void add_chars_to_set(struct bitset *this, char *charArray){
    for(int i = 0; charArray[i] != 0; i++){
        unsigned tmp = charArray[i];
        bitset_add(this, tmp);
    }
}

//Small routine to test a bitset  ------------- this is a testing function
void myTest(){
    struct bitset *a = bitset_new(256);
    struct bitset *b = bitset_new(256);
    struct bitset *c = bitset_new(256);
    char *string1 = "What can you hear";
    char *string2 = "Nothing but the rain";

    add_chars_to_set(a, string1);
    add_chars_to_set(a, string2);

    //print the contents of the sets
    bitset_print(a);
    bitset_print(b);

    //compute and print the union of sets
    bitset_union(c, a, b);
    bitset_print(c);

    //compute and print the intersection of sets
    bitset_intersect(c, a, b);
    bitset_print(c);
}

int main() {
    myTest();
    //test
}