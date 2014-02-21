#ifndef LTENSORDISPLAY_H_INCLUDED
#define LTENSORDISPLAY_H_INCLUDED

#include "../../ltensor/LTensor.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace std;


// To print LTensors with a format similar to that of NDarray :)
namespace LTensorDisplay
{
    void print(DTensor1 a, string name = "a", string msg = "", int baseindex = 0);
    void print(DTensor2 a, string name = "a", string msg = "", int baseindex = 0);
    void print(DTensor3 a, string name = "a", string msg = "", int baseindex = 0);
    void print(DTensor4 a, string name = "a", string msg = "", int baseindex = 0);
}
#endif // LTENSORDISPLAY_H_INCLUDED
