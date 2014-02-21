
rm -f plot.gnuplot.displacement.temp plot.gnuplot.displacement.final

sed "s:whichoutputfile:$1:" plot.gnuplot.displacement > plot.gnuplot.displacement.temp
sed "s:nameofoutputfile:$1:" plot.gnuplot.displacement.temp > plot.gnuplot.displacement.final
