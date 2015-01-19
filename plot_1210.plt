plot "Mg.csv" with linespoints title "Mg MD" ls 1, "Ti.csv" with linespoints title "Ti MD" ls 3, "test_mg.out" using 1:(.3444+$2)*1800 with lines title "Mg Model" ls 1 lw 3, "test_ti.out" using 1:(.3444+$2)*4300 with lines title "Ti Model" ls 3 lw 3
