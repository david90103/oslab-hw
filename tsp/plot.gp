set xrange[0:5000]
set yrange[420:650]
set xlabel "Number of evaluation"
set ylabel "Objective value (Distance)"
plot "output_aco_1.txt" w l lw 2
