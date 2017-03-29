set terminal png size 800, 600
set output 'plot.png'
set grid

plot 'sample/data.out' using 1:2 with points
