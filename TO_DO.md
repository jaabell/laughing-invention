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


###Analysis level

* Remove the need for equaldof by making elements with different DOF numbers at nodes
play nicely with each-other.  (Expand stiffness matrices when needed).


###Material models



###Elements

* LTensor based u-p-U elements
* Contact element (Sumeet)
* Surface-to-surface contact 

###Parser

* Better error reporting for runtime errors
* Have vectors of quantities
* Parallel parsing (pre-partition mesh and instruct sub-parsers to read certain files with nodes, elements and such)
* Physical groups and associated semantics. 

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

* Take into account machine topology and performance for partitioning.
* About 50kb are used per element (8 node brick, with linear material). This is too much!
* ParallelNumberer builds a graph of dofs and numbers them in P0, then distributing. This does not scale. (Might hurt performance for really large models, we haven't encountered these yet.)
* Partitioning (graph construction) is also done in P0. 
* 8NodeBrickLT and 27NodeBrickLT are generating material copies on P0... bad bad

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

