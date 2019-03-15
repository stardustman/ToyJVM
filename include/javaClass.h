#ifndef JAVACLASS_H_
#define JAVACLASS_H_
#include "classFile.h"

typedef struct JavaClass{
    //.class file 重构后的可用的classFile结构体
    ClassFile* classFile;
    field_info* fields;
}JavaClass;

void initInstanceFields(JavaClass* instance);

#endif