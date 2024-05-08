#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/*

    For this example, we'll try to allocate memory for 3 copies of our "PersonThing" struct... but how will we do that?

*/

typedef struct {
    uint8_t id;
    int xPos;
    int yPos;
    int16_t yaw;
} PersonThing;

int main()
{
    // So we want to allocate memory for our people, right?
    // How do we get the amount of bytes we need for that?
    // Well, let's try counting.

    // id is 1 byte.
    // xPos and xPos are both 4 bytes.
    // yaw, a binary angle, is 2 bytes.
    // So... we need 7 bytes, right? Sure, let's try that.

    PersonThing* people = (PersonThing*)calloc(3, 7); // Let's allocate our 3 people all cleared to 0.

    if (people != NULL) {
        // Now, our people are all 0, right? Let's check.
        for (int i = 0; i < 3; i++) {
            printf("id: %d, xPos: %d, yPos: %d, yaw: 0x%X\n\n", people[i].id, people[i].xPos, people[i].yPos, people[i].yaw);
        }

        printf("Wait a second... What?\n\n\n");
    }

    free(people);

    // That's right, printf. Only one of the people is empty.
    // Why? We put the 3 in calloc, didn't we? What went wrong?
    // Well, it's because we were wrong about the size of the person. It's not 7.
    // Because of alignment, the memory address for a type like int, which is 4 bytes, has to be aligned to 4 bytes.
    // It can't be at an address like 0x01024003. It can only be 0x01024000 or 0x01024004.
    // So there's 3 empty bytes between id and xPos.
    // Because we don't know what comes after the end of our person, we should align the end of the person to 4 bytes too.

    // The point of all of this is that 'sizeof' will do all of that for you!
    // Let's try our people again.

    people = (PersonThing*)calloc(3, sizeof(PersonThing));

    if (people != NULL) {
        // Now, our people are all 0, right? Let's check.
        for (int i = 0; i < 3; i++) {
            printf("id: %d, xPos: %d, yPos: %d, yaw: 0x%X\n\n", people[i].id, people[i].xPos, people[i].yPos, people[i].yaw);
        }

        printf("Great!!\n\n");
    }

    free(people);

    // It worked this time! We have some empty people.
    // We didn't change the values of the people earlier, and good thing too.
    // If we tried to edit person 2 or 3, we would actually be editing random memory without knowing what it is!

}
