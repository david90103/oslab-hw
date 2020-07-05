import sys

ALGORIGHMS = ("hc")

if len(sys.argv) != 4:
    print("Usage: python ./merge_output.py [algorithm] [runs] [iterations]\n"
          "Example: python ./merge_output.py hc 30 100   This will merge output_hc_run_1.txt ~ output_hc_run_30.txt")
    exit()

algo = sys.argv[1]
runs = int(sys.argv[2])
iterations = int(sys.argv[3])
values = [0] * iterations


if not algo in ALGORIGHMS:
    print("Support algorithms: ", end="")
    for i in ALGORIGHMS:
        print(i, end=" ")
    exit()

for f_index in range(1, runs + 1):
    filename = "output_" + algo + "_run_" + str(f_index) + ".txt"
    try:
        with open(filename, "r") as f:
            for line in f:
                line = line.split(" ")
                values[int(line[0]) - 1] += int(line[1])
    except IndexError:
        print("File not aligned:", filename)
        exit()
    except FileNotFoundError:
        print("File not found:", filename)
        exit()

values = map(lambda x: x / runs, values) 

with open("output_hc_merged.txt", "w") as f:
    for idx, v in enumerate(values):
        f.write(str(idx + 1) + " " + str(v) + "\n")
print("Done.")

    

