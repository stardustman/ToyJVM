#ifndef METHOD_H_
#define METHOD_H_
#include "attribute.h"
#include "field.h"

typedef struct field_info method_info; // same as field info

typedef enum method_access_flags{
    M_ACC_PUBLIC 	      = 0x0001, // Declared public; may be accessed from outside its package.
    M_ACC_PRIVATE 	      = 0x0002, // Declared private; accessible only within the defining class.
    M_ACC_PROTECTED 	  = 0x0004, // Declared protected; may be accessed within subclasses.
    M_ACC_STATIC  	      = 0x0008, // Declared static.
    M_ACC_FINAL   	      = 0x0010, // Declared final; must not be overridden (§5.4.5).
    M_ACC_SYNCHRONIZED    = 0x0020, // Declared synchronized; invocation is wrapped by a monitor use.
    M_ACC_BRIDGE  	      = 0x0040, // A bridge method, generated by the compiler.
    M_ACC_VARARGS 	      = 0x0080, // Declared with variable number of arguments.
    M_ACC_NATIVE  	      = 0x0100, // Declared native; implemented in a language other than Java.
    M_ACC_ABSTRACT 	      = 0x0400, // Declared abstract; no implementation is provided.
    M_ACC_STRICT  	      = 0x0800, // Declared strictfp; floating-point mode is FP-strict.        2048
    M_ACC_SYNTHETIC 	  = 0x1000 	// Declared synthetic; not present in the source code.         4096
}method_access_flags;

#define getMethod_Info(fd) getField_Info(fd) // same too, no need to reimplement  getMethod_Info(fd) 展开后就是get_Field_Info的实现
#define destroyMethod_Info(info) destroyField_Info(info)

#endif
