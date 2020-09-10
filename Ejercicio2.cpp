#include <iostream>

int32_t main()
{
    int (*(&(*(*var)())(const int&))[10])[10];
    return EXIT_SUCCESS;
}