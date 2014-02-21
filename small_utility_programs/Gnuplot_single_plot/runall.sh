#!/bin/bash
###############################################################################
# Written by: Nima Tafazzoli October 2010


for f in `ls Datafiles_Acceleration/`; do
echo Plotting : $f
sed_gnuplot_acceleration.sh $f
gnuplot plot.gnuplot.acceleration.final
done
rm plot.gnuplot.acceleration.temp plot.gnuplot.acceleration.final



for f in `ls Datafiles_Displacement/`; do
echo Plotting : $f
sed_gnuplot_displacement.sh $f
gnuplot plot.gnuplot.displacement.final
done
rm plot.gnuplot.displacement.temp plot.gnuplot.displacement.final
