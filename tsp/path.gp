set terminal png size 1024,768
set output "path.png"
plot "output_path.txt" w l lw 2, "output_cities.txt" w p pt 7, \
"test.txt" with labels
