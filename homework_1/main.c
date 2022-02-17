#include "print_utilities.h"

int main(int argc , char** argv)
{
    char greeting[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
    int value1 = 12;
    double value2 = 12.6;
    print_string(greeting,6);
    printf("\n");
    print_integer(value1);
    printf("\n");
    print_double(value2);
    return 0;
}
