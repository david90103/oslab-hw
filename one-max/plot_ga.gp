set xrange[0:1000]
set yrange[0:100]
set xlabel "Number of evaluation"
set ylabel "Objective value"
plot "ga-roulette.txt" w l lw 2, "ga-tournament.txt" w l lw 2, "hc-neighbor.txt" w l lw 2, "sa.txt" w l lw 2, "ts-neighbor.txt" w l lw 2
