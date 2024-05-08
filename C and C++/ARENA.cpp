#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/*

    Well, we've established I'm not great at coming up with examples on the spot.
    However, some things should have been learned through the examples.
        - Memory allocation is easy to mess up
        - Forgetting to free memory will cause a memory leak
        - It's easy to accidentally use memory that is not yours
        - There's a chance that the system will not give you the memory you want

    Those are the basic problems with memory allocation. Over the span of decades, programmers
    have tried to solve this problem in different ways, each with its own set of setbacks.

        - Arena Allocation
        - Garbage Collectors
        - Whatever Rust is

    This final C++/C example will attempt to tackle Arena allocation using a Linked-List structure.
    The pros of an Arena are:
        
        - You can destroy the entire Arena at once
        - It may be faster than asking the OS for memory
        - You only have to request memory from the OS once
        - (If it was implemented correctly) it doesn't leak

    C++ has some sort of built in Arena functionality, but I decided not to use it for this example.
    Telling people that the way to do something is to use something that does it for them is how knowledge disappears.
    Therefore, I have written this myself, and I would very much like to have someone smarter and more qualified proof-read it.
    Downsides:

        - Fragmentation is possible (common with Linked-Lists)
        - I wrote it, which is probably not good
        - The code would be simplified if the HeadNode existed within the Arena instead of being an outlier that exists independently
        - Each requested object is attached to an ObjectNode, which makes an object 0x18 bytes larger

    This example creates an "ObjectArena", an arena for holding onto objects of requested size.
    You can destroy an object, and new objects created that fit in a gap left behind by a deleted object will be placed inside of that gap.
    
    You may notice as you read this that the usage of this arena is remarkably similar to... malloc and free!
    Yes- we have created our own!


*/

// This Node is attached to each object created in the arena.
// It contains a pointer to the next and previous object, and the size of this object.
// This is used to get the empty space between objects.
typedef struct ObjectNode {
    struct ObjectNode* prev;
    struct ObjectNode* next;
    size_t size;
} ObjectNode;

// This is the Arena itself. It contains the pointer to the allocated arena memory,
// the size of the arena, and its own independent Node.
// (The code may be simplified if this node was inside of the arena instead.)
typedef struct {
    void* ptr;
    ObjectNode headNode;
    size_t heapSize;
} ObjectArena;

// This is a static object. It does not need to be allocated.
// Think of it as something that exists all the time as part of the program.
ObjectArena arena;

// This function allocates all of the memory for the arena. Returns 0 if the system could not do this.
int initArena(size_t arenaSize) {
    arena.ptr = malloc(arenaSize); // we'll make some space.
    if (arena.ptr == NULL) return 0; // We failed to make some space.

    arena.headNode.prev = arena.headNode.next = NULL;
    arena.headNode.size = sizeof(ObjectNode);
    arena.heapSize = arenaSize;

    return 1;
}

// This function immediately discards the entire arena!
void destroyArena() {
    free(arena.ptr);
}

// This function assumes that the pointer you gave it is an object from "addObject" and destroys that object.
// In doing so, it will connect the object nodes behind and in front of it to complete the linked-list.
void destroyObject(uintptr_t object) {
    // The returned pointer for an object takes place after its node so you don't accidentally destroy the node
    ObjectNode* delObjectNode = (ObjectNode*)(object - sizeof(ObjectNode));
    ObjectNode* prevObject = delObjectNode->prev;

    prevObject->next = delObjectNode->next;
    if (delObjectNode->next != NULL) {
        delObjectNode->next->prev = prevObject;
    }
}

// This is a safer version of the previous function that checks if the pointer you gave it is in the linked-list first.
// It's slower, but is a helpful example of how you can take extra steps to be more careful.
bool saferDestroyObject(uintptr_t object) {
    // The returned pointer for an object takes place after its node so you don't accidentally destroy the node
    ObjectNode* delObjectNode = (ObjectNode*)(object - sizeof(ObjectNode));
    ObjectNode* curNode = &arena.headNode;

    while (curNode->next != NULL) {
        if (curNode->next == delObjectNode) { // Found your Node

            ObjectNode* delObjectNode = (ObjectNode*)(object - sizeof(ObjectNode));

            delObjectNode->prev->next = delObjectNode->next;
            if (delObjectNode->next != NULL) {
                delObjectNode->next->prev = delObjectNode->prev;
            }
            return true;
        }
        curNode = curNode->next;
    }

    return false;
}

// This is the big one. It will reserve the requested size in the Arena.
// Read through the comments inside the function to step through how it works, but basically,
// It travels through the nodes, checking the space between that node and the one in front of it.
// If it finds a suitable space in-between the nodes, it will connect itself between the two nodes.
// If it reaches the end, it checks the space between the final node and the end of the arena.
// If it finds space there, it adds itself onto the end after the last node, and the process continues.
// If it fails to find space anywhere, it will return NULL, just like... malloc!!

uintptr_t addObject(size_t size) {
    size_t objectSize = sizeof(ObjectNode) + size; // size of the object + space for the object node
    ObjectNode* curNode = &arena.headNode;

    // There is another node after this node
    while (curNode->next != NULL) {
        // space between the end of this node and the start of the next node
        uintptr_t end = (uintptr_t)curNode + curNode->size;
        if (curNode == &arena.headNode) {
            end = (uintptr_t)arena.ptr;
        }
        size_t free = ((uintptr_t)curNode->next - end);

        // there is enough space for this object and its node
        if (free >= objectSize) { // In here should be its own function
            // The new Object (and node) is at the end of this object because there's space for it between this one and the one after it
            ObjectNode* newObject = (ObjectNode*)end;
            ObjectNode* nextObject = (ObjectNode*)curNode->next;
            // Empty out this space (This is where I'd write the object to newObject + sizeof(ObjectNode))
            memset(newObject, 0, objectSize);
            // Give the new node the size of itself and its object
            newObject->size = objectSize;
            // Place this new node in between the link this one had to the next one
            newObject->prev = curNode;
            newObject->next = curNode->next;
            nextObject->prev = newObject;
            curNode->next = newObject;
            // Return pointer to the object this node represents
            return (uintptr_t)newObject + sizeof(ObjectNode);
        } else {
            // The gap between these objects is not big enough to fit the new object, try the next one
            curNode = (ObjectNode*)curNode->next;
        }
    }

    // You made it through every node and didn't find any space
    size_t remainingSpace = arena.heapSize - curNode->size;
    uintptr_t end = (uintptr_t)curNode + curNode->size;

    // The head node is not actually in the arena.
    if (curNode == &arena.headNode) {
        end = (uintptr_t)arena.ptr;
    }

    // There is space at the end of the heap for your new object
    if (remainingSpace >= objectSize) { // In here should be its own function
        // The new Object (and node) is at the end of this object because there's space for it between this one and the one after it
        ObjectNode* newObject = (ObjectNode*)end;
        // Empty out this space (This is where I'd write the object to newObject + sizeof(ObjectNode))
        memset(newObject, 0, objectSize);
        // Give the new node the size of itself and its object
        newObject->size = objectSize;
        // Place this new node after the last one
        curNode->next = newObject;
        newObject->prev = curNode;
        newObject->next = NULL;
        // Return pointer to the object this node represents
        return (uintptr_t)newObject + sizeof(ObjectNode);
    }

    // There's no room for this.
    return NULL;
}

int main()
{
    // Create an arena with a size of 10000
    if (!initArena(10000)) {
        printf("Failed to initialize arena");
        return 1;
    }

    // Create a bunch of test objects
    uintptr_t testObject = addObject(0x40);
    uintptr_t testObject2 = addObject(0x40);
    uintptr_t testObject3 = addObject(0x40);

    // Destroy a test object
    saferDestroyObject(testObject);

    // Make more test objects
    uintptr_t testObject5 = addObject(0x30); // this object will occupy the space left behind after the death of "testObject"
    uintptr_t testObject6 = addObject(0x40);

    // Destroy the entire arena AT ONCE!
    destroyArena();
}
