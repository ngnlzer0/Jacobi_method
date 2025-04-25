set terminal pngcairo size 800,600
set output 'jacobi_plot_E=0.0010.png'
set title 'Jacobi Method Execution Time (E=0.001)'
set xlabel 'Matrix size (n)'
set ylabel 'Execution time (ms)'
set grid
set key left top
plot 'jacobi_results_E=0.0010.dat' using 1:2 with linespoints title 'Single-threaded', \
     'jacobi_results_E=0.0010.dat' using 1:3 with linespoints title 'Multi-threaded'
