#ifndef MACHINE_H_
#define MACHINE_H_
#include "opcode.h"
#include "stack.h"
#include "heap.h"

typedef struct Machine{
    struct ClassFile* classFiles;
    //记录加载的类的个数
    uint64_t numClasses;
    //N个栈
    Stack* JVMSTACK;
    Heap* heap;
}Machine;

void* executeCode(Machine* machine, OPCODE** opcodes);
void destroyMachine(Machine* machine);

#endif