// MOSS
#include <Information.h>
#include <Channel.h>
#include <FEM_ObjectBroker.h>
#include <Element.h>
#include <Node.h>
#include <ID.h>
#include <Domain.h>
#include <OPS_Globals.h>
#include <NDMaterial.h>

//NDArray
#include <Vector.h>


Vector calculate_cross_product(const Vector& a, const Vector& b)
{
    Vector c(3); // Store the result here

    if ( (a.Size() != 3) || (b.Size() != 3) )
    {
        cerr << "Error: calculate_cross_product only defined for 3x1 vectors.\n";
        exit(-1);
    }

    c(0) =   a(1) * b(2) - b(1) * a(2);
    c(1) = - a(0) * b(2) + b(0) * a(2);
    c(2) =   a(0) * b(1) - b(0) * a(1);

    return c;
}
