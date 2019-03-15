#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include <endian.h>
#include "constantPool.h"
#include "frame.h"
#include "classFile.h"
#include <string.h>
#include "javaClass.h"
#include "debug.h"
#include "printStream.h"
#include "stringBuilder.h"
#include <math.h>

uint8_t read1Byte(Frame* frame){
    return *(uint8_t*)&frame->code->code[++frame->pc]; // to get the first arg. of the opcode
}
uint16_t read2Bytes(Frame* frame){
    /*
    OP  A1  A2
    00  01  02
    ^
    */
    uint16_t val=be16toh(*(uint16_t*)&frame->code->code[frame->pc+1]);
    // pc自动增加
    frame->pc += 2;
    return val;
}
uint32_t read4Bytes(Frame* frame){
    uint32_t val=be32toh(*(uint32_t*)&frame->code->code[frame->pc+1]);
    frame->pc += 4;
    return val;
}
uint64_t read8Bytes(Frame* frame){
    uint64_t val=be64toh(*(uint64_t*)&frame->code->code[frame->pc+1]);
    frame->pc += 8;
    return val;
}
void* OPCODE_ICONST_M1(Frame* frame){
    uint64_t val = -1;
    //把valpush到操作数栈
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_0(Frame* frame){
    uint64_t val = 0;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_1(Frame* frame){
    uint64_t val = 1;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_2(Frame* frame){
    uint64_t val = 2;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_3(Frame* frame){
    uint64_t val = 3;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_4(Frame* frame){
    uint64_t val = 4;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_ICONST_5(Frame* frame){
    uint64_t val = 5;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_DCONST_0(Frame* frame){
    double val = 0.0;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_DCONST_1(Frame* frame){
    double val = 1.0;
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_DCONST_2(Frame* frame){
    double val = 2.0;
    pushStack(&val,frame->operandStack);
	return NULL;
}

void* OPCODE_BIPUSH(Frame* frame){
    uint64_t val = read1Byte(frame);
    pushStack(&val,frame->operandStack);
	return NULL;
}
void* OPCODE_SIPUSH(Frame* frame){
    uint64_t val = read2Bytes(frame);
    pushStack(&val,frame->operandStack);
	return NULL;
}
//load constant from run time constant 
void* OPCODE_LDC(Frame* frame){
    uint8_t index = read1Byte(frame);
    cp_info* aConstant = &frame->classRef->constant_pool[index-1];
    if (aConstant->tag == CONSTANT_Integer || aConstant->tag == CONSTANT_Float){
        uint64_t val;
        val = aConstant->info._4BYTES_info.bytes;
        pushStack(&val, frame->operandStack);
    }else{
        pushStack(&aConstant, frame->operandStack);
        /*
        Otherwise, if the run-time constant pool entry is a reference to an instance of class 
        String representing a string literal (§5.1), then a reference to that instance, value,
        is pushed onto the operand stack.
        
        Otherwise, if the run-time constant pool entry is a symbolic reference to a class (§5.1),
        then the named class is resolved (§5.4.3.1) and a reference to the Class object 
        representing that class, value, is pushed onto the operand stack.

        Otherwise, the run-time constant pool entry must be a symbolic reference to a method type
        or a method handle (§5.1). The method type or method handle is resolved (§5.4.3.5) and a
        reference to the resulting instance of java.lang.invoke.MethodType or 
        java.lang.invoke.MethodHandle, value, is pushed onto the operand stack. 
        */
    }
    return NULL;
}
void* OPCODE_LDC_W(Frame* frame){
    uint16_t index = read2Bytes(frame);
    cp_info* aConstant = &frame->classRef->constant_pool[index-1];
    if (aConstant->tag == CONSTANT_Integer || aConstant->tag == CONSTANT_Float){
        uint64_t val;
        val = aConstant->info._4BYTES_info.bytes;
        pushStack(&val, frame->operandStack);
    }else if(aConstant->tag == CONSTANT_Long || aConstant->tag == CONSTANT_Double){
        uint64_t val;
        val = aConstant->info._8BYTES_info.bytes;
        pushStack(&val, frame->operandStack);
    }else{
        pushStack(&aConstant, frame->operandStack);
    }
	return NULL;
}

// load
void* OPCODE_NLOAD(Frame* frame){ // N represents I,L or D
    uint8_t index = read1Byte(frame);
    pushStack(&frame->localVariables[index], frame->operandStack);
	return NULL;
}
<N>load0
void* OPCODE_NLOAD_0(Frame* frame){
    pushStack(&frame->localVariables[0], frame->operandStack);
	return NULL;
}
void* OPCODE_NLOAD_1(Frame* frame){
    pushStack(&frame->localVariables[1], frame->operandStack);
	return NULL;
}
void* OPCODE_NLOAD_2(Frame* frame){
    pushStack(&frame->localVariables[2], frame->operandStack);
	return NULL;
}
void* OPCODE_NLOAD_3(Frame* frame){
    pushStack(&frame->localVariables[3], frame->operandStack);
	return NULL;
}
void* OPCODE_NSTORE(Frame* frame){
    uint8_t index = read1Byte(frame);
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[index] = val;
	return NULL;
}

//store0
void* OPCODE_NSTORE_0(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[0] = val;
	return NULL;
}
//store1
void* OPCODE_NSTORE_1(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[1] = val;
	return NULL;
}
//store2
void* OPCODE_NSTORE_2(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[2] = val;
	return NULL;
}
//store3
void* OPCODE_NSTORE_3(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    frame->localVariables[3] = val;
	return NULL;
}
void* OPCODE_POP(Frame* frame){
    //popStack(frame->operandStack);
    int64_t discardedRetVal = *(int64_t*) popStack(frame->operandStack);
    DEBUG_PRINT(("Discarded Return Value is %d\n",discardedRetVal));
	return NULL;
}
//dup
void* OPCODE_DUP(Frame* frame){
    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DUP_X1(Frame* frame){
    uint64_t val1 = *(uint64_t*)popStack(frame->operandStack);
    uint64_t val2 = *(uint64_t*)popStack(frame->operandStack);
    pushStack(&val1, frame->operandStack);
    pushStack(&val2, frame->operandStack);
    pushStack(&val1, frame->operandStack);
	return NULL;
}

/////////////
// INTEGER //
/////////////
void* OPCODE_IADD(Frame* frame){
    int64_t val = (int32_t)*(int64_t*)popStack(frame->operandStack) + (int32_t)*(int64_t*)popStack(frame->operandStack); // java has no unsigned types, lol
    // the stacks are 64 bit but the int size is 32 bit
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_ISUB(Frame* frame){
    int32_t val2 = *(int64_t*) popStack(frame->operandStack);
    int32_t val1 = *(int64_t*) popStack(frame->operandStack);
    int64_t val = val1 - val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_IMUL(Frame* frame){
    int64_t val = (int32_t)*(int64_t*)popStack(frame->operandStack) * (int32_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_IDIV(Frame* frame){
    int32_t val2 = *(int64_t*) popStack(frame->operandStack);
    int32_t val1 = *(int64_t*) popStack(frame->operandStack);
    int64_t val = val1 / val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
/////////////
///  LONG ///
/////////////
void* OPCODE_LADD(Frame* frame){
    int64_t val = *(int64_t*)popStack(frame->operandStack) + *(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_LSUB(Frame* frame){
    int64_t val2 = *(int64_t*) popStack(frame->operandStack);
    int64_t val1 = *(int64_t*) popStack(frame->operandStack);
    int64_t val = val1-val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_LMUL(Frame* frame){
    int64_t val = *(int64_t*)popStack(frame->operandStack) * *(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_LDIV(Frame* frame){
    int64_t val2 = *(int64_t*) popStack(frame->operandStack);
    int64_t val1 = *(int64_t*) popStack(frame->operandStack);
    int64_t val = val1 / val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
/////////////
/// DOUBLE //
/////////////
void* OPCODE_DADD(Frame* frame){
    double val = *(double*)popStack(frame->operandStack) + *(double*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DSUB(Frame* frame){
    double val2 = *(double*) popStack(frame->operandStack);
    double val1 = *(double*) popStack(frame->operandStack);
    double val = val1-val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DMUL(Frame* frame){
    double val = *(double*)popStack(frame->operandStack) * *(double*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DDIV(Frame* frame){
    double val2 = *(double*) popStack(frame->operandStack);
    double val1 = *(double*) popStack(frame->operandStack);
    double val = val1 / val2;
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_D2F(Frame* frame){
    float val = (float)*(double*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_D2I(Frame* frame){
    int64_t val = (int32_t)*(double*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_D2L(Frame* frame){
    int64_t val = (int64_t)*(double*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DNEG(Frame* frame){
    double val = -*(double*) popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}
void* OPCODE_DREM(Frame* frame){
    double val2 = *(double*) popStack(frame->operandStack);
    double val1 = *(double*) popStack(frame->operandStack);
    double val = fmod(val1,val2);
    pushStack(&val, frame->operandStack);
	return NULL;
}

///////////////

void* OPCODE_I2D(Frame* frame){
    double val = (double)(int32_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2F(Frame* frame){
    float val = (float)(int32_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2B(Frame* frame){
    int64_t val = (int8_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2C(Frame* frame){
    //char, whose values are 16-bit unsigned integers representing Unicode code points ...
    uint64_t val = (uint16_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2L(Frame* frame){
    uint64_t val = (int32_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_I2S(Frame* frame){
    uint64_t val = (int16_t)*(int64_t*)popStack(frame->operandStack);
    pushStack(&val, frame->operandStack);
	return NULL;
}

///////////////

void* OPCODE_LCMP(Frame* frame){
    uint64_t val2 = *(uint64_t*) popStack(frame->operandStack);
    uint64_t val1 = *(uint64_t*) popStack(frame->operandStack);
    int64_t val = 0;
    if (val1 > val2){
        val = 1;
    }else if (val1 == val2){
        val = 0;
    }else {
        val = -1;
    }
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_IFEQ(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 == 0){
        frame->pc += offset - 3;
    }
    return NULL;
}
void* OPCODE_IFNE(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 != 0){
        frame->pc += offset - 3;
    }
    return NULL;
}
void* OPCODE_IFLT(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 < 0){
        frame->pc += offset - 3;
    }
    return NULL;
}
void* OPCODE_IFLE(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 <= 0){
        frame->pc += offset - 3;
    }
    return NULL;
}
void* OPCODE_IFGT(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 > 0){
        frame->pc += offset - 3;
    }
    return NULL;
}
void* OPCODE_IFGE(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 >= 0){
        frame->pc += offset - 3;
    }
    return NULL;
}

void* OPCODE_DCMPG(Frame* frame){
    double val2 = *(double*) popStack(frame->operandStack);
    double val1 = *(double*) popStack(frame->operandStack);
    int64_t val = 0;
    if (val1 > val2){
        val = 1;
    }else if (val1 == val2){
        val = 0;
    }else {
        val = -1;
    }
    pushStack(&val, frame->operandStack);
	return NULL;
}

void* OPCODE_IF_ICMPGE(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val2 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 >= val2){
        frame->pc += offset - 3;
        // -1 because the main loop will increase 1 after this opcode to
        // get the next opcode. Extra -2 because we moved 2 more bytes while
        // reading the offset
    }
	return NULL;
}

void* OPCODE_IF_ICMPGT(Frame* frame){
    uint16_t offset = read2Bytes(frame);
    int64_t val2 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    int64_t val1 = (int32_t)*(int64_t*) popStack(frame->operandStack);
    if (val1 > val2){
        frame->pc += offset - 3;
        // -1 because the main loop will increase 1 after this opcode to
        // get the next opcode. Extra -2 because we moved 2 more bytes while
        // reading the offset
    }
	return NULL;
}

void* OPCODE_IINC(Frame* frame){
    uint8_t index = read1Byte(frame);
    uint8_t incrementAmount = read1Byte(frame);
    *(int64_t*)&frame->localVariables[index] = (int32_t)(*(int64_t*)&frame->localVariables[index] + *(int8_t*)&incrementAmount);
    return NULL;
}

void* OPCODE_GOTO(Frame* frame){
    uint16_t offset = read2Bytes(frame); // branch offset
    frame->pc += *(int16_t*)&offset - 3;
    // -1 because the main loop will increase 1 after this opcode to
    // get the next opcode Extra -2 because we moved 2 more bytes while
    // reading the offset
    return NULL;
}

void* OPCODE_GETSTATIC(Frame* frame){
    uint16_t index = read2Bytes(frame);
    CONSTANT_Ref_info fieldRefInfo = frame->classRef->constant_pool[index-1].info.ref_info;

    CONSTANT_NameAndType_info nameAndType = frame->classRef->constant_pool[fieldRefInfo.name_and_type_index-1].info.nameAndType_info;
    CONSTANT_Utf8_info name_utf8 = frame->classRef->constant_pool[nameAndType.name_index-1].info.utf8_info;
    CONSTANT_Utf8_info desc_utf8 = frame->classRef->constant_pool[nameAndType.descriptor_index-1].info.utf8_info;

    CONSTANT_Class_info class = frame->classRef->constant_pool[fieldRefInfo.class_index -1].info.class_info;
    CONSTANT_Utf8_info className_utf8 = frame->classRef->constant_pool[class.name_index-1].info.utf8_info;
    
    ClassFile* cf = getClassFromUtf8(className_utf8,frame->machine);
    if (cf != NULL){
        initClass(cf, frame);
        field_info* field = getStaticField(cf, name_utf8, desc_utf8);
        pushStack(&field->value, frame->operandStack);
    }else{
        uint64_t val = 0;
        pushStack(&val, frame->operandStack);
    }
    return NULL;
}

void* OPCODE_NEW(Frame* frame){
    uint16_t index = read2Bytes(frame);
    CONSTANT_Class_info class = frame->classRef->constant_pool[index-1].info.class_info; // class or interface ref info

    CONSTANT_Utf8_info className_utf8 = frame->classRef->constant_pool[class.name_index-1].info.utf8_info;
    ClassFile* cf = getClassFromUtf8(className_utf8,frame->machine);
    uint64_t objectRef;
    if (cf != NULL){
        objectRef = hAlloc(sizeof(JavaClass),frame->machine->heap);
        JavaClass* newObject = hGet(objectRef,frame->machine->heap);
        newObject->classFile = cf;
        initClass(cf, frame);
        initInstanceFields(newObject);
        pushStack(&objectRef, frame->operandStack);
    }else{
        uint64_t objectRef = (uint64_t)NULL;
        if (isUtf8EqualsToString(className_utf8,"java/lang/StringBuilder")){
            objectRef = hAlloc(sizeof(cp_info), frame->machine->heap);
            cp_info* str = hGet(objectRef, frame->machine->heap);
            str->tag = CONSTANT_Utf8;
            uint64_t bytesRef = hAlloc(1,frame->machine->heap);
            str->info.utf8_info.bytes=(char*)bytesRef;
            char* bytes = hGet(bytesRef, frame->machine->heap);
            bytes[0]='\0';
            str->info.utf8_info.length=1;
        }
        pushStack(&objectRef, frame->operandStack);
    }
    
    return NULL;
}

void* OPCODE_PUTFIELD(Frame* frame){
    uint16_t index = read2Bytes(frame);
    CONSTANT_Ref_info fieldRefInfo = frame->classRef->constant_pool[index-1].info.ref_info;

    CONSTANT_NameAndType_info nameAndType = frame->classRef->constant_pool[fieldRefInfo.name_and_type_index-1].info.nameAndType_info;
    CONSTANT_Utf8_info name_utf8 = frame->classRef->constant_pool[nameAndType.name_index-1].info.utf8_info;
    CONSTANT_Utf8_info desc_utf8 = frame->classRef->constant_pool[nameAndType.descriptor_index-1].info.utf8_info;

    uint64_t val = *(uint64_t*)popStack(frame->operandStack);
    JavaClass* obj = (JavaClass*)hGet(*(uint64_t*)popStack(frame->operandStack),frame->machine->heap);

    DEBUG_PRINT(("%x, put val is %d, objRef is %p \n", obj->classFile->magic, val, obj));
    putField(obj, name_utf8, desc_utf8, val);

    return NULL;
}

void* OPCODE_NRETURN(Frame* frame){
    uint64_t* val = malloc(sizeof(val)); // freed later
    *val = *(uint64_t*)popStack(frame->operandStack);
    DEBUG_PRINT(("iret val : %d, stackTop: %d\n", *val, frame->operandStack->top));
    return val;
}
void* OPCODE_RETURN(Frame* frame){
    frame->pc = frame->code->code_length;
    // current frame is discarded
    return NULL;
}

void* OPCODE_GETFIELD(Frame* frame){
    
    JavaClass* obj = (JavaClass*)hGet(*(uint64_t*)popStack(frame->operandStack),frame->machine->heap);
    uint16_t index = read2Bytes(frame);
    CONSTANT_Ref_info fieldRef = frame->classRef->constant_pool[index-1].info.ref_info;

    CONSTANT_NameAndType_info* nameAndType = &frame->classRef->constant_pool[fieldRef.name_and_type_index-1].info.nameAndType_info;
    CONSTANT_Utf8_info name_utf8 = frame->classRef->constant_pool[nameAndType->name_index-1].info.utf8_info;
    CONSTANT_Utf8_info desc_utf8 = frame->classRef->constant_pool[nameAndType->descriptor_index-1].info.utf8_info;

    // different type fields can have the same name in bytecode but not in java.
    field_info* field = getField(obj,name_utf8, desc_utf8);
    pushStack(&field->value, frame->operandStack);
    return NULL;
}

void* OPCODE_INVOKEVIRTUAL(Frame* frame){
    uint16_t index = read2Bytes(frame);
    if (!(frame->classRef->constant_pool[index-1].tag == CONSTANT_Methodref || frame->classRef->constant_pool[index-1].tag == CONSTANT_InterfaceMethodref)){
        printf("invoked method ref is invalid, index is %d\n",index);
    }
    CONSTANT_Ref_info methodOrInterfaceRef = frame->classRef->constant_pool[index-1].info.ref_info;
    
    CONSTANT_NameAndType_info* nameAndType = &frame->classRef->constant_pool[methodOrInterfaceRef.name_and_type_index-1].info.nameAndType_info;
    CONSTANT_Utf8_info name_utf8 = frame->classRef->constant_pool[nameAndType->name_index-1].info.utf8_info;
    CONSTANT_Utf8_info desc_utf8 = frame->classRef->constant_pool[nameAndType->descriptor_index-1].info.utf8_info;
    DEBUG_PRINT( ("Method's Name: %.*s\n",name_utf8.length,name_utf8.bytes));

    CONSTANT_Class_info methodClass = frame->classRef->constant_pool[methodOrInterfaceRef.class_index - 1].info.class_info;
    CONSTANT_Utf8_info className_utf8 = frame->classRef->constant_pool[methodClass.name_index-1].info.utf8_info;
    DEBUG_PRINT( ("Class's Name: %.*s\n",className_utf8.length,className_utf8.bytes));
    ClassFile* cf = getClassFromUtf8(className_utf8, frame->machine);


    method_info* method = NULL;

    int numArgs = getNumArgs(frame->classRef, methodOrInterfaceRef) + 1; // +1 for ObjectRef which will be at localVars[0]

    uint64_t tempLocalVars[numArgs];
    for (int argId = numArgs-1; argId >= 0; argId--){
        tempLocalVars[argId] = *(uint64_t*) popStack(frame->operandStack);
    }
 

    // hack to run Println
    if (isUtf8EqualsToString(className_utf8,"java/io/PrintStream") && isUtf8EqualsToString(name_utf8,"println")){
        handlePrintStream(tempLocalVars, frame, name_utf8, desc_utf8);
        return NULL;
    }
    if (isUtf8EqualsToString(className_utf8,"java/lang/StringBuilder")){
        handleStringBuilder(tempLocalVars,frame, name_utf8, desc_utf8);
        return NULL;
    }

    ////  Let C be the class of objectref. The actual method to be invoked
    //// is selected by the following lookup procedure:
    
    // If C contains a declaration for an instance method m that overrides (§5.4.5)
    // the resolved method, then m is the method to be invoked.
    JavaClass* obj = (JavaClass*)hGet(tempLocalVars[0],frame->machine->heap);
    method_info* mPointer = canClassHandleMethod(obj->classFile, name_utf8, desc_utf8);

    if (mPointer){
        method = (mPointer)?(mPointer):method;
        DEBUG_PRINT(("first overrider runned\n"));
    }else{

        // Otherwise, if C has a superclass, a search for a declaration of an instance
        // method that overrides the resolved method is performed, starting with the
        // direct superclass of C and continuing with the direct superclass of that
        // class, and so forth, until an overriding method is found or no further
        // superclasses exist. If an overriding method is found, it is the method to be invoked. 
        ClassFile* superClass = obj->classFile;
        mPointer = NULL;
        while (superClass->super_class != 0){
            CONSTANT_Class_info superClassInfo = superClass -> constant_pool [superClass->super_class-1].info.class_info;
            CONSTANT_Utf8_info superClassName = superClass -> constant_pool [superClassInfo.name_index-1].info.utf8_info;
            superClass = getClassFromUtf8 (superClassName, frame->machine);
            mPointer = canClassHandleMethod(superClass, name_utf8, desc_utf8);
            if (mPointer){
                method = (mPointer)?(mPointer):method;
                DEBUG_PRINT(("found in superclass runned\n"));
                break;
            }
        }
        if (mPointer == NULL){
            DEBUG_PRINT(("fallBack runned\n"));
            mPointer = canClassHandleMethod(cf, name_utf8, desc_utf8);
            method = (mPointer)?(mPointer):method;
        }
    }

    if (method->access_flags != M_ACC_NATIVE){
        // todo: error? 
    }

    Code_attribute code;
    if (method != NULL){
        code = getCode_AttributeFromAttribute_info(method->attributes[0]);
    }else{
        printf("method not found \n");
    }
    Frame newFrame = createNewFrame(code, cf, frame->machine);

    memcpy(newFrame.localVariables, tempLocalVars, numArgs * sizeof(tempLocalVars[0]));

    pushStack(&newFrame, frame->machine->JVMSTACK);

    frame->pc++;
    return NULL;
}

void* OPCODE_INVOKESPECIAL(Frame* frame){
    uint16_t index = read2Bytes(frame);
    if (!(frame->classRef->constant_pool[index-1].tag == CONSTANT_Methodref || frame->classRef->constant_pool[index-1].tag == CONSTANT_InterfaceMethodref)){
        printf("invoked method ref is invalid, index is %d\n",index);
    }
    CONSTANT_Ref_info methodOrInterfaceRef = frame->classRef->constant_pool[index-1].info.ref_info;
    
    CONSTANT_NameAndType_info* nameAndType = &frame->classRef->constant_pool[methodOrInterfaceRef.name_and_type_index-1].info.nameAndType_info;
    CONSTANT_Utf8_info name_utf8 = frame->classRef->constant_pool[nameAndType->name_index-1].info.utf8_info;
    CONSTANT_Utf8_info desc_utf8 = frame->classRef->constant_pool[nameAndType->descriptor_index-1].info.utf8_info;
    DEBUG_PRINT( ("Method's Name: %.*s\n",name_utf8.length,name_utf8.bytes));

    CONSTANT_Class_info methodClass = frame->classRef->constant_pool[methodOrInterfaceRef.class_index - 1].info.class_info;
    CONSTANT_Utf8_info className_utf8 = frame->classRef->constant_pool[methodClass.name_index-1].info.utf8_info;
    DEBUG_PRINT( ("Class's Name: %.*s\n",className_utf8.length,className_utf8.bytes));
    ClassFile* cf = getClassFromUtf8(className_utf8, frame->machine);
    int numArgs = getNumArgs(frame->classRef, methodOrInterfaceRef) + 1; // +1 for ObjectRef which will be at localVars[0]

    if (cf == NULL){
        // clear stack for special methods of native classes.
        for (int i=0; i<numArgs; i++)
            popStack(frame->operandStack); // pop numArgs times
        return NULL;
    }

    method_info* method = canClassHandleMethod(cf, name_utf8, desc_utf8);
    Code_attribute code;
    if (method != NULL){
        code = getCode_AttributeFromAttribute_info(method->attributes[0]);
    }else{
        printf("method not found \n");
    }
    Frame newFrame = createNewFrame(code, cf, frame->machine);

    if (method->access_flags != M_ACC_NATIVE){
        for (int argId = numArgs-1; argId >= 0; argId--){
            newFrame.localVariables[argId] = *(uint64_t*) popStack(frame->operandStack);
        }

    }

    pushStack(&newFrame, frame->machine->JVMSTACK);

    frame->pc++;
    return NULL;
}

void* OPCODE_INVOKESTATIC(Frame* frame){
    uint16_t index = read2Bytes(frame);

    if (!(frame->classRef->constant_pool[index-1].tag == CONSTANT_Methodref || frame->classRef->constant_pool[index-1].tag == CONSTANT_InterfaceMethodref)){
        // index must be a symbolic reference to a method or an interface method
        // if the resolved method is an instance method, the invokestatic instruction throws an IncompatibleClassChangeError. 
        printf("invoked method ref is invalid, index is %d\n",index);
    }
    CONSTANT_Ref_info methodOrInterfaceRef = frame->classRef->constant_pool[index-1].info.ref_info;
    //DEBUG_PRINT( ("Method's Class Name: %.*s\n",name_utf8.length,name_utf8.bytes));

    CONSTANT_Class_info methodClass = frame->classRef->constant_pool[methodOrInterfaceRef.class_index - 1].info.class_info;
    CONSTANT_Utf8_info className_utf8 = frame->classRef->constant_pool[methodClass.name_index-1].info.utf8_info;
    DEBUG_PRINT( ("Class's Name: %.*s\n",className_utf8.length,className_utf8.bytes));
    ClassFile* cf = getClassFromUtf8(className_utf8, frame->machine);

    CONSTANT_NameAndType_info* nameAndType = &frame->classRef->constant_pool[methodOrInterfaceRef.name_and_type_index-1].info.nameAndType_info;
    CONSTANT_Utf8_info name_utf8 = frame->classRef->constant_pool[nameAndType->name_index-1].info.utf8_info;
    CONSTANT_Utf8_info desc_utf8 = frame->classRef->constant_pool[nameAndType->descriptor_index-1].info.utf8_info;

    method_info* method = canClassHandleMethod(cf, name_utf8, desc_utf8);
    Code_attribute code;
    if (method != NULL){
        code = getCode_AttributeFromAttribute_info(method->attributes[0]);
        //method->attributes[0]
    }else{
        // todo method not found
        printf("method not found \n");
    }
    Frame newFrame = createNewFrame(code, cf, frame->machine);


    if (method->access_flags != M_ACC_NATIVE){
        // On successful resolution of the method, the class or interface that declared the resolved method is initialized (§5.5) if that class or interface has not already been initialized. 
        int numArgs = getNumArgs(frame->classRef, methodOrInterfaceRef);
        for (int argId = numArgs-1; argId >= 0; argId--){
            newFrame.localVariables[argId] = *(uint64_t*) popStack(frame->operandStack);
        }

    }

    pushStack(&newFrame, frame->machine->JVMSTACK);

    frame->pc++;
    return NULL;
}
OPCODE** initOpcodes(){
    // jumptable
    OPCODE** opcodes = malloc (sizeof(OPCODE*) * 255); // leak, consider static

    opcodes[0x1] = OPCODE_ICONST_0; // null
    opcodes[0x2] = OPCODE_ICONST_M1;
    opcodes[0x3] = OPCODE_ICONST_0;
    opcodes[0x4] = OPCODE_ICONST_1;
    opcodes[0x5] = OPCODE_ICONST_2;
    opcodes[0x6] = OPCODE_ICONST_3;
    opcodes[0x7] = OPCODE_ICONST_4;
    opcodes[0x8] = OPCODE_ICONST_5;
    opcodes[0x9] = OPCODE_ICONST_0;
    opcodes[0xa] = OPCODE_ICONST_1;

    opcodes[0xb] = OPCODE_DCONST_0;
    opcodes[0xc] = OPCODE_DCONST_1;
    opcodes[0xd] = OPCODE_DCONST_2;
    opcodes[0xe] = OPCODE_DCONST_0; // double
    opcodes[0xf] = OPCODE_DCONST_1;

    opcodes[0x10] = OPCODE_BIPUSH;
    opcodes[0x11] = OPCODE_SIPUSH;
    
    opcodes[0x12] = OPCODE_LDC;
    opcodes[0x13] = OPCODE_LDC_W;
    opcodes[0x14] = OPCODE_LDC_W; // same but for 8 bytes. 

    opcodes[0x15] = OPCODE_NLOAD; //iload
    opcodes[0x16] = OPCODE_NLOAD; //lload
    opcodes[0x18] = OPCODE_NLOAD; //dload
    opcodes[0x19] = OPCODE_NLOAD; //aload
    
    // iload
    opcodes[0x1a] = OPCODE_NLOAD_0;
    opcodes[0x1b] = OPCODE_NLOAD_1;
    opcodes[0x1c] = OPCODE_NLOAD_2;
    opcodes[0x1d] = OPCODE_NLOAD_3;
    // lload
    opcodes[0x1e] = OPCODE_NLOAD_0;
    opcodes[0x1f] = OPCODE_NLOAD_1;
    opcodes[0x20] = OPCODE_NLOAD_2;
    opcodes[0x21] = OPCODE_NLOAD_3;
    // dload
    opcodes[0x26] = OPCODE_NLOAD_0;
    opcodes[0x27] = OPCODE_NLOAD_1;
    opcodes[0x28] = OPCODE_NLOAD_2;
    opcodes[0x29] = OPCODE_NLOAD_3;
    // aload
    opcodes[0x2a] = OPCODE_NLOAD_0;
    opcodes[0x2b] = OPCODE_NLOAD_1;
    opcodes[0x2c] = OPCODE_NLOAD_2;
    opcodes[0x2d] = OPCODE_NLOAD_3;


    opcodes[0x36] = OPCODE_NSTORE; // istore
    opcodes[0x37] = OPCODE_NSTORE; // lstore

    opcodes[0x3a] = OPCODE_NSTORE; // astore

    // istore
    opcodes[0x3b] = OPCODE_NSTORE_0;
    opcodes[0x3c] = OPCODE_NSTORE_1;
    opcodes[0x3d] = OPCODE_NSTORE_2;
    opcodes[0x3e] = OPCODE_NSTORE_3;

    // lstore
    opcodes[0x3f] = OPCODE_NSTORE_0;

    opcodes[0x39] = OPCODE_NSTORE; // dstore

    opcodes[0x40] = OPCODE_NSTORE_1;
    opcodes[0x41] = OPCODE_NSTORE_2;
    opcodes[0x42] = OPCODE_NSTORE_3;

    // dstore
    opcodes[0x47] = OPCODE_NSTORE_0;
    opcodes[0x48] = OPCODE_NSTORE_1;
    opcodes[0x49] = OPCODE_NSTORE_2;
    opcodes[0x4a] = OPCODE_NSTORE_3;

    // astore
    opcodes[0x4b] = OPCODE_NSTORE_0;
    opcodes[0x4c] = OPCODE_NSTORE_1;
    opcodes[0x4d] = OPCODE_NSTORE_2;
    opcodes[0x4e] = OPCODE_NSTORE_3;

    opcodes[0x57] = OPCODE_POP;
    opcodes[0x59] = OPCODE_DUP;
    
    opcodes[0x61] = OPCODE_LADD;

    opcodes[0x67] = OPCODE_DSUB;

    opcodes[0x68] = OPCODE_IMUL;

    opcodes[0x6b] = OPCODE_DMUL;
    opcodes[0x6d] = OPCODE_LDIV;
    opcodes[0x6f] = OPCODE_DDIV;

    opcodes[0x73] = OPCODE_DREM;
    opcodes[0x77] = OPCODE_DNEG;

    opcodes[0x84] = OPCODE_IINC;
    opcodes[0x85] = OPCODE_I2L;

    opcodes[0x8e] = OPCODE_D2I;
    opcodes[0x8f] = OPCODE_D2L;

    opcodes[0x90] = OPCODE_D2F;

    opcodes[0x94] = OPCODE_LCMP;

    opcodes[0x97] = OPCODE_DCMPG; //dcmpl
    opcodes[0x98] = OPCODE_DCMPG; //dcmpg

    opcodes[0x99] = OPCODE_IFEQ;
    opcodes[0x9a] = OPCODE_IFNE;
    opcodes[0x9b] = OPCODE_IFLT;
    opcodes[0x9c] = OPCODE_IFGE;
    opcodes[0x9d] = OPCODE_IFGT;
    opcodes[0x9e] = OPCODE_IFLE;


    opcodes[0xa2] = OPCODE_IF_ICMPGE;
    opcodes[0xa3] = OPCODE_IF_ICMPGT;
    opcodes[0xa7] = OPCODE_GOTO;
        
    opcodes[0xaf] = OPCODE_NRETURN; // d

    opcodes[0xb0] = OPCODE_NRETURN;
    opcodes[0xb1] = OPCODE_RETURN;
    opcodes[0xb2] = OPCODE_GETSTATIC;
    opcodes[0xb4] = OPCODE_GETFIELD;
    opcodes[0xb5] = OPCODE_PUTFIELD;
    opcodes[0xb6] = OPCODE_INVOKEVIRTUAL;
    opcodes[0xb7] = OPCODE_INVOKESPECIAL;
    opcodes[0xb8] = OPCODE_INVOKESTATIC;

    opcodes[0xbb] = OPCODE_NEW;
    
    opcodes[0x60] = OPCODE_IADD;
    opcodes[0xac] = OPCODE_NRETURN;
    return opcodes;
}