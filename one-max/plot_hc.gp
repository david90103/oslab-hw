set yrange[0:100]
set title "Hill climbing"
set xlabel "Iteration"
set ylabel "Objective value"
plot "output_hc_merged.txt" w lp
