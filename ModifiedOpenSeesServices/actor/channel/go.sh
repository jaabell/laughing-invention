#!/bin/bash

make
make test_hdf5_channel
./test_hdf5_channel
hdfview analysis.h5