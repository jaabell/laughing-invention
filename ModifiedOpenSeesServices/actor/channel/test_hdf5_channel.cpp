#include <iostream>
#include <string>
#include "HDF5_Channel.h"

using namespace std;

int main()
{
    string filename("analysis.h5");
    string model_name("test");
    string stage_name("stage1");
    int nsteps = 10;
    HDF5_Channel channel(filename, model_name, stage_name, nsteps);
    return 0;
}