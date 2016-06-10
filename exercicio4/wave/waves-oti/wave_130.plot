set terminal png
set output 'wave_130.png'
set xlabel 'x'
set xlabel 'y'
set pm3d map
set palette gray
set dgrid3d 100,100
splot 'wave_130.dat' u 1:2:3 t""
