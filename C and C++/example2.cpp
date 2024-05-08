#include <stdlib.h>

/*

    For this example, we "fixed" the paper example by catching errors and preventing them from causing harm.
    No memory will leak, and the program won't perform any undefined behavior.
    Since we prevented something bad from happening, we did fix the program as far as safety is concerned.
    There is still a slim possibility that an alloc function can fail at three different times, though. I wonder what to do...? (foreshadowing)

*/

int main()
{
    int testValue = 4;
    void* memoryPointer = NULL; // create a new NULL pointer named "memoryPointer". This is a reference to a memory address.

    // allocate 36 bytes, store the pointer in "memoryPointer"
    memoryPointer = malloc(36);

    if (memoryPointer != NULL) {
        // This is where we would do something with our 36 bytes.

        // Now we'll decide we want to get 12 more bytes at the end, so let's use realloc.
        void* tempPointer = realloc(memoryPointer, 48);
        if (tempPointer != NULL) {
            // realloc succeeded, so memoryPointer is no longer valid.
            // We can replace it with the pointer to our new memory.
            memoryPointer = tempPointer;

            // This is where we can do something with our 12 additional bytes.
        } else {
            // realloc failed. We did not overwrite memoryPointer though.
            // We can still recover from a potential memory leak since we
            // can free memoryPointer and our original 36 bytes.
        }
    } else {
        // malloc failed. We don't have the memory we wanted,
        // but we caught the error so we didn't use the NULL pointer on accident.
    }

    // This is where we will discard our 48 bytes. If realloc failed and we don't abort,
    // We still have a pointer to our original 36 bytes, so we can free that!
    // No memory leaks could occur.
    free(memoryPointer);

    // If testValue is 2 (it isn't) get 5 groups of 30 bytes (150 bytes). They are set to 0 by calloc.
    if (testValue == 2) {
        memoryPointer = calloc(5, 30);

        if (memoryPointer != NULL) {
            // This is where we would do something with our 150 bytes, because calloc succeeded.
        }

        // We're done with our 150 bytes. If we didn't get 150 bytes that's okay,
        // free will do nothing if the pointer is NULL.
        // At least we put it inside the if statement, so it will either be NULL if calloc fails,
        // or a pointer to 150 bytes if it succeeds, instead of a stale reference to our old 48 bytes.
        free(memoryPointer);
    }
}
