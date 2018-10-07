#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

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

/*void pushEnd(struct nodePointer *list, double data){
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
  double numB = pop(list);
  pushStart(list, numA);
} */

double pop(struct nodePointer *list) {
    double num = (list->head)->data;
    struct node *tmpHead = list->head;
    list->head = (list->head)->next;
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
    *numA = *numA * 10;
    *numA += numB;
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
    }
}

void postfix(struct nodePointer *list, struct nodePointer *opList, char *passChar, bool isbigNum, int numOfRun) {
    if (!isbigNum && numOfRun > 0) {
        if (isdigit(*passChar)) {
            double numA = pop(list);
            double *numP = &numA;
            normalise(numP, *numP);
            pushStart(list, *numP);
        }
    } else if (isdigit(passChar)) {
        double data = *passChar;
        pushStart(list, data);
    } else {
        double data = *passChar;
        pushStart(opList, data);
        evalInput(list, opList);
    }
}

void infix() {
}

double main() {
    char initChar;
    char *init;
    init = &initChar;
    FILE *file;
    file = fopen("/home/foesa/C-Programs/input.txt", "r");
    initChar = fgetc(file);
    struct nodePointer *list = newnodePointer();
    struct nodePointer *opList = newnodePointer();
    struct nodePointer *rpnList = newnodePointer();
    char str[] = "geeksers";
    printf("%c %s", *init, str);
    if (*init == 'p') {
        printf("Got here");
        while (*init != ' ') {
            *init = fgetc(file);
        }
        bool isbigNum = false;
        int numOfRun = -1;
        while (*init != '\n' || *init != EOF) {
            *init = fgetc(file);
            if (isdigit(*init) == 0) {
                isbigNum = true;
                numOfRun = -1;
            } else {
                isbigNum = false;
                numOfRun++;
            }
            if (*init != ' ') {
                postfix(list, opList, init, isbigNum, numOfRun);
            }
        }
    }
    printf("The answer is: %f", (list->head)->data);
}
