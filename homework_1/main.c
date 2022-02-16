#include "print_utilities.h"

int main(int argc , char** argv)
{
    char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    int value1 = 12;
    int value2 = 12.6;
    print_string(greeting,6);
    print_integer(value1);
    print_double(value2);
    return 0;
}