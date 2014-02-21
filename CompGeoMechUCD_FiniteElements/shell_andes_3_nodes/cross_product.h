#ifndef CROSS_PRODUCT_H_INCLUDED
#define CROSS_PRODUCT_H_INCLUDED

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

Vector calculate_cross_product(const Vector& a, const Vector& b);

#endif // CROSS_PRODUCT_H_INCLUDED
