Real ESSI Dependencies Source
=======================================

### Usage
This file contains the website link to dependencies of REAL ESSI for sequential as well as parallel version. 
All the libraries are created/linked in lib folder. 

### Instructions

1. In order to update any dependency just go to its website and download the current latest available compressed source and replace with the older version in the SRC directory
2. No need to change anything inside the source. Makefile will take care of that
3. To check the update dpendencies version do ```make list_dependencies```
4. Run ```make help``` to get the target options

### Dependencies Source 

**ARPACK**
http://www.caam.rice.edu/software/ARPACK/

**SUITESPARSE**
http://faculty.cse.tamu.edu/davis/SuiteSparse/

**HDF5**
https://www.hdfgroup.org/HDF5/release/obtainsrc.html

**PETSC**
git clone -b maint https://bitbucket.org/petsc/petsc petsc
http://www.mcs.anl.gov/petsc/download/

**SLEPC**
git clone https://bitbucket.org/slepc/slepc
http://slepc.upv.es/download/

**METIS**
http://glaros.dtc.umn.edu/gkhome/metis/metis/download

**PARMETIS**
http://glaros.dtc.umn.edu/gkhome/metis/parmetis/download

**ATLAS**
http://www.netlib.org/atlas/
https://sourceforge.net/projects/math-atlas/files/
http://math-atlas.sourceforge.net/

**LAPACK**
https://github.com/Reference-LAPACK/lapack
https://github.com/Reference-LAPACK/lapack-release
http://www.netlib.org/lapack/

**BLAS**
http://www.netlib.org/blas/

----
Sumeet Kumar Sinha (August,2016)	
[UCD CompGeoMech](http://sokocalo.engr.ucdavis.edu/~jeremic/)
University of California, Davis 
   
