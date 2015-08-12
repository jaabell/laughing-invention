#ifndef UNITDEFS_H
#define UNITDEFS_H

#include <iostream>
#include <string>

#include "siunit.h"
#include "quantity.h"

/*=====================================================
                    BASE UNITS
=====================================================*/
namespace ESSIunits
{
//                       sc , M, L, T  : sc = scale factor
static const SIUnit unitless   (1.0, 0, 0, 0);
static const SIUnit meters     (1.0, 0, 1, 0);
static const SIUnit kilograms  (1.0, 1, 0, 0);
static const SIUnit seconds    (1.0, 0, 0, 1);

//Aliases to these units
static const SIUnit& m = meters;
static const SIUnit& kg = kilograms;
static const SIUnit& s = seconds;
static const SIUnit& adim = unitless;


/*=====================================================
                    Derived units
=====================================================*/

// Length
static const SIUnit centimeter (0.01, 0, 1, 0, "cm");
static const SIUnit millimeter (0.001, 0, 1, 0, "mm");
static const SIUnit kilometer  (1000., 0, 1, 0, "km");
static const SIUnit& cm = centimeter;
static const SIUnit& mm = millimeter;
static const SIUnit& km = kilometer;

// Time
static const SIUnit Hertz      (1.0, 0, 0, -1, "Hz");
static const SIUnit& Hz = Hertz;
static const SIUnit microsecond      (1e-3, 0, 0, 1, "ms");
static const SIUnit nanosecond      (1e-6, 0, 0, 1, "ns");
static const SIUnit& ms = microsecond;
static const SIUnit& ns = nanosecond;

// Force
static const SIUnit Newton     (1.0, 1, 1, -2, "N");
static const SIUnit kN         (1000., 1, 1, -2, "kN");
static const SIUnit& N = Newton;

// Pressure
static const SIUnit Pascal     (1.0, 1, -1, -2, "Pa");
static const SIUnit& Pa = Pascal;
static const SIUnit kPa     (1.e3, 1, -1, -2, "kPa");
static const SIUnit MPa     (1.e6, 1, -1, -2, "MPa");
static const SIUnit GPa     (1.e9, 1, -1, -2, "GPa");


// Physical static constants
static const Quantity g(9.81, m / s / s);

}

#endif // UNITDEFS_H
