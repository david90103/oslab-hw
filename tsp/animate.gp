set terminal gif animate delay 50
set output "animation.gif"
set xrange[0:100]
set yrange[0:100]
do for [i=1:39] {
    set title "iter ".i
    plot './output/file_'.i
}