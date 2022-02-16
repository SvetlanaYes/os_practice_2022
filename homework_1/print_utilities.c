#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "print_utilities.h"

void print_string(const char* str, const size_t size)
{
    for (int i = 0 ; i < size; ++i)
    {
        printf("%c" ,str[i]);
    }
}


void print_integer(const int int_value)
{
    printf("%d", int_value);
}


void print_double(const double double_value)
{
    printf("%f", double_value);
}