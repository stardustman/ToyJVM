#ifndef HEAP_H_
#define HEAP_H_
#include <inttypes.h>
#include <stdlib.h>

// 创建的对象都在head上
typedef struct HeapObject{
    // sizeof(void *) = 8
    void* addr;
    size_t size;  //对象的大小??  unsigned int
    uint8_t isUsed;
}HeapObject;

typedef struct Heap{
    uint64_t size;
    uint64_t top;
    HeapObject* heap;
}Heap;

Heap initHeap(size_t size);
void* hGet(uint64_t addr, Heap* heap);
uint64_t hAlloc(size_t size, Heap* heap);
uint64_t hExtend(uint64_t addr, size_t size, Heap* heap);
void* hFree(uint64_t addr, Heap* heap);

void destroyHeap(Heap* heap);
#endif