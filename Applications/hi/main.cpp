#include <ASL/Types.h>
#include <iostream>
#include <vector>

struct Alloc {
    static int next()
    {
        return 3;
    }
};

int main()
{
    Alloc t;

    std::cout << t.next() << " " << Alloc::next() << std::endl;

    return 0;
}