#include <iostream>
#include <stdio.h>

/*
    This example program attempts to simplify the Project64 example from the paper with comments.\

    The PJ64 "TLB Map" is a page table that translates a virtual address to a physical one.
    It does this by dividing virtual memory into "pages", 0x1000 bytes in length.
    The map assigns each page a physical address.
    When using the table, the virtual address is broken down into a page and an offset.
    It then retrieves the physical address from that page in the table, and adds the offset onto it.
    In this example, the TLB Map is assigned values much like Project64,
    except instead of filling the map with physical addresses, the valid pages are just given a value of "1".
    That way, a valid address will return a 1, and an invalid address will return a 0.

*/

int* map; // Pointer to the TLB Map (Page Table)

int testTlbMap(unsigned int address) {
    int page = address >> 12; // This is the page number of the virtual address, as explained later in the loop that builds the map.
    int offset = address & 0xFFF; // In a regular implementation and not an example, this offset is the three digits that were removed. They get added back onto the physical address.

    printf("%x: %d\n", address, map[page]);
    return map[page];
}

int main()
{
    int mapSize = (0xFFFFF * sizeof(int)); // This is the size of Project64's page table. (TLB Map)
    map = (int*)malloc(mapSize); // Allocate the TLB Map

    if (map == NULL) { // Return early if the TLB Map couldn't be allocated (not part of the example)
        std::cout << "could not allocate map";
        return 1;
    }

    memset(map, 0, mapSize); // This sets the whole map to 0

    // This loop assigns pages between virtual addresses 0x80000000 to 0xC0000000 with values of 1.
    // However, it begins with page entry 0x80000, leaving pages 0x0 to 0x7FFFF EMPTY!
    for (unsigned int vaddr = 0x80000000; vaddr < 0xC0000000; vaddr += 0x1000) {
        // vaddr >> 12 is a bit shift that turns the virtual address into a page number;
        // Pages represent groups of 0x1000 bytes. Each digit in the Base 16 virtual address is 4 bits.
        // By shifting the virtual adddress 12 bits to the right, the last three digits are removed.
        // So for example, the number 0x1000 becomes the number 0x1.
        map[vaddr >> 12] = 1;
    }
 
    // Now we'll test some mock virtual memory reads that the Nintendo 64 would do and see how Project 64 would react.
    testTlbMap(0x80120000);
    testTlbMap(0x803D8000);
    testTlbMap(0x800D9200);
    testTlbMap(0x80780000);

    // And here's a mock virtual memory read that the Nintendo 64 wouldn't usually do, but is capable of doing if a modder asked it to
    testTlbMap(0xD1780000);
    testTlbMap(0x10002400);

    // When running the program you'll notice that the last 2 of those will return a 0 instead of a 1, because they are unassigned.

}
