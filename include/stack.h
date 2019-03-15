#ifndef STACK_H_
#define STACK_H_
#include <stdint.h>
#include "frame.h"

typedef enum StackType{
    //操作数栈值设置为 8
    TYPE_OPERANDSTACK = sizeof(uint64_t),
    // 设置为Frame的大小
    TYPE_JVMSTACK = sizeof(struct Frame)
}StackType;

typedef struct Stack{
    unsigned char* stack;
    StackType stackType;
    uint64_t top;
    uint64_t maxSize;
}Stack;

Stack initStack(uint64_t numItems, StackType stackType);
// size is given as numItems here

void* popStack(Stack* stack);
void pushStack(void* val, Stack* stack);
void* peekStack(Stack* stack);

void destroyStack(Stack* stack);
#endif 