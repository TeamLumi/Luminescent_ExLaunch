#include "nn_allocator.h"
#include "ro.h"

static unsigned long mallocFuncPtr;
static unsigned long freeFuncPtr;
static bool allocatorsLoaded = false;

void loadAllocators() {
    nn::ro::LookupSymbol(&mallocFuncPtr, "malloc");
    nn::ro::LookupSymbol(&freeFuncPtr, "free");
    allocatorsLoaded = true;
}

void* nn_malloc(std::size_t size) {
    if (!allocatorsLoaded)
        loadAllocators();

    void* (*func)(size_t _size) = reinterpret_cast<void *(*)(size_t)>(mallocFuncPtr);
    return func(size);
}

void nn_free(void *ptr) {
    if (!allocatorsLoaded)
        loadAllocators();

    void (*func)(void *_ptr) = reinterpret_cast<void (*)(void *)>(freeFuncPtr);
    func(ptr);
}