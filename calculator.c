#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <memory.h>

struct node {
    double data;
    struct node *next;
};

struct nodePointer {
    struct node *head;
};

struct nodePointer *newnodePointer() {
    struct nodePointer *nodeone;
    nodeone = malloc(sizeof(struct nodePointer));
    nodeone->head = NULL;
    return nodeone;
};

void pushStart(struct nodePointer *list, double data) {
    struct node *node = malloc(sizeof(struct node));
    node->data = data;
    node->next = list->head;
    list->head = node;
}

void pushEnd(struct nodePointer *list, double data){
  struct node *node = malloc(sizeof(struct node));
  struct node *tmpHead = list->head;
  if (list->head != NULL){
    while(tmpHead->next != NULL){
      tmpHead = tmpHead->next;
    }
    tmpHead->next = node;
  }
  else{
    list->head = node;
  }
  node->data = data;
  node->next = NULL;
}

double pop(struct nodePointer *list) {
    double num = (list->head)->data;
    struct node *tmpHead = list->head;
    list->head = (list->head)->next;
    free(tmpHead);
    return num;
}
double popEnd(struct nodePointer *list){
    struct node *tmpHead = list->head;
    if(list->head != NULL){
        while(tmpHead-> next != NULL) {
            tmpHead = tmpHead->next;
        }
    }
    double num = tmpHead->data;
    free(tmpHead);
    return num;
}

void multiply(struct nodePointer *list) {
    double num1 = pop(list);
    double num2 = pop(list);
    num1 = num1 * num2;
    pushStart(list, num1);
}

void divide(struct nodePointer *list) {
    double num1 = pop(list);
    double num2 = pop(list);
    num1 = num2 / num1;
    pushStart(list, num1);
}

void add(struct nodePointer *list) {
    double num1 = pop(list);
    double num2 = pop(list);
    num1 = num1 + num2;
    pushStart(list, num1);
}

void subtract(struct nodePointer *list) {
    double num1 = pop(list);
    double num2 = pop(list);
    num1 = num2 - num1;
    pushStart(list, num1);
}

void powerOf(struct nodePointer *list) {
    double numA = pop(list);
    double numB = pop(list);
    numA = pow(numB, numA);
    pushStart(list, numA);
}

void normalise(double *numA, double numB) {
    int n = 1;
    while(numB > pow(10,n)){
        n++;
    }
    *numA = *numA * pow(10,n);
    *numA = *numA + numB;
}

void evalInput(struct nodePointer *list, struct nodePointer *opList) {
    double operand = pop(opList);
    if (operand == 'X') {
        multiply(list);
    } else if (operand == '+') {
        add(list);
    } else if (operand == '/') {
        divide(list);
    } else if (operand == '-') {
        subtract(list);
    } else if (operand == '^') {
        powerOf(list);
    } else if (operand == '*'){
        double numB = pop(list);
        double numA = pop(list);
        double *num = &numA;
        normalise(num,numB);
        pushStart(list,*num);
    }
}

void postfix(struct nodePointer *list, struct nodePointer *opList, char *passChar, bool isbigNum, int numOfRun, bool isPostfix) {
    if(isPostfix){
         *passChar = *passChar + '0';
         if(isdigit(*passChar) == 0){
             *passChar = *passChar - '0';
         }
    }
    if (!isbigNum && numOfRun > 0) {
        if (isdigit(*passChar)) {
            double numA = pop(list);
            double *numP = &numA;
            double otherNum = *passChar - '0';
            normalise(numP, otherNum);
            pushStart(list, *numP);
        }
    } else if (isdigit(*passChar)) {
        double data = *passChar -'0';
        pushStart(list, data);
    }
    else {
        double data = *passChar;
        pushStart(opList, data);
        evalInput(list, opList);
    }
}

void infix(struct nodePointer *RPN, double passChar,struct nodePointer *opList,bool isBigNum, int numOfRun) {
    if(passChar == '('){
        pushStart(opList,passChar);
    }
    else if(passChar == ')'){
        while(opList->head != NULL && opList->head->data != '('){
            pushEnd(RPN,pop(opList));
        }
        if(opList->head != NULL){
            pop(opList);
        }
    }
    else if (passChar == '^'){ //check precedence
        while (opList->head != NULL && opList->head->data == '^'){ //check precedence
            pushEnd(RPN, pop(opList));
        }
        pushStart(opList, passChar);
    }else if (passChar == 'X' || passChar == '/'){ //check precedence
        while (opList->head != NULL && opList->head->data == '^'){ //check precedence
            pushEnd(RPN, pop(opList));
        }
        pushStart(opList, passChar);//push to stack
    }else if (passChar == '-' || passChar == '+'){ //check precedence
        while (opList->head != NULL && (opList->head->data == '^' || opList->head->data == 'X' || opList->head->data == '/')){ //check precedence
            pushEnd(RPN, pop(opList));
        }
        pushStart(opList, passChar);//push to stack
    }else {
        if (!isBigNum && numOfRun > 0) {
            if (isdigit(passChar)) {
                double num = passChar - '0';
                pushEnd(RPN,num);
                pushEnd(RPN,'*');
            }
        }
        else {
            double num = passChar - '0'; //convert to double
            pushEnd(RPN, num);
        }
    }
}
void clearOplist(struct nodePointer *opList,struct nodePointer *rpnList){
    if(opList->head != NULL){
        while((opList->head) != NULL){
            pushEnd(rpnList,pop(opList));
        }
    }else{
        pushEnd(rpnList,pop(opList));
    }
}
int main(int argc, char ** argv) {
    char * filename;

    if ( argc == 1 ) {
        printf("Error: No input filename provided\n");
        printf("Usage: %s <input filename>\n", argv[0]);
        exit(1);
    }
    else if ( argc > 2 ) {
        printf("Error: Too many command line parameters\n");
        printf("Usage: %s <input filename>\n", argv[0]);
        exit(1);
    }
    else {
        filename = argv[1];
    }

    char initChar;
    char *init;
    init = &initChar;
    FILE *file;
    FILE *outFile;
    file = fopen(filename, "r");
    char *newFile = strcat(filename, ".results");
    outFile = fopen(newFile,"w");
    if (file){
    initChar = fgetc(file);
    struct nodePointer *list = newnodePointer();
    struct nodePointer *opList = newnodePointer();
    struct nodePointer *rpnList = newnodePointer();
    struct nodePointer *rpnOpList = newnodePointer();
    bool isPostfix = false;
    bool isbigNum = false;
    int numOfRun = -1;
    if (*init == 'p') {
        while (*init != ' ') {
            *init = fgetc(file);
        }
        isPostfix = false;
    }
    else if(*init == 'i'){
        while(*init != ' '){
            *init = fgetc(file);
        }
        isPostfix = true;
    }
    *init = fgetc(file);
    while (*init != '\n' && *init != EOF) {
        if (isdigit(*init) == 0) {
            isbigNum = true;
            numOfRun = -1;
        } else {
            isbigNum = false;
            numOfRun++;
        }
        if (*init != ' ') {
            if(!isPostfix){
                postfix(list, opList, init, isbigNum, numOfRun,isPostfix);
            }
            else{
                double passer = *init;
                infix(rpnList,passer,rpnOpList,isbigNum,numOfRun);
            }
        }
        *init = fgetc(file);
        if(isPostfix && (*init == '\n'|| *init == EOF)){
            clearOplist(rpnOpList,rpnList);
            while(rpnList->head != NULL){
                char charpass = pop(rpnList);
                char *pass = &charpass;
                postfix(list,opList,pass,false,0,isPostfix);
            }
        }
    }
    printf("The answer is: %f", (list->head)->data);
}
    }