/// Copyright Mattias Lasersköld 2025

#include "print.h"

int main(int argc, char *argv[]) {
    print("hello {} you\n", "there");

    auto str = format("hello {} you\n", "there");
    print("{}", str);

    print("x: {}, y: {}\n", 10, 20);

    return 0;
}
