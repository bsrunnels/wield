set termoption dashed
set style line 1 lt 1 lc 0 lw 2
set style line 2 lt 3 lc 0 lw 2 pt 3
set multiplot layout 2,2;

set xlabel "Tilt Angle (degrees)"
set ylabel "Energy (J/m^2)"
set key box

set title "100 Tilt Axis"
#set output "FCC_100.png";
set xrange [0:90]    
set yrange [0:2]
plot "fcc_fcc_tilt_100_cauchy.dat" using (90-$1):2 with lines ls 1 title "Theory", "Shibuta_FCC_100.csv" with linespoints ls 2 title "MD"

set title "110 Tilt Axis"
#set output "FCC_110.png"
set xrange [0:180]
set yrange [-0.5:2]
plot "fcc_fcc_tilt_110_cauchy.dat" using (180-$1):2 with lines ls 1 title "Theory", "Shibuta_FCC_110.csv" with linespoints ls 2 title "MD"

set title "111 Tilt Axis"
#set output "FCC_111.png"
set xrange [0:60]
set yrange [0:2]
plot "fcc_fcc_tilt_111_cauchy.dat" with lines ls 1 title "Theory", "Shibuta_FCC_111.csv" with linespoints ls 2 title "MD"

set title "112 Tilt Axis"
#set output "FCC_112.png"
set xrange [0:105] 
plot "fcc_fcc_tilt_112_cauchy.dat" with lines ls 1 title "Theory", "Shibuta_FCC_112.csv" with linespoints ls 2 title "MD"

unset multiplot
#unset output