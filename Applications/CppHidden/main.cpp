#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <string>
#include <string_view>

struct params {
    std::string file;
    bool readonly;
    bool executable;
};

int rand_bool(float change) {
    return ((float) rand() / RAND_MAX) < change;
}

// Case range
char to_lower(char c) {
    switch (c) {
        case 'A'...'Z':
            return c - ('A' - 'a');
        default:
            return c;
    }
}

int main() {
    srand(time(nullptr));

    // Elvis operator
    bool v = rand_bool(0.5f) ?: false;
    printf("%d\n", v);

    printf("%c\n", tolower('F'));

    // Named params
    params p = params {
        .file = "Wow.txt",
        .readonly = true,
        .executable = false,
    };

    // Designated Array Initializers
    enum Color {
        BLACK,
        RED,
        ORANGE,
        YELLOW,
        GREEN,
        BLUE,
        CYAN,
        MAGENTA,
        WHITE
    };

    constexpr std::string_view color_name[] = {
        [BLACK] = "black",
        [RED] = "red",
        [ORANGE] = "orage",
        [YELLOW] = "yellow",
        [GREEN] = "green",
        [BLUE] = "blue",
        [CYAN] = "cyan",
        [MAGENTA] = "magenta",
        [WHITE] = "white"
    };

    std::cout << color_name[MAGENTA] << std::endl;

    // Designated Array Initializer
    int arr[] = { [0 ... 11] = 2, 3, 4, [16] = 5 };
    // 2 2 2 2 2 2 2 2 2 2 2 2 3 4 0 0 5

    for (int i = 0; i < sizeof(arr) / sizeof(int); i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Labels as value (goto)


    return 0;
}