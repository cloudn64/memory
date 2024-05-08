#include <stdlib.h>
#include <stdio.h>

int main()
{
    void* memoryPointer = NULL; // create a new NULL pointer named "memoryPointer". This is a reference to a memory address.
    int testValue = 4;

    // allocate 36 bytes, store the pointer in "memoryPointer"
    memoryPointer = malloc(36);

    // "resize" the section pointed to by "memoryPointer" to 48 bytes, store new pointer in "memoryPointer"
    // This assumes that the earlier malloc succeeded.
    // If it failed, memoryPointer would still be NULL, which would caused undefined behavior to occur.
    // If malloc succeeds but realloc fails, memoryPointer becomes NULL without freeing the memory allocated via malloc.
    // Without that pointer, the memory has leaked because we have no pointer to free it.
    memoryPointer = realloc(memoryPointer, 48);

    // give the 48 bytes pointed to by "memoryPointer" back to the system.
    // As mentioned previously, if realloc failed, memoryPointer will be NULL,
    // and the 36 bytes from before will never be freed ever.
    free(memoryPointer);

    // allocate 5 groups of 30 bytes (150 bytes) and set them to 0, store the pointer in "memoryPointer"
    // But only if testValue is 2 (it isn't 2, by the way.)
    if (testValue == 2) {
        memoryPointer = calloc(5, 30);
    }

    // Free our 150 bytes... right?
    // Well, no. testValue wasn't 2, so we didn't allocate 150 bytes at all.
    // What we do here instead is attempt to free an old pointer to our already freed 48 bytes, which is undefined behavior.
    free(memoryPointer);
}
