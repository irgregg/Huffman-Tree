#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "quiz8.h"
#include<assert.h>

#define DEBUG 0
int tm = 0;

struct HTreePriorityQueue{
  HTree* queue[2 * NUM_ASCII_CHARS];
  int position;
};

typedef struct HTreePriorityQueue PQ;

struct HTreeStack{
  int size;
  HTree* elements[2 * NUM_ASCII_CHARS];
};
typedef struct HTreeStack Stack;

PQ* createPQ(){
  tm++;
  PQ* pq = malloc(sizeof(PQ));
  pq -> position = 0;
  return pq;
}

void findPath(char** store, HTree* tree, char* path){
  if(tree -> c == 0){
    strcat(path,"0");
    findPath(store,tree->p0,path);
    path[strlen(path) -1] ='\0';
    strcat(path,"1");
    findPath(store,tree->p1,path);
    path[strlen(path) - 1] = '\0';
  } else {
    char* p = malloc(sizeof(char) * strlen(path) + 1);
    strcpy(p, path);
    store[tree->c] = p;
    //  path[strlen(path) - 1] = '\0';
  }
}

char** createLT(HTree* input){
  char** t = malloc(sizeof(char*) * NUM_ASCII_CHARS);
  tm++;
  int i;
  for(i = 0; i < NUM_ASCII_CHARS; i++){
    t[i] = "";
  }
  if(input == NULL) return t;
  char path[16];
  path[0] = '\0';
  findPath(t,input,path);
  return t;
}

void destroyPQ(PQ* pq){
  free(pq);
  tm--;
}

void bubble(PQ* pq, int pos){
  int p = (pos-1)/2;
  if(pq->queue[pos]->freq < pq->queue[p]->freq){
    HTree* temp = pq->queue[p];
    pq->queue[p] = pq->queue[pos];
    pq->queue[pos] = temp;
    bubble(pq, p);
  }
}

void enqueue(HTree* h, PQ* pq){
  pq -> queue[pq -> position] = h;
  bubble(pq , pq -> position++);
}

void sink(PQ* pq, int pos){
  HTree* temp;
  int minPos;
  if(pq->position <= 2 * pos) return;
  if(pq->position == 2*pos+1){
    minPos = 2*pos+1;
  } else if(pq->position >= 2*pos+2){
    if(pq->queue[pos*2+1]->freq <= pq->queue[pos*2+2]->freq){
      minPos = pos*2+1;
    } else{
      minPos = pos*2+2;
    }
  }
  if(minPos <= pq->position){
    if(pq->queue[pos]->freq > pq->queue[minPos]->freq){
      temp = pq->queue[minPos];
      pq->queue[minPos] = pq->queue[pos];
      pq->queue[pos] = temp;
      sink(pq, minPos);
    }
  }
}

HTree* dequeue(PQ* pq){
  HTree* temp = pq->queue[0];
  pq->queue[0] = pq->queue[--pq->position];
  sink(pq,0);
  return temp;
}

HTree* createEncodingTree(FreqTable* table){
  int i;
  HTree* toAdd;
  PQ* allItems = createPQ();
  HTree* left;
  HTree* right;
  HTree* toReturn;
  for(i = 0; i < NUM_ASCII_CHARS; i++){
    if(table->charCount[i] > 0){
    toAdd = malloc(sizeof(HTree));
    tm++;
    toAdd -> c = i;
    toAdd -> freq = table->charCount[i];
    toAdd -> p0 = NULL;
    toAdd -> p1 = NULL;
    enqueue(toAdd,allItems);
    }
  }
  if(allItems->position == 0){
    destroyPQ(allItems);
    return NULL;
  }
  while(allItems->position > 1){
    left = dequeue(allItems);
    right = dequeue(allItems);
    toAdd = malloc(sizeof(HTree));
    tm++;
    toAdd -> c = 0;
    toAdd -> freq = left->freq + right->freq;
    toAdd -> p0 = left;
    toAdd -> p1 = right;
    enqueue(toAdd,allItems);
  }
  toReturn = dequeue(allItems);
  destroyPQ(allItems);
  return toReturn;
}

void printHTree(HTree* toPrint){
  if(toPrint == NULL) return;
  if(toPrint -> c == 0){
    printHTree(toPrint->p0);
    printf("Null ");
    printHTree(toPrint->p1);
  } else{
    printf("%c ",toPrint->c);
  }
}

void destroyHTree(HTree* toKill){
  if(toKill != NULL){
    destroyHTree(toKill->p0);
    destroyHTree(toKill->p1);
    free(toKill);
    tm--;
  }
}

FreqTable* createTable(){
  FreqTable* table = malloc(sizeof(FreqTable));
  tm++;
  return table;
}

FreqTable* createFreqTable(char * in){
  FreqTable* table = createTable();
  int i;
  for(i = 0 ; i < NUM_ASCII_CHARS; i++){
    table -> charCount[i] = 0;
  }
  while(*in){
    table -> charCount[*in]++;
    in++;
  }
  return table;
}

void printFreqTable(FreqTable* table){
  int i;
  for(i = 0; i < NUM_ASCII_CHARS; i++){
    if(table -> charCount[i] > 0){
      printf("%c count : %d\n", i, table->charCount[i]);
    }
  }
}

void destroyFreqTable(FreqTable* table){
  tm--;
  free(table);
}

BitSeq* createBitSeq(){
  tm++;
  BitSeq* sequence = malloc(sizeof(BitSeq));
  sequence -> numBitsInSeq = 0;
  int i;
  for(i = 0; i < MAX_BIT_SEQ_LENGTH; i++){
    sequence -> bits[i] = 0;
  }
  return sequence;
}

void destroyBitSeq(BitSeq* sequence){
  tm--;
  free(sequence);
}

void displayBits(unsigned short value,int numBits){
  unsigned c, displayMask = 1 << 15; 

  // printf("%3c%7u = ", value, value); 
  
  for(c=1; c<=numBits; c++){
    putchar(value & displayMask ? '1' : '0'); 
    value <<= 1; 
    
    if (c % 8 == 0){
      putchar(' '); 
    }
  }
  putchar('\n');
}

void printBitSeq(BitSeq* b){
  int i = 0;
  while(b->bits[i] > 0){
    displayBits(b->bits[i],16);
    i++;
  }
}

void packBits(BitSeq* seq, char* newBitsAsChars){
  int i;
  int index;
  int arrayPosition;
  int total;
  unsigned short x;
  for(i = 0; newBitsAsChars[i] > 0; i++){
    total = seq->numBitsInSeq;
    index = total % 16;
    arrayPosition = total / 16;
    x = newBitsAsChars[i] - '0';
    seq->bits[arrayPosition] ^= (x << (15 - index));
    seq->numBitsInSeq++;
  }
}

HTree* encode(char* input, BitSeq* bit){
  //  printf("got here");
  FreqTable* table = createFreqTable(input);
  HTree* tree = createEncodingTree(table);
  //destroyFreqTable(table);
  //printf("got here");
  char** lt = createLT(tree);
  //printf("got here");
  int i;
  for(i = 0; input[i] != 0; i++){
    packBits(bit,lt[input[i]]);
  }
  return tree;
}

char* decode(BitSeq* bit, HTree* huff){
  int i;
  HTree* temp = huff;
  char* decoded = malloc(MAX_MESSAGE_LENGTH * sizeof(char));
  //printf("got here");
  int c;
  for(c = 0; c< MAX_MESSAGE_LENGTH; c++){
    decoded[c] = 0;
  }
  int index;
  int place;
  int storage;
  int count = 0;
  unsigned short x = 1;
  for(i = 0; i < bit->numBitsInSeq; i++){
    storage = bit->bits[i/16];
    storage &= (x << (15 - i % 16));
    //   printf("got here");
    if(storage == 0){
      temp = temp -> p0;
    } else {
      temp = temp -> p1;
    }
    if(temp->c != 0){
      decoded[count++] = temp -> c;
      //    printf("Got here");
      temp = huff;
    }
  }
  return decoded;
}


void pqUnitTest(){
  PQ* pq = createPQ();                // PriorityQueue Tests
  assert(tm == 1);
  assert(pq != NULL);
  
  HTree* temp = malloc(sizeof(HTree));
  temp -> c = 'c';
  temp -> freq = 5;
  temp -> p0 = NULL;
  temp -> p1 = NULL;
  HTree* test = malloc(sizeof(HTree));
  test -> c = 'a';
  test -> freq = 11;
  test->p0 = NULL;
  test->p1 =NULL;
  enqueue(temp,pq);
  assert(pq->queue[0] == temp);
  enqueue(test,pq);
  assert(pq->position == 2);
  assert(pq->queue[0] == temp);
  assert(pq->queue[1] == test);
  HTree* test2 = malloc(sizeof(HTree));
  test2 -> c = ' ';
  test2 -> freq = 6;
  test2 -> p0 = NULL;
  test2 -> p0 = NULL;
  enqueue(test2, pq);
  assert(pq->queue[2] == test2);
  enqueue(test2,pq);
  assert(pq->queue[0] == temp);
  assert(pq->queue[1] == test2);
  assert(pq->queue[2] == test2);
  assert(pq->queue[3] == test);
  HTree* test3 = dequeue(pq);
  assert(test3 == temp);
  assert(pq->queue[0] == test2);
  assert(pq->queue[1] == test);
  assert(pq->queue[2] == test2);
  test3 = dequeue(pq);
  assert(pq->queue[0] == test2);
  assert(pq->queue[1] == test);
  destroyPQ(pq);
  assert(tm == 0);

  
}

void hTreeUnitTest(){
  FreqTable* table = createFreqTable("aab");
  HTree* t = createEncodingTree(table);
  //  printf("%d\n",tm);
  assert(tm == 4);
  assert(t -> freq == 3);
  assert(t -> p0 -> c == 'b');
  //  printHTree(t);
  destroyHTree(t);
  destroyFreqTable(table);
  assert(tm == 0);
  FreqTable* big = createFreqTable("It was the best of times");
  HTree* bigTest = createEncodingTree(big);
  // printf("%d\n",tm);
  assert(bigTest->freq == 24);
  assert(bigTest->p0->p0->c == ' ');
  //  printHTree(bigTest);
  assert(tm == 26);
  destroyHTree(bigTest);
  destroyFreqTable(big);
  assert(tm == 0);
}

void bitSeqTest(){
  BitSeq* b = createBitSeq();
  char* toAdd = "we";
  packBits(b,toAdd);
  //  printf("%d\n",b->bits[0]);
  //printf("%d\n",toAdd[2]);
  assert(b->bits[0] == 30565);
  destroyBitSeq(b);
  BitSeq* big = createBitSeq();
  char* toAddBig = "It was the worst of times";
  packBits(big,toAddBig);
  assert(big->bits[0] == 18804);
  assert(big->bits[1] == 8311);
  assert(big->bits[12] == 29440);
  assert(big->bits[13] == 0);
  // printBitSeq(big);
  destroyBitSeq(big);
  assert(tm == 0);
}

void ltTest(){
  FreqTable* table = createFreqTable("this is a longer thing");
  HTree* tree = createEncodingTree(table);
  // printHTree(tree);
  char** t = createLT(tree);
  //  assert(tm == 33);
  printf("%s\n ",t['s']);
  printf("%s\n ",t['n']);
  //  assert(t['a'] == "1");
}

void encodeTest(){
  BitSeq* b = createBitSeq();
  encode("It was the worst of times",b);
  //printBitSeq(b);
}

void decodeTest(){
  BitSeq* b = createBitSeq();
  HTree* h = encode("It was the best of times",b);
  char* test = decode(b,h);
  printHTree(h);
  printf("%s\n",test);
}

void unitTest(){
  assert(tm == 0);
  
  FreqTable * table = createTable();  //createTable()
  assert(table != NULL);
  assert(tm == 1);
  
  destroyFreqTable(table);            //destroyFreqTable(FreqTable)
  assert(tm == 0);
  
  FreqTable * t = createFreqTable("aab");  //createFreq(String)
  assert(t->charCount['a'] == 2);
  assert(t->charCount['b'] == 1);
  assert(t->charCount[' '] == 0);
  destroyFreqTable(t);

  pqUnitTest();                       //PQ Tests

  hTreeUnitTest();                    //HTree Tests
  
  BitSeq* b = createBitSeq();         //createBitSeq()
  assert(tm == 1);
  assert(b ->numBitsInSeq == 0);
  assert(b -> bits[15] == 0);
  destroyBitSeq(b);                   //destroyBitSeq(BitSeq)
  assert(tm == 0);

  //  bitSeqTest();                       //BitSeq Tests

   encodeTest();

   decodeTest();

   ltTest();                          //LookupTable Tests
}

/*int main(){
  // printf("%d\n",(-1/2));
  unitTest();
  char* c = "It was the best of times";
  FreqTable* table = createFreqTable(c);
  // printf("%p\n",table);
  //printFreqTable(table);
  return 0;
  }*/






