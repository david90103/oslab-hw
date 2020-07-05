#!/bin/bash
ALGO=$1
RUNS=$2
ITERATIONS=$3
BITS=$4
SEEDFILE=$5

ALGORITHMS=("es" "hc")


if [[ -z $1 || -z $2 || -z $3 || -z $4 ]]; then
    echo "Usage: ./search.sh [algorithm] [runs] [iterations] [bits] [seedfile]"
    exit
fi

if [[ ! " ${ALGORITHMS[@]} " =~ " $1 " ]]; then
    printf "Support algorithms: "
    printf "%s " "${ALGORITHMS[@]}"
    exit
fi

echo "Algorithm: $ALGO Runs: $RUNS Iterations: $ITERATIONS Bits: $BITS Seedfile: $SEEDFILE"
echo "Start!"

for ((r = 0; r < RUNS; r++)); do
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        "./$ALGO" $RUNS $ITERATIONS $BITS $SEEDFILE
    else
        "./$ALGO.exe" $RUNS $ITERATIONS $BITS $SEEDFILE
    fi
    sleep 1
done
