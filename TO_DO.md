Todo with ESSI
==============

This markdown file gathers information on what needs to be done on ESSI. It is separated
into **features**, **performance enhancements**, **bug fixes**, **testing**.


Its a collective effort to populate this file with what we see needs to be done
with the program to make it better.  Please add o this file as you detect needs, or as
you fix them.

Also, it is meant to gather history on what is done on the code.

*CompGeoMech 2015*


Table of contents
=================

[TOC]



Todo
=======


Features
---------

These are new things we would like to see on ESSI. Further subdivided into

###Material models



###Elements

* LTensor based u-p-U elements
* Contact element
* Surface-to-surface contact

###Parser

* Better error reporting for runtime errors
* Have vectors of quantities
* Parallel parsing
* Physical groups

###Output

* Forces (not really needed)
* Information on analysis options used

###SOE Solvers


###Time integrators


###Constitutive integrators

* Have implicit integrators
* Do automatic subincrementation


Performance enhancements
------------------------

* Keep some elements in rank 0, to better use parallelism.
* Take into account machine topology and performance for partitioning.
* HDF5 mesh output can be done in parallel. 
* There is lots of room for tuning HDF5. Chunk sizes, filters (compression), using collective calls, are some of the things that could yield a performance increase.
* About 50kb are used per element (8 node brick, with linear material). This is too much!
* ParallelNumberer builds a graph of dofs and numbers them in P0, then distributing. This does not scale. (Might hurt performance for really large models, we haven't encountered these yet.)
* Partitioning (graph construction) is also done in P0. 

Bug fixes
---------



Testing
-------









What has been done
=============================



Features
---------

* Template EP materials programmed using C++ templates and LTensor library. Curiously recurring template pattern can be used to achieve polymorphism.

Performance enhancements
------------------------

* Link to ATLAS-tuned BLAS and LAPACK. (PAPI - through HPCToolkit - is [was] showing large ammouunt of L2 data-cache misses in the PETSc solver)



Bug fixes
---------



Testing
-------

