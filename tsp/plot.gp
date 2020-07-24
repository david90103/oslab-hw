set xrange[0:1000]
set yrange[400:700]
set xlabel "Number of evaluation"
set ylabel "Objective value (Distance)"
plot "output_aco_1.txt" w l lw 2
