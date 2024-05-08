/*
 
    Is there anything this C# can't handle?!?!?!
    Well, sort of. It's not indestructible. It lets you waste memory, that doesn't mean memory cannot be wasted.
    A bad programmer can very easily just waste tons of memory anyway, as you will see in this example.

    Note: Please don't run this program yourself...

 */

// Here's PersonThing from the C and C++ Example 4 again.
struct PersonThing
{
    public byte id;
    public int posX;
    public int posY;
    public Int16 rot;
}

public class Program
{

    // Here is a static object. It will always exist. It's also a resizeable list.
    // So what happens when we can continuously expand something that cannot be destroyed? Let's find out.
    static List<PersonThing> indestructiblePeopleList = new List<PersonThing>();

    static void Main()
    {
        while (true) { // Add people... forever. Literally forever.
            indestructiblePeopleList.Add(new PersonThing()); // Add a person
        }

        // By the way, it took me 11 seconds to make 16GB of people.
        // The memory isn't leaked, I suppose? But it's definitely a problem.
        // You could accidentally accumulate a mass of indestructible things and waste memory.
        // That's pretty much what a leak does anyway.
    }
}
