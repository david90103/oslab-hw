set xrange[0:1000]
set yrange[50:100]
set xlabel "Number of evaluation"
set ylabel "Objective value"
plot "output_ga_1.txt" w l lw 2, "output_ga_2.txt" w l lw 2, "output_sa_1.txt" w l lw 2
