// Test to check the maximum number assignable to "int" under these compiler conditions.
//
// The maximum number of an int in the current configuration is the
// maximum number of DOFs available in the system.
//
// This number might not be the expected one.
//
// On my system this system returns:
//
// $ ./test_int_max
// Minimum value for int: -2147483648
// Maximum value for int: 2147483647

#include <iostream>
#include <limits>

int main()
{

    std::cout << "Minimum value for int: " << std::numeric_limits<int>::min() << '\n';
    std::cout << "Maximum value for int: " << std::numeric_limits<int>::max() << '\n';


    return 0;
}