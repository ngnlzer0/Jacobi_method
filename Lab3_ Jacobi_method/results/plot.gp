set terminal pngcairo size 800,600
set output 'results/jacobi_plot_E=0.0100.png'
set title 'Jacobi Method Execution Time (E=0.01)'
set xlabel 'Matrix size (n)'
set ylabel 'Execution time (ms)'
set grid
set key left top
plot 'results/jacobi_results_E=0.0100.dat' using 1:2 with linespoints title 'Single-threaded', \
     'results/jacobi_results_E=0.0100.dat' using 1:3 with linespoints title 'Multi-threaded'
