#include <fmt/base.h>
#include <iostream>

int main() {
    fmt::print("Hello, World!\n");
    fmt::print("-10 // 3 = {}\n", -10 / 3);
    fmt::print("10 % -3 = {}\n", 10 % -3);
    fmt::print("-10 % -3 = {}\n", -10 % -3);
    fmt::print("-1 % 4 = {}\n", -1 % 4);
    return 0;
}