#include <ASL/Types.h>
#include <iostream>
#include <vector>

int main()
{

    std::vector<u32> a = { 1, 2, 3, 4, 5, 6 };

    for (auto& v : a) {
        std::cout << v << std::endl;
    }

    return 0;
}