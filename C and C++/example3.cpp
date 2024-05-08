#include <iostream>
#include <stdlib.h>

/*

    In Example 2, we made our program safe. We didn't do anything about the fact that allocation can fail three times though.
    This is one approach, where we allocate all the memory we'll need for the task ahead of time and reuse it.
    This idea was the foundation of arena.

*/

int main()
{
    int testValue = 4;
    void* memoryPointer = NULL; // create a new NULL pointer named "memoryPointer". This is a reference to a memory address.

    // allocate 150 bytes right from the beginning, store the pointer in "memoryPointer"
    memoryPointer = malloc(150);

    if (memoryPointer != NULL) {
        // This is where we would do something with our 36 bytes.
        // We have 150 of them, so we'd just use the first 36 bytes.

        // Now we'll decide we want to get 12 more bytes at the end-- oh wait!
        // We have 150 bytes! We already have 12 more bytes.

        // Now we'll want our 150 bytes all set to 0, just like calloc.
        // (if our testValue is 2.)
        if (testValue == 2) {
            memset(memoryPointer, 0, 150); // there, now it's 0. Wow!!

            // This is where we'll do something with our 150 bytes.
        }
    }

    // We're done with everything. memoryPointer is only 150 bytes or NULL, both of which are fine.
    free(memoryPointer);
}
