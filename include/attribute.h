#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_
#include <inttypes.h>
#include <stdio.h>

/**
 * Attributes are used in the ClassFile, field_info, method_info, and Code_attribute structures of the class file format
 * attribute_info {
    u2 attribute_name_index;
    u4 attribute_length;
    u1 info[attribute_length];
}
 */
typedef struct attribute_info {
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint8_t* info;
}attribute_info;

typedef struct exception_table{ // aligned.
    uint16_t start_pc;
    uint16_t end_pc;
    uint16_t handler_pc;
    uint16_t catch_type;
} exception_table;



//If the method is either native or abstract, its method_info structure must not have a 
//Code attribute in its attributes table. Otherwise, its method_info structure must have
//exactly one Code attribute in its attributes table

/**
 * The Code attribute is a variable-length attribute in the attributes table of a method_info structure (§4.6).
 * A Code attribute contains the Java Virtual Machine instructions and auxiliary information for a method, 
 * including an instance initialization method or a class or interface initialization method
 * Code_attribute {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 max_stack;
    u2 max_locals;
    u4 code_length;
    u1 code[code_length];
    u2 exception_table_length;
    {   u2 start_pc;
        u2 end_pc;
        u2 handler_pc;
        u2 catch_type;
    } exception_table[exception_table_length];
    u2 attributes_count;
    attribute_info attributes[attributes_count];
}
 */
typedef struct Code_attribute {
    uint16_t attribute_name_index;
    uint32_t attribute_length;
    uint16_t max_stack; //The value of the max_stack item gives the maximum depth of the operand stack of this method (§2.6.2) at any point during execution of the method.
    uint16_t max_locals; //编译的class文件里决定的
    uint32_t code_length;
    uint8_t* code;
    uint16_t exception_table_lenght;
    exception_table* exceptionTable; //异常处理
    uint16_t attributes_count;
    attribute_info* attributes;
}Code_attribute;

attribute_info getAttribute_Info(FILE* fd);
Code_attribute getCode_AttributeFromAttribute_info(attribute_info fd);

void destroyAttribute_Info(attribute_info* attribute);
#endif