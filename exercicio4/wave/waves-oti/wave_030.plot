set terminal png
set output 'wave_030.png'
set xlabel 'x'
set xlabel 'y'
set pm3d map
set palette gray
set dgrid3d 100,100
splot 'wave_030.dat' u 1:2:3 t""
