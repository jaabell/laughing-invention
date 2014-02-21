# Nima Tafazzoli and Boris Jeremic, December 2009, UCD

mkdir API_NewTemplate3Dep
cd API_NewTemplate3Dep
cd ..

doxygen Doxyfile-Cam_Clay
doxygen Doxyfile-Dafalias_Manzari
doxygen Doxyfile-Drucker_Prager
doxygen Doxyfile-NewTemplate3Dep
doxygen Doxyfile-Rounded_Mohr_Coulomb
doxygen Doxyfile-SANISAND
doxygen Doxyfile-von_Mises
doxygen Doxyfile-Yield_Surface


tar -cvzf API_NewTemplate3Dep.tgz API_NewTemplate3Dep
rm -r -f API_NewTemplate3Dep




