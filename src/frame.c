#include "frame.h"
#include <stdlib.h>
#include "classFile.h"
#include "attribute.h"

Frame createNewFrame (Code_attribute code, ClassFile* cf, Machine* machine){
    Frame newFrame;

    //Code_attribute pointer
    newFrame.code = malloc (sizeof(Code_attribute));
    *newFrame.code = code;
    
    // 分配本地方法表
    newFrame.localVariables = malloc (sizeof(LocalVariable) * newFrame.code->max_locals);
    
    //操作数栈
    newFrame.operandStack = malloc (sizeof(Stack));
    //初始化操作数栈   TYPE_OPERANDSTACK = 8
    *newFrame.operandStack = initStack(newFrame.code->max_stack, TYPE_OPERANDSTACK);

    //pc置零
    newFrame.pc = 0;
    //当前类的引用 
    newFrame.classRef = cf;
    newFrame.machine = machine;
    return newFrame;
}

void destroyFrame (Frame* frame){
    free(frame->localVariables);
    free(frame->code);
    destroyStack(frame->operandStack);
    // dont free the frame struct itself. It contains an address into JVMStack.
    // JVMStack is responsible for freeing or overwriting it.
}