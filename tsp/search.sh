# ./main aco 30 500 "data/eil51.tsp" 10 1.0 6.0 0.1 0.9
# ./main de 1 10000 "data/eil51.tsp" 30 0.7 0.5 # no boundary check
# ./main de 1 5000 "data/eil51.tsp" 40 0.7 0.3
# ./main de 30 20000 "data/eil51.tsp" 51 0.8 0.7
# ./main pso 1 1000 "data/eil51.tsp" 20 0.4 0.6 0.7 # bound 0 to 100
# ./main pso 1 10000 "data/eil51.tsp" 30 0.1 0.2 0.1 # bound 30 to 60
# ./main ga 30 5000 "data/eil51.tsp" 100 0.8 0.1 ox
./main gap 1 1000 "data/eil51.tsp" 4 100 0.8 0.1 ox
# ./main de1d 30 5000 "data/eil51.tsp" 80 0.5 0.3
# ./main pso1d 1 50000 "data/eil51.tsp" 30 0.6 0.7 0.6
