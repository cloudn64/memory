#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/*

    In the last very strange example, the point I tried to convey was that it's very easy to accidentally assume you have memory
    without having it. 
    In this example, we'll be making a ton of mistakes.
    First, we'll assume that my PersonThing struct has a size of 7 (it doesn't)
    Then we'll assume that CreditCard has a size of 6 (it doesn't!)
    We'll make an arena, then we'll use it without thinking about it at all.

*/

typedef struct {
    uint8_t id; // 1 byte
    int xPos; // 4 butes
    int yPos; // 4 bytes
    int16_t yaw; // 2 bytes
} PersonThing; // this is ""7"" bytes (no it isn't)

typedef struct {
    uint8_t id; // 1 byte
    int cardNumber; // 4 bytes
} CreditCard; // this is ""5"" bytes (no it isn't)

int main()
{
    // Let's just get a big chunk of memory.
    uint8_t* arena = (uint8_t*)calloc(100, 0x1000);

    if (arena == NULL) {
        abort(); // Stop here if we couldn't get it.
    }

    // Here's our first person
    PersonThing* person1 = (PersonThing*)arena;

    // Our second person comes after our first person
    PersonThing* person2 = (PersonThing*)arena + 7;

    // Our credit card comes after our two people
    CreditCard* card1 = (CreditCard*)arena + 7 + 7;

    // And our next credit card comes after two people and a credit card.
    CreditCard* card2 = (CreditCard*)arena + 7 + 7 + 5;

    // Here's Person 1
    person1->id = 0;
    person1->xPos = 40;
    person1->yPos = 80;
    person1->yaw = 0x4000;

    // And here's his credit card
    card1->id = 0;
    card1->cardNumber = 12345678;

    // Here's Person 2
    person2->id = 1;
    person2->xPos = 190;
    person2->yPos = 280;
    person2->yaw = 0x2000;

    // And here's his credit card
    card2->id = 1;
    card2->cardNumber = 87654321;

    // Now, let's check all of our information to make sure it's correct:
    printf("person1\nid: %d, xPos: %d, yPos: %d, yaw:0x%X\n", person1->id, person1->xPos, person1->yPos, person1->yaw);
    printf("card1:\nid: %d, cardNumber: %d\n\n", card1->id, card1->cardNumber);
    printf("person2\nid: %d, xPos: %d, yPos: %d, yaw:0x%X\n", person2->id, person2->xPos, person2->yPos, person2->yaw);
    printf("card2:\nid: %d, cardNumber: %d", card2->id, card2->cardNumber);

    // Person1's credit card broke!!

    // Furthermore, we never bothered to check what we were adding to our arena at all.
    // If we put this in a loop and kept adding more people and cards, we would eventually start writing outside
    // of our arena and destroying things. (OH NO!)
}
