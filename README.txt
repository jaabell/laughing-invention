Collection of Useful Libraries:

This repository (sources) consists of:

1.
A number of external libraries/software packages are linked  (for example:
lmfit, sundials, CLAPACK, gsl, ARPACK, BLAS, CBLAS, LAPACK, SuperLU, UMFPACK,
ITPACK, ParMetis, METIS...)

2.
A number of our libraries (CompGeoMechUCD_PEP, CompGeoMechUCD_FiniteElements,
CompGeoMechUCD_MaterialModeling, CompGeoMechUCD_SSEPFEM, nDarray...), including
Modified OpenSees Services library (which features a significant number of
changes to original OpenSees framework as available from 2001-2005) together
with main application source to form an executable application program (for
example the NRC ESSI Program, etc.)

It is highly recommended that all external libraries be downloaded from their
original internet locations and installed from scratch. They are provided in
this repository only for convenience...

All comments should be directed to:
Boris Jeremic
Professor, CEED, University of California, Davis, CA
Faculty Scientist, ESD, Lawrence Berkeley National Laboratory, Berkeley, CA 
jeremic@ucdavis.edu, bjeremic@lbl.gov
http://sokocalo.engr.ucdavis.edu/~jeremic
1.530.754.9248      
jeremic@ucdavis.edu 
http://sokocalo.engr.ucdavis.edu/~jeremic

----------------------------------------------------------------------------
NEEDED to compile (sequential):

1) The following package should be installed:
Ubuntu:
sudo apt-get install libmysqlclient-dev
Fedora:
yum install mysql 
yum install mysql-server 
yum install mysql-devel 

2) Mysql server should be restarted:
Fedora:
/sbin/service mysqld start 

2) A softlink to the following folder should be created:

cd /usr/local/lib
Ubuntu 10.04:
sudo ln -s /usr/lib/libmysqlclient.so 
Ubuntu 12.04:
sudo ln -s /usr/lib//x86_64-linux-gnu/libmysqlclient.so 
Fedora:
sudo ln -s /usr/lib64/mysql/libmysqlclient.so


----------------------------------------------------------------------------
NEEDED to compile (parallel):

In addition to the above (sequential) prerequisite,

1) Gfortran, OPENMPI and cmake

To install:

Ubuntu [As of Saucy Salamander, Feb 2014]:

sudo apt-get install build-essential gfortran cmake openmpi1.6-bin openmpi1.6-doc libopenmpi1.6-dev


2) Unzip and install ParMETIS (.tar.gz included in source distribution)

tar -xzvf parmetis-4.0.2.tar.gz
cd parmetis-4.0.2
make config prefix=<FEI_SOURCE_PATH>/parmetis-4.0.2_install debug=0
make install 
cd metis
make config prefix=<FEI_SOURCE_PATH>/metis-4.0.2_install debug=0
make install

3) PETSc

tar -xzvf petsc-3.3-p7.tar.gz
cd petsc-3.3-p7

  -> Replace in the command below the required directories (created above)

./configure --with-x=0 --with-debugging=no --with-shared-libraries=0 --with-cc=mpicc --with-fc=mpif77 --with-blacs=1 --download-blacs=yes --with-scalapack=1 --download-scalapack=yes --with-spai=1 --download-spai=yes --with-hypre=1–download-hypre=yes --with-plapack=1 –download-plapack=yes --download-f-blas-lapack=1 --download-plapack=1 --with-metis-dir=<FEI_SOURCE_PATH>/metis-4.0.2_install --with-parmetis-dir=<FEI_SOURCE_PATH>/parmetis-4.0.2_install --with-superlu_dist=1 --download-superlu_dist=yes --download-superlu=yes --download-superlu_dist --with-spooles=1 --download-spooles

./configure --with-x=0 --with-debugging=no --with-shared-libraries=0 --with-cxx=mpic++ --with-cc=mpicc --with-fc=mpif77 --with-blacs=1 --download-blacs=yes --with-scalapack=1 --download-scalapack=yes --with-spai=1 --download-spai=yes --with-hypre=1 --download-hypre=yes --with-plapack=1 --download-plapack=yes --download-f-blas-lapack=1 --download-plapack=1 --with-metis-dir=/home/jaabell/Repositories/newfei/metis-4.0.2_install --with-parmetis-dir=/home/jaabell/Repositories/newfei/parmetis-4.0.2_install --with-superlu_dist=1 --download-superlu_dist=yes --download-superlu=yes --download-superlu_dist --with-spooles=1 --download-spooles --with-mpi-dir=/usr/lib/openmpi/ 
make 
make install

