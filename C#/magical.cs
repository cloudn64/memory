/*
 
    C# has a garbage collector, which means you'll never have to allocate any memory for yourself.
    You can just create things, and they'll be destroyed when the garbage collector detects that you aren't using it anymore.
    It checks each thread for a reference to an object, and if one exists, it keeps the object alive.
    But when there are no references to an object, it will be destroyed.

    Very useful! Has some setbacks...
        
        - It can be slow, may also slow down multithreading
        - Pointers are considered unsafe operations and not encouraged. In other languages, you cannot use them at all.
          The inability to use pointers bars the programmer from some efficient techniques involving pointer manipulation.
        - If a lot of objects aren't being destroyed, the garbage collector will force garbage collection, which could be even slower based on how many objects are being destroyed
        - It may fool people into thinking it is perfect, though it is not entirely perfect
        - As mentioned in bullet 2, it may destroy large portions at once, causing a lag spike to occur

    This mini example will create a bunch of objects inconsequentially. As the main focus of this repo was C/C++ and basic memory allocation (with Arena as a final)
    The C# portion will be relatively short and simple.

    Also, although this brief section is very enthusiastic, let it be known that my heart belongs to original C.

 */

// Here's PersonThing from the C and C++ Example 4. See what I do with it...
struct PersonThing
{
    public byte id;
    public int posX;
    public int posY;
    public Int16 rot;
}

public class Program
{
    static void Main()
    {
        // I can create all of this text!
        // There was a time in which you would have needed to allocate bytes for each of these characters, plus control characters.
        // You no longer have to worry about whether or not things would live in the stack or not either.

        string someText = "doifhsdgfkjhdlfkgjshdfgkljsdhfgslkdfhglkdjhfasiuldhskdoifhsdgfkjhdlfkgjshdfgkljsdhfgslkdfhglkdjhfasiuldhskdoifhsdgfkjhdlfkgjshdfgkljsdhfgslkdfhglkdjhfasiuldhskdoifhsdgfkjhdlfkgjshdfgkljsdhfgslkdfhglkdjhfasiuldhsk";

        // Look at all this stuff! I can just HAVE this array!
        int[] ints = { 1, 2, 3, 2342, 234, 345743, 24352348, 85678, 234546, 856785, 2345 };

        // But why use an array? We have a neat thing, let's turn our old man array into this cool List!
        List<int> list = new List<int>(ints);

        // What's that? You want to add a 40000 to the list? Realloc...? Never heard of it.
        list.Add(40000);

        // You can use it the same way as an array!
        Console.Out.WriteLine(list[11]);

        // Remember in Example 4 when we "messed up" when making 3 people?
        // Try this!
        PersonThing[] people = new PersonThing[3];

        // Is 3 people not enough? Try 30,000 people!
        PersonThing[] morePeople = new PersonThing[30000];

        // 30,000,000 people?
        PersonThing[] evenMorePeople = new PersonThing[30000000];

        // 300,000,000 people?
        PersonThing[] absurdNumberOfPeople = new PersonThing[300000000];

        // Let's go ahead and set the ID... of the 299 millionth person!
        absurdNumberOfPeople[299999999].id = 4;
        Console.Out.WriteLine(absurdNumberOfPeople[299999999].id);
        // Works fine!
    }
}
