
rm -f plot.gnuplot.acceleration.temp plot.gnuplot.acceleration.final

sed "s:whichoutputfile:$1:" plot.gnuplot.acceleration > plot.gnuplot.acceleration.temp
sed "s:nameofoutputfile:$1:" plot.gnuplot.acceleration.temp > plot.gnuplot.acceleration.final
