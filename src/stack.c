#include "stack.h"
#include "frame.h"
#include <inttypes.h>
#include <stdlib.h>
#include "javaClass.h"
Stack initStack(uint64_t numItems, StackType stackType){ // size is in operands
    //The maximum depth of the operand stack of a frame is determined at compile-time and 
    //is supplied along with the code for the method associated with the frame
    Stack stack;
    stack.maxSize = stackType * numItems; // in bytes
    stack.top = 0; // in bytes, again
    stack.stackType = stackType;
    stack.stack = malloc(stackType * numItems); // afaik all stacks in here are frame sized. leak
    return stack;
}
 
void pushStack(void* val, Stack* stack){
    if (stack->top + stack->stackType <= stack->maxSize){
        // 新的栈帧
        if (stack->stackType == TYPE_JVMSTACK){
            *(Frame*)(&stack->stack[stack->top])=*(Frame*)val;
        }else if(stack->stackType == TYPE_OPERANDSTACK){
            // uint64_t unsigned long long 8 bytes
            // 取到栈顶指针,转换为(unsigned long long) 类型的指针
            // 把val值赋给栈顶
            *(uint64_t*)(&stack->stack[stack->top])=*(uint64_t*)val; 
            // this stack is implemented at DWORD size on 32 bit machines
            // QWORD size on 64 bit machines. On the other hand, the values we 
            // push on operand stack always 16 bit.
        } 
        //栈顶移动 stack->stackType个
        stack->top += stack->stackType;
    }else{
        // todo stackoverflow err
        printf("StackOverFlow Error\n");
    }
}

void* popStack(Stack* stack){
    //栈不空
    if ((int64_t)stack->top - stack->stackType >= 0){
        void* val;
        stack->top -= stack->stackType;
        if (stack->stackType == TYPE_JVMSTACK){
            val = (Frame*)(&stack->stack[stack->top]);
        }else if(stack->stackType == TYPE_OPERANDSTACK){
            val = (uint64_t*)(&stack->stack[stack->top]);
        }
        return val;
    }else{
        // todo stack empty
        return NULL;
    }
}

void* peekStack(Stack* stack){
    return (stack->top > 0)?(Stack*)(stack->stack + stack->top - stack->stackType):(NULL);
}

void destroyStack(Stack* stack){
    free(stack->stack);
    free(stack);
}