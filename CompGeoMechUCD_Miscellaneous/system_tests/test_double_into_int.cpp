// Test to check if ints fit in doubles without loss of precision.
//
// This is for the parer, that only parses doubles.
//
// On my system
//
// $ ././test_double_into_int
// :)

#include <iostream>
#include <limits>

int main()
{

    int maxint = std::numeric_limits<int>::max();
    int minint = std::numeric_limits<int>::min();

    double maxint_to_double = maxint;
    double minint_to_double = minint;

    int maxint_to_double_to_int = maxint_to_double;
    int minint_to_double_to_int = minint_to_double;

    if ( (maxint == maxint_to_double_to_int) &&
            (minint == minint_to_double_to_int))
    {
        std::cout << ":)\n";
    }
    else
    {
        std::cout << ":(\n";
    }

    return 0;
}